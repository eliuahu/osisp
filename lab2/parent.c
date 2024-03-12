#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

char** getEnvVar();
char* checkEnvp(char*envp[]);
char* checkEnviron();

int main(int argc, char*argv[], char*envp[]) {
    
    setenv("LC_COLLATE", "C", 1);
    char* CHILD_PATH = malloc(256);
    if(!getenv("CHILD_PATH"))
    {
        puts("Child path is not set!");
        exit(1);
    }

    getEnvVar();
   

    int numOfChildren = 0;
    
    int child_status;
    while(1)
    {
        if(numOfChildren >= 99) 
        {
            printf("\nToo many children!");
            return 0;
        }
        printf("Enter command ('+', '*', '&', 'q') : ");
        char command;
        scanf("%c", &command);
        while (getchar () != '\n') continue;
        if(command == 'q') break; 
        
            char child_num_str[3];
            sprintf(child_num_str, "%02d", numOfChildren);
            int child_name_len = strlen("child_") + strlen(child_num_str) + 1;
            char* child_name = (char*)malloc(child_name_len); 
            strcpy(child_name, "child_");
            strcat(child_name, child_num_str);

            char* args[] = {child_name, argv[1], NULL};
                 
            switch(command)
            {
            case '+': {numOfChildren++; strcpy(CHILD_PATH, getenv("CHILD_PATH"));
                break;}
            case '*': {numOfChildren++; strcpy(CHILD_PATH, checkEnvp(envp));
                break;}
            case '&': {numOfChildren++; strcpy(CHILD_PATH, checkEnviron());
                break;}
            default: {puts("Unknown command!"); continue;
                break;}
            }
            

        pid_t pid = fork();


        if (pid < 0) {
            printf("Error code: %d\n", errno); 
            exit(errno);
        } else if (pid == 0) {
            fprintf(stdout, "Child process created. wait...\n");
            execve(CHILD_PATH, args, envp);
        } else if(pid > 0) {
            wait(&child_status);
            fprintf(stdout, "Child process have ended with %d exit status\n", child_status);
        }   
    
    }
    free(CHILD_PATH);
    exit(0);
}

char* checkEnvp(char* envp[])
{
    int i = 0;
    while (envp[i])
    {
        if (strstr(envp[i], "CHILD_PATH"))
        {
            char* pTmpStr;
            pTmpStr = strstr(envp[i], "CHILD_PATH=");
            pTmpStr += strlen("CHILD_PATH=");

            char* result = (char*) malloc(strlen(pTmpStr) + 1);
            strcpy(result, pTmpStr);
            return result;
        }
        i++;
    }

    return NULL;
}

char* checkEnviron()
{
    extern char** environ;
    int i = 0;
    while (environ[i])
    {
        if (strstr(environ[i], "CHILD_PATH"))
        {
            char* pTmpStr;
            pTmpStr = strstr(environ[i], "CHILD_PATH=");
            pTmpStr += strlen("CHILD_PATH=");

            char* result = (char*) malloc(strlen(pTmpStr) + 1);
            strcpy(result, pTmpStr);
            return result;
        }
        i++;
    }

    return NULL;
}

char** getEnvVar()
{
    extern char **environ;
    int num_env_vars = 0;
    while (environ[num_env_vars] != NULL)
    {
        num_env_vars++;
    }

    char** env_vars = malloc(num_env_vars * sizeof(char*));
    for (int i = 0; i < num_env_vars; i++)
    {
        env_vars[i] = environ[i];
    }

    setenv("LC_COLLATE", "C", 1);
    qsort(env_vars, num_env_vars, sizeof(char*), (int (*)(const void*, const void*))strcmp);

    printf("ENV_VARS:\n");
    for (int i = 0; i < num_env_vars; i++)
    {
        printf("\t%s\n", env_vars[i]);
    }

    return env_vars;
}