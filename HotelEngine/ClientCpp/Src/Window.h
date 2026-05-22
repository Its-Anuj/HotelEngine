#ifndef HOTEL_ENGINE_WINDOW_H
#define HOTEL_ENGINE_WINDOW_H

#include "Maths.h"

struct GLFWwindow;

namespace HotelEngine
{
    namespace ClientCpp
    {
        constexpr static int WINDOW_NAME_LENGTH = 256;

        class Window
        {
        public:
            Window() {}
            Window(const char *Name, int Width, int Height) {
                Init(Name, Width, Height);
            }

            ~Window() {Destroy();}

            void Init(const char *Name, int Width, int Height);
            void Update();
            void PollEvents();
            void Destroy();

            IVec2 GetSize() const { return _WindowSize; }

            bool ShouldWindowClose() const;

            void* GetRawWindow() {return _Window;}

        private:
            GLFWwindow *_Window = nullptr;
            IVec2 _WindowSize;
            char _Name[WINDOW_NAME_LENGTH] = {0};
        };

    } // namespace ClientCpp

} // namespace HotelEngine

#endif
