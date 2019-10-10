/*
 * igrill-2-cloud: Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 */

#ifndef IG2C_CONFIG
#define IG2C_CONFIG

#define IG2C_SYSTEM_MODE MANUAL

// Only one of these can be set
#define IG2C_OUTPUT_SERIAL 1
#define IG2C_OUTPUT_CELLULAR 0

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

#define IG2C_SERIAL_BAUD 9600

// Enable debug prints over serial
#define IG2C_DEBUG 1

// Connect to every iGrill found and upload their data. Not very friendly if
// your neighbor has one too.
#define IG2C_PROMISCUOUS 1

#if IG2C_DEBUG
# define DEBUG_INIT() DEBUG_INIT(IG2C_SERIAL_BAUD)
# define DEBUG_INIT(baud) Serial.begin(baud)
# define DEBUG(msg) \
    Serial.print("["); \
    Serial.print(System.millis()); \
    Serial.print("] "); \
    Serial.println(msg)
#else
# define DEBUG_INIT()
# define DEBUG_INIT(baud)
# define DEBUG(msg)
#endif IG2C_DEBUG

#endif // IG2C_CONFIG