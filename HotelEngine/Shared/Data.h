#ifndef HOTEL_ENGINE_SHARED_DATA_H
#define HOTEL_ENGINE_SHARED_DATA_H

#include <stdint.h>
#include <ctime> // Import the ctime library
#include <optional>

namespace HotelEngine
{
    constexpr static uint32_t ROOM_NUMBER_NAME_LENGTH = 32;
    constexpr static uint32_t ROOM_DESCRIPTION_LENGTH = 256;
    constexpr static uint32_t GUEST_NAME_LENGTH = 256;
    constexpr static uint32_t GUEST_EMAIL_LENGTH = 256;
    constexpr static uint32_t GUEST_NATIONALITY_LENGTH = 256;

    struct RoomStats
    {
        uint32_t TotalCount = 0;
        uint32_t DirtyCount = 0;
        uint32_t AvailableCount = 0;
        uint32_t OcuppiedCount = 0;
    };

    enum class RoomStatus
    {
        Any,
        Available,
        Dirty,
        Occupied
    };

    enum class RoomType
    {
        Any,
        Standard,
        Delux,
        Suite
    };

    enum class PaymentStatus
    {
        Pending,
        Paid,
        Refuned
    };

    enum class PaymentMethod
    {
        Esewa,
        Khalti
    };

    struct Bed
    {
        uint32_t Id = 0;
        uint32_t RoomId = 0;
        // Bed specs
    };

    struct RoomQuery
    {
        std::optional<uint32_t> Id;
        std::optional<uint32_t> MinPrice;
        std::optional<uint32_t> MaxPrice;

        std::optional<HotelEngine::RoomType> Type;
        std::optional<HotelEngine::RoomStatus> Status;

        std::optional<uint32_t> MinBeds;
        std::optional<uint32_t> MaxGuests;

        std::string RoomNumberContains;
    };

    struct Room
    {
        uint32_t Id = 0;
        // Price Per Day
        uint32_t Price = 0;
        char RoomNumber[32] = {0};
        RoomType Type = RoomType::Standard;
        RoomStatus Status = RoomStatus::Available;
        uint32_t BedCount = 0;
        uint32_t MaxGuests = 0;
        char Description[256] = {0};

        static const char *GetCreateExeQuery();
        static const char *GetInsertQuerySqlite();
        static const char *GetTableCountQuery() { return "SELECT COUNT(*) FROM Room"; }
        static const char *GetName() { return "Room"; }
    };

    struct Guest
    {
        uint32_t Id = 0;
        uint32_t PhoneNumber = 0;
        uint32_t SecondaryPhoneNumber = 0;
        char Name[GUEST_NAME_LENGTH] = {0};
        char Email[GUEST_EMAIL_LENGTH] = {0};
        char Nationality[GUEST_NATIONALITY_LENGTH] = {0};
        RoomType Type = RoomType::Standard;
        RoomStatus Status = RoomStatus::Available;
        uint32_t BookingCount = 0;

        static const char *GetCreateExeQuery();
        static const char *GetInsertQuerySqlite();
        static const char *GetTableCountQuery() { return "SELECT COUNT(*) FROM Guest"; }
        static const char *GetName() { return "Guest"; }
    };

    struct PaymentTicket
    {
        uint32_t Id = 0;
        PaymentMethod PaidMethod;
        PaymentStatus Status = PaymentStatus::Pending;
        char Description[256] = {0};
        uint32_t PaidAmount = 0;
        struct tm CreatedDate;
        struct tm PaidDate;

        static const char *GetCreateExeQuery();
        static const char *GetInsertQuerySqlite();
        static const char *GetTableCountQuery() { return "SELECT COUNT(*) FROM PaymentTicket"; }
        static const char *GetName() { return "PaymentTicket"; }
    };

    struct BookingMetaData
    {
        uint32_t Id = 0;
        uint32_t GuestCount = 0;
        uint32_t TotalPrice = 0;
        struct tm CheckIn;
        struct tm CheckOut;
        uint32_t PaymentTicketId;
        bool Synced = false;
        struct tm CreatedAt;

        static const char *GetCreateExeQuery();
        static const char *GetInsertQuerySqlite();
        static const char *GetTableCountQuery() { return "SELECT COUNT(*) FROM BookingMetaData"; }
        static const char *GetName() { return "BookingMetaData"; }
    };

} // namespace HotelEngine

#endif
