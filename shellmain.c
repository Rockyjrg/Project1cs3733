#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#define MAX_INPUT 1000
#define MAX_TOKENS 10

void chgDir(char *path, char *currDir);
void showpid(int idList[], int count);

int main()
{
    char *tokenArr;
    char *argumentList[MAX_TOKENS + 1];
    char str[MAX_INPUT];
    char currDir[MAX_INPUT];
    int idList[5];    // stores last 5 PIDs
    int pidIndex = 0; // keeps track of stored PIDs
    char *command;

    for (i = 0; i < 10; i++) // malloc a char* pointer with length 81 and initialize it to NULL
    {
        argumentList[i] = (char *)malloc(sizeof(char) * 81);
        argumentList[i] = NULL;
    }

    if ((getcwd(currDir, sizeof(currDir))) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    while (1)
    {
        // get user input using fgets
        printf("\033[0;31m%s$ ", currDir);
        printf("\033[0m");
        if (fgets(str, 1000, stdin) != NULL)
        {
            // code goes here
        }

        // use strtok to create tokens and count each token as one word
        tokenArr = strtok(str, " ");
        i = 0;
        while (tokenArr != NULL)
        {
            if (i == 0)
            {
                command = strdup(tokenArr);
            }
            argumentList[i] = strdup(tokenArr);
            tokenArr = strtok(NULL, " ");
            i++;
        }

        if (strcmp(command, "cd") == 0)
        {
            chgDir(argumentList[1], currDir);
        }
        else if (strcmp(command, "showpid") == 0)
        {
            showpid(idList);
        }

        if ((pid = fork()) == 0)
        {
            execStatus = execvp(command, argumentList);

            if (execStatus == -1)
            {
                printf("Error: Command could not be executed\n");
                exit(1);
            }
            // kill(pid, SIGTERM);
        }
        else
        {
            // parent process(shell program itself)
            waitpid(pid, status, 0);
        }

        if (strncmp(str, "exit", 4) == 0)
        {
            printf("EXITING!\n");
            break;
        }
    }
}

void chgDir(char *path, char *currDir)
{
    if (chdir(path) == -1)
    {
        printf("Error: cannot change directory\n");
        return;
    }
    if ((getcwd(currDir, 1000)) == NULL)
    {
        perror("getcwd()error");
    }
    setenv("pwd", currDir, 1);
}

void showpid(int idList[], int count)
{
    int i = 0;
    for (i = 0; i < 5; i++)
    {
        printf("%d\n", idList[i]);
    }
    return;
}