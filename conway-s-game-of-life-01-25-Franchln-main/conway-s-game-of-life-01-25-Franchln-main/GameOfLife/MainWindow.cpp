#include "MainWindow.h"
#include "play.xpm"
#include "pause.xpm"
#include "next.xpm"
#include "trash.xpm"
#include "wx/artprov.h" 
#include <ctime>
#include <wx/numdlg.h>
#include "SettingsDialog.h"
#include <wx/filedlg.h>
#include <fstream>

enum
{
    ID_PLAY = 10001,
    ID_PAUSE,
    ID_NEXT,
    ID_CLEAR,
    ID_MENU_SETTINGS,
    ID_RESET_SETTINGS,
    ID_IMPORT,
    ID_TIMER = 20001,
    ID_VIEW_SHOW_NEIGHBORS,
    ID_RANDOMIZE,
    ID_RANDOMIZE_SEED,
    ID_VIEW_SHOW_GRID,      
    ID_VIEW_SHOW_THICKGRID,
    ID_VIEW_FINITE, 
    ID_VIEW_TOROIDAL,
    ID_VIEW_SHOW_HUD,
    ID_NEW,
    ID_OPEN,
    ID_SAVE,
    ID_SAVE_AS,
    ID_EXIT
};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_SIZE(MainWindow::OnSizeChange)
    EVT_MENU(ID_PLAY, MainWindow::OnPlay)
    EVT_MENU(ID_PAUSE, MainWindow::OnPause)
    EVT_MENU(ID_NEXT, MainWindow::OnNext)
    EVT_MENU(ID_CLEAR, MainWindow::OnClear)
    EVT_MENU(ID_MENU_SETTINGS, MainWindow::OnMenuSettings)
    EVT_MENU(ID_RESET_SETTINGS, MainWindow::OnResetSettings)
    EVT_MENU(ID_IMPORT, MainWindow::OnImport)
    EVT_MENU(ID_VIEW_SHOW_NEIGHBORS, MainWindow::OnToggleNeighborCount)
    EVT_MENU(ID_RANDOMIZE, MainWindow::OnRandomize)
    EVT_MENU(ID_RANDOMIZE_SEED, MainWindow::OnRandomizeWithSeed)
    EVT_MENU(ID_VIEW_SHOW_GRID, MainWindow::OnToggleGrid)
    EVT_MENU(ID_VIEW_SHOW_THICKGRID, MainWindow::OnToggleThickGrid)
    EVT_MENU(ID_VIEW_SHOW_HUD, MainWindow::OnToggleHUD)
    EVT_MENU(ID_NEW, MainWindow::OnNew)
    EVT_MENU(ID_OPEN, MainWindow::OnOpen)
    EVT_MENU(ID_SAVE, MainWindow::OnSave)
    EVT_MENU(ID_SAVE_AS, MainWindow::OnSaveAs)
    EVT_MENU(ID_EXIT, MainWindow::OnExit)
    EVT_MENU(ID_VIEW_FINITE, MainWindow::OnSetFiniteBoard)
    EVT_MENU(ID_VIEW_TOROIDAL, MainWindow::OnSetToroidalBoard)
    EVT_TIMER(ID_TIMER, MainWindow::OnTimer)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    // Load stored settings
    settings.LoadData();

    // Create menu bar
    wxMenuBar* menuBar = new wxMenuBar();

    // File Menu
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(ID_NEW, "New", "Clear the board and reset settings");
    fileMenu->Append(ID_OPEN, "Open...", "Open a saved game board");
    fileMenu->Append(ID_IMPORT, "Import...", "Import a game pattern");
    fileMenu->Append(ID_SAVE, "Save", "Save the current game board");
    fileMenu->Append(ID_SAVE_AS, "Save As...", "Save the board to a new file");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "Exit", "Exit the application");
    menuBar->Append(fileMenu, "File");

    // Options Menu
    wxMenu* optionsMenu = new wxMenu();
    optionsMenu->Append(ID_MENU_SETTINGS, "Settings...\tCtrl+S", "Open the settings dialog");
    optionsMenu->Append(ID_RESET_SETTINGS, "Reset Settings", "Reset all settings to default");
    optionsMenu->Append(ID_RANDOMIZE, "Randomize Board", "Randomize the board using current time");
    optionsMenu->Append(ID_RANDOMIZE_SEED, "Randomize with Seed...", "Randomize the board using a user-provided seed");
    menuBar->Append(optionsMenu, "Options");

    // View Menu
    wxMenu* viewMenu = new wxMenu();

    wxMenuItem* showNeighborsItem = new wxMenuItem(viewMenu, ID_VIEW_SHOW_NEIGHBORS, "Show Neighbor Count", "Toggle display of neighbor counts", wxITEM_CHECK);
    showNeighborsItem->SetCheckable(true);
    viewMenu->Append(showNeighborsItem);
    showNeighborsItem->Check(settings.showNeighborCount);

    wxMenuItem* showHUDItem = new wxMenuItem(viewMenu, ID_VIEW_SHOW_HUD, "Show HUD", "Toggle display of HUD", wxITEM_CHECK);
    showHUDItem->SetCheckable(true);
    viewMenu->Append(showHUDItem);
    showHUDItem->Check(settings.showHUD);

    wxMenuItem* finiteBoardItem = new wxMenuItem(viewMenu, ID_VIEW_FINITE, "Finite Board", "Use a finite universe", wxITEM_CHECK);
    wxMenuItem* toroidalBoardItem = new wxMenuItem(viewMenu, ID_VIEW_TOROIDAL, "Toroidal Board", "Use a toroidal universe", wxITEM_CHECK);

    finiteBoardItem->SetCheckable(true);
    toroidalBoardItem->SetCheckable(true);

    viewMenu->Append(finiteBoardItem);
    viewMenu->Append(toroidalBoardItem);

    // Ensure correct board type is selected at startup
    if (settings.isToroidal)
    {
        toroidalBoardItem->Check(true);
    }
    else
    {
        finiteBoardItem->Check(true);
    }

    wxMenuItem* showGridItem = new wxMenuItem(viewMenu, ID_VIEW_SHOW_GRID,
        "Show Grid", "Toggle grid visibility", wxITEM_CHECK);
    showGridItem->SetCheckable(true);
    viewMenu->Append(showGridItem);
    showGridItem->Check(settings.showGrid);

    
    wxMenuItem* showThickGridItem = new wxMenuItem(viewMenu, ID_VIEW_SHOW_THICKGRID,
        "Show 10x10 Grid", "Display thicker grid lines every 10 cells", wxITEM_CHECK);
    showThickGridItem->SetCheckable(true);
    viewMenu->Append(showThickGridItem);
    showThickGridItem->Check(settings.showThickGrid);

    menuBar->Append(viewMenu, "View");

    SetMenuBar(menuBar);

    // Create toolbar
    toolbar = CreateToolBar();

    wxBitmap playIcon(play_xpm);
    toolbar->AddTool(ID_PLAY, "Play", playIcon, "Start the simulation");

    wxBitmap pauseIcon(pause_xpm);
    toolbar->AddTool(ID_PAUSE, "Pause", pauseIcon, "Pause the simulation");

    wxBitmap nextIcon(next_xpm);
    toolbar->AddTool(ID_NEXT, "Next", nextIcon, "Advance one generation");

    wxBitmap clearIcon(trash_xpm);
    toolbar->AddTool(ID_CLEAR, "Clear", clearIcon, "Clear the board");

    toolbar->Realize();

    // Create timer
    timer = new wxTimer(this, ID_TIMER);

    // Create the drawing panel, passing gameBoard & settings
    drawingPanel = new DrawingPanel(this, GetClientSize(), gameBoard, settings, neighborCount);

    // Create status bar
    statusBar = CreateStatusBar();
    UpdateStatusBar();

    // Initialize the grid
    InitializeGrid();

    this->Layout();
}

