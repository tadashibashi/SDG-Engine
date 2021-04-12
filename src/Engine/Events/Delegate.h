/* =============================================================================
 * Delegate
 * An event observer that can make callbacks to subscribed listeners when an event has occured.
 * Listeners are created and stored within the Delegate, so no subclassing of a Listener class
 * is necessary as seen in the common Observer pattern.
 * Both member and global function callback Listeners are supported. To add one, you only need to pass
 * a function pointer to AddListener (and a corresponding object for member functions).
 * Listeners may be removed by a call to RemoveListener and passing the same pointer that was
 * previously added.
 * ===========================================================================*/
#pragma once
#include <any>
#include <functional>
#include <typeinfo>
#include <vector>

namespace SDG {

/**
 * @tparam Args Argument types that the Delegate requires for each of its callback listeners.
 */
template <typename... Args>
class Delegate {
    // Indicates whether a FunctionWrapper contains a Member of Global function
    enum class FunctionType { Member, Global };

    // Class wrapping a Delegate's callback functions
    class FunctionWrapper
    {
    public:
        template <typename T>
        FunctionWrapper(T *object, void (T::*func)(Args...)) :
            object(object), functionPtr(func), function([func, object](Args... args)->void { (object->*func)(args...); })
        {
        }

        explicit FunctionWrapper(void (*func)(Args...)) :
            object(nullptr), functionPtr(func), function(func)
        {
        }

        void operator()(Args ...args)
        {
            if (function)
                function(args...);
        }

        // ========== Equality Checks ==========

        // Gets whether this is a Global or Member function.
        FunctionType GetType() const
        {
            return (object == nullptr) ? FunctionType::Global : FunctionType::Member;
        }

        // Checks equality for a specific object and its member function
        template <typename T>
        bool CheckID(T *object, std::any functionPtr) const
        {
            return ((void *)object == this->object && functionPtr.type() == this->functionPtr.type()
                && std::any_cast<void(T::*)(Args...)>(functionPtr) == std::any_cast<void(T::*)(Args...)>(this->functionPtr));
        }

        // Checks equality for a specific global function
        bool CheckID(void(*func)(Args...)) const
        {
            return (this->object == nullptr && this->functionPtr.type() == typeid(func) &&
                std::any_cast<void(*)(Args...)>(this->functionPtr) == func);
        }

        // Whether or not this FunctionWrapper is to be removed or not.
        bool toRemove{false};
    private:
        void *object;
        std::any functionPtr;
        std::function<void(Args...)> function;

    };
public:
    // Gets the number of listeners currently attached to this Delegate.
    int GetHandleSize()
    {
        return (int)functions.size();
    }

    // Fires the callback to each subscribed listener
    void Invoke(Args... args)
    {
        if (functions.empty()) return;

        isCalling = true;
        for (FunctionWrapper &func : functions)
        {
            func(args...);
        }
        isCalling = false;

        ProcessRemovals();
    }

    // Fires the callback to each subscribed listener
    void operator()(Args... args)
    {
        Invoke(std::forward<Args>(args)...);
    }

    // Adds a member function listener for a particular object
    template <typename T>
    void AddListener(T *object, void (T::*func)(Args...))
    {
        functions.emplace_back(std::move(FunctionWrapper(object, func)));
    }

    // Adds a global function listener
    void AddListener(void (*func)(Args...))
    {
        functions.emplace_back(std::move(FunctionWrapper(func)));
    }

    // Removes a member function listener for a particular object that was previously added.
    template <typename T>
    void RemoveListener(T *object, void (T::*func)(Args...))
    {
        if (isCalling)
        {
            auto it = std::find_if(functions.begin(), functions.end(),
                                   [object, func](auto &f)
                                   {
                                       return f.CheckID(object, func);
                                   });
            if (it != functions.end())
                it->toRemove = true;

            removeThisFrame = true;
        }
        else
        {
                auto it = std::find_if(functions.begin(), functions.end(),
                             [object, func](auto &f)
                             {
                                return f.CheckID(object, func);
                             });
                if (it != functions.end())
                    functions.erase(it);
        }
    }

    // Removes a global function listener that was previously added.
    void RemoveListener(void (*func)(Args...))
    {
        if (isCalling)
        {
            auto it = std::find_if(functions.begin(), functions.end(),
                                   [func](auto &f)
                                   {
                                       return f.CheckID(func);
                                   });
            if (it != functions.end())
                it->toRemove = true;

            removeThisFrame = true;
        }
        else
        {
            auto it = std::find_if(functions.begin(), functions.end(),
                                   [func](auto &f)
                                   {
                                       return f.CheckID(func);
                                   });
            if (it != functions.end())
                functions.erase(it);
        }
    }
private:
    // Processes removals after delegate has been fired.
    void ProcessRemovals()
    {
        // Only perform removals if flag was set
        if (removeThisFrame)
        {
            // Erase all function wrappers with "toRemove" flag set
            functions.erase(
                    std::remove_if(functions.begin(), functions.end(),
                                   [](const FunctionWrapper &f) {
                                       return f.toRemove;
                                   }),
                    functions.end());

            // Unset flag
            removeThisFrame = false;
        }
    }

    // List of listeners
    std::vector<FunctionWrapper> functions{};

    // Flag indicating if this delegate is currently firing callbacks or not.
    bool isCalling{false};

    // Flag indicated whether or not removals need to be processed.
    bool removeThisFrame{false};
};


}