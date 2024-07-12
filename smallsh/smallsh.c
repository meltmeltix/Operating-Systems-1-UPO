#include "smallsh.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

char *prompt = "Scrivere un comando>";

void procline(void) {
	char *arg[MAXARG + 1];
    int toktype;
    int narg;
	int background;

    narg = 0;
	background = 0;

    do {
        switch (toktype = gettok(&arg[narg])) {
            case ARG:
                if (narg < MAXARG) narg++;
                break;

            case EOL:
            case SEMICOLON:
				if (toktype == BACKGROUND) { background = 1; }

                if (narg != 0) {
                    arg[narg] = NULL;
                    runcommand(arg, background);
                }

                if (toktype != EOL) narg = 0;
				if (toktype == BACKGROUND) { background = 0; }

                break;
        }
    } while (toktype != EOL);
}

void runcommand(char **cline, int background) {
    pid_t pid;
    int exitstat, ret;

    pid = fork();
    if (pid == (pid_t)-1) {
        perror("smallsh: fork fallita");
        return;
    }

    if (pid == (pid_t)0) {
        execvp(*cline, cline);
        perror(*cline);
        exit(1);
    }

	if (!background) {
		ret = wait(&exitstat);
		if (ret == -1) perror("wait");
	} else {
		printf("Processo %d in esecuzione in background\n", pid);
	}
}

int main() {
    while (userin(prompt) != EOF) procline();
    return 0;
}