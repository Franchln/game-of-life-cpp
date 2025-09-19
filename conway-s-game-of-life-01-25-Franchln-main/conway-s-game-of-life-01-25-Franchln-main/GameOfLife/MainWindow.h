#pragma once
#include "wx/wx.h"
#include "DrawingPanel.h"
#include "Settings.h"
#include "SettingsDialog.h"
#include <vector>
#include <string> 

class MainWindow : public wxFrame
{
public:
    MainWindow(const wxString& title, const wxPoint& pos = wxPoint(0, 0), const wxSize& size = wxSize(600, 600));
    ~MainWindow();

    void InitializeGrid();
    void OnSizeChange(wxSizeEvent& event);
    void UpdateStatusBar();

    void CalculateNextGeneration();
    int CountLivingNeighbors(int row, int col);

    // Event handlers
    void OnPlay(wxCommandEvent& event);
    void OnPause(wxCommandEvent& event);
    void OnNext(wxCommandEvent& event);
    void OnClear(wxCommandEvent& event);
    void OnTimer(wxTimerEvent& event);

    void OnMenuSettings(wxCommandEvent& event);
    void OnToggleNeighborCount(wxCommandEvent& event);
    void OnResetSettings(wxCommandEvent& event);
    void OnRandomize(wxCommandEvent& event);
    void OnRandomizeWithSeed(wxCommandEvent& event);
    void OnImport(wxCommandEvent& event);
    void OnNew(wxCommandEvent& event);
    void OnOpen(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnSaveAs(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnToggleGrid(wxCommandEvent& event);  
    void OnToggleThickGrid(wxCommandEvent& event);
    void OnSetFiniteBoard(wxCommandEvent& event);
    void OnSetToroidalBoard(wxCommandEvent& event);
    void OnToggleHUD(wxCommandEvent& event);

    int GetGenerationCount() const 
    { 
        return generation; 
    }
    int GetLivingCellCount() const 
    { 
        return livingCells; 
    }

private:
    Settings settings;
    std::vector<std::vector<bool>> gameBoard;
    std::vector<std::vector<int>> neighborCount;

    DrawingPanel* drawingPanel = nullptr;

    int generation = 0;
    int livingCells = 0;
    wxStatusBar* statusBar = nullptr;
    wxToolBar* toolbar = nullptr;
    wxTimer* timer = nullptr;

    void RandomizeGrid(int seed);

    
    std::string currentFileName; // Stores the current file name

    void SaveToFile(const std::string& filename);
    void LoadFromFile(const std::string& filename);

    wxDECLARE_EVENT_TABLE();
};