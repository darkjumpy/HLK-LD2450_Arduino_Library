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