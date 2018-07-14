#ifndef COMM_H
#define COMM_H

#include <Arduino.h>
#include <RFM69.h>
#include "SoftwareSerial.h"
#include <IRremote.h>
#include "Weapon.h"
#include <ArduinoJson.h>

class Mediator;

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
        RFM69 _radio;
        byte _nodeId;
        byte _networkId;
        IRsend _irsend;                                       // IR Send
        IRrecv _irrecv = IRrecv(IR_INPUT);                    // IR Receive
        decode_results _results;                              // Decoded IR

        typedef struct {
            byte command;
            byte data;
        } Payload;
        Mediator* _mediator;
        Command* command;
        SoftwareSerial *_ble;
    public:
        Comm(byte nodeId, byte networkId, Mediator &mediator);
        void update();
        void sendShot(Damage damage);
        void sendRadioCommand(Command command, byte data);
        Command commandFromJson(ArduinoJson::JsonObject& jsonObject);

    private:
        void processRadioData(Payload payload);
        void commandMessage(byte in);
        void systemData(byte in);
        void processMessagePacket(byte command, byte data);
        void processShotPacket(unsigned long in);
        void receiveIR();
        void receiveSerial();
        void receiveRadio();
        void blinkLed();
        unsigned long createShotPacket(Damage damage);
};

#endif
