#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

int main()
{
    bool execute = true;

    while(execute)
    {

        //extra credit part
        char* username;
        char hostname[128];
        username = getlogin();
        gethostname(hostname, 128);
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
            char** argv;
            argv = (char**) malloc (400);
            char* token2 = strtok(commandvector.at(x), " ");
            int y = 0;
            for(; token2 != NULL; y++)
            {
                string stringtoken2 = token2;
                if(stringtoken2 == "exit" || stringtoken2 == "Exit"){
                    return 0;
                }
                argv[y] = new char[stringtoken2.size()];
                argv[y] = token2;
                token2 = strtok(NULL, " ");
            }
            
            argv[y] = new char[5];
            argv[y] = NULL;

            int pid = fork();
            if(pid == -1){
                perror("fork");
                exit(1);
            }
            if(pid == 0)
            {
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
