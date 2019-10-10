/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * cellular.cpp - handle connecting over cell and uploading the data to the
 *                specified server. This does *not* use the particle cloud,
 *                but instead uploads to a UDP Graphite endpoint. It's also
 *                not meant to be battery efficient.
 *
 */

#include "Particle.h"

#include "../igrill-config.h"

#if IG2C_OUTPUT_CELLULAR

#include "output/output.h"

static TCPClient clientConn;
char lineBuffer[128];

void output_setup() {
    if (Cellular.getActiveSim() != IG2C_SIM_LOCATION) {
        Cellular.setActiveSim(IG2C_SIM_LOCATION);

        if (IG2C_SIM_LOCATION == EXTERNAL_SIM) {
            Cellular.setCredentials(IG2C_CELL_SIM_APN_NAME, IG2C_CELL_SIM_APN_USERNAME, IG2C_CELL_SIM_APN_PASSWORD);
        }
    }

    Cellular.on();
    Cellular.connect();
}

void output_data(CircularBuffer<Measurement>& buffer) {
    if (!Cellular.ready()) {
        IG2C_DEBUG("Cell connection not ready, can't send data");
        return;
    }

    if (!clientConn.connected() || !clientConn.status()) {
        bool connected = clientConn.connect(IG2C_CELL_GRAPHITE_SERVER, IG2C_CELL_GRAPHITE_PORT);
        if (!connected) {
            IG2C_DEBUG("Could not connect to " IG2C_CELL_GRAPHITE_SERVER ":" IG2C_CELL_GRAPHITE_PORT);
            return;
        }
    }

    Measurement* meas = buffer.ReadElement();
    int err = 0;
    while (meas != nullptr) {
        size_t written = meas->toGraphite(lineBuffer, 128);

        if (written > 0) {
            client_conn.write(measStr.c_str(), measStr.length());
            err = client_conn.getWriteError();
            if (err != 0) {
                IG2C_DEBUG("TCP Write Error");
                IG2C_DEBUG(err);
                client_conn.stop();
                return;
            }
        }

        meas = buffer.ReadElement();
    }
}

#endif // IG2C_OUTPUT_CELLULAR