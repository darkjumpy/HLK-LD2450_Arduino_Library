#include "HLK-LD2450.h"

bool HLK_LD2450::connect(int bitrate, int rx, int tx) {
  if(bitrate <= 0) {
    bitrate = 256000;
  }

  HLK_LD2450_Serial.begin(bitrate, SWSERIAL_8N1, rx, tx, false);

  if (!HLK_LD2450_Serial) radarConnected = false;
  else radarConnected = true;

  return radarConnected;
}

bool HLK_LD2450::refreshRadarData()
{
    if (HLK_LD2450_Serial.available() > 0 && radarConnected)
    {
      byte rec_buf[256] = "";
      int len = HLK_LD2450_Serial.readBytes(rec_buf, sizeof(rec_buf));

    for (int i = 0; i < len; i++)
    {
        if (rec_buf[i] == 0xAA && rec_buf[i + 1] == 0xFF && rec_buf[i + 2] == 0x03 && rec_buf[i + 3] == 0x00 && rec_buf[i + 28] == 0x55 && rec_buf[i + 29] == 0xCC)
        {
            String targetInfo = "";
            int index = i + 4;

            for (int targetCounter = 0; targetCounter < 3; targetCounter++)
            {
                if (index + 7 < len)
                {
                    RadarTarget target;
                    target.x = (int16_t)(rec_buf[index] | (rec_buf[index + 1] << 8));
                    target.y = (int16_t)(rec_buf[index + 2] | (rec_buf[index + 3] << 8));
                    target.speed = (int16_t)(rec_buf[index + 4] | (rec_buf[index + 5] << 8));
                    target.resolution = (uint16_t)(rec_buf[index + 6] | (rec_buf[index + 7] << 8));

                    if (rec_buf[index + 1] & 0x80)
                        target.x -= 0x8000;
                    else
                        target.x = -target.x;
                    if (rec_buf[index + 3] & 0x80)
                        target.y -= 0x8000;
                    else
                        target.y = -target.y;
                    if (rec_buf[index + 5] & 0x80)
                        target.speed -= 0x8000;
                    else
                        target.speed = -target.speed;

                    radarTargets[targetCounter].id = targetCounter + 1;
                    radarTargets[targetCounter].x = target.x;
                    radarTargets[targetCounter].y = target.y;
                    radarTargets[targetCounter].speed = target.speed;
                    radarTargets[targetCounter].resolution = target.resolution;

                    index += 8;
                }
            }
            i = index;
        }
    }
    return true;
    }
  return false;
}

void HLK_LD2450::sendCommand(char *commandStr, char *commandValue, int commandValueLen){
    // Start frame
    HLK_LD2450_Serial.write(0xFD);
    HLK_LD2450_Serial.write(0xFC);
    HLK_LD2450_Serial.write(0xFB);
    HLK_LD2450_Serial.write(0xFA);

    // Command
    int len = 2;
    if (commandValue != nullptr)
      len += commandValueLen;
    HLK_LD2450_Serial.write(lowByte(len));
    HLK_LD2450_Serial.write(highByte(len));

    HLK_LD2450_Serial.write(commandStr[0]);
    HLK_LD2450_Serial.write(commandStr[1]);

    int index = 256000;
    if (commandValue != nullptr) {
      for (int i = 0; i < commandValueLen; i++) {
        HLK_LD2450_Serial.write(commandValue[i]);
      }
    }

    // End frame
    HLK_LD2450_Serial.write(0x04);
    HLK_LD2450_Serial.write(0x03);
    HLK_LD2450_Serial.write(0x02);
    HLK_LD2450_Serial.write(0x01);
    delay(100);
}

void HLK_LD2450::setConfigMode(bool enable){
    char cmd[2] = {enable ? (char)0xFF : (char)0xFE, 0x00};
    char value[2] = {0x01, 0x00};
    sendCommand(cmd, enable ? value : nullptr, enable ? 2 : 0);
}

  void HLK_LD2450::setSingle() {
    char cmd[2] = {0x80, 0x00};
    setConfigMode(true);
    sendCommand(cmd, nullptr, 0);
    setConfigMode(false);
  }

  void HLK_LD2450::setMultiple() {
    char cmd[2] = {0x90, 0x00};
    setConfigMode(true);
    sendCommand(cmd, nullptr, 0);
    setConfigMode(false);
  }

  void HLK_LD2450::getFwVersion() {
    char cmd[2] = {0xA0, 0x00};
    setConfigMode(true);
    sendCommand(cmd, nullptr, 0);
    setConfigMode(false);
  }

  void HLK_LD2450::setBaudrate(int index) {
    char cmd[2] = {0xA1, 0x00};
    char value[2] = {(char)index, 0x00};
    setConfigMode(true);
    sendCommand(cmd, value, 2);
    setConfigMode(false);
  }

  void HLK_LD2450::factoryReset() {
    char cmd[2] = {0xA2, 0x00};
    setConfigMode(true);
    sendCommand(cmd, nullptr, 0);
    setConfigMode(false);
  }

  void HLK_LD2450::reboot() {
    char cmd[2] = {0xA3, 0x00};
    setConfigMode(true);
    sendCommand(cmd, nullptr, 0);
    setConfigMode(false);
    // Not need to exit config mode because the ra2413mt will reboot automatically
  }

  void HLK_LD2450::setBluetooth(bool enable) {
    char cmd[2] = {0xA4, 0x00};
    char value[2] = {enable? (char)0x01: (char)0x00, 0x00};
    setConfigMode(true);
    sendCommand(cmd, value, 2);
    setConfigMode(false);
  }

  void HLK_LD2450::getMacAddress() {
    char cmd[2] = {0xA5, 0x00};
    char value[2] = {0x01, 0x00};
    setConfigMode(true);
    sendCommand(cmd, value, 2);
    setConfigMode(false);
  }

  void HLK_LD2450::getZone() {
    char cmd[2] = {0xC1, 0x00};
    setConfigMode(true);
    sendCommand(cmd, nullptr, 0);
    setConfigMode(false);
  }

  void HLK_LD2450::setZone(int type, int z1x1, int z1y1, int z1x2, int z1y2, int z2x1, int z2y1, int z2x2, int z2y2, int z3x1, int z3y1, int z3x2, int z3y2) {
    char cmd[2] = {0xC2, 0x00};
    char value[26] = {lowByte(type), highByte(type), \
        lowByte(z1x1), highByte(z1x1), lowByte(z1y1), highByte(z1y1), lowByte(z1x2), highByte(z1x2), lowByte(z1y2), highByte(z1y2), \
        lowByte(z2x1), highByte(z2x1), lowByte(z2y1), highByte(z2y1), lowByte(z2x2), highByte(z2x2), lowByte(z2y2), highByte(z2y2), \
        lowByte(z3x1), highByte(z3x1), lowByte(z3y1), highByte(z3y1), lowByte(z3x2), highByte(z3x2), lowByte(z3y2), highByte(z3y2)};
    setConfigMode(true);
    sendCommand(cmd, value, 26);
    setConfigMode(false);
  }

  void HLK_LD2450::getInfo() {
    setConfigMode(true);
    getMacAddress();
    getFwVersion();
    getZone();
    setConfigMode(false);
  }