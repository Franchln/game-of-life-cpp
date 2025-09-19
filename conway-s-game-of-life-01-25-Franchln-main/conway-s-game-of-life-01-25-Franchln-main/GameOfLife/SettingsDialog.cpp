#include "SettingsDialog.h"
#include "wx/sizer.h" 

enum
{
    ID_GRID_SPIN = wxID_HIGHEST + 1,
    ID_INTERVAL_SPIN,
    ID_LIVING_COLOR,
    ID_DEAD_COLOR
};

wxBEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
EVT_BUTTON(wxID_OK, SettingsDialog::OnOk)
EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
wxEND_EVENT_TABLE()

SettingsDialog::SettingsDialog(wxWindow* parent, Settings& settingsRef)
    : wxDialog(parent, wxID_ANY, "Settings", wxDefaultPosition, wxSize(300, 220)),
    settings(settingsRef)
{
    // Main vertical sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Grid Size Row
    {
        wxBoxSizer* gridSizeSizer = new wxBoxSizer(wxHORIZONTAL);
        gridSizeSizer->Add(new wxStaticText(this, wxID_ANY, "Grid Size:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

        gridSizeCtrl = new wxSpinCtrl(this, ID_GRID_SPIN, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
            wxSP_ARROW_KEYS, 5, 200, settings.gridSize);
        gridSizeSizer->Add(gridSizeCtrl, 1, wxALIGN_CENTER_VERTICAL);

        mainSizer->Add(gridSizeSizer, 0, wxEXPAND | wxALL, 5);
    }

    // Interval Row
    {
        wxBoxSizer* intervalSizer = new wxBoxSizer(wxHORIZONTAL);
        intervalSizer->Add(new wxStaticText(this, wxID_ANY, "Interval (ms):"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

        intervalCtrl = new wxSpinCtrl(this, ID_INTERVAL_SPIN, wxEmptyString, wxDefaultPosition, wxSize(60, -1),
            wxSP_ARROW_KEYS, 10, 10000, settings.interval);
        intervalSizer->Add(intervalCtrl, 1, wxALIGN_CENTER_VERTICAL);

        mainSizer->Add(intervalSizer, 0, wxEXPAND | wxALL, 5);
    }

    // Living Cell Color Row
    {
        wxBoxSizer* livingColorSizer = new wxBoxSizer(wxHORIZONTAL);
        livingColorSizer->Add(new wxStaticText(this, wxID_ANY, "Living Cell Color:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

        livingColorPicker = new wxColourPickerCtrl(this, ID_LIVING_COLOR,
            wxColour(settings.livingCellR, settings.livingCellG, settings.livingCellB, settings.livingCellA));
        livingColorSizer->Add(livingColorPicker, 1, wxALIGN_CENTER_VERTICAL);

        mainSizer->Add(livingColorSizer, 0, wxEXPAND | wxALL, 5);
    }

    // Dead Cell Color Row
    {
        wxBoxSizer* deadColorSizer = new wxBoxSizer(wxHORIZONTAL);
        deadColorSizer->Add(new wxStaticText(this, wxID_ANY, "Dead Cell Color:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

        deadColorPicker = new wxColourPickerCtrl(this, ID_DEAD_COLOR,
            wxColour(settings.deadCellR, settings.deadCellG, settings.deadCellB, settings.deadCellA));
        deadColorSizer->Add(deadColorPicker, 1, wxALIGN_CENTER_VERTICAL);

        mainSizer->Add(deadColorSizer, 0, wxEXPAND | wxALL, 5);
    }

    // OK / CANCEL buttons
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(mainSizer);
}

void SettingsDialog::OnOk(wxCommandEvent& event)
{
    // Save user's changes
    settings.gridSize = gridSizeCtrl->GetValue();
    settings.interval = intervalCtrl->GetValue();

    // Living color
    wxColour livingColor = livingColorPicker->GetColour();
    settings.livingCellR = livingColor.Red();
    settings.livingCellG = livingColor.Green();
    settings.livingCellB = livingColor.Blue();
    settings.livingCellA = livingColor.Alpha();

    // Dead color
    wxColour deadColor = deadColorPicker->GetColour();
    settings.deadCellR = deadColor.Red();
    settings.deadCellG = deadColor.Green();
    settings.deadCellB = deadColor.Blue();
    settings.deadCellA = deadColor.Alpha();

    EndModal(wxID_OK); // Close dialog with OK status
}

void SettingsDialog::OnCancel(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL); // Close dialog without saving
}