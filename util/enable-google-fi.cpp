/* Setup the Particle Boron to use the external SIM
 * Copied straight from https://docs.particle.io/support/particle-devices-faq/electron-3rdparty-sims/#setting-up-a-3rd-party-sim-card-boron
 * More info: https://www.reddit.com/r/ProjectFi/comments/a8ccw1/support_for_lte_cat_m1/
 * Compile with: particle compile boron util/enable-google-fi.cpp --saveTo build/firmware.bin --target 0.9.0 --verbose
 * Install with: particle flash --usb build/firmware.bin
 */

#include "Particle.h"

#include "dct.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

static PinState ps = LOW;

void setup() {
    Cellular.setActiveSim(EXTERNAL_SIM);
    Cellular.setCredentials("h2g2");

    Cellular.on();
    Cellular.connect();

    // This is just so you know the operation is complete
    pinMode(D7, OUTPUT);
    digitalWrite(D7, HIGH);
}

void loop() {
    ps = ps == LOW ? HIGH : LOW;
    pinMode(D7, OUTPUT);
    digitalWrite(D7, ps);

    delay(Cellular.ready() ? 1000 : 250);
}