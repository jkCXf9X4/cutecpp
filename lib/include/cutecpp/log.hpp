
#pragma once

#include <cutecpp/log_level.hpp>

#include <cutecpp/socket.hpp>

#include <fstream>
#include <memory>
#include <mutex>
#include <string>
#include <source_location>
#include <vector>

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

    public:
        static inline bool file_sink_enabled = false;
        static inline std::ofstream file_sink_stream;
        static inline std::string file_sink_path;

        static inline bool socket_sink_enabled = false;
        static inline std::unique_ptr<SocketWrapper> socket;

        static inline std::mutex print_mutex;

        std::string name;
        LogLevel console_level;

        Logger();

        Logger(std::string name_, LogLevel lvl);

        static void enable_file_sink(const std::string &path, bool append = true);

        static void disable_file_sink();

        static bool is_file_sink_enabled();

        static void enable_socket_sink(const std::string &address);

        static bool is_socket_sink_enabled();

        auto operator()(LogLevel level, std::source_location loc = std::source_location::current())
        {
            // This gives the option to return a function that does not do anything.
            // May enable some optimization, we will see
            return [=, this]<class... Args>(std::format_string<Args...> fmt, Args &&...args)
            {
                auto message = std::format(fmt, std::forward<Args>(args)...);
                // std::cout << message << std::endl;
                this->ll(level, loc, message);
            };
        }

        // Long log
        void ll(LogLevel level, std::source_location loc, std::string_view message) const;

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
