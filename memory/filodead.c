#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <wait.h>

/* 5 filosofi con possibile deadlock */

#define NFILOSOFI 5

sem_t *sem;
sem_t count;
int shm_fd1, shm_fd2;

char *stato; /* puntera' ad un array di 5 caratteri in
                memoria condivisa che serve solo per illustrare
                all'utente lo stato dei 5 filosofi,
                T=thinking, H=hungry, E=eating */

void cleanup(int sig) {
    munmap(stato, NFILOSOFI * sizeof(char));
    close(shm_fd1);
    munmap(sem, NFILOSOFI * sizeof(sem_t));
    close(shm_fd2);
	sem_destroy(&count);
    exit(sig);
}

void print() { /* stampa lo stato dei filosofi */
    int i;
    for (i = 0; i < NFILOSOFI; i++) putchar(stato[i]);
    printf("\n");
}

void proc(int i) {
    int j, n;
    int m;

    m = 50000000 + 30000000 * i; /* tempo per cui usera' le forchette;
			i filosofi con i piu' alto sono piu' lenti a mangiare, questo
			differenzia un pochino il loro comportamento e su qualche sistema
          	favorisce il verificarsi del deadlock */

    for (n = 0; n < 10; n++) {
        printf("FILOSOFO %d E' AFFAMATO:       ", i);
        stato[i] = 'H';
        print();
		sem_wait(&count);
        sem_wait(&sem[i]);
        printf("FILOSOFO %d HA PRESO LA FORCHETTA %d \n", i, i);
        sem_wait(&(sem[(i + 1) % NFILOSOFI]));
        printf("FILOSOFO %d HA PRESO LA FORCHETTA %d \n", i, (i + 1) % NFILOSOFI);
        sem_post(&count);
		stato[i] = 'E';
        printf("FILOSOFO %d MANGIA:       ", i);
        print();
        for (j = 0; j < m; j++); /* ogni processo usa le risorse per un tempo diverso */
        stato[i] = 'T';
        printf("FILOSOFO %d SMETTE:       ", i);
        print();
        sem_post(&(sem[(i + 1) % NFILOSOFI]));
        sem_post(&(sem[i]));
    }
}

int main() {
    int i;
    pid_t pid;
    struct sigaction sa;

    shm_fd1 = shm_open("/myshm1", O_CREAT | O_RDWR, 0600);
    if (shm_fd1 == -1) 
		perror("Creazione memoria condivisa: stato");
    ftruncate(shm_fd1, NFILOSOFI * sizeof(sem_t));
    sem = mmap(0, NFILOSOFI * sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd1, 0);
    if (sem == MAP_FAILED) 
		perror("Creazione memoria condivisa: stato");

    shm_fd2 = shm_open("/myshm2", O_CREAT | O_RDWR, 0600);
    if (shm_fd2 == -1) 
		perror("Creazione memoria condivisa: semafori");
    ftruncate(shm_fd2, NFILOSOFI * sizeof(char));
    stato = mmap(0, NFILOSOFI * sizeof(char), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);
    if (stato == MAP_FAILED) 
		perror("Creazione memoria condivisa: semafori");

	sem_init(&count, 1, NFILOSOFI - 1);

    for (i = 0; i < NFILOSOFI; i++) {
        sem_init(&sem[i], 1, 1);
        stato[i] = 'T';
    }

    for (i = 0; i < NFILOSOFI; i++) {
        if (fork() == 0) {
            proc(i);
            exit(0);
        }
    }

    sa.sa_handler = cleanup;
    sigaction(SIGINT, &sa, NULL);
    /* se vanno in deadlock, bisogna interrompere tutto,
        il processo padre associa cleanup a SIGINT per rimuovere
        semafori e memoria condivisa in questo caso */

    for (i = 0; i < NFILOSOFI; i++) {
        pid = wait(0);
        printf("Terminato processo %d\n", pid);
    }
    cleanup(0);
}
