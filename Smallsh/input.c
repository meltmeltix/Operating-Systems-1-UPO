#include "smallsh.h"

static char inpbuf[MAXBUF], tokbuf[2 * MAXBUF], *ptr, *tok;

static char special[] = {' ', '\t', ';', '\n', '\0', '&'};

int userin(char *p) {
    int c, count;

    ptr = inpbuf;
    tok = tokbuf;

    printf("%s ", p);

    count = 0;

    while (1) {
        if ((c = getchar()) == EOF) return (EOF);

        if (count < MAXBUF) inpbuf[count++] = c;

        if (c == '\n' && count < MAXBUF) {
            inpbuf[count] = '\0';
            return (count);
        }

        if (c == '\n') {
            printf("riga in input troppo lunga\n");
            count = 0;
            printf("%s ", p);
        }
    }
}

int gettok(char **outptr) { 
    int type;

    *outptr = tok;

    while (*ptr == ' ' || *ptr == '\t') ptr++;

    *tok++ = *ptr;

    switch (*ptr++) {
        case '\n':
            type = EOL;
            break;
        case ';':
            type = SEMICOLON;
            break;
        case '&':
			type = BACKGROUND; 
            break;
        default:
            type = ARG;
            while (inarg(*ptr)) *tok++ = *ptr++;
    }

    *tok++ = '\0';
    return (type);
}

int inarg(char c) {
    char *wrk;

    for (wrk = special; *wrk != '\0'; wrk++)
        if (c == *wrk) return (0);

    return (1);
}