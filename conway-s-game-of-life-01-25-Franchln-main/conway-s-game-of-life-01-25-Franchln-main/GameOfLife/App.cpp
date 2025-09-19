#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    // Create and show the MainWindow
    mainWindow = new MainWindow("Game of Life", wxPoint(0, 0), wxSize(600, 600));
    mainWindow->Show();

    return true;
}