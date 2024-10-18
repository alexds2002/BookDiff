<>======================================================< HOW TO BUILD >======================================================<>
                                                                                                                              <>
Minimum requirements:                                                                                                         <>
cmake version 3.16                                                                                                            <>
gcc 9.4.0                                                                                                                     <>
                                                                                                                              <>
How to build:                                                                                                                 <>
mkdir build && cmake -G "Unix Makefiles" .. && make -j4                                                                       <>
                                                                                                                              <>
><======================================================< HOW TO BUILD >======================================================><

<>======================================================< NOTES >======================================================<>
                                                                                                                       <>
TODO after:                                                                                                            <>
Unit testing                                                                                                           <>
Add sanitaizer(use static and dynamic tools)                                                                           <>
                                                                                                                       <>
My Assumptions:                                                                                                        <>
1. The events(Book diff) from the Binance book dept stream are in a specific format.                                   <>
2. I will need to track both bid and ask prices, with each side being ordered.                                         <>
3. The updates are absolute values, so the new update is the new value i need to place                                 <>
4. The snapshot contains the Symbol, otherwise we have to get it from somewhere else                                   <>
*                                                                                                                      <>
The solution should maintain the two sides of the book in order.                                                       <>
The solution should scale to larger books than the sample data shows.                                                  <>
The solution should maintain a ‘best price’ for each side (bids & asks).                                               <>
*                                                                                                                      <>
                                                                                                                       <>
Tracking the prices with unordered_map would give constant insertion and deletion but would add more complexity for    <>
tracking the max/min elements. On top of that the data will not be sorted. For that reason i will use std::map.        <>
><======================================================< NOTES >======================================================><

<>======================================================< KEY REQUIREMENTS >======================================================<>
                                                                                                                                  <>
Maintain Two Sides of the Book (Bids and Asks):                                                                                   <>
You'll need to track both bid and ask prices, with each side being ordered.                                                       <>
Efficient Updates:                                                                                                                <>
Price levels should be added, updated, or removed based on incoming events.                                                       <>
Need to efficiently handle updates like setting quantities to zero, which removes the price point.                                <>
Best Price Tracking:                                                                                                              <>
Constantly track and maintain the best bid (highest bid price) and best ask (lowest ask price).                                   <>
Scalability:                                                                                                                      <>
Support a large number of books (1000-2000 symbols).                                                                              <>
Test Suite & Benchmarking:                                                                                                        <>
Write tests to ensure the solution is correct.                                                                                    <>
Implement benchmarks to measure performance (latency and memory usage).                                                           <>
                                                                                                                                  <>
><======================================================< KEY REQUIREMENTS >======================================================><

<>======================================================< OBJECTIVE >======================================================<>
                                                                                                                           <>
Optimized Price Point Book                                                                                                 <>
Implement a data structure capable of handling events from the Binance book dept stream:                                   <>
https://binance-docs.github.io/apidocs/spot/en/#diff-depth-stream.                                                         <>
                                                                                                                           <>
Success metrics                                                                                                            <>
Please complete the task in C, C++, C# or Java.                                                                            <>
A solution will handle events correctly; price point levels should be added, removed or adjusted accordingly.              <>
The code should be as clear and maintainable as you would like to find code in a high quality production code-base.        <>
The code should be highly optimized for latency and memory usage.                                                          <>
Memory usage should be considerate of the fact that there may be 1000-2000                                                 <>
such price point books in use in a single process.                                                                         <>
The code should have an appropriate test suite for correctness.                                                            <>
The code should have a benchmark suite demonstrating how you would measure its performance and memory usage.               <>
Like the correctness test suite, this should be easy to run in a reproducible manner.                                      <>
Assumptions                                                                                                                <>
The following assumptions can be made when designing the software:                                                         <>
You will receive a snapshot, followed by consecutive book diff updates.                                                    <>
There will be no gaps in the book diff sequence numbers.                                                                   <>
The snapshots and book diff events will be for a range of symbols, with a total of 1000-2000 distinct symbols.             <>
Requirements                                                                                                               <>
The solution should maintain the two sides of the book in order.                                                           <>
The solution should scale to larger books than the sample data shows.                                                      <>
The solution should maintain a ‘best price’ for each side (bids & asks).                                                   <>
Out of Scope(Things not to do)                                                                                             <>
The following are out of scope:                                                                                            <>
Live data connection.                                                                                                      <>
Snapshot synchronisation - see Assumptions.                                                                                <>
Any form of user input.                                                                                                    <>
Sample Data                                                                                                                <>
Snapshot:                                                                                                                  <>
                                                                                                                           <>
{                                                                                                                          <>
                                                                                                                           <>
"bids" : [                                                                                                                 <>
 ["0.0024","14.70000000"],                                                                                                 <>
 ["0.0022","6.40000000"],                                                                                                  <>
                                                                                                                           <>
["0.0020","9.70000000"]                                                                                                    <>
 ]                                                                                                                         <>
 "asks" : [                                                                                                                <>
 ["0.0024","14.90000000"],                                                                                                 <>
 ["0.0026","3.60000000"],                                                                                                  <>
                                                                                                                           <>
["0.0028","1.00000000"]                                                                                                    <>
                                                                                                                           <>
                                                                                                                           <>
 ]                                                                                                                         <>
}                                                                                                                          <>
Book diff:                                                                                                                 <>
Update 1:                                                                                                                  <>
{ "e": "depthUpdate", "s": "BNBBTC", "b": [ ["0.0024","10"] ], "a": [ ["0.0026","100"] ] }                                 <>
                                                                                                                           <>
Update 2:                                                                                                                  <>
{ "e": "depthUpdate", "s": "BNBBTC", "b": [ ["0.0024","8"] ], "a": [ ["0.0028","0"] ] }                                    <>
                                                                                                                           <>
Update 3:                                                                                                                  <>
{ "e": "depthUpdate", "s": "BNBBTC", "b": [ ["0.0024","0"] ], "a": [ ["0.0026","15"],["0.0027","5"] ] }                    <>
                                                                                                                           <>
Update 4:                                                                                                                  <>
{ "e": "depthUpdate", "s": "BNBBTC", "b": [ ["0.0025","100"] ], "a": [ ["0.0026","0"],["0.0027","5"] ] }                   <>
                                                                                                                           <>
Update 5:                                                                                                                  <>
{ "e": "depthUpdate", "s": "BNBBTC", "b": [ ["0.0025","0"] ], "a": [ ["0.0026","15"],["0.0024","0"] ] }                    <>
                                                                                                                           <>
><======================================================< OBJECTIVE >======================================================><
