/* Setup the Particle Boron to use the external SIM
 * Copied straight from https://docs.particle.io/support/particle-devices-faq/electron-3rdparty-sims/#setting-up-a-3rd-party-sim-card-boron
 * More info: https://www.reddit.com/r/ProjectFi/comments/a8ccw1/support_for_lte_cat_m1/
 * Compile with: particle compile boron util/enable-google-fi.cpp --saveTo build/firmware.bin --target 1.4.1-rc.1 --verbose
 * Install with: particle flash --usb build/firmware.bin
 */

#include "Particle.h"

#include "dct.h"

SYSTEM_MODE(SEMI_AUTOMATIC);

static int count = 1;

void setup() {
    delay(15000);

    Serial.begin();
    Serial.println("Start");

    Cellular.setActiveSim(EXTERNAL_SIM);
    Cellular.setCredentials("h2g2");
    Serial.println("Credentials set");

    Cellular.on();
    Serial.println("Cell on");

    Cellular.connect();
    Serial.println("Cell connect");
}

void loop() {
    Serial.println(count++);

    CellularSignal sig = Cellular.RSSI();
    Serial.println(sig);

    CellularDevice dev;
    cellular_device_info(&dev, NULL);
    String id = spark_deviceID();
    Serial.print("Device ID: ");
    Serial.print(id.c_str());
    Serial.print("\r\n");
    Serial.print("IMEI: ");
    Serial.print(dev.imei);
    Serial.print("\r\n");
    Serial.print("ICCID: ");
    Serial.print(dev.iccid);
    Serial.print("\r\n");

    if (Cellular.ready()) {
        Serial.println("Cell connected and ready");
        delay(20000);
    }
    else
    {
        Serial.println("Cell not ready");
        delay(1000);
    }
}