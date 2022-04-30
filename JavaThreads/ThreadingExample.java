/**
 * Threading example in Java for SER 334.
 * @author Joseph Luensmann
 * @version 1.0
 */

public class ThreadingExample {
    private static final int THREAD_COUNT = 4;

    public static void main(String[] args) {
        // declare an integer array
        Integer[] num = new Integer[THREAD_COUNT];
        // fill it with multiples of 5
        for(int i = 0; i < THREAD_COUNT; i++) {
            num[i] = i * 5;
        }
        /**  
         * In Java, methods can either implement the Runnable interface, or they
         * can extend Thread.  This one implements Runnable, which means the threads
         * are launched in the Run method.
         *
         * Here we initialize three arrays of size THREAD_COUNT.  To keep your programs
         * flexible, it is always recommended to define constants instead of hardcoded values.
         */
        Count[] threads1 = new Count[THREAD_COUNT];
        Count[] threads2 = new Count[THREAD_COUNT];
        Count[] threads3 = new Count[THREAD_COUNT];
        
        /**
         * This loop is for loading data into the threads.  All it does is loop through the
         * number array above where the multiples of 5 are generated and copy the numbers into 
         * our arrays of Count objects.  The Count parameter will take that integer and store it
         * as a class-level variable.
         */
        for(int i = 0; i < THREAD_COUNT; i++) {
            threads1[i] = new Count(num[i]);
            threads2[i] = new Count(num[i]);
            threads3[i] = new Count(num[i]);
        }

        /**
         * Now we're actually running threads.
         * This first version slows down the execution and pulls the threads apart.
         * Instead of running concurrently, it waits for each thread to finish execution
         * before the next one starts with the Thread's isAlive() method, which 
         * returns true while the thread is running.
         * It isn't true concurrency because the threads aren't executing "simultaneously,"
         * but is a useful tool for debugging when it becomes hard to trace execution behavior
         * on multiple concurrently executing threads.
         */
        System.out.println("First set of numbers");
        for(int i = 0; i < THREAD_COUNT; i++) {
            threads1[i].start();
            while(threads1[i].isAlive());
        }
        
        /**
         * This second version starts running all the threads at once.
         * Try setting a breakpoint here and tracing execution once the threads start.
         * How hard is it to try and figure out what's going on in each thread?
         * What order do you think the numbers will print out?
         */
        System.out.println("Second set of numbers");
        for(int i = 0; i < THREAD_COUNT; i++) {
            threads2[i].start();
        }
        
        /**
         * The third version starts all the threads, then waits for them in sequence.
         * The goal is to get the numbers back in a specific order; when this prints out,
         * it should print 0-19 in sequential order.  Does it work?  Why or why not?
         */
        System.out.println("Third set of numbers");
        for(int i = 0; i < THREAD_COUNT; i++) {
            threads3[i].start();
        }
        while(threads3[0].isAlive());
        while(threads3[1].isAlive());
        while(threads3[2].isAlive());
        while(threads3[3].isAlive());
    }
}