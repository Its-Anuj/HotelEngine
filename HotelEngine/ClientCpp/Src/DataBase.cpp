#include <sstream>
#include <memory.h>
#include <optional>
#include <iostream>
#include "DataBase.h"

namespace HotelEngine
{
    namespace ClientCpp
    {
        const char *UserConfig::GetInsertQuerySqlite()
        {
            static std::string Query;

            std::ostringstream Stream;

            Stream << "INSERT INTO UserConfig ("

                   << "HotelName, "
                   << "BranchName, "
                   << "Gmail, "
                   << "PhoneNumber, "
                   << "Country, "
                   << "Location, "
                   << "Password, "
                   << "UniqueID"

                   << ") VALUES ("

                   << "?, ?, ?, ?, ?, ?, ?, ?"

                   << ")";

            Query = Stream.str();

            return Query.c_str();
        }

        const char *UserConfig::GetCreateExeQuery()
        {
            static std::string Query;

            std::ostringstream Stream;

            Stream << "CREATE TABLE UserConfig ("

                   << "Id INTEGER PRIMARY KEY, "

                   << "HotelName VARCHAR("
                   << USER_CONFIG_HOTEL_NAME_LENGTH
                   << "), "

                   << "BranchName VARCHAR("
                   << USER_CONFIG_BRANCH_NAME_LENGTH
                   << "), "

                   << "Gmail VARCHAR("
                   << USER_CONFIG_GMAIL_LENGTH
                   << "), "

                   << "PhoneNumber VARCHAR("
                   << PHONENUMBER_LENGTH
                   << "), "

                   << "Country VARCHAR("
                   << USER_CONFIG_COOUNTRY_LENGTH
                   << "), "

                   << "Location VARCHAR("
                   << USER_CONFIG_LOCATION_LENGTH
                   << "), "

                   << "Password VARCHAR("
                   << USER_CONFIG_PASSWORD_LENGTH
                   << "), "

                   // UUID stored as uint64
                   << "UniqueID INTEGER UNIQUE"

                   << ")";

            Query = Stream.str();

            return Query.c_str();
        }

        void HotelInsertCollection::InsertRoom(SQLite::Database &Db, const HotelEngine::Room &RoomData)
        {
            auto Insert = SQLite::Statement(Db, HotelEngine::Room::GetInsertQuerySqlite());
            Insert.bind(1, RoomData.Price);
            Insert.bind(2, RoomData.RoomNumber);
            Insert.bind(3, (int)RoomData.Type);
            Insert.bind(4, (int)RoomData.Status);
            Insert.bind(5, RoomData.BedCount);
            Insert.bind(6, RoomData.MaxGuests);
            Insert.bind(7, RoomData.Description);

            Insert.exec();
        }

        void HotelInsertCollection::InsertGuest(SQLite::Database &Db, const HotelEngine::Guest &GuestData)
        {
            auto Insert = SQLite::Statement(Db, HotelEngine::Guest::GetInsertQuerySqlite());

            Insert.bind(1, GuestData.PhoneNumber);
            Insert.bind(2, GuestData.SecondaryPhoneNumber);
            Insert.bind(3, GuestData.Name);
            Insert.bind(4, GuestData.Email);
            Insert.bind(5, GuestData.Nationality);
            Insert.bind(6, (int)GuestData.Type);
            Insert.bind(7, (int)GuestData.Status);
            Insert.bind(8, GuestData.BookingCount);

            Insert.exec();
        }

        void HotelInsertCollection::InsertUserConfigData(
            SQLite::Database &Db,
            const UserConfig &UserConfigData)
        {
            auto Insert =
                SQLite::Statement(
                    Db,
                    UserConfig::GetInsertQuerySqlite());

            Insert.reset();

            Insert.bind(
                1,
                std::string(UserConfigData.HotelName));

            Insert.bind(
                2,
                std::string(UserConfigData.BranchName));

            Insert.bind(
                3,
                std::string(UserConfigData.Gmail));

            Insert.bind(
                4,
                std::string(UserConfigData.PhoneNumber));

            Insert.bind(
                5,
                std::string(UserConfigData.Country));

            Insert.bind(
                6,
                std::string(UserConfigData.Location));

            Insert.bind(
                7,
                std::string(UserConfigData.Password));

            Insert.bind(
                8,
                (int64_t)(uint64_t)UserConfigData.UniqueID);

            Insert.exec();
        }

