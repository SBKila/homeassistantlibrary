#include "myDebugLogger.h"

// Only compile this implementation if Debug is enabled
#if defined(ENABLE_DEBUG)

#include <stdio.h>  // Required for vsnprintf
#include <stdlib.h> // Required for malloc and free

// Define global buffer size locally (implementation detail)
#ifndef DEBUG_BUFFER_SIZE
#define DEBUG_BUFFER_SIZE 128
#endif

// Forward declaration of internal helper (private to this file)
void _internalLog(const char *module, const char *format, va_list args);

// -------------------------------------------------------------------------
// Overload 1: Handles standard strings stored in RAM (const char*)
// -------------------------------------------------------------------------
void logDebug(const char *module, const char *format, ...) {
    va_list arg;
    va_start(arg, format);
    _internalLog(module, format, arg);
    va_end(arg);
}

// -------------------------------------------------------------------------
// Overload 2: Handles strings stored in FLASH memory (F() macro)
// -------------------------------------------------------------------------
void logDebug(const char *module, const __FlashStringHelper *format, ...) {
    va_list arg;
    va_start(arg, format);
    
    PGM_P p = reinterpret_cast<PGM_P>(format);
    size_t n = strlen_P(p);
    
    char *buffer = (char*)malloc(n + 1);

    if (buffer) {
        strcpy_P(buffer, p);
        _internalLog(module, buffer, arg);
        free(buffer); 
    } else {
        MY_DEBUG_ESP_PORT.print(F("[ERROR] Log buffer allocation failed"));
    }
    
    va_end(arg);
}

// -------------------------------------------------------------------------
// Internal Logic: Formats and prints the message
// -------------------------------------------------------------------------
void _internalLog(const char *module, const char *format, va_list args) {
    // 1. Calculate readable time (uptime)
    unsigned long current_millis = millis();
    unsigned long seconds = current_millis / 1000;
    unsigned long minutes = seconds / 60;
    unsigned long hours = minutes / 60;

    // 2. Print Formatted Timestamp [HH:MM:SS.ms] and Module
    MY_DEBUG_ESP_PORT.printf("[%02lu:%02lu:%02lu.%03lu] [%s] ", 
        hours, 
        minutes % 60, 
        seconds % 60, 
        current_millis % 1000, 
        module
    );

    // OPTIMIZATION: Use static buffer to save Stack memory
    static char temp[DEBUG_BUFFER_SIZE];
    char* buffer = temp;
    
    // Try printing to the static buffer first
    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(temp, sizeof(temp), format, args_copy);
    va_end(args_copy);

    // 3. Handle buffer sizing
    if ((size_t)len >= sizeof(temp)) {
        buffer = (char*)malloc(len + 1);
        if (buffer) {
            vsnprintf(buffer, len + 1, format, args);
            MY_DEBUG_ESP_PORT.print(buffer);
            free(buffer); 
        } else {
            MY_DEBUG_ESP_PORT.print(temp); 
        }
    } else {
        MY_DEBUG_ESP_PORT.print(temp);
    }
    
    // 4. Auto newline
    // MY_DEBUG_ESP_PORT.println();
}

#endif // End ENABLE_DEBUG