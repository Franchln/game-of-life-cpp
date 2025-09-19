#pragma once
#include "wx/wx.h"
#include "wx/spinctrl.h"
#include "wx/clrpicker.h"
#include <wx/sizer.h> 
#include "Settings.h"

class SettingsDialog : public wxDialog
{
public:
    SettingsDialog(wxWindow* parent, Settings& settingsRef);

private:
    Settings& settings; // Reference to the Settings struct

    // Controls
    wxSpinCtrl* gridSizeCtrl = nullptr;
    wxSpinCtrl* intervalCtrl = nullptr;
    wxColourPickerCtrl* livingColorPicker = nullptr;
    wxColourPickerCtrl* deadColorPicker = nullptr;

    // Event Handlers
    void OnOk(wxCommandEvent& event);
    void OnCancel(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};
