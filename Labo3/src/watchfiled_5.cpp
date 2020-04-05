//
// Created by lorenzodb on 05/04/2020.
//

#include <fstream>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <map>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    //open file for reading
    fstream src("watchfile.txt", ios::in);
    map<string, int> pidsByFilename;
    //check if watchfile exists --> if it doesn't we will1 keep forking, NOT GOOD
    struct stat buffer;
    if(stat("./watchfile", &buffer) != 0){
        std::cout << "watchfile not found!" << std::endl;
        exit(1);
    }
    string line;
    do {
        //read whole file
        while(!src.eof()) {
            getline(src, line);
            if(!line.empty() && pidsByFilename.find(line) == pidsByFilename.end()) {
                int childpid;
                if((childpid = fork()) == -1) {
                    perror(argv[0]);
                    exit(1);
                }
                if(childpid != 0)
                    pidsByFilename.insert(pair<string, int>(line, childpid));
                else {
                    //lauch watchfile for this file
                    int status = execle("./watchfile", "watchfile", line.c_str(), (char*) 0, NULL);
                    //if we get here, watchfile does not exist --> we exit
                    perror(argv[0]);
                    exit(1);
                }
            }
        }
        src.clear();

        //bool to check if we need to rewrite watchfile.txt
        bool rewriteFile = false;
        //check if all children are still running
        auto iterator = pidsByFilename.begin();
        while(iterator != pidsByFilename.end()) {
            int pid = 0;
            if((pid = waitpid(iterator->second, nullptr, WNOHANG)) != 0) {
                cout << iterator->first << " has been deleted!" << std::endl;
                //child has exited --> remove this from the map
                iterator = pidsByFilename.erase(iterator);
                rewriteFile = true;
            }
            else
                iterator++;
        }

        if(rewriteFile) {
            //close readonly
            src.close();
            //reopen file with out and trunc
            src.open("watchfile.txt", ios::out | ios::trunc);
            cout << "now rewriting file:" << endl;
            //write all still existing filenames to file
            for(const auto child : pidsByFilename) {
                src << child.first << std::endl;
                cout << child.first << endl;
            }
            //close write
            src.close();
            cout << "done writing to file" << endl;
            //reopen read only
            src.open("watchfile.txt", ios::in);
        }

        src.seekg(0, ios::beg);
    }while(!pidsByFilename.empty());
    src.close();
    return 0;
}