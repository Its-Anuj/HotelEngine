#include <iostream>
#include <memory.h>
#include <optional>
#include <assert.h>
#include "Log.h"
#include "Src/Window.h"
#include "Data.h"
#include "Src/DataBase.h"
#include "SQLiteCpp/SQLiteCpp.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <nlohmann/json.hpp>
#define _WIN32_WINNT 0x0A00

#include <httplib.h>

#include "GL/gl.h"

int main(int argc, char const *argv[])
{
    std::cout << "Hello Client" << "\n";

    std::string DataBaseName = "HotelEngineClient.db3";

    HotelEngine::ClientCpp::Window Window("HotelEngine", 800, 600);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)Window.GetRawWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    HotelEngine::ClientCpp::HotelCollection App;
    App.Init(DataBaseName, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

    bool show_demo_window = true;

    bool Registered = false;
    HotelEngine::ClientCpp::UserConfig Config;

    // Check if registered before
    if (App.DoesUserConfigTableExist() == true)
    {
        Registered = true;
        Config = App.QueryConfigData();
    }

    enum class DashboardSection
    {
        Home,
        Rooms,
        Staffs,
        Bookings
    };

    static DashboardSection CurrentSection =
        DashboardSection::Home;

    while (!Window.ShouldWindowClose())
    {
        Window.PollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (Registered == false)
        {
            ImGui::Begin("Register Window"); // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Register With Your Gmail and password"); // Display some text (you can use a format strings too)

            ImGui::Text("HotelName:");
            ImGui::SameLine();
            ImGui::InputText("##HoteName", Config.HotelName, HotelEngine::ClientCpp::USER_CONFIG_HOTEL_NAME_LENGTH);

            ImGui::Text("BranchName:");
            ImGui::SameLine();
            ImGui::InputText("##BranchName", Config.BranchName, HotelEngine::ClientCpp::USER_CONFIG_BRANCH_NAME_LENGTH);

            ImGui::Text("Gmail:");
            ImGui::SameLine();
            ImGui::InputText("##Gmail", Config.Gmail, HotelEngine::ClientCpp::USER_CONFIG_GMAIL_LENGTH);

            ImGui::Text("Password:");
            ImGui::SameLine();
            ImGui::InputText("##Password", Config.Password, HotelEngine::ClientCpp::USER_CONFIG_PASSWORD_LENGTH);

            ImGui::Text("PhoneNumber:");
            ImGui::SameLine();
            ImGui::InputText("##PhoneNumber", Config.PhoneNumber, HotelEngine::ClientCpp::PHONENUMBER_LENGTH);

            ImGui::Text("Country:");
            ImGui::SameLine();
            ImGui::InputText("##Country", Config.Country, HotelEngine::ClientCpp::USER_CONFIG_COOUNTRY_LENGTH);

            ImGui::Text("Location:");
            ImGui::SameLine();
            ImGui::InputText("##Location", Config.Location, HotelEngine::ClientCpp::USER_CONFIG_LOCATION_LENGTH);

            if (ImGui::Button("Submit"))
            {
                // Check online later
                bool Valid = false;
                Valid = true;

                if (Valid)
                {
                    Registered = true;
                    App.CreateUserConfigTable();

                    App.InsertUserConfig(Config);
                }
            }

            ImGui::End();
        }
        else
        {
            auto WinName = std::string(std::string(Config.HotelName) + "-" + std::string(Config.BranchName));
            ImGui::Begin(WinName.c_str()); // Create a window called "Hello, world!" and append into it.

            ImGui::SeparatorText("Dashboard");

            if (ImGui::Button("Home"))
                CurrentSection = DashboardSection::Home;

            ImGui::SameLine();

            if (ImGui::Button("Rooms"))
                CurrentSection = DashboardSection::Rooms;

            ImGui::SameLine();

            if (ImGui::Button("Staffs"))
                CurrentSection = DashboardSection::Staffs;

            ImGui::SameLine();

            if (ImGui::Button("Bookings"))
                CurrentSection = DashboardSection::Bookings;

            ImGui::Separator();

            switch (CurrentSection)
            {
            case DashboardSection::Home:
            {
                ImGui::Text("Welcome to Hotel Engine");

                ImGui::Spacing();

                ImGui::Text("Hotel:");
                ImGui::SameLine();
                ImGui::Text(Config.HotelName);

                ImGui::Text("Branch:");
                ImGui::SameLine();
                ImGui::Text(Config.BranchName);

                break;
            }

            case DashboardSection::Rooms:
            {
                enum class RoomSection
                {
                    None,
                    EditRoom,
                    DeleteRoom,
                    CreateNewRoom,
                };

                static RoomSection RoomSection = RoomSection::None;
                static char SearchBuffer[64] = "";
                static int TypeFilter = 0;
                static int StatusFilter = 0;
                static uint32_t EditRoomId = -1;

                if (ImGui::Button("Create New Room"))
                    RoomSection = RoomSection::CreateNewRoom;

                ImGui::SameLine();
                // Write Rooms UI here
                ImGui::Separator();

                //
                // Header
                //
                ImGui::Text("Rooms List");

                ImGui::SameLine();
                ImGui::InputText("##SearchRoom", SearchBuffer, sizeof(SearchBuffer));

                ImGui::SameLine();
                if (ImGui::Button("Search"))
                {
                    // trigger search logic
                }

                ImGui::SameLine();
                ImGui::Combo("Type", &TypeFilter,
                             "Any\0Standard\0Delux\0Suite\0");

                ImGui::SameLine();
                ImGui::Combo("Status", &StatusFilter,
                             "Any\0Available\0Dirty\0Occupied\0");

                ImGui::Separator();
                //
                // Rooms Table
                //
                static HotelEngine::RoomStats RoomStats;

                static bool UpdateRoomList = true;
                // Example data (replace with DB data)
                HotelEngine::RoomQuery RoomQuery;
                static std::vector<HotelEngine::Room> rooms = App.QueryRoom(RoomQuery); // <- your DB result here

                if (UpdateRoomList)
                {
                    rooms = App.QueryRoom(RoomQuery); // <- your DB result here
                    App.UpdateRoomStats(RoomStats);
                    UpdateRoomList = false;
                }

                if (ImGui::BeginTable("RoomsTable", 6,
                                      ImGuiTableFlags_Borders |
                                          ImGuiTableFlags_RowBg |
                                          ImGuiTableFlags_Resizable |
                                          ImGuiTableFlags_Sortable))
                {
                    ImGui::TableSetupColumn("Room");
                    ImGui::TableSetupColumn("Type");
                    ImGui::TableSetupColumn("Status");
                    ImGui::TableSetupColumn("Price");
                    ImGui::TableSetupColumn("Beds");
                    ImGui::TableSetupColumn("Actions");

                    ImGui::TableHeadersRow();

                    for (auto &r : rooms)
                    {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", r.RoomNumber);

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%d", (int)r.Type);

                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%d", (int)r.Status);

                        ImGui::TableSetColumnIndex(3);
                        ImGui::Text("%u", r.Price);

                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("%u", r.BedCount);

                        ImGui::TableSetColumnIndex(5);
                        {
                            if (ImGui::Button(("Edit##" + std::to_string(r.Id)).c_str()))
                            {
                                RoomSection = RoomSection::EditRoom;

                                EditRoomId = r.Id;
                            }

                            ImGui::SameLine();

                            if (ImGui::Button(("Delete##" + std::to_string(r.Id)).c_str()))
                            {
                                RoomSection = RoomSection::DeleteRoom;

                                EditRoomId = r.Id;
                            }
                        }
                    }

                    ImGui::EndTable();
                }

                ImGui::Separator();

                //
                // Stats Section
                //
                ImGui::Text("Stats:");

                ImGui::Text("Total: %d", RoomStats.TotalCount);
                ImGui::SameLine();
                ImGui::Text("Available: %d", RoomStats.AvailableCount);
                ImGui::SameLine();
                ImGui::Text("Occupied: %d", RoomStats.OcuppiedCount);
                ImGui::SameLine();
                ImGui::Text("Dirty: %d", RoomStats.DirtyCount);

                switch (RoomSection)
                {
                case RoomSection::CreateNewRoom:
                {
                    static HotelEngine::Room NewRoom;
                    ImGui::Begin("Create New Room");
                    // Room Number
                    ImGui::Text("Room Number: ");
                    ImGui::SameLine();
                    ImGui::InputText("##Room Number", NewRoom.RoomNumber, sizeof(NewRoom.RoomNumber));

                    // Price
                    ImGui::Text("Price Per Day: ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Price Per Day", (int *)&NewRoom.Price);

                    // Bed Count
                    ImGui::Text("Bed Count: ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Bed Count", (int *)&NewRoom.BedCount);

                    const char *roomTypes[] =
                        {
                            "Any",
                            "Standard",
                            "Delux",
                            "Suite"};

                    int currentType = (int)NewRoom.Type;

                    ImGui::Text("Room Type: ");
                    ImGui::SameLine();
                    if (ImGui::Combo("##Room Type", &currentType, roomTypes, IM_ARRAYSIZE(roomTypes)))
                    {
                        NewRoom.Type = (HotelEngine::RoomType)currentType;
                    }

                    const char *roomStatus[] =
                        {
                            "Any",
                            "Available",
                            "Dirty",
                            "Occupied"};

                    int currentStatus = (int)NewRoom.Status;

                    ImGui::Text("Room Status^: ");
                    ImGui::SameLine();
                    if (ImGui::Combo("##Room Status", &currentStatus, roomStatus, IM_ARRAYSIZE(roomStatus)))
                    {
                        NewRoom.Status = (HotelEngine::RoomStatus)currentStatus;
                    }

                    // Max Guests
                    ImGui::Text("Max Guests: ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Max Guests", (int *)&NewRoom.MaxGuests);

                    // Description
                    ImGui::Text("Description: ");
                    ImGui::SameLine();
                    ImGui::InputTextMultiline(
                        "##Description",
                        NewRoom.Description,
                        sizeof(NewRoom.Description),
                        ImVec2(0, 80));

                    if (ImGui::Button("Create"))
                    {
                        // Check online later
                        bool Valid = false;
                        Valid = true;

                        if (Valid)
                        {
                            App.InsertRoom(NewRoom);
                            UpdateRoomList = true;
                            App.UpdateRoomStats(RoomStats);
                            NewRoom = HotelEngine::Room();

                            RoomSection = RoomSection::None;
                        }
                    }

                    ImGui::End();

                    break;
                }
                case RoomSection::EditRoom:
                {
                    HotelEngine::RoomQuery Query;
                    Query.Id = EditRoomId;

                    static HotelEngine::Room EditRoom = App.QueryRoom(Query)[0];

                    if (EditRoomId != EditRoom.Id)
                    {
                        EditRoom = App.QueryRoom(Query)[0];
                    }

                    ImGui::Begin("Edit Room");
                    // Room Number
                    ImGui::Text("Room Number: ");
                    ImGui::SameLine();
                    ImGui::InputText("##Room Number", EditRoom.RoomNumber, sizeof(EditRoom.RoomNumber));

                    // Price
                    ImGui::Text("Price Per Day: ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Price Per Day", (int *)&EditRoom.Price);

                    // Bed Count
                    ImGui::Text("Bed Count: ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Bed Count", (int *)&EditRoom.BedCount);

                    const char *roomTypes[] =
                        {
                            "Any",
                            "Standard",
                            "Delux",
                            "Suite"};

                    int currentType = (int)EditRoom.Type;

                    ImGui::Text("Room Type: ");
                    ImGui::SameLine();
                    if (ImGui::Combo("##Room Type", &currentType, roomTypes, IM_ARRAYSIZE(roomTypes)))
                    {
                        EditRoom.Type = (HotelEngine::RoomType)currentType;
                    }

                    const char *roomStatus[] =
                        {
                            "Any",
                            "Available",
                            "Dirty",
                            "Occupied"};

                    int currentStatus = (int)EditRoom.Status;

                    ImGui::Text("Room Status: ");
                    ImGui::SameLine();
                    if (ImGui::Combo("##Room Status", &currentStatus, roomStatus, IM_ARRAYSIZE(roomStatus)))
                    {
                        EditRoom.Status = (HotelEngine::RoomStatus)currentStatus;
                    }

                    // Max Guests
                    ImGui::Text("Max Guests: ");
                    ImGui::SameLine();
                    ImGui::InputInt("##Max Guests", (int *)&EditRoom.MaxGuests);

                    // Description
                    ImGui::Text("Description: ");
                    ImGui::SameLine();
                    ImGui::InputTextMultiline(
                        "##Description",
                        EditRoom.Description,
                        sizeof(EditRoom.Description),
                        ImVec2(0, 80));

                    if (ImGui::Button("Submit"))
                    {
                        // Check online later
                        bool Valid = false;
                        Valid = true;

                        if (Valid)
                        {
                            App.ChangeRoom(EditRoom.Id, EditRoom);
                            UpdateRoomList = true;
                            RoomSection = RoomSection::None;
                            App.UpdateRoomStats(RoomStats);
                        }
                    }

                    ImGui::End();
                    break;
                }
                case RoomSection::DeleteRoom:
                {
                    
                }
                case RoomSection::None:
                {
                    break;
                }
                };

                break;
            }

            case DashboardSection::Staffs:
            {
                ImGui::Text("Staffs Section");

                // Write Staff UI here

                break;
            }

            case DashboardSection::Bookings:
            {
                ImGui::Text("Bookings Section");

                // Write Booking UI here

                break;
            }
            }

            ImGui::End();
        }

        HotelEngine::Vec4 ClearColor{0.2f, 0.2f, 0.2f, 1.0f};

        // Rendering
        ImGui::Render();
        glViewport(0, 0, 800, 600);
        glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, ClearColor.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Window.Update();
    }

    HE_PRINTLN("Window Destroyed");

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    Window.Destroy();

    std::cin.get();

    return 0;
}
