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
void count(void* data);
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
    // and explore some concepts
    // let's make some room for an array of integers
    // we'll pass these into each thread to help know which thread should print what data
    int* threadNum = (int*)malloc(THREAD_COUNT * sizeof(int));
    // then intialize the array with multiples of 5
    for(int i = 0; i < THREAD_COUNT; i++) {
        threadNum[i] = i * 5;
    }
    // here we declare some pthreads our method will use to actually launch the threads
    pthread_t t1, t2, t3, t4;
    printf("Is this really concurrent?\n--------------------\n");
    /* Now we're creating threads with pthread_create().
    The first argument is a reference to the thread, the pthread_t variables declared above.
    Notice we don't initialize them with anything -- the pthread_create function takes care of that.
    
    The second argument is for thread attributes.  These are very advanced and fiddly, so
    we don't really need to worry about them except that they should all be default.  Passing
    in a value of NULL just initializes the thread with default values.
    
    The third argument is the method that the thread should run when it is created.
    The pointer cast looks kind of crazy, but if you break it down:
    void* : the return value is a void pointer
    (*): a pointer to a function
    (void *): which takes an untyped pointer as a parameter

    The last argument is a pointer to a piece of data to pass into the function.

    It might help to visualize this as
    count(&threadNum[0])
    to think about what's really happening.  But because we can pass anything in here, it appears in
    the method itself as a void* -- it will work if you know what kind of data it is and attempt to use it,
    but compilers and people reading your code would prefer you cast it to the kind of data you actually use.
    Otherwise the type of data you're actually receiving will seem like a mystery and manipulating it seems like magic.
    Not good for anyone who has to maintain your code.
     */ 
    pthread_create(&t1, NULL, (void *(*)(void*)) count, &threadNum[0]);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL, (void *(*)(void*)) count, &threadNum[1]);
    pthread_join(t2, NULL);
    pthread_create(&t3, NULL, (void *(*)(void*)) count, &threadNum[2]);
    pthread_join(t3, NULL);
    pthread_create(&t4, NULL, (void *(*)(void*)) count, &threadNum[3]);
    pthread_join(t4, NULL);
    // The above block of code starts four threads and waits for them to join.  Is this really concurrency?
    printf("\nDynamically created threads.\n--------------------\n");
    // then we head into the next code block to do it again
    // Manually creating a block of threads is pretty clunky.  Let's explore how to do it dynamically.
    // Again, we're allocating an array, but this time the array is of type pthread_t, so
    // these will all be pointers to thread variables.
    pthread_t* threadArr = (pthread_t*)malloc(THREAD_COUNT * sizeof(pthread_t));
    for(int i = 0; i < THREAD_COUNT; i++) {
        // Note that we didn't initialize these.  Are we going to get segmentation faults?  Why or why not?
        pthread_create(&threadArr[i], NULL, (void *(*)(void *)) count, &threadNum[i]);
    }
    // well, we've dynamically allocated these pthreads, let's dynamically join them.
    for(int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threadArr[i], NULL);
    }
    printf("\nThreads created by a custom datatype.\n--------------------\n");
    // We've explored two approaches to creating threads, now let's try a third.
    // for this, we'll use a new struct.
    thread_info* threadStruct = (thread_info*)malloc(THREAD_COUNT * sizeof(struct thread_info));
    for(int i = 0; i < THREAD_COUNT; i++) {
        threadStruct[i].num = i * 5;
    }
    for(int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threadStruct[i].thread, NULL, (void *(*) (void *)) count, &threadStruct[i].num);
    }
    // for(int i = 0; i < THREAD_COUNT; i++) {
    //     pthread_join(threadStruct[i].thread, NULL);
    // }
    free(threadNum); // always free your memory!  no leaks!
    threadNum = NULL;
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
void count(void* data) {
    // now we'll cast the memory reference received from the thread's creation
    // notice there's no direct function call here, it's all through pthread_create
    int* num = (int*)data;  
    for(int i = *num; i < *num+5; i++) { 
        // we know there's a multiple of 5 in here -- notice that this is dereferenced.  What will happen if we don't?
        printf("%d ", i); // and here we just print the numbers that are missing
    }
    printf("\n"); // newline character for readability
}

// now, we put an int* in here, but there's no reason it couldn't be a char* or any other data type
// but what if you want to pass in more than one value?  Hmmmm....