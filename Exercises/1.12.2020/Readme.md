# TASK
Write a program that uses a thread to calculate the approximation of the number π.
- The main program creates some threads (for example 2) that will count
partial approximations of the number π by the Monte Carlo method.
- Each thread in the loop does this:  
1. make N attempts to shoot in a circle and remember how many times it is
 inside (Z).  
2. Add the numbers N and Z to the sum of Ns and Zs (global
variables). In doing so, use a lock (mutex) so
Ns and Zs do not corrupt due to simultaneous access of multiple threads.  
3. When the thread updates Ns and Zs, it calculates the current approximation π ≈ 4 * Zs / Ns.  
4. If the number of Ns attempts has already exceeded a predetermined maximum
number of attempts, the thread ends.  
- The main program then combines all the threads and destroys the mutex and
displays the calculated approximation of the number π.
