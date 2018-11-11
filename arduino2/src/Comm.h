#ifndef COMM_H
#define COMM_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <StandardCplusplus.h>

#include <IRLibRecvPCI.h>
#include <IRLibSendBase.h>    //We need the base code
#include <IRLibDecodeBase.h>
#include <IRLib_P02_Sony.h>
#include <IRLibCombo.h>

#include <vector>
#include "Hit.h"

class Manager;
using namespace std;
/**
 * Communication tier for bluetooth/android
 * */
enum class Command {
    AdminKill        = 0x00,
    PauseUnpause     = 0x01,
    StartGameDelay   = 0x02,
    RestoreDefaults  = 0x03,
    Respawn          = 0x04,
    NewGameImmediate = 0x05,
    FullAmmo         = 0x06,
    EndGame          = 0x07,
    ResetClock       = 0x08,
    InitializePlayer = 0x0A,
    ExplodePlayer    = 0x0B,
    NewGameReady     = 0x0C,
    FullHealth       = 0x0D,
    FullArmor        = 0x0F,
    ClearScores      = 0x14,
    TestSensors      = 0x15,
    StunPlayer       = 0x16,
    DisarmPlayer     = 0x17,
    SetPlayerName    = 0x18,
    Fire             = 0x1A,
    Reload           = 0x1B,
    GetScores        = 0x1C,
    SetWeapon        = 0x1D,
    SetHealth        = 0x1F,
    StopGame         = 0x20,
};

enum class SystemData {
    CloningData             = 0x01,
    ScoreDataPart1          = 0x03,
    ScoreDataPart2          = 0x04,
    ScoreDataPart3          = 0x05
};

class Comm {
        const byte IR_INPUT                                = 23;
        const byte PLAYER_OFFSET                           = 6;
        const byte TEAM_OFFSET                             = 4;

        const unsigned int PACKET_TYPE                     = 0x2000;          // 10000000000000, 0 = shot packet, 1 = message packet
        const unsigned int PLAYER_MASK                     = 0x1FC0;          // 01111111000000
        const unsigned int SECOND_BYTE_MASK                = 0x003F;          // 00000000111111
        const unsigned int TEAM_MASK                       = 0x0030;          // 00000000110000
        const unsigned int DAMAGE_MASK                     = 0x000F;          // 00000000001111

        const static byte ADD_HEALTH                       = 0x80;
        const static byte ADD_ROUNDS                       = 0x81;
        const static byte COMMAND                          = 0x83;
        const static byte SYSTEM_DATA                      = 0x87;
        const static byte CLIPS_PICKUP                     = 0x8A;
        const static byte HEALTH_PICKUP                    = 0x8B;
        const static byte FLAG_PICKUP                      = 0x8C;
        // Extended Instruction Set
        const static byte PLAY_SOUND                       = 0x90;

        const static byte MESSAGE_PACKET_SIZE              = 14;

        typedef struct {
            byte command;
            byte data;
        } Payload;

        Command* command;
        BluetoothSerial* _btserial;
        IRrecvPCI* _irReceiver;
        Manager* _manager;  
        IRsend irsend;
        IRdecode myDecoder;

    public:
        Comm(BluetoothSerial &btserial, Manager &manager, IRrecvPCI &irReceiver);
        void update();
        Command commandFromJson(ArduinoJson::JsonObject& jsonObject);
        void serialPrintScore(vector<Hit> hits);
        void sendInfraredBurst();

    private:
        void commandMessage(byte in);
        void systemData(byte in);
        void processMessagePacket(byte command, byte data);
        void processShotPacket(unsigned long in);
        unsigned long createShotPacket(byte damage);
        void receiveIR();
        void receiveBLESerial();
        void blinkLed();
};

#endif