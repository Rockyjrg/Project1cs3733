#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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

    // for (i = 0; i < 10; i++) // malloc a char* pointer with length 81 and initialize it to NULL
    // {
    //     argumentList[i] = (char *)malloc(sizeof(char) * 81);
    //     argumentList[i] = NULL;
    // }

    if ((getcwd(currDir, sizeof(currDir))) == NULL)
    {
        perror("getcwd() error");
        return 1;
    }

    while (1)
    {
        // display prompt
        printf("\033[0;31m%s$ \033[0m", currDir);
        fflush(stdout);

        // get user input
        if (fgets(str, sizeof(str), stdin) == NULL)
        {
            perror("fgets() error");
            continue;
        }

        // remove newline character from input
        str[strcspn(str, "\n")] = '\0';

        // check if user wants to exit
        if (strncmp(str, "exit", 4) == 0)
        {
            printf("EXITING!\n");
            break;
        }

        // use strtok to create tokens and count each token as one word
        int i = 0;
        tokenArr = strtok(str, " ");
        while (tokenArr != NULL && i < MAX_TOKENS)
        {
            // if (i == 0)
            // {
            //     command = strdup(tokenArr);
            // }
            argumentList[i] = strdup(tokenArr);
            tokenArr = strtok(NULL, " ");
            i++;
        }
        argumentList[i] = NULL; // null terminate the array for execvp()

        if (i == 0)
            continue;

        command = argumentList[0];

        if (strcmp(command, "cd") == 0)
        {
            if (i > 1)
            {
                chgDir(argumentList[1], currDir);
            }
            else
            {
                printf("cd: missing argument\n");
            }
            continue;
        }
        else if (strcmp(command, "showpid") == 0)
        {
            showpid(idList, pidIndex);
            continue;
        }

        // forking process for external commands
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork() failed");
            continue;
        }
        else if (pid == 0) // child process
        {
            if (execvp(command, argumentList) == -1)
            {
                printf("Error: Command could not be executed\n");
                exit(1);
            }
        }
        else // parent process
        {
            int status;
            waitpid(pid, &status, 0);

            // store last 5 child PIDs
            if (pidIndex < 5)
            {
                idList[pidIndex++] = pid;
            }
            else
            {
                // shift array left and store the new PID
                for (int j = 1; j < 5; j++)
                {
                    idList[j - 1] = idList[j];
                }
                idList[4] = pid;
            }

            // free allocated memory
            for (int j = 0; j < i; j++)
            {
                free(argumentList[j]);
            }
        }
    }
    return 0;
}

void chgDir(char *path, char *currDir)
{
    if (chdir(path) == -1)
    {
        perror("getcwd() error");
        return;
    }
    if ((getcwd(currDir, 1000)) == NULL)
    {
        perror("getcwd()error");
    }
    setenv("PWD", currDir, 1);
}

void showpid(int idList[], int count)
{
    // int i = 0;
    // for (i = 0; i < 5; i++)
    // {
    //     printf("%d\n", idList[i]);
    // }
    // return;

    printf("Last %d PIDs:\n", count < 5 ? count : 5);
    for (int i = 0; i < count; i++)
    {
        printf("%d\n", idList[i]);
    }
}