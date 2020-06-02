//
// Created by lorenzodb on 05/04/2020.
//

#include <fstream>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <map>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    //open file for reading
    ifstream src("watchfile.txt", ios::in);
    map<string, int> pidsByFilename;
    //check if watchfile exists --> if it doesn't we will1 keep forking, NOT GOOD
    struct stat buffer;
    if(stat("./watchfile", &buffer) != 0){
        std::cout << "watchfile not found!" << std::endl;
        exit(1);
    }
    string line;
    while(1) {
        //read whole file
        while(!src.eof()) {
            getline(src, line);
            if(pidsByFilename.find(line) == pidsByFilename.end()) {
                int childpid;
                if((childpid = fork()) == -1) {
                    perror(argv[0]);
                    exit(1);
                }
                if(childpid == 0)
                    pidsByFilename.insert(pair<string, int>(line, childpid));
                else {
                    //lauch watchfile for this file
                    int status = execle("./watchfile", "watchfile", line.c_str(), (char*) 0, NULL);
                    perror(argv[0]);
                    exit(1);
                }
            }
        }
        src.clear();
        src.seekg(0, ios::beg);
    }
    src.close();
    return 0;
}