MainWindow::~MainWindow()
{
    if (timer)
    {
        timer->Stop();
        delete timer;
    }
}

void MainWindow::InitializeGrid()
{
    gameBoard.resize(settings.gridSize);
    neighborCount.resize(settings.gridSize);

    for (int r = 0; r < settings.gridSize; ++r)
    {
        gameBoard[r].resize(settings.gridSize, false);
        neighborCount[r].resize(settings.gridSize, 0); // Initialize to 0**********************
    }
    drawingPanel->Refresh();
}

void MainWindow::OnSizeChange(wxSizeEvent& event)
{
    if (drawingPanel)
    {
        drawingPanel->SetSize(GetClientSize());
    }
    event.Skip();
}

void MainWindow::UpdateStatusBar()
{
    livingCells = 0;
    for (auto& row : gameBoard)
    {
        for (bool cell : row)
        {
            if (cell)
                ++livingCells;
        }
    }

    wxString statusText = wxString::Format("Generation: %d | Living Cells: %d", generation, livingCells);
    statusBar->SetStatusText(statusText);
}

void MainWindow::CalculateNextGeneration()
{

    std::vector<std::vector<bool>> sandbox(settings.gridSize, std::vector<bool>(settings.gridSize, false));
    int newLivingCells = 0;

    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            int neighbors = CountLivingNeighbors(row, col);
            bool currentlyAlive = gameBoard[row][col];

            if (currentlyAlive)
            {
                if (neighbors == 2 || neighbors == 3)
                {
                    sandbox[row][col] = true;
                    ++newLivingCells;
                }
            }
            else if (neighbors == 3)
            {
                sandbox[row][col] = true;
                ++newLivingCells;
            }
        }
    }


    gameBoard.swap(sandbox);
    ++generation;
    livingCells = newLivingCells;


    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            // Count neighbors based on the NEW generation swapped in
            neighborCount[row][col] = CountLivingNeighbors(row, col);
        }
    }

    UpdateStatusBar();
    drawingPanel->Refresh();
}

