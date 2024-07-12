# Deadlock

## The program

### Setting up

This program uses signals during it's process. That action is represented by `sa`.

The program assigns to `shm_fd1` the shared memory segment with name `/myshm1`, `O_CREAT | O_RDWR` flags and `0600` mode.

With `ftruncate`, takes the memory segment `shm_fd1` and sets it's size to `NFILOSOFI` (number of philosophers) times the size of a semaphore `sem_t`.

>This works because `ftruncate` takes and returns and int.

After this, the a virtal map inside the first memory segment gets mapped with the same size as before.

This happens again for `shm_fd2`.

> **What is going on?**  
> We are creating two memory segments:
>
> - The first, holds the state of each philosopher
> - The second, holds the semaphore of each philosopher

### Assigning Values

An additional semaphor gets initialized, taking care of counting the amount of philosophers.

The program then loops through all the semaphores contained in `sem`, initializing them, and setting the state to `T`.

New processes then get initialized, making them all run `proc` method, passing `i` as parameter, used as index to access each pihlosopher calculating the correct offset.

After passing the cleanup method to `sa`, the program then sets the singal action to interactive signal `SIGINT`.

Then, loops, waiting for each child to die, cleaning up right after.