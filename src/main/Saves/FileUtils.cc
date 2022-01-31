//
// Created by rhys on 16/07/2021.
//

#include "FileUtils.h"

#include <iostream>
#include <sstream>
#include <fstream>

#if UNIX
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#include <dirent.h> 
#include <errno.h>
#elif WINDOWS
#include <windows.h>
#include <lmcons.h>
#endif

#define SAVE_FOLDER "CircuitSimulator"

bool FileUtils::pathExists(std::string path){
#if UNIX
    // Adapted from https://stackoverflow.com/questions/3828192/checking-if-a-directory-exists-in-unix-system-call
    struct stat sb;
    if (stat(path.c_str(), &sb) == 0){
        return true;
    }
    return false;
#elif WINDOWS
    // Adapted from https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
    DWORD ftyp = GetFileAttributesA(path.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES){
        return false;
    }

    if (ftyp){
        return true;
    }

    return false;
#else
#error Cannot determine OS (dirExists)!
#endif
}

bool FileUtils::createDirTree(std::string tree){
#if UNIX
    // Adapted from https://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux
    mode_t mode = 0755;
    if (mkdir(tree.c_str(), mode) == 0) { // Already exists.
        return true;
    }

    switch (errno){
        case ENOENT: // File does not exist.
            {
                int pos = tree.find_last_of('/');

                // No separators in the path, all finished.
                if (pos == std::string::npos){
                    return false;
                }

                // Recursively create again with path up to the last separator.
                if (!createDirTree( tree.substr(0, pos) )){
                    return false;
                }
            }
            // Check if file exists.
            return 0 == mkdir(tree.c_str(), mode);

        case EEXIST: // File already exists
            return pathExists(tree);

        default:
            return false;
    }
#elif WINDOWS
    // Adapted from https://stackoverflow.com/questions/1517685/recursive-createdirectory#12745750
    size_t pos = 0;
    do {
        // Find position of the last path separator.
        pos = tree.find_first_of("\\/", pos + 1);
        // CreateDirectory repeatedly until pos is end of the string.
        CreateDirectory(tree.substr(0, pos).c_str(), NULL);
    } while (pos != std::string::npos);

    return false;
#else
#error Cannot determine OS (createDirTree)!
#endif
}

std::string FileUtils::getSaveDir() {
    std::stringstream out;

#if defined(__APPLE__)
    out << "/Users/" << getUserName() << "/Library/" << SAVE_FOLDER << "/save/";
#elif defined(__linux)|| UNIX
    out << "/home/" << getUserName() << "/.local/share/" << SAVE_FOLDER << "/save/";
#elif defined(_WIN32)
    out << R"(C:\Users\)" << getUserName() << R"(\AppData\Local\)" << SAVE_FOLDER << R"(\save\)";
#else
#error Cannot determine OS (getOs)!
#endif

    return out.str();
}

std::string FileUtils::getUserName() {
#if UNIX
    uid_t uid = getuid(); // https://stackoverflow.com/questions/8953424/how-to-get-the-username-in-c-c-in-linux
    struct passwd* pw = getpwuid(uid);
    if (pw){
        return std::string(pw->pw_name);
    }

    std::cerr << "Cannot find unix username!" << std::endl;
    _exit(1);
#elif WINDOWS
    char username[UNLEN+1]; // https://stackoverflow.com/questions/11587426/get-current-username-in-c-on-windows
    DWORD username_len = UNLEN+1;
    GetUserName(username, &username_len);
    return std::string(username);
#else
#error Cannot determine OS (getUsername)!
#endif
}

bool FileUtils::saveDirExists() {
    return pathExists(getSaveDir());
}

bool FileUtils::createSaveDir() {
    return createDirTree(getSaveDir());
}

void FileUtils::copyFile(std::string src, std::string dst){
    std::ifstream in(src.c_str());
    std::ofstream out(dst.c_str());

    // Write content of in to out.
    out << in.rdbuf();

    in.close();
    out.close();
}

bool FileUtils::endsWith(std::string fullString, std::string ending) {
    if (fullString.length() >= ending.length()) {
        // Compare the last n (length of ending) chars of fullString to all the chars of ending.
        // Compare returns 0 if they are identical.
        return (fullString.compare(fullString.length() - ending.length(), ending.length(), ending) == 0);
    }

    return false;
}

std::string FileUtils::replace(std::string subj, std::string from, std::string to) {
    // Find position of the from string in the subj string
    size_t pos = subj.find(from);
    // If the position is not a position, return the unmodified string.
    if(pos == std::string::npos) {
        return subj;
    }

    // Replace the position pos in subject with to.
    subj.replace(pos, from.length(), to);
    return subj;
}


std::vector<std::string> FileUtils::getSaveFiles(){
    std::vector<std::string> out;
#if UNIX
  DIR *d; // https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
  struct dirent *dir;
  d = opendir(getSaveDir().c_str());
  if (d) {
    while ((dir = readdir(d)) != NULL) {
        if(endsWith(dir->d_name, ".cir")){
            out.push_back(replace(std::string(dir->d_name), ".cir", ""));
        }
    }
    closedir(d);
  }
#elif WINDOWS
    WIN32_FIND_DATA fdFile; // https://stackoverflow.com/questions/2314542/listing-directory-contents-using-c-and-windows
    HANDLE hFind = NULL;

    char sPath[2048];

    sprintf(sPath, "%s\\*.*", getSaveDir().c_str());

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE){
        printf("Path not found: [%s]\n", getSaveDir().c_str());
        return out;
    }

    do{
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0){
            sprintf(sPath, "%s\\%s", getSaveDir().c_str(), fdFile.cFileName);

            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY){}else{
                if(endsWith(sPath, ".cir")){
                    out.push_back(replace(std::string(fdFile.cFileName), ".cir", ""));
                }
            }
        }
    } while(FindNextFile(hFind, &fdFile));

    FindClose(hFind);
#else
#error Cannot determine OS (getSaveFiles)!
#endif
    return out;
}
