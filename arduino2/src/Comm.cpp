#include "Comm.h"
#include <ArduinoJson.h>
#include <BluetoothSerial.h>
#include <IRLibRecvPCI.h>
#include <IRLibSendBase.h>    //We need the base code
#include <IRLibDecodeBase.h>
#include <IRLib_P02_Sony.h>
#include <IRLibCombo.h>
#include "Manager.h"
#include "Player.h"

#define DEBUG                                        // Allow Debug Statements (Serial)

/**
    Initializes the Comm
*/
Comm::Comm(BluetoothSerial &btserial, Manager &manager, IRrecvPCI &irReceiver) {
    _btserial = &btserial;
    _manager = &manager;
    _manager->registerComm(this);       
    _irReceiver = &irReceiver;
    _irReceiver->enableIRIn(); // Start the receiver
}

/**
    Receives ble json data and handles the message
*/
void Comm::receiveBLESerial() {
    if (_btserial->available() > 0) {
      StaticJsonBuffer<200> jsonBuffer;
      //Serial.println(_btserial->readString());
      JsonObject& json = jsonBuffer.parseObject(_btserial->readString());

    //Payload _serialData = *(Payload*)json;
    // Payload payload;
    // payload.command = (byte)commandFromJson(json);
    // if(json.success() && json.containsKey("command")) {
    //   processMessagePacket(payload.command , payload.data);
    // }
      commandMessage(int(commandFromJson(json)));
    #ifdef DEBUG
        Serial.println("**** Received command");
        Serial.println(int(commandFromJson(json)));
    #endif
  }
}

void Comm::update() {
    receiveBLESerial();
}

/**
    Generates a shot packet containing the Players Team ID,
    the Players ID and the damage
*/
unsigned long Comm::createShotPacket(byte damage) {
    unsigned long shotPacket = (int(_manager->getPlayerId()) << PLAYER_OFFSET) | (int(_manager->getTeamId()) << TEAM_OFFSET) | int(damage);
#ifdef DEBUG
    Serial.print(F("createShotPacket(")); Serial.print(shotPacket, BIN); Serial.println(F(")"));
#endif
    return shotPacket;
}

void Comm::sendInfraredBurst(){
    Serial.println("Send infrared burst");
   // irsend.send(rawData,RAW_DATA_LEN,36);//Pass the buffer,length, optionally frequency
   irsend.send(SONY,createShotPacket(_manager->getWeapon()->getDamageCode()), 20);
   Serial.println("Infrared burst sent");
}

void Comm::receiveIR() {
    if(_manager->getPlayer()->isAlive()){
        if (_irReceiver->getResults()) { 
            _irReceiver->disableIRIn();
            myDecoder.decode();           //Decode it
            //myDecoder.dumpResults(true);  //Now print results. Use false for less detail
            processShotPacket(myDecoder.value);
            _manager->getPlayer()->stun();
            delay(500);
            _irReceiver->enableIRIn();      //Restart receiver
        }
    }
}



Command Comm::commandFromJson(JsonObject &jsonObject)
{
  // extract args from request
  const char* command = jsonObject["command"];
  const char* data = jsonObject["data"];
  Serial.println("JSON command value:  ");
  Serial.println(command);
  // convert to state
  Command currentCommand;

    if (strcmp(command, "AdminKill") == 0)
    {
      currentCommand = Command::AdminKill;
    }
    else if (strcmp(command, "PauseUnpause") == 0)
    {
      currentCommand = Command::PauseUnpause;
    }
    else if (strcmp(command, "StartGameDelay") == 0)
    {
      currentCommand = Command::StartGameDelay;
    }
    else if (strcmp(command, "RestoreDefaults") == 0)
    {
      currentCommand = Command::RestoreDefaults;
    }
    else if (strcmp(command, "Respawn") == 0)
    {
      currentCommand = Command::Respawn;
    }
    else if (strcmp(command, "NewGameImmediate") == 0)
    {
      currentCommand = Command::NewGameImmediate;
    }
    else if (strcmp(command, "FullAmmo") == 0)
    {
      currentCommand = Command::FullAmmo;
    }
    else if (strcmp(command, "EndGame") == 0)
    {
      currentCommand = Command::EndGame;
    }
    else if (strcmp(command, "ResetClock") == 0)
    {
      currentCommand = Command::ResetClock;
    }
    else if (strcmp(command, "InitializePlayer") == 0)
    {
      currentCommand = Command::InitializePlayer;
    }
    else if (strcmp(command, "ExplodePlayer") == 0)
    {
      currentCommand = Command::ExplodePlayer;
    }
    else if (strcmp(command, "NewGameReady") == 0)
    {
      currentCommand = Command::NewGameReady;
    }
    else if (strcmp(command, "FullHealth") == 0)
    {
      currentCommand = Command::FullHealth;
    }
    else if (strcmp(command, "FullArmor") == 0)
    {
      currentCommand = Command::FullArmor;
    }
    else if (strcmp(command, "ClearScores") == 0)
    {
      currentCommand = Command::ClearScores;
    }
    else if (strcmp(command, "TestSensors") == 0)
    {
      currentCommand = Command::TestSensors;
    }
    else if (strcmp(command, "StunPlayer") == 0)
    {
      currentCommand = Command::StunPlayer;
    }
    else if (strcmp(command, "DisarmPlayer") == 0)
    {
      currentCommand = Command::DisarmPlayer;
    }
    else if (strcmp(command, "Fire") == 0)
    {
      currentCommand = Command::Fire;
    }
    else if (strcmp(command, "Reload") == 0)
    {
      currentCommand = Command::Reload;
    }
    return currentCommand;
  }
