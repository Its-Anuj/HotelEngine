#ifndef HOTEL_ENGINE_CLEINT_CPP_DATABASE_H
#define HOTEL_ENGINE_CLEINT_CPP_DATABASE_H

#include "SQLiteCpp/SQLiteCpp.h"
#include <vector>
#include "UUID.h"
#include "Data.h"

namespace HotelEngine
{
    namespace ClientCpp
    {
        static constexpr int USER_CONFIG_HOTEL_NAME_LENGTH = 256;
        static constexpr int USER_CONFIG_BRANCH_NAME_LENGTH = 256;
        static constexpr int USER_CONFIG_GMAIL_LENGTH = 256;
        static constexpr int USER_CONFIG_PASSWORD_LENGTH = 256;
        static constexpr int USER_CONFIG_LOCATION_LENGTH = 256;
        static constexpr int USER_CONFIG_COOUNTRY_LENGTH = 64;
        static constexpr int PHONENUMBER_LENGTH = 16;

        struct UserConfig
        {
            uint32_t Id = 0;
            char HotelName[128] = {0};
            char BranchName[128] = {0};
            char Gmail[256] = {0};
            char PhoneNumber[16] = {0};
            char Country[64] = {0};
            char Location[256] = {0};
            // Will be encrypted
            char Password[256] = {0};

            UUID UniqueID;

            static const char *GetCreateExeQuery();
            static const char *GetInsertQuerySqlite();
            static const char *GetTableCountQuery() { return "SELECT COUNT(*) FROM UserConfig"; }
            static const char *GetName() { return "UserConfig"; }
        };

        struct HotelInsertCollection
        {
            void InsertRoom(SQLite::Database &Db, const HotelEngine::Room &RoomData);                               
            void InsertGuest(SQLite::Database &Db, const HotelEngine::Guest &GuestData);
            void InsertPaymentTicket(SQLite::Database &Db, const HotelEngine::PaymentTicket &TicketData);
            void InsertBookingMetaData(SQLite::Database &Db, const HotelEngine::BookingMetaData &BookingData);
            void InsertUserConfigData(SQLite::Database &Db, const UserConfig &UserConfigData);
        };

        struct HotelCollection
        {
            HotelInsertCollection Insert;
            std::unique_ptr<SQLite::Database> DataBase;

            std::vector<std::string> ActiveTableNames;

            void _ScanActiveTable();
            void Init(const std::string &PathToDb, int Flags);

            void InsertPaymentTicket(const HotelEngine::PaymentTicket &TicketData);
            void InsertRoom(const HotelEngine::Room &RoomData);
            void InsertBookingMetaData(const HotelEngine::BookingMetaData &BookingMetaDataData);
            void InsertGuest(const HotelEngine::Guest &GuestData);
            void InsertUserConfig(const UserConfig &UserConfigData);

            void CreateRoomTable();
            bool DoesRoomTableExist();

            void CreateGuestTable();
            bool DoesGuestTableExist();

            void CreatePaymentTicketTable();
            bool DoesPaymentTicketTableExist();

            void CreateBookingMetaDataTable();
            bool DoesBookingMetaDataTableExist();

            void CreateUserConfigTable();
            bool DoesUserConfigTableExist();
            void WriteUserConfig(const std::string &HotelName, const std::string &BranchName,
                                 const std::string &Gmail, const std::string &Password, const std::string &PhoneNumber,
                                 const std::string &Location, const std::string &Country);

            bool DoesTableExist(const std::string &TableName);

            std::vector<HotelEngine::Room> QueryRoom(const RoomQuery &q);
            UserConfig QueryConfigData();
            void ChangeRoom(uint32_t Id, const HotelEngine::Room& NewRoomData);
            void UpdateRoomStats(RoomStats& Status);
            void DeleteRoom(uint32_t Id);
        };
    } // namespace
}
#endif
