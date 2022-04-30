/**
 * @file threads.c
 * @author Joseph Luensmann (jluensma@asu.edu)
 * @brief A short example to show threading syntax in C and demonstrate some of its issues.
 * @version 2.0
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/////////////////////////////////////////////////////////////
// INCLUDE STATEMENTS
#include <pthread.h> // we need pthread.h for, well, threading
#include <stdio.h>   // standard IO, this is printf()
#include <stdlib.h>  // when we allocate and deallocate memory that comes from the standard library
/////////////////////////////////////////////////////////////
// MACRO DEFINITIONS
#define THREAD_COUNT 4 // this is how we define a macro for the number of threads.  Might be useful...
/////////////////////////////////////////////////////////////
// STRUCTS
struct thread_info {
    pthread_t thread;
    int num;
} typedef thread_info;
/////////////////////////////////////////////////////////////
// FUNCTION PROTOTYPING
void * count(void* data);
/////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
/**
 * @brief Main function launches the program
 * 
 * @param argc the first argument passed through the command line
 * @param argv an array holding the rest of the arguments
 * @return int status code.  0 is normal exit, otherwise error
 */
int main(int argc, char* argv[]) {
    // We'll do something easy, just print out a bunch of numbers, but we'll do it a couple different ways
    // and explore some concepts.
    
    // First, let's make some room for an array of integers.
    // We'll pass these into each thread to help know which thread should print what data.
    int* threadNum = (int*)malloc(THREAD_COUNT * sizeof(int));
    // then intialize the array with multiples of 5
    for(int i = 0; i < THREAD_COUNT; i++) {
        threadNum[i] = i * 5;
    }
    // Here we declare some pthreads our method will use to actually launch the threads.
    pthread_t t1, t2, t3, t4;
    printf("Is this really concurrent?\n--------------------\n");

    /* 
    Now we're creating threads with pthread_create().
    The first argument is a reference to the thread, the pthread_t variables declared above.
    Notice we don't initialize them with anything -- the pthread_create function takes care of that.
    
    The second argument is for thread attributes.  These are very advanced and fiddly, so
    we don't really need to worry about them except that they should all be default.  Passing
    in a value of NULL just initializes the thread with default values.
    
    The third argument is the method that the thread should run when it is created.
    When defining this method, if there is no return value, declare it like this:

    void * method(void* data)

    Which indicates to the C compiler that you're not expecting anything back.  If you just leave it as

    void method(void* data)

    You'll get warnings when you pass it into pthread_create, either from the compiler or your IDE's static analysis tools
    that the call is formatted wrong and it will suggest this: void *(*)(void *) method
    
    This is a function pointer to a function that returns void and takes void*, which pops up because the method you're actually
    trying to call doesn't actually return a void *.  So a small change makes for simpler syntax and fewer warnings.

    The last argument is a pointer to a piece of data to pass into the function.

    It might help to visualize this as
    count(&threadNum[0])
    to think about what's really happening.  But because we can pass anything in here, it appears in
    the method itself as a void* -- it will work if you know what kind of data it is and attempt to use it,
    but compilers and people reading your code would prefer you cast it to the kind of data you actually use.
    Otherwise the type of data you're actually receiving will seem like a mystery and manipulating it seems like magic.
    Not good for anyone who has to maintain your code.
     */ 

    /* 
    Let's dig into thread creation.  This version of this method pulls the threads of execution apart
    so they're not actually running concurrently.  This is for two reasons: first, it's an example of what's happening
    when you're not actually running your threads together and what that looks like.  Second, this is a useful debugging
    tool to slow things down and see how each thread is acting on its data set to detect odd behavior.
    */
    pthread_create(&t1, NULL, count, &threadNum[0]);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL, count, &threadNum[1]);
    pthread_join(t2, NULL);
    pthread_create(&t3, NULL, count, &threadNum[2]);
    pthread_join(t3, NULL);
    pthread_create(&t4, NULL, count, &threadNum[3]);
    pthread_join(t4, NULL);
    printf("\nDynamically created threads.\n--------------------\n");
    // then we head into the next code block to do it again
    // Manually creating a block of threads is pretty clunky.  Let's explore how to do it dynamically.
    // Again, we're allocating an array, but this time the array is of type pthread_t, so
    // these will all be pointers to thread variables.
    pthread_t* threadArr = (pthread_t*)malloc(THREAD_COUNT * sizeof(pthread_t));
    for(int i = 0; i < THREAD_COUNT; i++) {
        // Note that we didn't initialize these.  Are we going to get segmentation faults?  Why or why not?
        // As an exercise, set a breakpoint here on this thread creation line and try to trace the flow
        // of execution.  Can you tell what's going on in each thread?  Can you see why debugging is a threading issue?
        pthread_create(&threadArr[i], NULL, count, &threadNum[i]);
    }
    // Now we've dynamically allocated these pthreads, let's dynamically join them.
    for(int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threadArr[i], NULL);
    }
    printf("\nThreads created by a custom datatype.\n--------------------\n");
    // We've explored two approaches to creating threads, now let's try a third.
    // for this, we'll use a new struct.

    /*
    This approach stores its needed data inside of a struct so that it's available during execution.
    If you scroll up and look at how the struct is written, it's holding its own thread data inside of
    the thread field.  Now we don't have to worry about keeping track of a list of running threads,
    just one array of data structures, making it much more flexible and dynamic.
    */ 
    thread_info* threadStruct = (thread_info*)malloc(THREAD_COUNT * sizeof(struct thread_info));
    for(int i = 0; i < THREAD_COUNT; i++) {
        threadStruct[i].num = i * 5; // this is familiar, we're loading the number fields with multiples of 5
    }
    for(int i = 0; i < THREAD_COUNT; i++) {
        // now to create the thread, we just point at the .thread field of each element,
        // and the data passed through is held in the .num field.
        // Consider: Could you send a reference to the entire threadStruct struct, holding
        // both the thread and number data?  Or the whole array holding all the structs?
        // Why or why not?  Try it, play around with it, see if it works.
        pthread_create(&threadStruct[i].thread, NULL, count, &threadStruct[i].num);
    }
    for(int i = 0; i < THREAD_COUNT; i++) {
        // The last step is to make sure all the threads have finished executing before cleaning up.
        // If we clean up without waiting we might interrupt the flow of execution and lose data,
        // or worse, get a segmentation fault.
        pthread_join(threadStruct[i].thread, NULL);
    }
    free(threadNum); // always free your memory!  no leaks!
    threadNum = NULL; // and set the pointers to null to remove the dangling reference
    free(threadArr);
    threadArr = NULL;
    free(threadStruct);
    threadStruct = NULL;
    return 0;
}

/**
 * @brief Counts numbers according to a passed data value.
 * 
 * @param data A starting point to count some numbers
 */
void * count(void* data) {
    // now we'll cast the memory reference received from the thread's creation
    // notice there's no direct function call here, it's all through pthread_create
    int* num = (int*)data;  
    for(int i = *num; i < *num+5; i++) { 
        // we know there's a multiple of 5 in here -- notice that this is dereferenced.  What will happen if we don't?
        printf("%d ", i); // and here we just print the numbers that are missing
    }
    printf("\n"); // newline character for readability
}

// Now, we put an int* in here, but there's no reason it couldn't be a char* or any other data type.
// But what if you want to pass in more than one value?  Is there a way you could do that?