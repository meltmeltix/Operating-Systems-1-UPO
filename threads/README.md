# Threads with semaphores

- [Threads with semaphores](#threads-with-semaphores)
  - [The program](#the-program)
    - [Setting up](#setting-up)
    - [Creation of consumer/producer](#creation-of-consumerproducer)
    - [What are threads doing?](#what-are-threads-doing)
      - [Producer](#producer)
      - [Consumer](#consumer)
    - [After the threads](#after-the-threads)
    - [TLDR](#tldr)

## The program

### Setting up

First, the program sets up a `c` array of `NPROD` and a thread array `t`.

Three semaphores gets initialized:

- `mutex`, not shared, of value 1;
- `empty`, not shared, of value `N`, currently set to 5
- `full`, not shared, of value 0;

The program then checks for arguments, looking for a filename.
Then, it initializes the buffer struct by setting in, out and count to 0.

### Creation of consumer/producer

Begins by flushing the out buffer.

Then, by looping from 0 to `NPROD`, starts adding values to `c` while intializing each thread within the array of threads as producer (passing the `producer` method), passing the values added to `c` as argument.

After the loop it initializes the last thread in the array as consumer (passing the `consumer` method), passing the first program argument (the filename) as argument.

### What are threads doing?

#### Producer

Method that repeats `NGIRI` times a "printing" operation. It receives as parameter the number of the `c` array.

In each loop, there is another loop used to slow things down.
After that, waits (down) both `empty` and `mutex` semaphores. It then puts `p` in the buffer using `put`.

`put` adds to the `pool` the character, assigns it to `in` and increases the `count`. Then it prints the buffer using `printstatus`, after which it prints the new added character. It then flushes the output buffer.

The producer then posts (up) the `mutex` and `full` semaphores, returning then null.

#### Consumer

Copies the buffer and inserts it into the file with name `filename`. The consumer opens the file, and then loops through the buffer until it stumbles upon a `\0`.

During this process, it waits (down) `full` and `mutex`, gets the character from the buffer using `get`, which behaves similarly to `put`, but it assigns to `c` (`ip` as parameter) the value in the struct `out`, decreasing the counter.

It then posts `mutex` and `empty`, writing after the character to the file, returning then null.

### After the threads

The main process waits for all the threads to finish, terminating then the program.

### TLDR

This program is a simple back and forth between threads where the main objective is to print characters to a file. Using semaphores, and letting each thread work independently, it's possible to let them pass each value correctly.