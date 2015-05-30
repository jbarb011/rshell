#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

bool inside = false;

void handler(int signum)
{
    if(signum == SIGINT){
        if(SIG_ERR == (signal(SIGINT, handler))){
            perror("signal");
            exit(1);
        }
        if(inside){
            exit(1);
        }
    }
    return;
}

int main()
{
    bool run= true;

    if(SIG_ERR == (signal(SIGINT, handler))){
        perror("signal");
        exit(1);
    }

    while(run)
    {
        bool changedir = false;
        //extra credit part
        char* username;
        char hostname[128];
        username = getlogin();
        if(NULL == username)
        {
            perror("getlogin");
            exit(1);
        }
        int errorhost = gethostname(hostname, 128);
        if(errorhost == -1){
            perror("gethostname");
            exit(1);
        }
        cerr << username << "@" << hostname << "$ ";
        
        //get command
        vector<char*> commandvector;
        char* token;
        char* fullcommand;
        fullcommand = new char[600];
        string stringcommand;
           
        //
        getline(cin, stringcommand);
        memcpy(fullcommand, stringcommand.c_str(), stringcommand.size()+1);
        
        //break command into smaller pieces according to ";"
        token = strtok(fullcommand, ";");
        while(token != NULL)
        {
            commandvector.push_back(token);
            token = strtok(NULL, ";"); 
        }

        for(unsigned x = 0; x < commandvector.size(); x++)
        {
            //ints and bools for redirection
            int new_in = -2, new_out = -2, new_append = -2;
            int old_in = -2, old_out = -2, old_append = -2;
            bool do_in = false, do_out = false, do_append = false;
            int out_file = 1;

            char** argv;
            argv = (char**) malloc (400);
            char* token2 = strtok(commandvector.at(x), " ");
            int y = 0;
            bool iscomment = false;
            for(; (token2 != NULL) && (!iscomment); y++)
            {
                string stringtoken2 = token2;
                if(stringtoken2 == "exit"){
                    return 0;
                }
                if(stringtoken2.at(0) == '#'){
                    iscomment = true;
                    token2 = NULL;
                    commandvector.clear();
                }
                
                if(stringtoken2 == "cd" && y == 0)
                {
                    token2 = strtok(NULL, " ");
                    if(-1 == chdir(token2))
                    {
                        perror("chdir");
                    }
                    changedir = true;
                }
                for(unsigned i = 0; i < stringtoken2.size(); i++)
                {
                    if(stringtoken2.at(i) == '<')
                    {
                        do_in = true;
                    }

                    else if(stringtoken2.at(i) == '>'){
                        if(stringtoken2.size() > 1 && !(do_append||do_out))
                        {
                            if(stringtoken2.size() - 1 > i){
                                if(stringtoken2.at(i+1) == '>')
                                {
                                    do_append = true;
                                }
                            }
                            if(i > 0)
                            {
                                if(token2[i-1] >= '0' && token2[i-1] <= '9')
                                {
                                    out_file = token2[i-1] - 48;
                                }
                            }
                        }

                        if(!(do_append))
                        {
                            do_out = true;
                        }
                    }

                }
                if(!(iscomment|do_in|do_out|do_append)){
                    argv[y] = new char[stringtoken2.size()];
                    argv[y] = token2;
                    token2 = strtok(NULL, " ");
                }
                else if(do_in)
                {
                    token2 = strtok(NULL, " ");
                    new_in = open(token2, O_CREAT | O_RDONLY, 0666);
                    if(new_in < 0)
                    {
                        perror("new_in open");
                        exit(1);
                    }
                    do_in = false;
                    token2 = strtok(NULL, " ");
                }
                else if(do_out)
                {
                    token2 = strtok(NULL, " ");
                    new_out = open(token2, O_CREAT | O_TRUNC | O_WRONLY, 0666);
                    if(new_out < 0)
                    {
                        perror("new_out open");
                        exit(1);
                    }
                    do_out = false;
                    token2 = strtok(NULL, " ");
                }
                else if(do_append)
                {
                    token2 = strtok(NULL, " ");
                    new_append = open(token2, O_CREAT | O_APPEND | O_WRONLY, 0666);
                    if(new_append < 0)
                    {
                        perror("new_append open");
                        exit(1);
                    }
                    do_append = false;
                    token2 = strtok(NULL, " ");
                }

            }
            

            argv[y] = new char[5];
            argv[y] = NULL;

            if(changedir){
                continue;
            }

            int pid = fork();
            if(pid == -1){
                perror("fork");
                exit(1);
            }
            if(pid == 0)
            {   
                if(new_in != -2){
                    if(-1 == (old_in = dup(0))){
                        perror("dup old_in");
                        exit(1);
                    }
                    if( -1 == close(0)){
                        perror("close 0");
                        exit(1);
                    }
                    if(-1 == dup(new_in)){
                        perror("dup new_in");
                        exit(1);
                    }
                }
                if(new_out != -2){
                    if(-1 == (old_out = dup(out_file))){
                        perror("dup out");
                        exit(1);
                    }
                    if( -1 == close(out_file)){
                        perror("close 1 out");
                        exit(1);
                    }
                    if(-1 == dup(new_out)){
                    perror("dup new_out");
                    exit(1);
                    }
                }
                if(new_append != -2){
                    if(-1 == (old_append = dup(out_file))){
                        perror("dup new_append");
                        exit(1);
                    }
                    if( -1 == close(out_file)){
                        perror("close 1 append");
                    exit(1);
                    }
                    if(-1 == dup(new_append)){
                        perror("dup new_append");
                        exit(1);
                    }
                }
                inside = true;
                int execvperror = execvp(argv[0], argv);
                if(execvperror == -1){
                    perror("execvp");
                    exit(1);
                }
            }
            else
            {
                int waiterror = wait(NULL);
                if(waiterror == -1)
                {
                    perror("wait");
                    exit(1);
                }
            }

        }

    }

    return 0;
}