void MainWindow::OnPlay(wxCommandEvent& event)
{
    if (timer)
    {
        timer->Start(settings.interval);
    }
}

void MainWindow::OnPause(wxCommandEvent& event)
{
    if (timer)
    {
        timer->Stop();
    }
}

void MainWindow::OnNext(wxCommandEvent& event)
{
    CalculateNextGeneration();
}

void MainWindow::OnClear(wxCommandEvent& event)
{
    for (auto& row : gameBoard)
    {
        std::fill(row.begin(), row.end(), false);
    }
    generation = 0;
    UpdateStatusBar();
    drawingPanel->Refresh();
}

void MainWindow::OnTimer(wxTimerEvent& event)
{
    CalculateNextGeneration();
}

void MainWindow::OnMenuSettings(wxCommandEvent& event)
{
    // Make a backup in case user clicks Cancel
    Settings oldSettings = settings;

    SettingsDialog dialog(this, settings);
    if (dialog.ShowModal() == wxID_OK)
    {
        // User accepted the changes
        settings.SaveData();
        InitializeGrid();
        UpdateStatusBar();
        drawingPanel->Refresh();
    }
    else
    {
        // User hit CANCEL: revert to old settings
        settings = oldSettings;
    }
}

void MainWindow::OnToggleNeighborCount(wxCommandEvent& event)
{
    // The menu item is checkable. can check if it's checked or not.
    bool isChecked = event.IsChecked();
    settings.showNeighborCount = isChecked;

    // Save data so user’s preference persists
    settings.SaveData();

    // Refresh to show/hide neighbor counts
    drawingPanel->Refresh();
}

void MainWindow::RandomizeGrid(int seed)
{
    srand(seed); // Set the random seed
    generation = 0;
    livingCells = 0;

    // Loop through the gameBoard, randomizing each cell
    for (int r = 0; r < settings.gridSize; ++r)
    {
        for (int c = 0; c < settings.gridSize; ++c)
        {
            bool isAlive = (rand() % 100 < 45); //*****Can change the random amount here
            gameBoard[r][c] = isAlive;
            if (isAlive) livingCells++;
        }
    }

    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            neighborCount[row][col] = CountLivingNeighbors(row, col);
        }
    }

    UpdateStatusBar();
    drawingPanel->Refresh();
}

void MainWindow::OnRandomize(wxCommandEvent& event)
{
    int seed = static_cast<int>(time(NULL));
    RandomizeGrid(seed);
}

void MainWindow::OnRandomizeWithSeed(wxCommandEvent& event)
{
    
    long userSeed = wxGetNumberFromUser(
        "Enter a number to use as the seed:",
        "Seed: ",
        "Randomize with Seed",
        123, // default
        0, // minimum
        999999999, // maximum
        this
    );

    if (userSeed == -1)
    {
        //cancel
        return;
    }

    RandomizeGrid(static_cast<int>(userSeed));
}

void MainWindow::OnNew(wxCommandEvent& event)
{
    gameBoard.assign(settings.gridSize, std::vector<bool>(settings.gridSize, false));
    currentFileName.clear();
    generation = 0;
    UpdateStatusBar();
    drawingPanel->Refresh();
}

void MainWindow::OnOpen(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Open Life File", "", "", "Game of Life files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL) return;

    LoadFromFile(openFileDialog.GetPath().ToStdString());
}

void MainWindow::OnSave(wxCommandEvent& event)
{
    if (currentFileName.empty())
    {
        OnSaveAs(event);
    }
    else
    {
        SaveToFile(currentFileName);
    }
}

