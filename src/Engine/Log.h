#pragma once
#include <Engine/Core.h>

#if  defined(_DEBUG)
#include <iostream>
#include <string>
#include <cassert>
#include <utility>
#include <spdlog/spdlog.h>
#include <memory>

namespace SDG
{
    class Log {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_clientLogger; }
        static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_coreLogger; }
    private:
        static std::shared_ptr<spdlog::logger> s_clientLogger;
        static std::shared_ptr<spdlog::logger> s_coreLogger;
    };
}

// This concept requires that type T is ostream outputtable.
template <typename T>
concept Printable = requires (std::ostream &os, T a) { os << a; };

// Implemtation for logging a variadic argument pack to the console.
template<Printable Arg, Printable... Args>
static std::ostream &LogStream(std::ostream &os, bool log_endline,
    const std::string &delim, Arg &&arg, Args && ...args)
{
    os << std::forward<Arg>(arg);
    ((os << delim << std::forward<Args>(args)), ...);

    if (log_endline)
        os << '\n';
    return os;
}

//namespace SDG
//{
//    class Console {
//    public:
//        explicit Console(std::ostream &output = std::cout, std::ostream &err_output = std::cerr, std::string delim = " ")
//            : out_(output), err_(err_output), delim_(std::move(delim)) { }
//
//        // Prints each argument with delimiter in between each.
//        template<Printable Arg, Printable ...Args>
//        void WriteLine(Arg &&arg, Args && ...args) const
//        {
//            LogStream(out_, true, delim_, arg, args...);
//        }
//
//        // Prints each argument on a separate line.
//        template<Printable Arg, Printable ...Args>
//        void WriteLines(Arg &&arg, Args && ...args) const
//        {
//            LogStream(out_, true, "\n", arg, args...);
//        }
//
//        // Print each argument with no delimiters in between.
//        template<Printable Arg, Printable ...Args>
//        void Write(Arg &&arg, Args && ...args) const
//        {
//            LogStream(out_, false, std::string(), arg, args...);
//        }
//
//        void EndLine() const
//        {
//            out_ << '\n';
//        }
//
//        template<Printable Arg, Printable ...Args>
//        void Error(Arg &&arg, Args && ...args) const
//        {
//            LogStack(err_);
//            LogStream(err_, true, delim_, arg, args...);
//        }
//
//        void LogStack() const
//        {
//            LogStack(out_);
//        }
//
//        template<Printable Arg, Printable ...Args>
//        void Trace(Arg &&arg, Args && ...args) const
//        {
//            out_ << "--> " << GetStackFrame(STACK_BEFORE_INDEX) << "\n";
//            LogStream(out_, true, delim_, arg, args...);
//        }
//
//        static std::string GetStackFrame(int index);
//
//        template<Printable T>
//        const Console &operator<<(T printable) const
//        {
//            out_ << printable;
//            return *this;
//        }
//
//    private:
//
//
//        // Logs stack
//        static void LogStack(std::ostream &os);
//
//        std::ostream &out_;
//        std::ostream &err_;
//        std::string delim_;
//    };
//}

//#define SDG_LOG(...) LogStream(std::cout, true, " ", __VA_ARGS__)
//#define SDG_ERR(...) LogStream(std::cerr, true, " ", __VA_ARGS__)

#define SDG_LOG(...) ::SDG::Log::GetClientLogger()->info(__VA_ARGS__)
#define SDG_TRACE(...) ::SDG::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SDG_WARN(...) ::SDG::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SDG_ERR(...) ::SDG::Log::GetClientLogger()->error(__VA_ARGS__)
#define SDG_FATAL(...) ::SDG::Log::GetClientLogger()->critical(__VA_ARGS__)

#define SDG_CORE_LOG(...) ::SDG::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SDG_CORE_TRACE(...) ::SDG::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SDG_CORE_WARN(...) ::SDG::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SDG_CORE_ERR(...) ::SDG::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SDG_CORE_FATAL(...) ::SDG::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define SDG_ASSERT(statement) assert(statement)
#else
    #ifdef _MSC_VER
        #define SDG_LOG __noop
        #define SDG_ERR __noop
        #define SDG_ASSERT __noop
        #define SDG_TRACE __noop
        #define SDG_WARN __noop
        #define SDG_FATAL __noop
        #define SDG_CORE_LOG __noop
        #define SDG_CORE_TRACE __noop
        #define SDG_CORE_WARN __noop
        #define SDG_CORE_ERR __noop
        #define SDG_CORE_FATAL __noop
    #elif defined(__EMSCRIPTEN)
        #define SDG_LOG(...)
        #define SDG_TRACE(...)
        #define SDG_WARN(...)
        #define SDG_ERR(...)
        #define SDG_FATAL(...)

        #define SDG_CORE_LOG(...)
        #define SDG_CORE_TRACE(...)
        #define SDG_CORE_WARN(...)
        #define SDG_CORE_ERR(...)
        #define SDG_CORE_FATAL(...)
        #define SDG_ASSERT(...)
    #else
        #define SDG_LOG(...)
        #define SDG_TRACE(...)
        #define SDG_WARN(...)
        #define SDG_ERR(...)
        #define SDG_FATAL(...)

        #define SDG_CORE_LOG(...)
        #define SDG_CORE_TRACE(...)
        #define SDG_CORE_WARN(...)
        #define SDG_CORE_ERR(...)
        #define SDG_CORE_FATAL(...)
        #define SDG_ASSERT(...)
    #endif
#endif
