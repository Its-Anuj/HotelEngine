#ifndef HOTEL_ENGINE_SHARED_UUID_H
#define HOTEL_ENGINE_SHARED_UUID_H

#include <functional> // if you're using std::hash>
#include <stdint.h> // if you're using std::hash>
// ^-- Remove the semicolon that was here

namespace HotelEngine
{
    class UUID
    {
    public:
        UUID();
        UUID(uint64_t id) : ID(id) {}
        UUID(const UUID &) = default;

        operator uint64_t() const { return ID; }

    private:
        uint64_t ID;
    };
} // namespace Chilli

namespace std
{
    template <>
    struct hash<HotelEngine::UUID>
    {
        std::size_t operator()(const HotelEngine::UUID &uuid) const
        {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };
} // namespace std

#endif