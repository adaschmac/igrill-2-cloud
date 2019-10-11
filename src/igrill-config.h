/*
 * igrill-2-cloud: Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 */

#ifndef IG2C_CONFIG
#define IG2C_CONFIG

// Only one of these can be set
#define IG2C_OUTPUT_SERIAL 1
#define IG2C_OUTPUT_CELLULAR 0

/* Serial output defines                                  */
#define IG2C_SERIAL_BAUD 115200

/* Cellular output defines                                */
// Whether the internal or external SIM should be used.
// Must be 'INTERNAL_SIM' or 'EXTERNAL_SIM'
#define IG2C_SIM_LOCATION EXTERNAL_SIM

// APN info for external/3rd party networks. Ignored if
// IG2C_SIM_LOCATION is INTERNAL_SIM
#define IG2C_CELL_SIM_APN_NAME "h2g2"
#define IG2C_CELL_SIM_APN_USERNAME ""
#define IG2C_CELL_SIM_APN_PASSWORD ""

#ifndef IG2C_CELL_GRAPHITE_SERVER
#define IG2C_CELL_GRAPHITE_SERVER "fake"
#endif
#ifndef IG2C_CELL_GRAPHITE_PORT
#define IG2C_CELL_GRAPHITE_PORT 2003
#endif

/* BLE Scanner defines                                    */

// Connect to every iGrill found and upload their data. Not very friendly if
// your neighbor has one too.
#define IG2C_PROMISCUOUS 1

#define IG2C_BLE_SCAN_RESULT_MAX 30

// Scan for 1 second (why is this in tens of milliseconds?)
#define IG2C_BLE_SCAN_TIMEOUT_TENS_OF_MS 100

#define IG2C_BLE_IGRILL_PREFIX "iGrill"
#define IG2C_BLE_IGRILL_PREFIX_LEN 5

// Documentation says max 3 BLE devices
#define IG2C_MAX_BLE_DEVICES 3

/* Misc defines                                           */
// Number of measurement entries in the circular buffer
#define IG2C_MEAS_BUFFER_SIZE 20
// Mininum time a loop() call can take, in milliseconds.
#define IG2C_TICK_TIME_MS 25
// Max number of input devices
#define IG2C_MAX_INPUT_DEVICES 8

/* Debug defines                                          */

// Enable debug prints over serial
#define IG2C_ENABLE_DEBUG 1

#if IG2C_ENABLE_DEBUG
// Print debug messages for the circular buffer. Very spammy.
#define IG2C_ENABLE_DEBUG_BUFFER 0
#include "stdio.h"

# define IG2C_DEBUG_INIT() IG2C_DEBUG_INIT_BAUD(IG2C_SERIAL_BAUD)
# define IG2C_DEBUG_INIT_BAUD(baud) Serial.begin(baud)
# define IG2C_DEBUG(msg) { static char dLB[256]; size_t l = snprintf(dLB, 256, "[%lu] " msg "\r\n", millis()); Serial.write((const uint8_t*)dLB, l); }
# define IG2C_DEBUG_F(msg, ...) { static char dLB[256]; size_t l = snprintf(dLB, 256, "[%lu] " msg "\r\n", millis(), __VA_ARGS__); Serial.write((const uint8_t*)dLB, l); }
# define IG2C_DUMP_UCHAR_16(a) { \
    static char dLB[512]; \
    size_t l = snprintf(dLB, 512, "[%lu] %s [ %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X ]\r\n", \
        millis(), \
        #a, \
        a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7], \
        a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15]); \
        Serial.write((const uint8_t*)dLB, l); }
#else
# define IG2C_DEBUG_INIT()
# define IG2C_DEBUG_INIT_BAUD(baud)
# define IG2C_DEBUG(msg)
# define IG2C_DEBUG_F(msg, ...)
# define IG2C_DUMP_UCHAR_16(a)
#endif // IG2C_ENABLE_DEBUG

#endif // IG2C_CONFIG