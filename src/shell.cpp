#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

int main()
{
    bool execute = true;

    while(execute){

        //extra credit part
        char* username;
        char hostname[128];
        username = getlogin();
        gethostname(hostname, 128);
        cerr << username << "@" << hostname << "$ ";
        
        //get command
        string fullcommand;
        getline(cin, fullcommand);



    }

    return 0;
}
