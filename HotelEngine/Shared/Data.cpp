#include <iostream>
#include <sstream>
#include <string>
#include "Data.h"

namespace HotelEngine
{
    const char *Room::GetCreateExeQuery()
    {
        static std::string Query;

        std::ostringstream Stream;

        Stream << "CREATE TABLE Room ("
               << "Id INTEGER PRIMARY KEY, "
               << "Price INTEGER, "
               << "RoomNumber VARCHAR(" << ROOM_NUMBER_NAME_LENGTH << "), "
               << "Type INTEGER, "
               << "Status INTEGER, "
               << "BedCount INTEGER, "
               << "MaxGuests INTEGER, "
               << "Description VARCHAR(" << ROOM_DESCRIPTION_LENGTH << ")"
               << ")";

        Query = Stream.str();
        return Query.c_str();
    }

    const char *Room::GetInsertQuerySqlite()
    {
        static std::string Query;
        std::ostringstream Stream;

        Stream << "INSERT INTO Room ("
               << "Price, "
               << "RoomNumber, "
               << "Type, "
               << "Status, "
               << "BedCount, "
               << "MaxGuests, "
               << "Description"
               << ") VALUES ("
               << "?, ?, ?, ?, ?, ?, ?"
               << ")";

        Query = Stream.str();
        return Query.c_str();
    }

    const char *Guest::GetCreateExeQuery()
    {
        static std::string Query;

        std::ostringstream Stream;

        Stream << "CREATE TABLE Guest ("
               << "Id INTEGER PRIMARY KEY, "
               << "PhoneNumber INTEGER, "
               << "SecondaryPhoneNumber INTEGER, "
               << "Name VARCHAR(" << GUEST_NAME_LENGTH << "), "
               << "Email VARCHAR(" << GUEST_EMAIL_LENGTH << "), "
               << "Nationality VARCHAR(" << GUEST_NATIONALITY_LENGTH << "), "
               << "Type INTEGER, "
               << "Status INTEGER, "
               << "BookingCount INTEGER"
               << ")";

        Query = Stream.str();
        return Query.c_str();
    }

    const char *Guest::GetInsertQuerySqlite()
    {
        static std::string Query;
        std::ostringstream Stream;

        Stream << "INSERT INTO Guest ("
               << "PhoneNumber, "
               << "SecondaryPhoneNumber, "
               << "Name, "
               << "Email, "
               << "Nationality, "
               << "Type, "
               << "Status, "
               << "BookingCount"
               << ") VALUES ("
               << "?, ?, ?, ?, ?, ?, ?, ?"
               << ")";

        Query = Stream.str();
        return Query.c_str();
    }

    const char *PaymentTicket::GetCreateExeQuery()
    {
        static std::string Query;

        std::ostringstream Stream;

        Stream << "CREATE TABLE PaymentTicket ("
               << "Id INTEGER PRIMARY KEY, "
               << "PaidMethod INTEGER, "
               << "Status INTEGER, "
               << "Description VARCHAR(" << ROOM_DESCRIPTION_LENGTH << "), "
               << "PaidAmount INTEGER, "
               << "CreatedDate INTEGER, "
               << "PaidDate INTEGER"
               << ")";

        Query = Stream.str();
        return Query.c_str();
    }

    const char *PaymentTicket::GetInsertQuerySqlite()
    {
        static std::string Query;
        std::ostringstream Stream;

        Stream << "INSERT INTO PaymentTicket ("
               << "PaidMethod, "
               << "Status, "
               << "Description, "
               << "PaidAmount, "
               << "CreatedDate, "
               << "PaidDate"
               << ") VALUES ("
               << "?, ?, ?, ?, ?, ?"
               << ")";

        Query = Stream.str();
        return Query.c_str();
    }

    const char *BookingMetaData::GetCreateExeQuery()
    {
        static std::string Query;

        std::ostringstream Stream;

        Stream << "CREATE TABLE BookingMetaData ("
               << "Id INTEGER PRIMARY KEY, "
               << "GuestCount INTEGER, "
               << "TotalPrice INTEGER, "
               << "CheckIn INTEGER, "
               << "CheckOut INTEGER, "
               << "PaymentTicketId INTEGER, "
               << "Synced INTEGER, "
               << "CreatedAt INTEGER"
               << ")";

        Query = Stream.str();
        return Query.c_str();
    }

    const char *BookingMetaData::GetInsertQuerySqlite()
    {
        static std::string Query;
        std::ostringstream Stream;

        Stream << "INSERT INTO BookingMetaData ("
               << "GuestCount, "
               << "TotalPrice, "
               << "CheckIn, "
               << "CheckOut, "
               << "PaymentTicketId, "
               << "Synced, "
               << "CreatedAt"
               << ") VALUES ("
               << "?, ?, ?, ?, ?, ?, ?"
               << ")";

        Query = Stream.str();
        return Query.c_str();
    }

} // namespace HotelEngine
