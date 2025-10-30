#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

namespace cutecpp
{

    class SocketWrapper
    {
    public:
        static inline void *socket;

        SocketWrapper(const std::string &adress);

        ~SocketWrapper();

        bool connect();
        bool send(std::byte *payload, std::size_t size);
        bool is_valid();
        bool close();
        std::uint32_t to_network_order(std::uint32_t i);
    };

}

