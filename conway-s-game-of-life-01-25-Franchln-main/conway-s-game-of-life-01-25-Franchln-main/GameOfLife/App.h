#pragma once
#include "wx/wx.h"    
#include "MainWindow.h" 


class App : public wxApp
{
public:
    virtual bool OnInit();

private:
    MainWindow* mainWindow = nullptr; // Pointer to the MainWindow
};

