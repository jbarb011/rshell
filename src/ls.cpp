#include <iostream>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>


using namespace std;

#define Flag_a 1
#define Flag_l 2
#define Flag_R 4

void read_from(int Flag, string directory_name){
    DIR *DIRp;
    const char* char_directory_name = directory_name.c_str();
    DIRp = opendir(char_directory_name);

    if(DIRp == NULL){
        cerr << directory_name << ": ";
        perror("opendir");
        exit(1);
    }

    struct dirent *rddir;
    vector<char*> file_names;

    bool a_flag = false;
    if((Flag & Flag_a) == Flag_a){
        a_flag = true;
    }
    // left off here


}

int main(int argc, char** argv){
    int Flag = 0;
    vector<string> directories;
    vector<string> files;
    vector<bool> do_directory;    //true = do a directory, false = do a file
    bool docurrdirect = true;

    for(unsigned x = 1; x < argc; x++){
        if(argv[x][0] == '-'){
            for(unsigned y = 1; argv[x][y] != 0; y++){
                if(argv[x][y] == 'a'){
                    Flag |= Flag_a;
                }
                if(argv[x][y] == 'l'){
                    Flag |= Flag_l;
                }
                if(argv[x][y] == 'R'){
                    Flag |= Flag_R;
                }
            }
       }  
       else{
           struct stat file_buffer;
           if(-1 == stat(argv[x],&file_buffer)){
                perror("stat");
                exit(1);
           }

           if(S_ISDIR(file_buffer.st_mode)){
               string temp = argv[x];
               directories.push_back(temp);
               
               bool temp2 = true;
               do_directory.push_back(temp2);

               files.push_back("");

               docurrdirect = false;
           }

           else if(S_ISREG(file_buffer.st_mode)){
               string temp = argv[x];
               files.push_back(temp);

               bool temp2 = false;
               do_directory.push_back(temp2);

               directories.push_back("");

               docurrdirect = false;
           }
           else{
               cout << argv[x] <<  ": Not a valid arguement." << endl;
           }
        }
    }
    // list of directories, list of files, list of bools, flags
    
    if(docurrdirect){
        directories.push_back(".");
        files.push_back("");
        bool temp = true;
        do_directory.push_back(temp);
    }

    for(unsigned i = 0; i < do_directory.size(); i++){
        if(do_directory.at(i)){
            read_from(Flag, directories.at(i));
        }
        else{
            read_file(Flag, files.at(i));
        }
    }
    return 0;
}
