#pragma once

#define DEBUG_MODE

/**
 * @file heap_memory_tracker_component.h
 * @brief Heap memory tracking and leak detection macros.
 *
 * This header provides macros to track heap memory allocations and detect memory leaks.
 * It overloads the global `new` and `delete` operators to track all heap allocations
 * and deallocations. These macros are only active in `DEBUG_MODE`.
 *
 * Usage:
 * 1. To enable heap memory tracking:
 *    - Place `TRACK_HEAP_ONLY()` above your `main()` function.
 *    - Call `PRINT_HEAP_MEMORY()` to print the current heap size at any point.
 *    - Use `TRACK_HEAP_AND_LEAKS()` to also track and report memory leaks.
 *
 * 2. In `RELEASE_MODE`, all these macros are disabled, and memory tracking does not occur.
 *
 * Example usage:
 * @code
 * TRACK_HEAP_ONLY();
 * // Allocate some memory
 * PRINT_HEAP_MEMORY(); // Print current heap memory usage
 * @endcode
 *
 * @note Do not call the tracking macro multiple times in the same scope, as it defines static variables
 * that may cause conflicts or crashes when defined more than once.
 */

/**
 * @def TRACK_HEAP_ONLY()
 * @brief Macro to track heap memory allocations without detecting leaks.
 *
 * This macro overloads the global `new` and `delete` operators to track the total heap memory allocated.
 * It keeps a running total of all heap memory currently in use.
 *
 * In `DEBUG_MODE`, the macro initializes the `total_allocated_memory` counter and implements custom memory
 * allocation and deallocation functions. The memory usage is updated with each allocation and deallocation.
 *
 * The macro defines a static `MemoryReporter` object that prints the total allocated memory when the program exits.
 *
 * @note Calling this macro multiple times in the same scope will result in a crash due to variable redefinitions.
 */

/**
 * @def TRACK_HEAP_AND_LEAKS()
 * @brief Macro to track heap memory allocations and detect memory leaks.
 *
 * This macro extends the functionality of `TRACK_HEAP_ONLY()` by additionally tracking
 * individual allocations and reporting memory leaks at program termination.
 *
 * It stores all allocations in an array of `AllocationRecord` structures. On deallocation,
 * the corresponding entry is removed. At the end of the program, the remaining entries
 * (representing memory leaks) are printed.
 *
 * The macro defines a static `LeakReporter` object that reports leaks when the program exits.
 *
 * @note Like `TRACK_HEAP_ONLY()`, calling this macro multiple times in the same scope will cause a crash.
 */

/**
 * @def PRINT_HEAP_MEMORY()
 * @brief Macro to print the current heap memory usage.
 *
 * This macro prints the total amount of memory currently allocated on the heap.
 * It uses the `total_allocated_memory` atomic counter defined by `TRACK_HEAP_ONLY()`.
 *
 * Example usage:
 * @code
 * PRINT_HEAP_MEMORY();
 * @endcode
 */

#ifdef DEBUG_MODE
#include <iostream>
#include <cstdlib>
#include <new>
#include <mutex>
#include <atomic>
#include <cstdlib>
#endif

#ifdef DEBUG_MODE
#define TRACK_HEAP_ONLY() \
 \
std::atomic<size_t> total_allocated_memory(0); \
 \
void* operator new(size_t size) { \
    size_t total_size = size + sizeof(size_t); \
    void* ptr = std::malloc(total_size); \
    if (!ptr) { \
        throw std::bad_alloc(); \
    } \
    *static_cast<size_t*>(ptr) = size; \
    total_allocated_memory += size; \
    return static_cast<char*>(ptr) + sizeof(size_t); \
} \
 \
void* operator new[](size_t size) { \
    size_t total_size = size + sizeof(size_t); \
    void* ptr = std::malloc(total_size); \
    if (!ptr) { \
        throw std::bad_alloc(); \
    } \
    *static_cast<size_t*>(ptr) = size; \
    total_allocated_memory += size; \
    return static_cast<char*>(ptr) + sizeof(size_t); \
} \
void operator delete(void* ptr) noexcept { \
    if (ptr) { \
        void* original_ptr = static_cast<char*>(ptr) - sizeof(size_t); \
        size_t size = *static_cast<size_t*>(original_ptr); \
        total_allocated_memory -= size; \
        std::free(original_ptr); \
    } \
} \
void operator delete[](void* ptr) noexcept { \
    if (ptr) { \
        void* original_ptr = static_cast<char*>(ptr) - sizeof(size_t); \
        size_t size = *static_cast<size_t*>(original_ptr); \
        total_allocated_memory -= size; \
        std::free(original_ptr); \
    } \
} \
void operator delete(void* ptr, size_t size) noexcept { \
    if (ptr) { \
        total_allocated_memory -= size; \
        std::free(ptr); \
    } \
} \
 \
