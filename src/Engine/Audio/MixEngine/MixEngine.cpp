#include "MixEngine.h"
#include <iostream>

const int INFINITE_LOOPS = -1;
const int NEXT_FREE_CHANNEL = -1;
const int MIXPLAYMUSIC_ERROR = -1;
const int MIXPLAYMUSIC_SUCCESS = 0;

const int MIX_OPEN_FAILED = -1;
const int MAX_SOUND_VOLUME = 255;
const float MS_TO_SECONDS = 0.001f;

using namespace std;

namespace SDG
{
    MixEngine::MixEngine()
        : ticketCounter_(0), musicVolume_(MAX_SOUND_VOLUME)
    {
        volTween_ = new Tween([this](float val) -> void {
            int clamped = Math::Clamp((int) val, 0, MAX_SOUND_VOLUME);
            Mix_VolumeMusic(clamped);
            musicVolume_ = clamped;
        }, 0, 0, 0, TweenFunctions::EaseInOutQuad);
    }

    bool MixEngine::Init()
    {
        // Initialize SDL_mixer with stereo output
        if (Mix_OpenAudio(SAMPLE_RATE, MIX_DEFAULT_FORMAT, 
            CHANNEL_NUM, CHUNK_SIZE) == MIX_OPEN_FAILED) {
            cerr << "Error initializing SDL_mixer! " << Mix_GetError() << '\n';
            return false;
        }
        return true;
    }

    void MixEngine::Update(const float deltaTime)
    { 
        // Remove and erase channels that are not playing
        channels_.erase(std::remove_if(channels_.begin(), channels_.end(),
                                       [] (const SoundID& id) {
                return !Mix_Playing(id.getChannelId());
            }), channels_.end());

        // Update the volume tween if it exists
        if (volTween_) {
            volTween_->Update(deltaTime);
        }
    }

    void MixEngine::Close()
    {
        delete volTween_;
        volTween_ = nullptr;

        // Free sfx and clear internal sfx map
        for (auto & pair : sounds_)
            Mix_FreeChunk(pair.second);
        sounds_.clear();

        // Free music
        for (auto & m : music_) {
            Mix_FreeMusic(m.second);
        }
        music_.clear();
        
        Mix_Quit();
    }

    bool MixEngine::LoadSFX(const char* tagName, const char* filepath)
    {
        // Make sure id is unique, it does not already exist
        if (sounds_.find(tagName) != sounds_.end()) {
            cerr << "The id given already exists in the sound "
                "storage container! Aborting sound load.\n";
            return false;
        }

        // Safely load the sound chunk and put it in storage
        Mix_Chunk* chunk = Mix_LoadWAV(filepath);
        if (chunk == nullptr) {
            cerr << "Error loading sound! SDL_mix error: " << 
                Mix_GetError() << '\n';
            return false;
        }

        // Store the chunk in the sound map
        sounds_[tagName] = chunk;
        return true;
    }

    bool MixEngine::LoadMusic(const char* tagName, const char* filepath)
    {
        // Make sure id does not exist
        if (music_.find(tagName) != music_.end()) {
            cerr << "The id given already exists in the music "
                "storage container! Aborting music load.\n";
            return false;
        }

        // load the music chunk
        Mix_Music* chunk = Mix_LoadMUS(filepath);

        // Check if load was successful
        if (chunk == nullptr) {
            cerr << "Error loading music! SDL_mix error: " << 
                Mix_GetError() << '\n';
            return false;
        }

        // Store music chunk in the internal map
        this->music_[tagName] = chunk;
        return true;
    }

    bool MixEngine::PlayMusic(const char* tagName, bool loop,
                              int volumeLevel, int fadeInTime)
    {
        // Check if tagName exists in the music map
        if (music_.find(tagName) == music_.end()) {
            std::cerr << "The tag name " << tagName << 
                " does not exist in the music map!\n";
            return false; 
        }

        // Set volume level immediately
        SetMusicVolume(volumeLevel, 0);

        // Get the music
        Mix_Music* music = music_.at(tagName);

        // Make sure retrieval was successful
        if (music == nullptr) {
            cerr << "Error retrieving music from the music map!\n";
            return false;
        }

        int mixPlayResult;
        // Call play function depending on fade-in time
        if (fadeInTime <= 0) {
            mixPlayResult = Mix_PlayMusic(music, loop ? INFINITE_LOOPS : 0);
        } else {
            mixPlayResult = Mix_FadeInMusic(music, loop ? INFINITE_LOOPS : 0, 
                fadeInTime);
        }

        // Check if playback error
        if (mixPlayResult == MIXPLAYMUSIC_ERROR) {
            cerr << "Error playing music: " << Mix_GetError() << '\n';
            return false;
        }

        return true;
    }

    SoundID MixEngine::PlaySFXLoop(const char* soundNameId,
                                   int loopNum, int fadeInTime)
    {
        SoundID soundId;
        // Call the Mix play function depending on fade in time
        if (fadeInTime > 0) {
            soundId._setChannelId(
                Mix_FadeInChannel(NEXT_FREE_CHANNEL, sounds_.at(soundNameId),
                                  loopNum, fadeInTime));
        } else {
            soundId._setChannelId(
                Mix_PlayChannel(NEXT_FREE_CHANNEL, sounds_.at(soundNameId),
                                loopNum));
        }

        // Error check, if no error, emplace in played channels set
        if (soundId.getChannelId() == DEFAULT_SOUNDID_VAL) {
            std::cerr << "Could not play SFX! SDL_mix error: " << 
                Mix_GetError() << '\n';
        } else {
            soundId._setInnerId(ticketCounter_++);
            channels_.emplace_back(soundId);
        }

        return soundId;
    }

    void MixEngine::StopAllSFX(int fadeOutTime) const
    {
        // Stop every SFX currently playing
        for (auto channel : channels_) {
            if (fadeOutTime <= 0)
                Mix_HaltChannel(channel.getChannelId());
            else
                Mix_FadeOutChannel(channel.getChannelId(), fadeOutTime);
        }
    }

    void MixEngine::StopSFX(const SoundID& id, int fadeOutTime) const
    {
        // Check that the sound effect is playing
        if (!IsSFXPlaying(id))
            return;

        // Call appropriate stop channel Mix function depending on fade time
        if (fadeOutTime <= 0) {
            Mix_HaltChannel(id.getChannelId());
        } else {
            Mix_FadeOutChannel(id.getChannelId(), fadeOutTime);
        }
    }

    void MixEngine::StopMusic(int fadeOutTime) const
    {
        // Call the appropriate stop music Mix function depending on fade time
        if (fadeOutTime <= 0) {
            Mix_HaltMusic();
        } else {
            Mix_FadeOutMusic(fadeOutTime);
        }
    }

    bool MixEngine::IsSFXPlaying(const SoundID& id) const
    {
        // Search channel vector for sound id
        auto iterator = std::find_if(channels_.begin(), channels_.end(),
                                     [id](auto searchId)->bool {
                return (id.getInnerId() == searchId.getInnerId());
            });

        return iterator != channels_.end();
    }

    void MixEngine::SetMusicVolume(int volume, int fadeTime,
                                   const TweenFunction& func)
    {
        // Directly set volume, or set it by Tween fade
        if (fadeTime <= 0) {
            Mix_VolumeMusic(volume);
            musicVolume_ = volume;
        } else {
            volTween_->Restart((float)musicVolume_, (float)volume,
                                (float)fadeTime * MS_TO_SECONDS, func);
        }
    }
}
