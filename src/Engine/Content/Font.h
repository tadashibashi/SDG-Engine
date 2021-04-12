/**
 * Author: Aaron Ishibashi
 * Date: 3/14/2020
 *
 */
#ifndef SDGENGINE_FONT_H
#define SDGENGINE_FONT_H
#include <Engine/Core.h>
#include <SDL2/SDL_ttf.h>

namespace SDG {
    
    /**
     * Struct that manages an SDL_Texture*. Automatically cleans up on deletion
     */
    class  Font {
    public:
        /**
         * @param filepath {const char*} filepath of the image to load
         * @param renderer {Renderer*} reference to a Renderer
         */
        Font();
        ~Font();
    private:
        // Pointer to the internal TTF_Font
        TTF_Font* m_font;
        // Flag indicating if font is currently loaded
        bool m_isLoaded;
        // Filepath to the font
        const char* m_filepath;
        // The font's point size
        int m_ptSize;
    public:
        /**
         * Loads a font into this Font container
         * @param filepath {const char*}
         * @param ptSize {int} point size
         * @returns {bool} whether or not load was successful
         */
        bool load(const char* filepath, int ptSize);
        
        /**
         * Unloads the currently loaded font if there is one loaded
         */
        void unload();
        
        /**
         * Gets whether or not the font is currently loaded
         */
        [[nodiscard]] bool isLoaded() const {return m_isLoaded;}
        
        /**
         * Gets the inner TTF_Font
         */
        [[nodiscard]] TTF_Font* getTTFFont() const {return m_font;}
        
        /**
         * Get the point size
         */
        [[nodiscard]] int getPtSize() const {return m_ptSize;}
    };
}

#endif //SDGENGINE_FONT_H
