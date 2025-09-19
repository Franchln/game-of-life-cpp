#pragma once
#include "wx/wx.h"
#include "wx/graphics.h"
#include "wx/dcbuffer.h"
#include "Settings.h"
#include <vector>


class MainWindow;
class DrawingPanel : public wxPanel
{
public:
    DrawingPanel(wxWindow* parent,
        const wxSize& size,
        std::vector<std::vector<bool>>& boardRef,
        Settings& settingsRef,
        std::vector<std::vector<int>>& neighborCountRef);
    ~DrawingPanel();

    void OnPaint(wxPaintEvent& event);
    void SetSize(const wxSize& size);
    void OnMouseUp(wxMouseEvent& event);

private:
    
    std::vector<std::vector<bool>>& gameBoard;

    std::vector<std::vector<int>>& neighborCount;
    Settings& settings;

    MainWindow* mainWindow = nullptr;

    wxDECLARE_EVENT_TABLE();
};