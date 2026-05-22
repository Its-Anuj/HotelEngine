#ifndef HOTEL_ENGINE_SHARED_LOG_H
#define HOTEL_ENGINE_SHARED_LOG_H

#define HE_PRINT(x)     \
    {                   \
        std::cout << x; \
    }
#define HE_PRINTLN(x)           \
    {                           \
        std::cout << x << "\n"; \
    }
#define HE_ERROR(x)                          \
    {                                        \
        std::cout << "Error: " << x << "\n"; \
    }
#define HE_ASSERT(eq, errormsg)      \
    {                                \
        if ((eq) == false)          \
        {                            \
            std::cout << "Error: " << errormsg << "\n";    \
            assert((eq)); \
        }                            \
    }

#endif