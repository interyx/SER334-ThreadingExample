/**
 * @file threads.c
 * @author Joseph Luensmann (jluensma@asu.edu)
 * @brief A short example to show threading syntax in C and demonstrate some of its issues.
 * @version 1.0
 * @date 2022-04-05
 * 
 * @copyright Copyright (c) 2022
 * 
 */

/////////////////////////////////////////////////////////////
// INCLUDE STATEMENTS
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////////
// MACRO DEFINITIONS
#define THREAD_COUNT 4
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
    int* threadNum = (int*)malloc(THREAD_COUNT * sizeof(int));
    for(int i = 0; i < THREAD_COUNT; i++) {
        threadNum[i] = i * 5;
    }
    pthread_t t1, t2, t3, t4;
    printf("Is this really concurrent?\n");
    pthread_create(&t1, NULL, (void *(*)(void*)) count, &threadNum[0]);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL, (void *(*)(void*)) count, &threadNum[1]);
    pthread_join(t2, NULL);
    pthread_create(&t3, NULL, (void *(*)(void*)) count, &threadNum[2]);
    pthread_join(t3, NULL);
    pthread_create(&t4, NULL, (void *(*)(void*)) count, &threadNum[3]);
    pthread_join(t4, NULL);
    printf("Let's try that again.\n");
    pthread_create(&t1, NULL, (void *(*)(void*)) count, &threadNum[0]);
    pthread_create(&t2, NULL, (void *(*)(void*)) count, &threadNum[1]);
    pthread_create(&t3, NULL, (void *(*)(void*)) count, &threadNum[2]);
    pthread_create(&t4, NULL, (void *(*)(void*)) count, &threadNum[3]);
    printf("But what if we want them in a specific order?\n");
    pthread_create(&t1, NULL, (void *(*)(void*)) count, &threadNum[0]);
    pthread_create(&t2, NULL, (void *(*)(void*)) count, &threadNum[1]);
    pthread_create(&t3, NULL, (void *(*)(void*)) count, &threadNum[2]);
    pthread_create(&t4, NULL, (void *(*)(void*)) count, &threadNum[3]);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    printf("Did you catch what happened and why?");
    free(threadNum);
    return 0;
}

/**
 * @brief Counts numbers according to a passed data value.
 * 
 * @param data A starting point to count some numbers
 */
void count(void* data) {
    int* num = (int*)data;
    for(int i = *num; i < *num+5; i++) {
        printf("%d\n", i);
    }
}