/**
    Process Command Message
*/
void Comm::commandMessage(byte in) {
#ifdef DEBUG
    Serial.print(F("commandMessage(")); Serial.print(in, BIN); Serial.println(F(")"));
#endif
    switch (in) {
        case int(Command::AdminKill):
            //process admin kill
            Serial.println("Booom! youre dead! muahahaha");
            break;
        case int(Command::GetScores):
            //_manager->printScores();
            break;    
        case int(Command::PauseUnpause):

            break;
        case int(Command::StartGameDelay):
            break;
        case int(Command::RestoreDefaults):
            // TODO: Resets a tagger to its default (factory settings)
            break;
        case int(Command::Respawn):
        
            break;
        case int(Command::NewGameImmediate):
            break;
        case int(Command::FullAmmo):
            break;
        case int(Command::EndGame):
            break;
        case int(Command::ResetClock):
            break;
        case int(Command::InitializePlayer):
            // TODO: Resets to default values
            break;
        case int(Command::ExplodePlayer):
            break;
        case int(Command::NewGameReady):
            // TODO: Prepares device for the 0x02 start game command
            break;
        case int(Command::FullHealth):
            break;
        case int(Command::FullArmor):
            break;
        case int(Command::ClearScores):
            // TODO: Resets all scoring data
            break;
        case int(Command::TestSensors):
            // TODO: Flash the sensor LED
            break;
        case int(Command::StunPlayer):
            break;
        case int(Command::DisarmPlayer):
            break;
        case int(Command::Fire):
            break;
        case int(Command::Reload):
            break;
        default:
#ifdef DEBUG
            Serial.println("commandMessage() - Near Miss");
#endif
            break;
    }
}

/**
    System Data
*/
void Comm::systemData(byte in) {
#ifdef DEBUG
    Serial.print(F("systemData(")); Serial.print(in, BIN); Serial.println(F(")"));
#endif
    // TODO
    switch (in) {
        case int(SystemData::CloningData):
            // TODO
            break;
        case int(SystemData::ScoreDataPart1):
            // TODO
            break;
        case int(SystemData::ScoreDataPart2):
            // TODO
            break;
        case int(SystemData::ScoreDataPart3):
            // TODO
            break;
    }
}

/**
    Process Message Packet
*/
void Comm::processMessagePacket(byte command, byte data) {
#ifdef DEBUG
    Serial.print(F("processMessagePacket(")); Serial.print(command, BIN); Serial.print(data, BIN); Serial.println(F(")"));
#endif
    switch (command) {
        case ADD_HEALTH:
            break;
        case ADD_ROUNDS:
            break;
        case COMMAND:
            commandMessage(data);
            break;
        case SYSTEM_DATA:
            systemData(data);
            break;
        case CLIPS_PICKUP:
            if (data >= 0 && data <= 15) {
                //_mediator->clipsPickup();
            }
            break;
        case HEALTH_PICKUP:
            if (data >= 0 && data <= 15) {
            }
    }

}

void Comm::blinkLed() {
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);
  delay(500);
}

void Comm::serialPrintScore(vector<Hit> hits){
     _btserial->println(F("You were hit by the following players: ")); 
    for (Hit hit : hits)
    {
        _btserial->println(_manager->getNameById(static_cast<int>(hit.playerId)));
    }
}