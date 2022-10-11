#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MYSH_TOK_DELIM " \t\r\n"
#define MYSH_TOK_BUFFER_SIZE 64

int mysh_cd(char **args);
int mysh_help(char **args);
int mysh_exit(char **args);

int mysh_builtin_nums();

char * builtin_cmd[] =
{
    "cd",
    "help",
    "exit"
};

int (*builtin_func[])(char **) = 
{
    &mysh_cd,
    &mysh_help,
    &mysh_exit
};

int mysh_cd(char **args)
{
    if(args[1] == NULL) 
    {
        perror("Mysh error at cd, lack of args\n");
    }
    // cd 
    else
    {
        if(chdir(args[1]) != 0)
            perror("Mysh error at chdir\n");
    }
    return 1;
}

int mysh_help(char **args)
{
    puts("This is Mt's shell");
    puts("Here are some built in cmd:");
    for (int i = 0; i < mysh_builtin_nums(); i ++)
        printf("%s\n", builtin_cmd[i]);
    return 1;
}

int mysh_exit(char **args)
{
    return 0;
}

int mysh_builtin_nums()
{
    return sizeof(builtin_cmd) / sizeof(builtin_cmd[0]);
}

char * mysh_read_line()
{
    char * line = NULL;
    ssize_t bufsize;
    getline(&line, &bufsize, stdin);
    return line;
}

//parsing 


char ** mysh_split_line(char * line)
{
    int buffer_size = MYSH_TOK_BUFFER_SIZE, position = 0;
    char **tokens = malloc(buffer_size * sizeof(char *));
    char *token;
    // "cd /desk/main"
    token = strtok(line, MYSH_TOK_DELIM);
    while(token != NULL)
    {
        tokens[position++] = token;
        token = strtok(NULL, MYSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

int mysh_launch(char **args)
{
    // execvp
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        if(execvp(args[0], args) == -1)
            perror("Mysh error at execvp\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int mysh_execute(char **args)
{
    if(args[0] == NULL) return 1;
    
    for (int i = 0; i < mysh_builtin_nums(); i ++)
        if(strcmp(args[0], builtin_cmd[i]) == 0)
            return (*builtin_func[i])(args);
    return mysh_launch(args);
}

void mysh_loop()
{
    char *line;
    // "cd /desk/main"
    // "cd" "/desk/main"
    char **args;
    int status;

    do
    {
        char path[100];
        getcwd(path, 100);
        char now[200] = "[mysh ";
        strcat(now, path);
        strcat(now, " ]$");
        printf("%s", now);

        line = mysh_read_line();
        args = mysh_split_line(line);
        status = mysh_execute(args);

        free(line);
        free(args);
    } while (status);
    
}

int main(int argc, char *argv[])
{
    mysh_loop();
    return 0;
}