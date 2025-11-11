
#include <cutecpp/log.hpp>
#include "socket.hpp"

#include <utils.hpp>

#include <chrono>
#include <ctime>
#include <cstring>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <source_location>
#include <stacktrace>

namespace cutecpp
{

    Logger::Logger()
    {
        console_level = LogLevel::debug;
        name = "null";
    }

    Logger::Logger(std::string name_, LogLevel lvl)
    {
        console_level = lvl;
        name = name_;
    }

    void Logger::enable_file_sink(const std::string &path, bool append)
    {
        std::ios_base::openmode mode = std::ios::out;
        mode |= append ? std::ios::app : std::ios::trunc;

        if (Logger::file_sink_stream.is_open())
        {
            Logger::file_sink_stream.close();
        }

        Logger::file_sink_stream.open(path, mode);

        if (!Logger::file_sink_stream.is_open())
        {
            Logger::file_sink_enabled = false;
            Logger::file_sink_path.clear();
            throw std::runtime_error("failed to open log file: " + path);
        }

        Logger::file_sink_enabled = true;
        Logger::file_sink_path = path;
    }

    void Logger::disable_file_sink()
    {
        Logger::file_sink_enabled = false;
        Logger::file_sink_path.clear();

        if (Logger::file_sink_stream.is_open())
        {
            Logger::file_sink_stream.flush();
            Logger::file_sink_stream.close();
        }
    }

    bool Logger::is_file_sink_enabled()
    {
        return Logger::file_sink_enabled && Logger::file_sink_stream.is_open();
    }

    void Logger::enable_socket_sink(const std::string &address)
    {
        Logger::socket = new SocketWrapper(address);
        if (Logger::socket->connect())
        {
            auto payload = make_cutelog_payload("!!cutelog!!format=json");
            Logger::socket->send((std::byte *)payload.data(), payload.size());
            Logger::socket_sink_enabled = true;
        }
        else
        {
            std::cout << "Failed to set up socket connection" << std::endl;
        }
    }

    bool Logger::is_socket_sink_enabled()
    {
        return Logger::socket_sink_enabled && Logger::socket != nullptr && Logger::socket->is_valid();
    }

    void Logger::LogCall::l(std::string_view fmt,
                            std::format_args args) const
    {
        self->ll(level, loc, std::vformat(fmt, args));
    }

    Logger::LogCall Logger::operator()(LogLevel level, std::source_location loc)
    {
        return LogCall{this, level, loc};
    }

    std::string Logger::format_view(std::string_view fmt,
                            std::format_args args)
    {
        return std::vformat(fmt, args);
    }

    // Long log
    void Logger::ll(LogLevel level, std::source_location loc, std::string_view message) const
    {
        std::unique_lock<std::mutex> lock(Logger::print_mutex);
        auto log_level = log_level_to_str(level);

        if (level >= this->console_level)
        {
            auto formatted_msg = std::format("[{}] {}", log_level, message);
            Logger::write_to_console(level, formatted_msg);
        }
        if (level > LogLevel::ext_trace)
        {
            auto json_str = build_json_entry(
                this->name, log_level, escape_json_string(message), loc.file_name(), loc.line(), loc.function_name());

            if (Logger::is_file_sink_enabled())
                Logger::write_to_file(json_str);
            if (Logger::is_socket_sink_enabled())
                Logger::write_to_socket(json_str);
        }
        if (level == error)
        {
            // auto trace = std::stacktrace::current();
            // Logger::write_to_console(level, std::to_string(trace));
        }
    }

    void Logger::write_to_console(LogLevel level, const std::string_view message)
    {
        const std::string_view color_prefix = log_level_to_color(level);
        const std::string_view color_suffix = color_reset();

        if (level < LogLevel::error)
        {
            std::cout << color_prefix << message << color_suffix << "\n";
        }
        else
        {
            std::cerr << color_prefix << message << color_suffix << "\n";
        }
    }

    void Logger::write_to_file(const std::string_view message)
    {
        if (!Logger::file_sink_stream.is_open())
        {
            Logger::file_sink_enabled = false;
            return;
        }

        Logger::file_sink_stream << message;
        Logger::file_sink_stream.flush();
    }

    std::vector<uint8_t> Logger::make_cutelog_payload(const std::string_view json)
    {
        uint32_t len = static_cast<uint32_t>(json.size());
        uint32_t be = socket->to_network_order(len); // convert to big-endian

        std::vector<uint8_t> out(4 + json.size());
        std::memcpy(out.data(), &be, 4);
        std::memcpy(out.data() + 4, json.data(), json.size());
        return out;
    }

    void Logger::write_to_socket(const std::string_view message)
    {
        auto payload = make_cutelog_payload(message);
        Logger::socket->send((std::byte *)payload.data(), payload.size());
    }

    std::string Logger::build_json_entry(const std::string_view logger_name,
                                         const std::string_view level,
                                         const std::string_view message,
                                         const std::string_view file,
                                         const uint32_t line,
                                         const std::string_view function)
    {

        const auto time = std::chrono::system_clock::now();
        const auto str_time = std::format("{}", time);

        std::ostringstream entry;
        entry << '{';
        entry << "\"time\":\"" << str_time << "\", ";
        entry << "\"name\":\"" << logger_name << "\", ";
        entry << "\"level\":\"" << level << "\", ";
        entry << "\"msg\":\"" << message << "\", ";
        entry << "\"file\":\"" << file << "\", ";
        entry << "\"line\":\"" << line << "\", ";
        entry << "\"function\":\"" << function << "\"";
        entry << "}\n";

        return entry.str();
    }
}