void MainWindow::OnSaveAs(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, "Save Life File", "", "", "Game of Life files (*.cells)|*.cells", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_CANCEL) return;

    currentFileName = saveFileDialog.GetPath().ToStdString();
    SaveToFile(currentFileName);
}

void MainWindow::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MainWindow::SaveToFile(const std::string& filename)
{
    std::ofstream file(filename);
    for (auto& row : gameBoard)
    {
        for (bool cell : row)
        {
            file << (cell ? '*' : '.');
        }
        file << '\n';
    }
    file.close();
}

void MainWindow::LoadFromFile(const std::string& filename)
{
    std::ifstream file(filename);
    std::string line;
    gameBoard.clear();

    while (std::getline(file, line))
    {
        gameBoard.emplace_back();
        for (char c : line)
        {
            gameBoard.back().push_back(c == '*');
        }
    }
    file.close();
    drawingPanel->Refresh();
}

void MainWindow::OnSetFiniteBoard(wxCommandEvent& event)
{
    settings.isToroidal = false;
    settings.SaveData();
    InitializeGrid();
    Refresh();
}

void MainWindow::OnSetToroidalBoard(wxCommandEvent& event)
{
    settings.isToroidal = true;
    settings.SaveData();
    InitializeGrid();
    Refresh();
}

int MainWindow::CountLivingNeighbors(int row, int col)
{
    int count = 0;
    for (int dr = -1; dr <= 1; ++dr)
    {
        for (int dc = -1; dc <= 1; ++dc)
        {
            if (dr == 0 && dc == 0)
                continue;

            int neighborRow = row + dr;
            int neighborCol = col + dc;

            if (settings.isToroidal)
            {
                neighborRow = (neighborRow + settings.gridSize) % settings.gridSize;
                neighborCol = (neighborCol + settings.gridSize) % settings.gridSize;
            }
            else if (neighborRow < 0 || neighborRow >= settings.gridSize ||
                neighborCol < 0 || neighborCol >= settings.gridSize)
            {
                continue;
            }

            if (gameBoard[neighborRow][neighborCol])
                ++count;
        }
    }
    return count;
}

void MainWindow::OnResetSettings(wxCommandEvent& event)
{
    settings.ResetToDefaults(); // Call the reset function
    settings.SaveData(); // Save defaults
    wxMessageBox("Settings have been reset to default.", "Reset Settings", wxOK | wxICON_INFORMATION, this);
}

void MainWindow::OnImport(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, "Import Pattern", "", "",
        "Cells Files (*.cells)|*.cells", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return; // User canceled import

    std::ifstream file(openFileDialog.GetPath().ToStdString());
    if (!file.is_open())
    {
        wxMessageBox("Failed to open file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    std::vector<std::vector<bool>> tempBoard;
    std::string line;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '!') continue; 
        std::vector<bool> row;
        for (char c : line)
        {
            if (c == '*') row.push_back(true);
            else if (c == '.') row.push_back(false);
        }
        tempBoard.push_back(row);
    }

    file.close();

    if (tempBoard.empty())
    {
        wxMessageBox("Invalid or empty pattern file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    int patternRows = tempBoard.size();
    int patternCols = tempBoard[0].size();

   
    int startRow = (settings.gridSize - patternRows) / 2;
    int startCol = (settings.gridSize - patternCols) / 2;

    // Ensure not out of bounds
    startRow = std::max(0, startRow);
    startCol = std::max(0, startCol);

    // Apply the pattern to the existing game board
    for (int r = 0; r < patternRows; ++r)
    {
        for (int c = 0; c < patternCols; ++c)
        {
            if ((startRow + r) < settings.gridSize && (startCol + c) < settings.gridSize)
            {
                gameBoard[startRow + r][startCol + c] = tempBoard[r][c];
            }
        }
    }

    UpdateStatusBar();
    drawingPanel->Refresh();
}

void MainWindow::OnToggleGrid(wxCommandEvent& event)
{
    settings.showGrid = event.IsChecked();
    settings.SaveData();
    drawingPanel->Refresh();
}

void MainWindow::OnToggleThickGrid(wxCommandEvent& event)
{
    settings.showThickGrid = event.IsChecked();
    settings.SaveData();
    drawingPanel->Refresh();
}

void MainWindow::OnToggleHUD(wxCommandEvent& event)
{
    settings.showHUD = event.IsChecked();
    settings.SaveData();
    drawingPanel->Refresh();
}