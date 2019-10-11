/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * igrill_input.cpp - connect, authenticate with, and get temperature readings from an iGrill
 *
 */

#include "igrill-config.h"
#include "input/igrill_input.h"

static const char* sChanNames[] = { "probe1", "probe2", "probe3", "probe4", "battery" };

// https://github.com/bjoernhoefer/igrill/blob/master/igrill.py
static const BleUuid sAppChallenge("64AC0002-4A4B-4B58-9F37-94D3C52FFDF7");
static const BleUuid sDeviceChallenge("64AC0003-4A4B-4B58-9F37-94D3C52FFDF7");
static const BleUuid sDeviceResponse("64AC0004-4A4B-4B58-9F37-94D3C52FFDF7");
static const BleUuid sProbeTemps[] = { BleUuid("06ef0002-2e06-4b79-9e33-fce2c42805ec"),
                                       BleUuid("06ef0004-2e06-4b79-9e33-fce2c42805ec"),
                                       BleUuid("06ef0006-2e06-4b79-9e33-fce2c42805ec"),
                                       BleUuid("06ef0008-2e06-4b79-9e33-fce2c42805ec") };
static const BleUuid sBatteryLevel("00002A19-0000-1000-8000-00805F9B34FB");

static const unsigned char sEncryptionKey[] = {0xDF, 0x33, 0xE0, 0x89, 0xF4, 0x48, 0x4E, 0x73, 0x92, 0xD4, 0xCF, 0xB9, 0x46, 0xE7, 0x85, 0xB6};

iGrillInput::iGrillInput(uint8_t inputIndex, uint32_t inputPeriodMs, BleAddress address, const String& name)
    : Input(inputIndex, inputPeriodMs),
      m_name(name),
      m_address(address),
      m_isReady(false),
      m_aesCtx(),
      m_device(),
      m_challengeBits()
{
    mbedtls_aes_init(&m_aesCtx);
    mbedtls_aes_setkey_dec(&m_aesCtx, sEncryptionKey, 256);
    mbedtls_aes_setkey_enc(&m_aesCtx, sEncryptionKey, 256);

    memset(m_challengeBits, 0, 16);
    for (int i=0; i<8; ++i) {
        m_challengeBits[i] = (unsigned char)random(256);
    }

    m_device = BLE.connect(m_address);

    if (m_device.connected()) {
        IG2C_DEBUG_F("Device '%s', connected, attempting auth...", m_name.c_str());
        Authenticate();
    }
}

iGrillInput::~iGrillInput() {
    if (m_device.connected()) {
        BLE.disconnect(m_device);
    }
}

bool iGrillInput::IsReady()  {
    return m_isReady;
}

size_t iGrillInput::GetDeviceName(char* buffer, size_t max_len) {
    snprintf(buffer, max_len, "%s_%d", m_name.c_str(), m_index);
    return strlen(buffer);
}

size_t iGrillInput::GetChannelName(uint8_t chanIndex, char* buffer, size_t max_len) {
    strncpy(buffer, sChanNames[chanIndex], max_len);
    return strlen(buffer);
}

void onDeviceChallengeReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
    IG2C_DEBUG_F("onDeviceChallengeReceived called with %u bytes of data", len);

    iGrillInput* grillIn = (iGrillInput*)context;
    unsigned char deviceEncryptedChallengeBits[16] = {'\0'};
    unsigned char deviceDecryptedChallengeBits[16] = {'\0'};
    unsigned char deviceDecryptedResponseBits[16] = {'\0'};
    unsigned char deviceEncryptedResponseBits[16] = {'\0'};

    BleCharacteristic deviceResponse;

    if (!grillIn->m_device.getCharacteristicByUUID(deviceResponse, sDeviceResponse)) {
        IG2C_DEBUG("Could not get device response characteristic");
        return;
    }

    memcpy(deviceEncryptedChallengeBits, data, len);
    if (mbedtls_internal_aes_decrypt(&grillIn->m_aesCtx, deviceEncryptedChallengeBits, deviceDecryptedChallengeBits) != 0) {
        IG2C_DEBUG("AES Decryption of the device challenge failed");
        return;
    }

    if (memcmp(grillIn->m_challengeBits, deviceDecryptedChallengeBits, 8) != 0) {
        IG2C_DEBUG("Decrpyted device challenge does not match what we sent to the device");
        IG2C_DUMP_UCHAR_16(grillIn->m_challengeBits);
        IG2C_DUMP_UCHAR_16(deviceEncryptedChallengeBits);
        IG2C_DUMP_UCHAR_16(deviceDecryptedChallengeBits);
        return;
    }

    memcpy(&deviceDecryptedResponseBits[8], &deviceDecryptedChallengeBits[8], 8);
    if (mbedtls_internal_aes_encrypt(&grillIn->m_aesCtx, deviceDecryptedResponseBits, deviceEncryptedResponseBits) != 0) {
        IG2C_DEBUG("AES Encrpytion of the device response failed");
        return;
    }

    deviceResponse.setValue(deviceEncryptedResponseBits, 16, BleTxRxType::AUTO);

    IG2C_DEBUG("Challenge/response handshake passed");
    grillIn->m_isReady = true;
}

// https://github.com/bjoernhoefer/igrill/blob/master/igrill.py#L54
void iGrillInput::Authenticate() {
    BleCharacteristic appChallenge;
    BleCharacteristic deviceChallenge;


    if (!m_device.getCharacteristicByUUID(appChallenge, sAppChallenge)) {
        IG2C_DEBUG("Could not get app challenge characteristic");
        return;
    }

    if (!m_device.getCharacteristicByUUID(deviceChallenge, sDeviceChallenge)) {
        IG2C_DEBUG("Could not get device challenge characteristic");
        return;
    }

    deviceChallenge.onDataReceived(onDeviceChallengeReceived, this);

    ssize_t retVal = appChallenge.setValue(m_challengeBits, 16, BleTxRxType::AUTO);
    if (retVal != 16) {
        IG2C_DEBUG_F("appChallenge.setValue(...) returned %u instead of 16", retVal);
    }
}

size_t iGrillInput::ReadInternal(CircularBuffer<Measurement>& buffer, uint32_t currentTime) {
    (void)sProbeTemps;
    return 0;
}