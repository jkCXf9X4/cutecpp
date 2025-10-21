#pragma once

#include <cstddef>
#include <string>

namespace cutecpp
{

    class SocketWrapper
    {
    public:
        static inline void *socket;

        explicit SocketWrapper(const std::string &adress);
        ~SocketWrapper();

        bool connect();
        bool send(std::byte *payload, std::size_t size);
        bool is_valid();
        bool close();
    };

}
