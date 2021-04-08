/* =============================================================================
 * Delegate
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <Engine/Events/EventListener.h>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>

namespace SDG
{
    // A handle that EventListeners can tune into for certain events.
    template <typename... Args>
    class Delegate2
    {
        // A pair that the Delegate stores each subscribing Listener in.
        // First: EventListener reference,
        // Second: Flag for removal where true == should remove.
        typedef std::pair<EventListener<Args...> *, bool> ListenerHandle;
    public:
        Delegate2 &operator+=(EventListener<Args...> *listener)
        {
            listeners.emplace_back(std::make_pair(listener, false));
            return *this;
        }

        Delegate2 &operator-=(EventListener<Args...> *listener)
        {
            // Check if this Delegate contains the listener
            for (auto it = listeners.begin(), end = listeners.end();
                it != end; ++it)
            {
                if (it->first == listener)
                {
                    if (isCalling)
                    {
                        it->second = true;
                        toRemove = true;
                    }
                    else
                    {
                        listeners.erase(it);
                    }

                    break;
                }
            }

            return *this;
        }

        void operator()(Args ...args)
        {
            if (toRemove)
            {
                // Remove-erase idiom
                listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
                               [](const ListenerHandle &handle) -> bool {
                                    return handle.second;
                               }), listeners.end());

                toRemove = false;
            }

            isCalling = true;
            for (auto &[listener, shouldRemove] : listeners)
            {
                listener->Callback((args)...);
            }
            isCalling = false;
        }
    private:
        std::vector<ListenerHandle> listeners;
        bool toRemove{false};
        bool isCalling{false};
    };
}