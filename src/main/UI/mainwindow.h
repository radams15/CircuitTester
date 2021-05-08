#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/** @brief Main window class for the program
 * @author Rhys Adams
 * @date 11/03/2021
 */

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    /**
     * Initialises the main window
     * @param parent - The parent window, default to nullptr for toplevel
     */
    MainWindow(QWidget *parent = nullptr);
    /**
     * Deletes the window when finished
     */
    ~MainWindow();

private:

};
#endif // MAINWINDOW_H
