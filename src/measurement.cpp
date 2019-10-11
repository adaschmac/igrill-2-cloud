/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * measurement.cpp - Basic storage element for measurement data
 *
 */

#include "igrill-config.h"
#include "input/input.h"
#include "measurement.h"

static char deviceNameBuffer[32];
static char channelNameBuffer[32];

Input** Measurement::m_inputData;

size_t Measurement::toGraphite(char* buffer, size_t max_len) {
    m_inputData[device_id]->GetDeviceName(deviceNameBuffer, 32);
    m_inputData[device_id]->GetChannelName(channel_id, channelNameBuffer, 32);

    snprintf(buffer, max_len, "%s.%s.%s %s %lu\r\n", System.deviceID().c_str(), deviceNameBuffer, channelNameBuffer, data, timestamp);
    return strlen(buffer);
}