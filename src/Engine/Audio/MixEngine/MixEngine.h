// Author: Aaron Ishibashi
// Project: SDG Engine
// File: Audio/AudioEngine.h
//
// This file includes all custom project math function headers for
// convenient reference
//
#pragma once
#ifndef SDGENGINE_AUDIOENGINE_H
#define SDGENGINE_AUDIOENGINE_H
#include <Engine/Core.h>
#include <SDL2/SDL_mixer.h>

#include <map>
#include <string>
#include <vector>
#include <algorithm>

#include "SoundID.h"
#include <Engine/Math/Math.h>
#include <Engine/Math/Tween.h>

const int SAMPLE_RATE = 44100;
const int CHUNK_SIZE = 2048;
const int CHANNEL_NUM = 2;

namespace SDG
{
    /**
     * MixEngine manages initializing SDL_mixer, loading and calling music and
     * sfx, tracking playing status, and safely closes/deletes all audio assets.
     * Dependency: SDL_mixer
     */
    class SDG_API MixEngine
    {
    public:
        MixEngine();

    private:
        // Internal storage of loaded sounds
        std::map<std::string, Mix_Chunk*> sounds_;

        // Mixer's single music instance
        std::map<std::string, Mix_Music*> music_;

        // A list of channels currently active
        std::vector<SoundID> channels_;
        
		// Id tickets for sound instances
		int ticketCounter_;

        Tween* volTween_;
        int musicVolume_; // need to store since no way to get this val from API

    public:

        /**
         * Initializes the AudioEngine, returns a boolean if the engine 
         * initialized successfully or not
         */
        static bool Init();

        /**
         * Safely shuts down the AudioEngine, cleans up sound references,
         * ready to be deleted
         */
        void Close();

        /**
         * Updates the SoundEngine, keeping inner data relevant to sound status
         * and updating volume fade
         */
        void Update(float deltaTime);

        /**
         * Safely load a sound effect to the SoundEngine. Returns whether or not
         * the sound loaded successfully
         * @param tagName - A string identifier by which to recall the sound
         * at a later time
         * @param filepath - The filepath of the sound to load
         */
        bool LoadSFX(const char* tagName, const char* filepath);

        /**
         * Safely load a music file to the SoundEngine.
         * @param tagName - Unique string to reference the music track by
         * @param filepath - Filepath of the music file to load
         * @returns Whether or not the music loaded successfully
         */
        bool LoadMusic(const char* tagName, const char* filepath);

        /**
         * Plays a sound (no looping). Use playSFXLoop for looping options.
         * @param tagName - Unique string to reference the sound by
         * @param fadeInTime - Number of millisdeconds to fade sound in
         * @returns SoundID reference of the new sound
         */
        SoundID playSound(const char* tagName, int fadeInTime = 0)
        {
            return PlaySFXLoop(tagName, 0, fadeInTime);
        }

        /**
         * Play a previously loaded music track
         * @param tagName - The name of the id
         * @param loop - whether or not this track should loop
         * @param volumeLevel - the starting volume level (0 - 255)
         * @returns Whether or not the music has successfully played
         */
        bool PlayMusic(const char* tagName, bool loop = true,
                       int volumeLevel = 255, int fadeInTime = 0);

        /**
         * Play a loaded sound effect chunk
         * @param soundNameId - The sound name reference
         * @param loopNum - The number of times to loop the sound,
         *      -1 = infinite, 0 = non-looping
         * @param fadeInTime - The number of milliseconds of fade-in
         */
        SoundID PlaySFXLoop(const char* tagName, int loopNum = -1,
                            int fadeInTime = 0);

        /** 
         * Stop an individual sound effect with fade out time
         * @param id - SoundID of sound to stop
         * @param fadeOutTime - Milliseconds to fade out sound
         */
        void StopSFX(const SoundID& id, int fadeOutTime = 0) const;

        /** 
         * Stop the music with fade out time
         * @param fadeOutTime - Milliseconds to fade out music
         */
        void StopMusic(int fadeOutTime = 2000) const;

        /**
         * Stop all currently active SFX
         * @param fadeOutTime - number of milliseconds of fade out time
         */
        void StopAllSFX(int fadeOutTime = 500) const;

        /**
         * Stop all currently active SFX and the currently active music track
         * @param musicFadeMs - Milliseconds to fade music
         * @param sfxFadeMs - Milliseconds to fade all sfx
         */
        void stopAll(int musicFadeMs = 0, int sfxFadeMs = 0) const
        {
            StopAllSFX(sfxFadeMs);
            StopMusic(musicFadeMs);
        }

        // Check if a particular SoundID's sound is currently playing
        [[nodiscard]] bool IsSFXPlaying(const SoundID& id) const;

        // Check if music is currently playing
        [[nodiscard]] bool isMusicPlaying() const
        {
            return Mix_PlayingMusic();
        }

        // Set a sound effect's spatial position
        void setSFXPosition(const SoundID& id, int angle, int distance) const
        {
            if (IsSFXPlaying(id))
                Mix_SetPosition(id.getChannelId(), angle, distance);
        }

        void SetMusicVolume(int volume, int fadeTime = 0,
                            const TweenFunction& func = TweenFunctions::EaseInOutQuad);

        void stopMusicFade() const
        {
            volTween_->Stop();
        }
    };
}


#endif
