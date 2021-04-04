/**
 * Author: Aaron Ishibashi
 * Date: 3/14/2020
 *
 */
#include "Font.h"
#include <iostream>
#include <Engine/EngineConfig.h>

using namespace std;

namespace SDG
{
    Font::Font()
    {
        m_filepath = "\0";
        m_font = nullptr;
        m_isLoaded = false;
        m_ptSize = 0;
    }
    
    Font::~Font()
    {
        unload();
    }
    
    bool Font::load(const char* filepath, int ptSize)
    {
        // Check if already loaded
        if (m_isLoaded) {
            cerr << "TTF_Font is already loaded from file at: " <<
                m_filepath << "\nCancelling load of file: "<< filepath << '\n';
            return false;
        }
        
        // Load the font, and check for errors
        TTF_Font* font = TTF_OpenFont(filepath, ptSize);
        if (!font) {
            // Report load error
            cerr << "Error loading font \"" << filepath <<
            "\": " << TTF_GetError() << '\n';
            return false;
        }
        
        // Set member variables
        m_filepath = filepath;
        m_ptSize = ptSize;
        
        return(m_isLoaded = true);
    }
    
    void Font::unload()
    {
        if (m_isLoaded) {
            TTF_CloseFont(m_font);
            m_font = nullptr;
        }
    }
}
