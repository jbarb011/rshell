#include <iostream>
#include <unistd.h>
#include <string>

using namespace std;

int main()
{
    bool execute = true;

    while(execute){
        char* username;
        char hostname[128];
        username = getlogin();
        gethostname(hostname, 128);
        cerr << username << "@" << hostname << "$ ";
        execute = false;
    }

    return 0;
}
