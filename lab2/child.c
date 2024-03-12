#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>

char* checkEnvp(char* envp[], char* var_name);
void read_file(char* filename, char* envp[]);

int main(int argc, char *argv[], char*envp[]) 
{
    system("clear");
    printf("Child process %s started...\n", argv[0]);
    setenv("LC_COLLATE", "C", 1);
    printf("pid: %d\n", getpid());
    printf("ppid: %d\n", getppid()); 
    printf("env vars: \n");
    read_file(argv[1], envp);
    printf("\n");
    exit(0);
}

void read_file(char* filename, char* envp[]) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    char var_name[256];
    while (fgets(var_name, sizeof(var_name), fp)) {
        int len = strlen(var_name);
        if (len > 0 && var_name[len-1] == '\n') {
            var_name[len-1] = '\0';
        }

        char* var_value = getenv(var_name);
       

        if (var_value != NULL) {
            printf("\t%s=%s\n", var_name, var_value);
        } else if(!strcmp(var_name, "HOSTNAME"))
        {
            char hostname[256];
            gethostname(hostname, sizeof(hostname));
            printf("\t%s=%s\n", var_name, hostname);
        }
        else {
            printf("%s not found in environment\n", var_name);
        }
    }

    fclose(fp);
}

char* checkEnvp(char* envp[], char* var_name)
{
    int i = 0;
    while (envp[i])
    {
        if (strstr(envp[i], var_name))
        {
            char* pTmpStr;
            pTmpStr = strstr(envp[i], var_name);
            pTmpStr += strlen(var_name);

            char* result = (char*) malloc(strlen(pTmpStr) + 1);
            strcpy(result, pTmpStr);
            return result;
        }
        i++;
    }

    return NULL;
}