        void HotelInsertCollection::InsertPaymentTicket(SQLite::Database &Db, const HotelEngine::PaymentTicket &TicketData)
        {
            auto Insert = SQLite::Statement(Db, HotelEngine::PaymentTicket::GetInsertQuerySqlite());

            Insert.bind(1, (int)TicketData.PaidMethod);
            Insert.bind(2, (int)TicketData.Status);
            Insert.bind(3, TicketData.Description);
            Insert.bind(4, TicketData.PaidAmount);

            Insert.bind(5, (int64_t)mktime((tm *)&TicketData.CreatedDate));
            Insert.bind(6, (int64_t)mktime((tm *)&TicketData.PaidDate));

            Insert.exec();
        }

        void HotelInsertCollection::InsertBookingMetaData(SQLite::Database &Db, const HotelEngine::BookingMetaData &BookingData)
        {
            auto Insert = SQLite::Statement(Db, HotelEngine::BookingMetaData::GetInsertQuerySqlite());

            Insert.bind(1, BookingData.GuestCount);
            Insert.bind(2, BookingData.TotalPrice);

            Insert.bind(3, (int64_t)mktime((tm *)&BookingData.CheckIn));
            Insert.bind(4, (int64_t)mktime((tm *)&BookingData.CheckOut));

            Insert.bind(5, BookingData.PaymentTicketId);
            Insert.bind(6, (int)BookingData.Synced);

            Insert.bind(7, (int64_t)mktime((tm *)&BookingData.CreatedAt));

            Insert.exec();
        }

#pragma region HotelCollection

        void HotelCollection::_ScanActiveTable()
        {
            auto Db = DataBase.get();
            SQLite::Statement Query(*Db, "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;");
            ActiveTableNames.clear();
            // Loop through the results
            while (Query.executeStep())
            {
                std::string tableName = Query.getColumn(0);
                ActiveTableNames.push_back(tableName);
            }
        }

        void HotelCollection::Init(const std::string &PathToDb, int Flags)
        {
            DataBase = std::make_unique<SQLite::Database>(PathToDb.c_str(), Flags);
            _ScanActiveTable();
            auto Db = DataBase.get();

            CreateRoomTable();
            CreateGuestTable();
            CreateBookingMetaDataTable();
            CreatePaymentTicketTable();
        }

        void HotelCollection::InsertPaymentTicket(const HotelEngine::PaymentTicket &TicketData)
        {
            auto Db = DataBase.get();
            Insert.InsertPaymentTicket(*Db, TicketData);
        }

        void HotelCollection::InsertRoom(const HotelEngine::Room &RoomData)
        {
            auto Db = DataBase.get();
            Insert.InsertRoom(*Db, RoomData);
        }

        void HotelCollection::ChangeRoom(uint32_t Id, const HotelEngine::Room &NewRoomData)
        {
            auto Db = DataBase.get();
            int count = Db->execAndGet(HotelEngine::Room::GetTableCountQuery());

            try
            {
                SQLite::Statement query(
                    *Db,
                    "UPDATE Room SET "
                    "Price = ?, "
                    "RoomNumber = ?, "
                    "Type = ?, "
                    "Status = ?, "
                    "BedCount = ?, "
                    "MaxGuests = ?, "
                    "Description = ? "
                    "WHERE Id = ?");

                query.bind(1, (int)NewRoomData.Price);

                query.bind(2,
                           std::string(NewRoomData.RoomNumber));

                query.bind(3,
                           (int)NewRoomData.Type);

                query.bind(4,
                           (int)NewRoomData.Status);

                query.bind(5,
                           (int)NewRoomData.BedCount);

                query.bind(6,
                           (int)NewRoomData.MaxGuests);

                query.bind(7,
                           std::string(NewRoomData.Description));

                query.bind(8,
                           (int)NewRoomData.Id);

                query.exec();

                std::cout
                    << "Room updated successfully"
                    << std::endl;
            }
            catch (const SQLite::Exception &e)
            {
                std::cerr
                    << "SQLite Error: "
                    << e.what()
                    << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr
                    << "Standard Error: "
                    << e.what()
                    << std::endl;
            }
        }

