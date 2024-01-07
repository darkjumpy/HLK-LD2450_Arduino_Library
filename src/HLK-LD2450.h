#ifndef __HLK_LD2450_H__
#define __HLK_LD2450_H__
#include "Arduino.h"
#include <SoftwareSerial.h>

class HLK_LD2450
{
  private:

    EspSoftwareSerial::UART HLK_LD2450_Serial;

    bool radarConnected = false;

    typedef struct RadarTarget
    {
        uint16_t id;
        int16_t x;
        int16_t y;
        int16_t speed;
        uint16_t resolution;
    } RadarTarget_t;

    RadarTarget_t radarTargets[3];
    
  public:
    // Methods
    bool connect(int bitrate, int rx, int tx);
    bool refreshRadarData();

    // Radar commands
    void sendCommand(char *commandStr, char *commandValue, int commandValueLen);
    void setConfigMode(bool enable);

    void factoryReset();
    void reboot();

    void setSingle();
    void setMultiple();
    void setBluetooth(bool enable);
    void setBaudrate(int index);
    void setZone(int type, int z1x1, int z1y1, int z1x2, int z1y2, int z2x1, int z2y1, int z2x2, int z2y2, int z3x1, int z3y1, int z3x2, int z3y2);

    void getMacAddress();
    void getZone();
    void getInfo();
    void getFwVersion();

    // Getters for the first target
    int16_t getTarget1X(){return radarTargets[0].x;}
    int16_t getTarget1Y(){return radarTargets[0].y;}
    int16_t getTarget1Speed(){return radarTargets[0].speed;}
    uint16_t getTarget1Resolution(){return radarTargets[0].resolution;}

    // Getters for the second target
    int16_t getTarget2X(){return radarTargets[1].x;}
    int16_t getTarget2Y(){return radarTargets[1].y;}
    int16_t getTarget2Speed(){return radarTargets[1].speed;}
    uint16_t getTarget2Resolution(){return radarTargets[1].resolution;}

    // Getters for the third target
    int16_t getTarget3X(){return radarTargets[2].x;}
    int16_t getTarget3Y(){return radarTargets[2].y;}
    int16_t getTarget3Speed(){return radarTargets[2].speed;}
    uint16_t getTarget3Resolution(){return radarTargets[2].resolution;}
};
#endif
