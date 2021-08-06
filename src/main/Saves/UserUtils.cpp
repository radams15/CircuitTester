//
// Created by rhys on 16/07/2021.
//

#include "UserUtils.h"

#include <iostream>
#include <sstream>

#define UNIX defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__linux) || defined(__linux__)
#define WINDOWS defined(_WIN32)

#if UNIX
#include <unistd.h>
#include <pwd.h>
#include <sys/stat.h>
#elif WINDOWS
#include <windows.h>
#include <lmcons.h>
#endif

#define SAVE_FOLDER "CircuitSimulator"

bool dirExists(std::string dir){
#if UNIX
    struct stat sb; // https://stackoverflow.com/questions/3828192/checking-if-a-directory-exists-in-unix-system-call
    if (stat(dir.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)){
        return true;
    }
    return false;
#elif WINDOWS
    DWORD ftyp = GetFileAttributesA(dir.c_str()); // https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
    if (ftyp == INVALID_FILE_ATTRIBUTES){
        return false;
    }

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY){
        return true;
    }

    return false;
#else
    std::cerr << "Cannot determine OS (dirExists)!" << std::endl;
    exit(1);
#endif
}

bool createDirTree(std::string tree){ // https://stackoverflow.com/questions/675039/how-can-i-create-directory-tree-in-c-linux
#if UNIX
    mode_t mode = 0755;
    int ret = mkdir(tree.c_str(), mode);
    if (ret == 0)
        return true;

    switch (errno)
    {
        case ENOENT:
            {
                int pos = tree.find_last_of('/');

                if (pos == std::string::npos){
                    return false;
                }

                if (!createDirTree( tree.substr(0, pos) )){
                    return false;
                }
            }
            return 0 == mkdir(tree.c_str(), mode);

        case EEXIST:
            return dirExists(tree);

            default:
                return false;
    }
#elif WINDOWS
    size_t pos = 0; // https://stackoverflow.com/questions/1517685/recursive-createdirectory#12745750
    do {
        pos = tree.find_first_of("\\/", pos + 1);
         CreateDirectory(tree.substr(0, pos).c_str(), NULL);
    } while (pos != std::string::npos);

    return false;
#else
#error "Cannot determine OS (createDirTree)!"
#endif
}

std::string UserUtils::getSaveDir() {
    std::stringstream out;

#if defined(__APPLE__)
    out << "/Users/" << getUserName() << "/Library/" << SAVE_FOLDER << "/save/";
#elif defined(__linux)
    out << "/home/" << getUserName() << "/.local/share/" << SAVE_FOLDER << "/save/";
#elif defined(_WIN32)
    out << R"(C:\Users\)" << getUserName() << R"(\AppData\Local\)" << SAVE_FOLDER << R"(\save\)";
#else
#error "Cannot determine OS (getOs)!"
#endif

    return out.str();
}

std::string UserUtils::getUserName() {
#if UNIX
    uid_t uid = getuid(); // https://stackoverflow.com/questions/8953424/how-to-get-the-username-in-c-c-in-linux
    struct passwd* pw = getpwuid(uid);
    if (pw){
        return std::string(pw->pw_name);
    }

    std::cerr << "Cannot find unix username!" << std::endl;
    exit(1);
#elif WINDOWS
    char username[UNLEN+1]; // https://stackoverflow.com/questions/11587426/get-current-username-in-c-on-windows
    DWORD username_len = UNLEN+1;
    GetUserName(username, &username_len);
    return std::string(username);
#else
#error "Cannot determine OS (getUsername)!"
#endif
}

bool UserUtils::saveDirExists() {
    return dirExists(getSaveDir());
}

bool UserUtils::createSaveDir() {
    return createDirTree(getSaveDir());
}