void operator delete[](void* ptr, size_t size) noexcept { \
    if (ptr) { \
        total_allocated_memory -= size; \
        std::free(ptr); \
    } \
} \
 \
size_t Get_Total_Allocated_Memory() { \
    return total_allocated_memory.load(); \
} \
 \
void Report_Total_Allocated_Memory() { \
    std::cout << "Total allocated memory: " << Get_Total_Allocated_Memory() << " bytes" << std::endl; \
} \
 \
struct MemoryReporter { \
    ~MemoryReporter() { \
        Report_Total_Allocated_Memory(); \
    } \
}; \
 \
static MemoryReporter memoryReporter;
// TRACK_HEAP_ONLY() END
#else
#define TRACK_HEAP_ONLY() (void*)(0);
#endif /* DEBUG_MODE */

#ifdef DEBUG_MODE
#define TRACK_HEAP_AND_LEAKS()\
 \
constexpr size_t MAX_TRACKED_ALLOCATIONS = 107374182; \
 \
struct AllocationRecord { \
    void* ptr; \
    size_t size; \
}; \
 \
std::mutex heap_tracker_mutex; \
AllocationRecord allocation_records[MAX_TRACKED_ALLOCATIONS]; \
size_t allocation_count = 0; \
size_t total_allocated_memory = 0; \
 \
void Add_Allocation(void* ptr, size_t size) { \
    std::lock_guard<std::mutex> guard(heap_tracker_mutex); \
    if (allocation_count < MAX_TRACKED_ALLOCATIONS) { \
        allocation_records[allocation_count++] = { ptr, size }; \
        total_allocated_memory += size; \
    } else { \
        std::cerr << "Warning: Maximum tracked allocations reached." << std::endl; \
    } \
} \
 \
void Remove_Allocation(void* ptr) { \
    std::lock_guard<std::mutex> guard(heap_tracker_mutex); \
    for (size_t i = 0; i < allocation_count; ++i) { \
        if (allocation_records[i].ptr == ptr) { \
            total_allocated_memory -= allocation_records[i].size; \
            allocation_records[i] = allocation_records[--allocation_count]; \
            return; \
        } \
    } \
} \
 \
void* operator new(size_t size) { \
    void* ptr = std::malloc(size); \
    if (!ptr) { \
        throw std::bad_alloc(); \
    } \
    Add_Allocation(ptr, size); \
    return ptr; \
} \
 \
void* operator new[](size_t size) { \
    void* ptr = std::malloc(size); \
    if (!ptr) { \
        throw std::bad_alloc(); \
    } \
    Add_Allocation(ptr, size); \
    return ptr; \
} \
 \
void operator delete(void* ptr) noexcept { \
    if (ptr) { \
        Remove_Allocation(ptr); \
        std::free(ptr); \
    } \
} \
 \
void operator delete[](void* ptr) noexcept { \
    if (ptr) { \
        Remove_Allocation(ptr); \
        std::free(ptr); \
    } \
} \
void operator delete(void* ptr, size_t size) noexcept { \
    if (ptr) { \
        Remove_Allocation(ptr); \
        std::free(ptr); \
    } \
} \
 \
void operator delete[](void* ptr, size_t size) noexcept { \
    if (ptr) { \
        Remove_Allocation(ptr); \
        std::free(ptr); \
    } \
} \
 \
void Report_Leaks() { \
    std::lock_guard<std::mutex> guard(heap_tracker_mutex); \
    if (allocation_count == 0) { \
        std::cout << "No memory leaks detected." << std::endl; \
    } else { \
        std::cout << "Memory leaks detected:" << std::endl; \
        for (size_t i = 0; i < allocation_count; ++i) { \
            std::cout << "Leaked " << allocation_records[i].size << " bytes at address " << allocation_records[i].ptr << std::endl; \
        } \
    } \
} \
 \
struct LeakReporter { \
    ~LeakReporter() { \
        Report_Leaks(); \
    } \
}; \
 \
static LeakReporter leakReporter;
// TRACK_HEAP_AND_LEAKS() END
#else
#define TRACK_HEAP_AND_LEAKS() (void*)(0);
#endif /* DEBUG_MODE */

#ifdef DEBUG_MODE
#define PRINT_HEAP_MEMORY() \
    std::cout << "Current Heap size: " << total_allocated_memory << "bytes" << std::endl;
#else
#define PRINT_HEAP_MEMORY() (void*)(0);
#endif /* DEBUG_MODE */