        void HotelCollection::UpdateRoomStats(RoomStats &Stats)
        {
            auto Db = DataBase.get();

            Stats.TotalCount =
                Db->execAndGet(
                    HotelEngine::Room::GetTableCountQuery());

            try
            {
                {
                    SQLite::Statement q(
                        *Db,
                        "SELECT COUNT(*) FROM Room WHERE Status = ?;");

                    q.bind(1, (int)RoomStatus::Available);

                    if (q.executeStep())
                        Stats.AvailableCount =
                            q.getColumn(0).getInt();
                }

                {
                    SQLite::Statement q(
                        *Db,
                        "SELECT COUNT(*) FROM Room WHERE Status = ?;");

                    q.bind(1, (int)RoomStatus::Dirty);

                    if (q.executeStep())
                        Stats.DirtyCount =
                            q.getColumn(0).getInt();
                }

                {
                    SQLite::Statement q(
                        *Db,
                        "SELECT COUNT(*) FROM Room WHERE Status = ?;");

                    q.bind(1, (int)RoomStatus::Occupied);

                    if (q.executeStep())
                        Stats.OcuppiedCount =
                            q.getColumn(0).getInt();
                }
            }
            catch (const SQLite::Exception &e)
            {
                std::cerr << "SQLite Error: " << e.what() << std::endl;
            }
        }

        void HotelCollection::DeleteRoom(uint32_t Id)
        {
            auto Db = DataBase.get();

            try
            {
                SQLite::Statement q(
                    *Db,
                    "DELETE Room WHERE Id = ?;");

                q.bind(1, Id);
                q.exec();
            }
            catch (const SQLite::Exception &e)
            {
                std::cerr << "SQLite Error: " << e.what() << std::endl;
            }
        }

        void HotelCollection::InsertBookingMetaData(const HotelEngine::BookingMetaData &BookingMetaDataData)
        {
            auto Db = DataBase.get();
            Insert.InsertBookingMetaData(*Db, BookingMetaDataData);
        }

        void HotelCollection::InsertGuest(const HotelEngine::Guest &GuestData)
        {
            auto Db = DataBase.get();
            Insert.InsertGuest(*Db, GuestData);
        }

        void HotelCollection::InsertUserConfig(const UserConfig &UserConfigData)
        {
            auto Db = DataBase.get();

            Insert.InsertUserConfigData(*Db, UserConfigData);
        }

        void HotelCollection::CreateRoomTable()
        {
            if (DoesRoomTableExist())
                return;
            auto Db = DataBase.get();
            Db->exec(HotelEngine::Room::GetCreateExeQuery());
        }

        bool HotelCollection::DoesRoomTableExist()
        {
            return DoesTableExist(HotelEngine::Room::GetName());
        }

        void HotelCollection::CreateGuestTable()
        {
            if (DoesGuestTableExist())
                return;
            auto Db = DataBase.get();
            Db->exec(HotelEngine::Guest::GetCreateExeQuery());
        }

        bool HotelCollection::DoesGuestTableExist()
        {
            return DoesTableExist(HotelEngine::Guest::GetName());
        }

        void HotelCollection::CreatePaymentTicketTable()
        {
            if (DoesPaymentTicketTableExist())
                return;
            auto Db = DataBase.get();
            Db->exec(HotelEngine::PaymentTicket::GetCreateExeQuery());
        }

        bool HotelCollection::DoesPaymentTicketTableExist()
        {
            return DoesTableExist(HotelEngine::PaymentTicket::GetName());
        }

        void HotelCollection::CreateBookingMetaDataTable()
        {
            if (DoesBookingMetaDataTableExist())
                return;
            auto Db = DataBase.get();
            Db->exec(HotelEngine::BookingMetaData::GetCreateExeQuery());
        }

        bool HotelCollection::DoesBookingMetaDataTableExist()
        {
            return DoesTableExist(HotelEngine::BookingMetaData::GetName());
        }

        bool HotelCollection::DoesTableExist(const std::string &TableName)
        {
            _ScanActiveTable();
            for (auto &name : ActiveTableNames)
                if (name == TableName)
                    return true;
            return false;
        }

        void HotelCollection::CreateUserConfigTable()
        {
            if (DoesUserConfigTableExist())
                return;
            auto Db = DataBase.get();
            Db->exec(UserConfig::GetCreateExeQuery());
        }

        bool HotelCollection::DoesUserConfigTableExist()
        {
            return DoesTableExist(UserConfig::GetName());
        }

        void HotelCollection::WriteUserConfig(const std::string &HotelName, const std::string &BranchName,
                                              const std::string &Gmail, const std::string &Password, const std::string &PhoneNumber,
                                              const std::string &Location, const std::string &Country)
        {
            UserConfig Config;
            Config.UniqueID = UUID();
            memcpy(Config.Gmail, Gmail.data(), Gmail.size());
            memcpy(Config.Password, Password.data(), Password.size());
            memcpy(Config.BranchName, BranchName.data(), BranchName.size());
            memcpy(Config.HotelName, HotelName.data(), HotelName.size());
            memcpy(Config.PhoneNumber, PhoneNumber.data(), PhoneNumber.size());
            memcpy(Config.Location, Location.data(), Location.size());
            memcpy(Config.Country, Country.data(), Country.size());

            Insert.InsertUserConfigData(*DataBase.get(), Config);
        }

