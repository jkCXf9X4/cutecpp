
#pragma once

#include <cutecpp/log_level.hpp>

#include <fstream>
#include <memory>
#include <mutex>
#include <vector>

#include <string>
#include <string_view>
#include <source_location>

// If you import this file you will get the namespace
using namespace cutecpp;

// log_toggle.hpp
#ifdef _LOG_
#define IF_LOG(stmt) \
    do               \
    {                \
        stmt         \
    } while (0)
#else
#define IF_LOG(stmt) \
    do               \
    {                \
    } while (0)
#endif

namespace cutecpp
{
    class SocketWrapper;

    /**
     * @brief Simple logger implementation used throughout the library.
     *
     * The logger provides several convenience functions for different
     * log levels. Each function ultimately forwards to the templated
     * log() method which handles formatting and output.
     */

    using enum LogLevel;

    class Logger
    {

        static inline bool file_sink_enabled = false;
        static inline std::ofstream file_sink_stream;
        static inline std::string file_sink_path;

        static inline bool socket_sink_enabled = false;
        static inline SocketWrapper *socket = nullptr;

        static inline std::mutex print_mutex;

        std::string name;
        LogLevel console_level;

    public:
        Logger();

        Logger(std::string name_, LogLevel lvl);

        static void enable_file_sink(const std::string &path, bool append = true);

        static void disable_file_sink();

        static bool is_file_sink_enabled();

        static void enable_socket_sink(const std::string &address);

        static bool is_socket_sink_enabled();

        struct LogCall
        {
            Logger *self;
            LogLevel level;
            std::source_location loc;

            void operator()(std::string_view fmt, auto &&...args) const
            {
                auto fa = std::make_format_args(args...);
                l(fmt, fa);
            }

            void l(std::string_view fmt,
                   std::format_args args) const;
        };

        LogCall operator()(LogLevel level, std::source_location loc = std::source_location::current());

        // Long log
        void ll(LogLevel level, std::source_location loc, std::string_view message) const;

        // hides <format>
        static std::string format(std::string_view fmt, auto &&...args)
        {
            auto fa = std::make_format_args(args...);
            return format_view(fmt, fa);
        }

        static std::string format_view(std::string_view fmt,
                                   std::format_args args);

    private:
        static void write_to_console(LogLevel level, const std::string_view message);

        static void write_to_file(const std::string_view message);

        static std::vector<uint8_t> make_cutelog_payload(const std::string_view json);

        static void write_to_socket(const std::string_view message);

        static std::string build_json_entry(const std::string_view logger_name,
                                            const std::string_view level,
                                            const std::string_view message,
                                            const std::string_view file,
                                            const uint32_t line,
                                            const std::string_view function);
    };

}
