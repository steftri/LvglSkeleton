#ifndef ARDUINO_COMPAT_H
#define ARDUINO_COMPAT_H

/* Arduino.h compatibility shim for desktop builds (Windows + macOS/Linux).
 * Provides Serial, millis(), delay(), and ESP32-specific FreeRTOS wrappers.
 */

#include <cstdio>
#include <cstdint>
#include <cstring>

#ifdef __cplusplus
#include <iostream>
#include <string>
#endif

/* ------------------------------------------------------------------ */
/* millis() / delay()  – platform-specific                            */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
  #include <windows.h>
  static inline uint32_t millis(void)  { return (uint32_t)GetTickCount(); }
  static inline void     delay(uint32_t ms) { Sleep(ms); }
  static inline uint32_t micros(void)  { return millis() * 1000UL; }
#else
  /* macOS / Linux (POSIX) */
  #include <time.h>
  #include <unistd.h>
  static inline uint32_t millis(void)
  {
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      return (uint32_t)(ts.tv_sec * 1000UL + ts.tv_nsec / 1000000UL);
  }
  static inline void delay(uint32_t ms) { usleep((useconds_t)ms * 1000U); }
  static inline uint32_t micros(void)
  {
      struct timespec ts;
      clock_gettime(CLOCK_MONOTONIC, &ts);
      return (uint32_t)(ts.tv_sec * 1000000UL + ts.tv_nsec / 1000UL);
  }
#endif

#ifdef __cplusplus
}
#endif


/* ------------------------------------------------------------------ */
/* Serial                                                              */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus

class HardwareSerial
{
public:
    void begin(unsigned long /*baud*/) {}

    template<typename T>
    void println(T val)
    {
        std::cout << val << "\n";
    }

    void println(void)
    {
        std::cout << "\n";
    }

    template<typename T>
    void print(T val)
    {
        std::cout << val;
    }

    void printf(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
    }
};

extern HardwareSerial Serial;

#endif /* __cplusplus */


/* ------------------------------------------------------------------ */
/* ESP32-specific FreeRTOS extensions                                  */
/* Mapped to standard FreeRTOS static task creation (core pin ignored) */
/* ------------------------------------------------------------------ */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#ifndef xTaskCreateStaticPinnedToCore
#define xTaskCreateStaticPinnedToCore(pvTaskCode,        \
                                       pcName,           \
                                       usStackDepth,     \
                                       pvParameters,     \
                                       uxPriority,       \
                                       puxStackBuffer,   \
                                       pxTaskBuffer,     \
                                       xCoreID)          \
    xTaskCreateStatic((pvTaskCode),                      \
                      (pcName),                          \
                      (usStackDepth),                    \
                      (pvParameters),                    \
                      (uxPriority),                      \
                      (puxStackBuffer),                  \
                      (pxTaskBuffer))
#endif


/* ------------------------------------------------------------------ */
/* String type stub (if needed by libraries)                           */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
using String = std::string;
#endif

#endif /* ARDUINO_COMPAT_H */
