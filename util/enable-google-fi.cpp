/* Setup the Particle Boron to use the external SIM
 * Copied straight from https://docs.particle.io/support/particle-devices-faq/electron-3rdparty-sims/#setting-up-a-3rd-party-sim-card-boron
 * More info: https://www.reddit.com/r/ProjectFi/comments/a8ccw1/support_for_lte_cat_m1/
 * Compile with: particle compile boron util/enable-google-fi.cpp --saveTo build/firmware.bin --target 0.9.0 --verbose
 * Install with: particle flash --usb build/firmware.bin
 */

#include "Particle.h"

#include "dct.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

void setup() {
    Cellular.setActiveSim(EXTERNAL_SIM);
    Cellular.setCredentials("h2g2");

    // This clears the setup done flag on brand new devices so it won't stay in listening mode
    const uint8_t val = 0x01;
    dct_write_app_data(&val, DCT_SETUP_DONE_OFFSET, 1);

    // This is just so you know the operation is complete
    pinMode(D7, OUTPUT);
    digitalWrite(D7, HIGH);
}

void loop() {
}