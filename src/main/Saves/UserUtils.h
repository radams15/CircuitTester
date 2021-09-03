/**
 * @author Rhys Adams
 * @date 16/07/2021
 */

#ifndef SAVELOAD1_USERUTILS_H
#define SAVELOAD1_USERUTILS_H

#include <string>

#define UNIX (defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__linux) || defined(__linux__))
#define WINDOWS (defined(_WIN32))

class UserUtils {
private:
    static std::string getUserName();

    static bool createDirTree(std::string tree);

public:
#if UNIX
    static const char sep = '/';
#elif WINDOWS
    const static char sep = '\\';
#endif

    static std::string getSaveDir();
    static bool saveDirExists();
    static bool createSaveDir();
    static bool pathExists(std::string dir);

    static void copyFile(std::string src, std::string dst);

    static bool endsWith(std::string fullString, std::string ending);
};


#endif //SAVELOAD1_USERUTILS_H
