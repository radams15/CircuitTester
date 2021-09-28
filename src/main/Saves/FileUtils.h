/**
 * @author Rhys Adams
 * @date 16/07/2021
 */

#ifndef SAVELOAD1_USERUTILS_H
#define SAVELOAD1_USERUTILS_H

#include <string>
#include <vector>

// Definitions to decide if host is unix (linux/mac) or windows.
#define UNIX (defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__) || defined(__linux) || defined(__linux__))
#define WINDOWS (defined(_WIN32))

/** @brief A class that handles file and user utilities such as username,
 * home directories and file copying.
 *
 */
class FileUtils {
private:
    /** @brief Returns the username.
     *
     * @return Username
     */
    static std::string getUserName();

    /** @brief Recursively creates directory trees.
     *
     * @param tree The path to create.
     * @return true if error, false if success.
     */
    static bool createDirTree(std::string tree);

public:
#if UNIX
    /** @brief Unix path separator: /
     *
     */
    static const char sep = '/';
#elif WINDOWS
    /** @brief Windows path separator: \
     *
     */
    const static char sep = '\\';
#endif

    /** @brief Get the save directory for the user.
     *
     * For Windows: C:\Users\%USER%\AppData\Local\CircuitTester\
     *
     * For Linux: /home/$USER/.local/share/CircuitTester/
     *
     * For Mac: /Users/$USER/Library/CircuitTester/
     *
     * @return The save directory.
     */
    static std::string getSaveDir();
    
    static std::vector<std::string> getSaveFiles();
    

    /** @brief Does the save directory exist?
     *
     * @return Whether the save directory exists.
     */
    static bool saveDirExists();

    /** @brief Creates the save directory.
     *
     * @return true if error, false if success.
     */
    static bool createSaveDir();

    /** @brief Does a specific path exist?
     *
     * @param path The path to check existence of.
     * @return True if exists else false.
     */
    static bool pathExists(std::string path);

    /** @brief Copies src to dst.
     *
     * @param src Source file.
     * @param dst Destination path.
     */
    static void copyFile(std::string src, std::string dst);

    /** @brief Does fullString end with ending
     *
     * @param fullString Subject string to test.
     * @param ending The ending to test for.
     * @return True if fullString ends with ending, else false.
     */
    static bool endsWith(std::string fullString, std::string ending);
};


#endif //SAVELOAD1_USERUTILS_H
