# SER334-ThreadingExample
A simple example of a program involving multiple threads of execution to demonstrate the funamentals.

USAGE
From this directory, run this command to compile:
> gcc threads.c -o thread -lpthread

And to run:
> ./thread

BEHAVIOR
This program initializes some numbers and passes them into a threaded function to be printed.

While the code is well formatted, it includes some logical errors that make the code act in certain undesirable ways.

In addition, there are several enhancements we could think about like creating a function to launch all of the threads and be able to dynamically generate the pthreads.  Also look at how the data is passed into the thread and think about different ways to be able to pass more parameters into this function that might be needed if it was more complicated.
