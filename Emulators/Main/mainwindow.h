#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <Windows.h>
#include "controllerinput.h"
#include "viewmodel.h"
#include "mocmodel.h"
#include "loader.h"
#include "logger.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow( char *argv[], QWidget *parent = 0);
    ~MainWindow();

protected:
   void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *mUi;
    ViewModel *mModel;
    void updateRomList(); 
    void handleNavigationKeys(Input::Keys keys, QListWidget* list);
    void startROM(const Emulator* emulator, const ROM* rom);
    // stuff for closing a running program
    // WINAPI callbacks have to be static
    static DWORD sAppHandleId;
    static WINBOOL CALLBACK findAndKill (HWND hwnd, LPARAM lParam);
    static WINBOOL CALLBACK findAndClose (HWND hwnd, LPARAM lParam);
    static WINBOOL CALLBACK findAndEscape (HWND hwnd, LPARAM lParam);

private slots:
    void onControllerInput(Input::Keys keys);
    void onCurrentRowChanged(int currentRow);
    void onRomListItemDoubleClicked(QListWidgetItem* item);
};

#endif // MAINWINDOW_H
