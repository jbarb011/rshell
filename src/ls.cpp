#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <fcntl.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <errno.h>
using namespace std;

#define Flag_a 1
#define Flag_l 2
#define Flag_R 4

void read_file(int Flag, string file_name){
    /*bool l_flag = false;
    if((Flag & Flag_l) == Flag_l){
        l_flag = true;
    }*/

    const char* file = file_name.c_str();
    struct stat statbuf;
    //bool isdir = false, isex = false, ishidden = false;

    if(-1 == stat(file,&statbuf)){
        perror("stat");
        exit(1);
    }

    /*if(file_name.at(0) == '.'){
        ishidden = true;
    }*/

    if(S_ISDIR(statbuf.st_mode)){
        cout << "d";
        //isdir = true;
    } else cout << "-";

    if (statbuf.st_mode & S_IRUSR) {
    cout << "r";
    } else cout << "-";
    if (statbuf.st_mode & S_IWUSR) {
    cout << "w";
    } else cout << "-";
    if (statbuf.st_mode & S_IXUSR) {
    cout << "x";
    //isex = true;
    } else cout << "-";
    
    if (statbuf.st_mode & S_IRGRP) {
    cout << "r";
    } else cout << "-";
    if (statbuf.st_mode & S_IWGRP) {
    cout << "w";
    } else cout << "-";
    if (statbuf.st_mode & S_IXGRP) {
    cout << "x";
    //isex = true;
    } else cout << "-";

    if (statbuf.st_mode & S_IROTH) {
    cout << "r";
    } else cout << "-";
    if (statbuf.st_mode & S_IWOTH) {
    cout << "w";
    } else cout << "-";
    if (statbuf.st_mode & S_IXOTH) {
    cout << "x";
    //isex = true;
    } else cout << "-";

    struct passwd *password;
    struct group *grp;
    password = getpwuid(statbuf.st_uid);
    grp = getgrgid(statbuf.st_gid);

    cout << " " << statbuf.st_nlink << " ";
    cout << password->pw_name << " " << grp->gr_name << " ";
    cout << statbuf.st_size << " ";

    struct tm *curr_time;
    char time_buffer[80];
    curr_time = localtime(&statbuf.st_atime);
    strftime(time_buffer, 80, "%b %d %R", curr_time);
    cout << time_buffer << " ";

    cout << file_name << endl;
}

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
    
    while((rddir = readdir(DIRp))){
        int err = errno;
        if(err != 0){
            perror("readdir");
            exit(1);
        }

        if(a_flag || rddir->d_name[0] != '.'){
            file_names.push_back(rddir->d_name);
        }

    }

    for(unsigned i = 0; i < file_names.size(); i++){
        for(unsigned j = 0; j < file_names.size(); j++){
            if(strcmp(file_names.at(i),file_names.at(j)) < 0){
                char* bop = file_names.at(i);
                file_names.at(i) = file_names.at(j);
                file_names.at(j) = bop;
            }
        }
    }
    
    for(unsigned i = 0; i < file_names.size(); i++){
        string fil = file_names.at(i);
        read_file(Flag, fil);
    }
}

int main(int argc, char** argv){
    int Flag = 0;
    vector<string> directories;
    vector<string> files;
    vector<bool> do_directory;    //true = do a directory, false = do a file
    bool docurrdirect = true;

    for(int x = 1; x < argc; x++){
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
