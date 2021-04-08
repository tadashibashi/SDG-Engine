/* =============================================================================
 * Delegate
 * 
 * 
 * ===========================================================================*/
#pragma once
#include <any>
#include <functional>
#include <vector>
#include <typeinfo>

namespace SDG {

template <typename... Args>
class Delegate {
    enum class FunctionType { Member, Global };
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

        bool CheckID(void(*func)(Args...))
        {
            return (this->object == nullptr && this->functionPtr.type() == typeid(func) &&
                std::any_cast<void(*)(Args...)>(this->functionPtr) == func);
        }

        bool toRemove{false};
    private:
        void *object;
        std::any functionPtr;
        std::function<void(Args...)> function;

    };
public:
    int GetHandleSize()
    {
        return (int)functions.size();
    }

    void Invoke(Args... args)
    {
        if (functions.empty()) return;

        this->isCalling = true;
        for (FunctionWrapper &f : functions)
        {
            f(args...);
        }
        this->isCalling = false;

        ProcessRemovals();
    }

    void operator()(Args... args)
    {
        if (functions.empty()) return;

        this->isCalling = true;
        for (FunctionWrapper &f : functions)
        {
            f(args...);
        }
        this->isCalling = false;

        ProcessRemovals();
    }

    template <typename T>
    void AddListener(T *object, void (T::*func)(Args...))
    {
        functions.emplace_back(std::move(FunctionWrapper(object, func)));
    }

    void AddListener(void (*func)(Args...))
    {
        functions.emplace_back(std::move(FunctionWrapper(func)));
    }

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

    std::vector<FunctionWrapper> functions;
    bool isCalling{false};
    bool removeThisFrame{false};
};


}