#pragma once
#include "wx/wx.h"
#include <fstream>

//Settings
struct Settings
{
    // Living cell color RGBA
    unsigned int livingCellR = 128;
    unsigned int livingCellG = 128;
    unsigned int livingCellB = 128;
    unsigned int livingCellA = 255;

    // Dead cell color RGBA
    unsigned int deadCellR = 255;
    unsigned int deadCellG = 255;
    unsigned int deadCellB = 255;
    unsigned int deadCellA = 255;

    // Grid size and timer interval
    int gridSize = 15;
    int interval = 50; // ms

    bool showNeighborCount = false;
    bool isToroidal = false;
    bool showGrid = true;      
    bool showThickGrid = false;
    bool showHUD = true;
    //get wxColor objects
    wxColor GetLivingCellColor() const
    {
        return wxColor(livingCellR, livingCellG, livingCellB, livingCellA);
    }

    wxColor GetDeadCellColor() const
    {
        return wxColor(deadCellR, deadCellG, deadCellB, deadCellA);
    }

    void SaveData()
    {
        std::ofstream file("settings.bin", std::ios::out | std::ios::binary);
        if (file.is_open())
        {
            file.write(reinterpret_cast<char*>(this), sizeof(Settings));
            file.close();
        }
    }

    void LoadData()
    {
        std::ifstream file("settings.bin", std::ios::in | std::ios::binary);
        if (file.is_open())
        {
            file.read(reinterpret_cast<char*>(this), sizeof(Settings));
            file.close();
        }
    }

    void ResetToDefaults()
    {
        livingCellR = 128;
        livingCellG = 128;
        livingCellB = 128;
        livingCellA = 255;

        deadCellR = 255;
        deadCellG = 255;
        deadCellB = 255;
        deadCellA = 255;

        gridSize = 15;
        interval = 50;
        showNeighborCount = false;
        isToroidal = false;

        SaveData(); // Overwrite saved settings with default values
    }
};