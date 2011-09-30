#include "utility.h"

void my_perror(char *perror_str, bool die)
{
    char msg[256]="* ";

    if (errno == 0)
        return;

    if (strlen(perror_str) > 128)
        return;

    strcat(msg, perror_str);
    strcat(msg, " ");


    perror(msg);

    if (die)
        exit(1);
}

void *my_malloc(int size)
{
    void *p = malloc(size);

    if (!p)
        my_perror("malloc()",1);

    return p;
}

void cleanTailSpaces(char *s) /* ORRIILE */
{
    int i;

    if (!s)
        return;

    while(1)
    {
        i=strlen(s)-1;
        if (i < 0)
            break;
        if (s[i] == ' ' || s[i] == '\t')
        {
            s[i]=0;
            continue;
        }
        break;
    }
}

void stepProgressBar(long i,int steps,long elements)
{
    long j,k,d=elements/steps;

    if (i>elements)
        i=elements;
    if (i<0)
        i=0;

    printf("[");
    for(j=0;j<=i/d;j++)
        printf("=");
    for(k=0;k<=(steps-j);k++)
        printf(" ");
    printf("] %d%%", (int)(100*i/elements));
    if (i!=elements)
        printf("\r");
    else
        printf("\n");

    fflush(stdout);
}

void num2human(long double n)
{
    int i=0;
    long pw=1;
    const char *lbl[] = {"","k","M","G","T","P","E","X","Y", 0};

    if (n<1)
        printf("%Lg",n);

    while(lbl[i]!=0)
    {
        if (n>=pw && n < pw*1024)
        {
            printf("%Lg %s",n/(long double)pw,lbl[i]);
            break;
        }
        else if (lbl[i+1] == 0)
        {
            printf("%Lg %s",n/(long double)pw,lbl[i]);
        }
        i++;
        pw*=1024;
    }
}