        std::vector<HotelEngine::Room> HotelCollection::QueryRoom(const RoomQuery &q)
        {
            auto Db = DataBase.get();
            std::stringstream ss;

            ss << "SELECT * FROM Room WHERE 1=1";

            if (q.Id.has_value())
                ss << " AND Id = ?";

            if (q.MinPrice.has_value())
                ss << " AND Price >= ?";

            if (q.MaxPrice.has_value())
                ss << " AND Price <= ?";

            if (q.Type.has_value())
                ss << " AND Type = ?";

            if (q.Status.has_value())
                ss << " AND Status = ?";

            if (q.MinBeds.has_value())
                ss << " AND BedCount >= ?";

            if (q.MaxGuests.has_value())
                ss << " AND MaxGuests >= ?";

            if (!q.RoomNumberContains.empty())
                ss << " AND RoomNumber LIKE ?";

            SQLite::Statement query(*Db, ss.str());

            int index = 1;

            if (q.Id.has_value())
                query.bind(index++, (int)*q.Id);

            if (q.MinPrice.has_value())
                query.bind(index++, (int)*q.MinPrice);

            if (q.MaxPrice.has_value())
                query.bind(index++, (int)*q.MaxPrice);

            if (q.Type.has_value())
                query.bind(index++, (int)*q.Type);

            if (q.Status.has_value())
                query.bind(index++, (int)*q.Status);

            if (q.MinBeds.has_value())
                query.bind(index++, (int)*q.MinBeds);

            if (q.MaxGuests.has_value())
                query.bind(index++, (int)*q.MaxGuests);

            if (!q.RoomNumberContains.empty())
            {
                std::string pattern =
                    "%" + q.RoomNumberContains + "%";

                query.bind(index++, pattern);
            }

            std::vector<HotelEngine::Room> rooms;

            while (query.executeStep())
            {
                HotelEngine::Room room;

                room.Id =
                    query.getColumn("Id").getUInt();

                room.Price =
                    query.getColumn("Price").getUInt();

                strncpy(
                    room.RoomNumber,
                    query.getColumn("RoomNumber").getString().c_str(),
                    sizeof(room.RoomNumber));

                room.Type =
                    (HotelEngine::RoomType)query.getColumn("Type").getInt();

                room.Status =
                    (HotelEngine::RoomStatus)query.getColumn("Status").getInt();

                room.BedCount =
                    query.getColumn("BedCount").getUInt();

                room.MaxGuests =
                    query.getColumn("MaxGuests").getUInt();

                strncpy(
                    room.Description,
                    query.getColumn("Description").getString().c_str(),
                    sizeof(room.Description));

                rooms.push_back(room);
            }

            return rooms;
        }

        UserConfig HotelCollection::QueryConfigData()
        {
            auto Db = DataBase.get();

            std::stringstream ss;

            ss << "SELECT * FROM UserConfig WHERE Id = 0";

            SQLite::Statement query(*Db, ss.str());

            while (query.executeStep())
            {
                UserConfig Config;

                Config.Id =
                    query.getColumn("Id").getUInt();

                strncpy(
                    Config.HotelName,
                    query.getColumn("HotelName")
                        .getString()
                        .c_str(),
                    sizeof(Config.HotelName));

                strncpy(
                    Config.BranchName,
                    query.getColumn("BranchName")
                        .getString()
                        .c_str(),
                    sizeof(Config.BranchName));

                strncpy(
                    Config.Gmail,
                    query.getColumn("Gmail")
                        .getString()
                        .c_str(),
                    sizeof(Config.Gmail));

                strncpy(
                    Config.PhoneNumber,
                    query.getColumn("PhoneNumber")
                        .getString()
                        .c_str(),
                    sizeof(Config.PhoneNumber));

                strncpy(
                    Config.Country,
                    query.getColumn("Country")
                        .getString()
                        .c_str(),
                    sizeof(Config.Country));

                strncpy(
                    Config.Location,
                    query.getColumn("Location")
                        .getString()
                        .c_str(),
                    sizeof(Config.Location));

                strncpy(
                    Config.Password,
                    query.getColumn("Password")
                        .getString()
                        .c_str(),
                    sizeof(Config.Password));

                Config.UniqueID =
                    UUID(
                        (uint64_t)
                            query.getColumn("UniqueID")
                                .getInt64());

                return Config;
            }

            return UserConfig();
        }

#pragma endregion
    }
} // namespace HotelEngine
