#include "Comm.h"
#include "Mediator.h"
#include <ArduinoJson.h>

#define KEY    "sampleEncryptKey"                    // Must be the same on all devices and exactly 16 characters


Comm::Comm(byte nodeId, byte networkId, Mediator &mediator) {

    _irrecv.enableIRIn();             // Start the IR Receiver
    _irrecv.blink13(true);            // Blink the Onboard LED when we receive an IR signal
    _nodeId = nodeId;
    _networkId = networkId;
    // _radio.initialize(RF69_433MHZ, _nodeId, _networkId);
    // _radio.encrypt(KEY);
    // _radio.promiscuous(false);
    _mediator = &mediator;
    _mediator->registerComm(this);
    _ble = new SoftwareSerial(10,11);
    _ble->begin(9600);

}

/**
    Receives IR Packets and decodes the message
*/
void Comm::receiveIR() {
    if (_irrecv.decode(&_results)) {
        if (_results.decode_type == SONY) {
            // Determine whether we received a Shot or Message packet
            if ((_results.value & PACKET_TYPE) == 0) {
                processShotPacket(_results.value);
            } else {
                byte firstByte  = _results.value >> PLAYER_OFFSET;
                byte secondByte = _results.value & SECOND_BYTE_MASK;
                processMessagePacket(firstByte, secondByte);
            }
        }
        _irrecv.resume();
    }
}

/**
    Receives ble json data and handles the message
*/
void Comm::receiveSerial() {
  // char reply[50];
  // int i = 0;
  // while (_ble->available()) {
  //   reply[i] = _ble->read();
  //   i += 1;
  // }
  // //end the string
  // reply[i] = '\0';
  // if(strlen(reply) > 0){
  //   Serial.println(reply);
  //   Serial.println("We have just read some data");
  // }
    if (_ble->available() > 0) {
      StaticJsonBuffer<200> jsonBuffer;
      JsonObject& json = jsonBuffer.parseObject(_ble->readString());
    //Payload _serialData = *(Payload*)json;
    // Payload payload;
    // payload.command = (byte)commandFromJson(json);
    // if(json.success() && json.containsKey("command")) {
    //   processMessagePacket(payload.command , payload.data);
    // }
      blinkLed();
    #ifdef DEBUG
        Serial.println("**** Received command");
        Serial.println(int(commandFromJson(json)));
    #endif
  }
}
/**
    Receives Radio Packets and decodes the message
*/
void Comm::receiveRadio() {
    // If we received a Radio Signal
    if (_radio.receiveDone()) {
        // If the size of the Payload is correct
        if (_radio.DATALEN == sizeof(Payload)) {
            Payload _radioData = *(Payload*)_radio.DATA;

            // If the message is specifically for me or broadcasted to all players
            if (_radio.TARGETID == _nodeId || _radio.TARGETID == RF69_BROADCAST_ADDR) {
                processMessagePacket(_radioData.command, _radioData.data);
            }
        }

        if (_radio.ACKRequested()) {
            _radio.sendACK();
        }
    }
}

void Comm::update() {
    receiveIR();
    //receiveRadio();
    receiveSerial();
}


Command Comm::commandFromJson(JsonObject &jsonObject)
{
  // extract args from request
  const char* command = jsonObject["command"];
  const char* data = jsonObject["data"];

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
            _mediator->adminKill();
            break;
        case int(Command::PauseUnpause):
            _mediator->pauseUnpause();
            break;
        case int(Command::StartGameDelay):
            _mediator->startGameWithDelay();
            break;
        case int(Command::RestoreDefaults):
            // TODO: Resets a tagger to its default (factory settings)
            break;
        case int(Command::Respawn):
            _mediator->respawn();
            break;
        case int(Command::NewGameImmediate):
            _mediator->newGameImmediate();
            break;
        case int(Command::FullAmmo):
            _mediator->fullAmmo();
            break;
        case int(Command::EndGame):
            _mediator->endGame();
            break;
        case int(Command::ResetClock):
            _mediator->resetClock();
            break;
        case int(Command::InitializePlayer):
            // TODO: Resets to default values
            break;
        case int(Command::ExplodePlayer):
            _mediator->explodePlayer();
            break;
        case int(Command::NewGameReady):
            // TODO: Prepares device for the 0x02 start game command
            break;
        case int(Command::FullHealth):
            _mediator->fullHealth();
            break;
        case int(Command::FullArmor):
            _mediator->fullArmor();
            break;
        case int(Command::ClearScores):
            // TODO: Resets all scoring data
            break;
        case int(Command::TestSensors):
            // TODO: Flash the sensor LED
            break;
        case int(Command::StunPlayer):
            _mediator->stun();
            break;
        case int(Command::DisarmPlayer):
            _mediator->disarm();
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
            _mediator->addHealth(data);
            break;
        case ADD_ROUNDS:
            _mediator->addRounds(data);
            break;
        case COMMAND:
            commandMessage(data);
            break;
        case SYSTEM_DATA:
            systemData(data);
            break;
        case CLIPS_PICKUP:
            if (data >= 0 && data <= 15) {
                _mediator->clipsPickup();
            }
            break;
        case HEALTH_PICKUP:
            if (data >= 0 && data <= 15) {
                _mediator->healthPickup();
            }
            break;
        case FLAG_PICKUP:
            if (data >= 0 && data <= 15) {
                // TODO
            }
            break;
        default:
#ifdef DEBUG
            Serial.println(F("processMessagePacket - Near Miss"));
#endif
            break;
    }
}


/**
    Process Shot Packet
*/
void Comm::processShotPacket(unsigned long in) {
#ifdef DEBUG
    Serial.print(F("processShotPacket(")); Serial.print(in, BIN); Serial.println(F(")"));
#endif
    byte playerId = (in & PLAYER_MASK) >> PLAYER_OFFSET;
    byte teamId = (in & TEAM_MASK) >> TEAM_OFFSET;
    byte damageCode = in & DAMAGE_MASK;

#ifdef DEBUG
    Serial.print(F("Player: ")); Serial.print(playerId, DEC); Serial.print(F(" - Team: ")); Serial.print(teamId, DEC); Serial.print(F(" - Damage: ")); Serial.println(damageCode, DEC);
#endif
    _mediator->applyDamage(damageCode, teamId);
}

/**
    Generates a shot packet containing the Players Team ID,
    the Players ID and the damage
*/
unsigned long Comm::createShotPacket(Damage damage) {
    unsigned long shotPacket = (int(_mediator->getPlayerId()) << PLAYER_OFFSET) | (int(_mediator->getTeamId()) << TEAM_OFFSET) | int(damage);
#ifdef DEBUG
    Serial.print(F("createShotPacket(")); Serial.print(shotPacket, BIN); Serial.println(F(")"));
#endif
    return shotPacket;
}

void Comm::blinkLed() {
  digitalWrite(13, HIGH);
  delay(2000);
  digitalWrite(13, LOW);
  delay(500);
}

void Comm::sendShot(Damage damage) {
    _irsend.sendSony(createShotPacket(damage), MESSAGE_PACKET_SIZE);
    // Sending disables IR receive so we need to re-enable it
    _irrecv.enableIRIn();
}

void Comm::sendRadioCommand(Command command, byte data) {
    Payload stuff;
    _radio.sendWithRetry(RF69_BROADCAST_ADDR, (const void*)(&stuff), sizeof(stuff), 0);
}
