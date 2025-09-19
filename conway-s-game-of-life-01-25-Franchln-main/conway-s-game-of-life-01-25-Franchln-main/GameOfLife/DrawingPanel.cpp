#include "DrawingPanel.h"
#include "MainWindow.h"

wxBEGIN_EVENT_TABLE(DrawingPanel, wxPanel)
EVT_PAINT(DrawingPanel::OnPaint)
EVT_LEFT_UP(DrawingPanel::OnMouseUp)
wxEND_EVENT_TABLE()

DrawingPanel::DrawingPanel(wxWindow* parent, const wxSize& size,
    std::vector<std::vector<bool>>& boardRef,
    Settings& settingsRef,
    std::vector<std::vector<int>>& neighborCountRef)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, size),
    gameBoard(boardRef),
    settings(settingsRef),
    neighborCount(neighborCountRef),
    mainWindow(dynamic_cast<MainWindow*>(parent))
{
    SetBackgroundStyle(wxBG_STYLE_PAINT);
}

DrawingPanel::~DrawingPanel()
{
}

void DrawingPanel::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (!gc) return;

    gc->SetPen(*wxBLACK);

    wxSize panelSize = GetClientSize();
    int cellWidth = panelSize.GetWidth() / settings.gridSize;
    int cellHeight = panelSize.GetHeight() / settings.gridSize;

    for (int row = 0; row < settings.gridSize; ++row)
    {
        for (int col = 0; col < settings.gridSize; ++col)
        {
            int x = col * cellWidth;
            int y = row * cellHeight;

            if (gameBoard[row][col])
            {
                gc->SetBrush(wxBrush(settings.GetLivingCellColor()));
            }
            else
            {
                gc->SetBrush(wxBrush(settings.GetDeadCellColor()));
            }

            gc->DrawRectangle(x, y, cellWidth, cellHeight);
        }
    }

    if (settings.showGrid)
    {
        wxPen gridPen(wxColor(0, 0, 0, 64), 1); // Light grid lines
        dc.SetPen(gridPen);

        for (int i = 1; i < settings.gridSize; ++i)
        {
            // Vertical Lines
            int x = i * cellWidth;
            dc.DrawLine(x, 0, x, panelSize.GetHeight());

            // Horizontal Lines
            int y = i * cellHeight;
            dc.DrawLine(0, y, panelSize.GetWidth(), y);
        }

        if (settings.showThickGrid)
        {
            wxPen thickPen(*wxBLACK, 2); // Thicker black grid lines
            dc.SetPen(thickPen);

            int thickLines = settings.gridSize / 10;
            for (int i = 1; i <= thickLines; ++i)
            {
                // Every 10th vertical line
                int thickX = i * 10 * cellWidth;
                dc.DrawLine(thickX, 0, thickX, panelSize.GetHeight());

                // Every 10th horizontal line
                int thickY = i * 10 * cellHeight;
                dc.DrawLine(0, thickY, panelSize.GetWidth(), thickY);
            }
        }
    }

    if (settings.showHUD && mainWindow)
    {
        gc->SetFont(wxFontInfo(16), wxColor(255, 255, 255));  

        wxString hudText = wxString::Format(
            "Generations: %d\nLiving Cells: %d\nBoundary: %s\nGrid Size: %dx%d",
            mainWindow->GetGenerationCount(),
            mainWindow->GetLivingCellCount(),
            settings.isToroidal ? "Toroidal" : "Finite",
            settings.gridSize, settings.gridSize
        );

        double textWidth, textHeight;
        gc->GetTextExtent(hudText, &textWidth, &textHeight);

       
        wxBrush hudBackground(wxColor(0, 0, 0, 128)); 
        gc->SetBrush(hudBackground);
        gc->DrawRectangle(5, panelSize.GetHeight() - textHeight - 20, textWidth + 10, textHeight + 10);

      
        gc->SetFont(wxFontInfo(16), *wxRED); //Hud Color
        gc->DrawText(hudText, 10, panelSize.GetHeight() - textHeight - 15);
    }

    delete gc;
}

void DrawingPanel::SetSize(const wxSize& newSize)
{
    wxPanel::SetSize(newSize);
    Refresh();
}

void DrawingPanel::OnMouseUp(wxMouseEvent& event)
{
    int mouseX = event.GetX();
    int mouseY = event.GetY();

    wxSize panelSize = GetClientSize();
    int cellWidth = panelSize.GetWidth() / settings.gridSize;
    int cellHeight = panelSize.GetHeight() / settings.gridSize;

    int col = mouseX / cellWidth;
    int row = mouseY / cellHeight;

    if (row >= 0 && row < settings.gridSize &&
        col >= 0 && col < settings.gridSize)
    {
        // Toggle the cell state in the gameBoard
        gameBoard[row][col] = !gameBoard[row][col];
        Refresh();
    }
}