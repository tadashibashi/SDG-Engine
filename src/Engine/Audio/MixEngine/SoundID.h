#pragma once
#ifndef SDGENGINE_SOUNDID_H
#define SDGENGINE_SOUNDID_H

const int DEFAULT_SOUNDID_VAL = -1;

namespace SDG {
    class SoundID 
    {
    public:
        SoundID() : m_innerId(DEFAULT_SOUNDID_VAL), m_chanId(DEFAULT_SOUNDID_VAL)
        {
        }

        // Get the inner unique id for this sound
        [[nodiscard]] int getInnerId() const
        {
            return this->m_innerId;
        }

        // Get the current channel id for this sound
        [[nodiscard]] int getChannelId() const
        {
            return this->m_chanId;
        }

        [[nodiscard]] bool isValid() const
        {
            return this->m_innerId != -1;
        }

        void _setChannelId(int id)
        {
            m_chanId = id;
        }

        void _setInnerId(int id)
        {
            m_innerId = id;
        }
    private:
        // The inner id (read-only)
        int m_innerId;
        // The audio channel associated with this id (read-only)
        int m_chanId;
    };
}

#endif
