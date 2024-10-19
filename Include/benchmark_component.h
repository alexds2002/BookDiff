#pragma once

#include <stack>
#include <chrono>
#include <iostream>

/**
 * @brief Tracks and benchmarks execution time for code segments.
 *
 * The `BenchMarkExecution` class allows you to time different parts of code by
 * saving time points and calculating the time elapsed between them. It supports
 * saving multiple time points in a stack, allowing you to measure nested or
 * sequential code segments.
 *
 * @note
 * - The first time point is automatically saved at the time of construction.
 * - The class provides functions to save time points, calculate time passed since
 *   the last point, and print or return the elapsed time.
 *
 * Example usage:
 * @code
 * // Code to benchmark
 * timer.Save_Time_Point();
 * // More code
 * auto elapsed_time = timer.Get_Time_From_Last_Point();  // Get elapsed time
 * timer.Print_Time_From_Last_Point();  // Print elapsed time
 * timer.Pop_Last_Point();  // Remove the last time point and get elapsed time
 * @endcode
 */
class BenchMarkExecution
{
public:
    /**
     * @brief Constructs the benchmark timer
     *
     * Saves the initial time point at the moment of construction. The default print
     */
    BenchMarkExecution() noexcept;

    /**
     * @brief Destructor for `BenchMarkExecution`.
     *
     * Currently does nothing, but ensures that no resources are left unmanaged.
     */
    ~BenchMarkExecution() noexcept;

    /**
     * @brief Saves the current time point.
     *
     * This method pushes the current time onto the `m_time_segments` stack.
     * Useful for marking new time points to measure the time between them.
     */
    void Save_Time_Point() noexcept;

    /**
     * @brief Gets the time elapsed since the last saved time point.
     *
     * Returns the time in milliseconds that has passed since the last saved time point.
     * The first time point is set during construction.
     *
     * @return std::chrono::milliseconds Time elapsed since the last saved point.
     */
    std::chrono::milliseconds Get_Time_From_Last_Point() const noexcept;

    /**
     * @brief Prints the time elapsed since the last saved time point.
     *
     * Prints the time in milliseconds passed since the last saved time point
     */
    void Print_Time_From_Last_Point() const noexcept;

    /**
     * @brief Pops the last time point and returns the time passed since that point.
     *
     * Removes the most recently saved time point from the stack and returns the
     * time elapsed since that point.
     *
     * @return std::chrono::milliseconds Time passed since the last point.
     */
    std::chrono::milliseconds Pop_Last_Point() noexcept;

private:
    /**
     * @brief The time point of the class construction.
     *
     * This is the initial time point, set when the `BenchMarkExecution` instance
     * is created. It is used to calculate the time from the start if no other
     * time points have been saved.
     */
    std::chrono::time_point<std::chrono::steady_clock> m_begin_time;

    /**
     * @brief Stack of saved time points for benchmarking.
     *
     * A stack of `std::chrono::time_point` objects that stores multiple time points.
     * Each time a time point is saved, it's added to this stack, and the time between
     * points can be measured by popping and comparing them.
     */
    std::stack<std::chrono::time_point<std::chrono::steady_clock>> m_time_segments;

};

inline BenchMarkExecution::BenchMarkExecution() noexcept
{
    m_begin_time = std::chrono::steady_clock::now();
    m_time_segments.emplace(std::chrono::steady_clock::now());
}

inline BenchMarkExecution::~BenchMarkExecution() noexcept
{
}

inline void BenchMarkExecution::Save_Time_Point() noexcept
{
    m_time_segments.emplace(std::chrono::steady_clock::now());
}

inline std::chrono::milliseconds BenchMarkExecution::Get_Time_From_Last_Point() const noexcept
{
    auto end_time = std::chrono::steady_clock::now();
    auto time_passed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_time_segments.top());
    return time_passed;
}

inline void BenchMarkExecution::Print_Time_From_Last_Point() const noexcept
{
    auto end_time = std::chrono::steady_clock::now();
    auto time_passed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_time_segments.top());
    std::cout << "Time passed is: " << time_passed.count() << "ms" << std::endl;
}

inline std::chrono::milliseconds BenchMarkExecution::Pop_Last_Point() noexcept
{
    auto end_time = std::chrono::steady_clock::now();
    auto time_passed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - m_time_segments.top());
    m_time_segments.pop();
    return time_passed;
}

