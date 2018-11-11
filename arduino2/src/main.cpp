#include <BluetoothSerial.h>
#include <IRLibRecvPCI.h>
#include <IRLibSendBase.h>    //We need the base code
#include <IRLibDecodeBase.h>
#include <IRLib_P02_Sony.h>
#include <IRLibCombo.h>
#include "Arduino.h"
#include "Timer.h"
#include "Player.h"
#include "Comm.h"
#include "Manager.h"
#include "Weapon.h"

const byte TRIGGER_PIN                      = 12;     // Primary Trigger
const byte RELOAD_PIN                       = 13;     // Reload Button
const byte MUZZLE_LED_PIN                   = 8;     // Muzzle LED
const byte STATUS_LED_PIN                   = 9;     // Muzzle LED
const byte IR_INPUT                         = 2;    //pin2 is used for interrupt

int score = 0; //if you hit someone you get +1, if you get hit -1, in the end the one with the highest score wins

/**
 * Bluetooth settings
 * **/
/// Bluetooth module interface pins
#define BT_EN_PIN   5  // orange wire
#define BT_RXD_PIN  6  // green wire
#define BT_TXD_PIN  7  // yellow wire
#define BT_DSR_PIN  4  // blue wire (STATUS pin)

// BluetoothSerial instance
BluetoothSerial bt = BluetoothSerial(BT_RXD_PIN, BT_TXD_PIN,
                            BluetoothControlPIns()
                              .statePin(BT_DSR_PIN)
                              .enPin(BT_EN_PIN) );


IRrecvPCI irReceiver(IR_INPUT);
/**
 * Game settings
 * */
Manager manager;
Player player(TeamId::Blue, PlayerId::JAIDEN, true, manager);
Weapon weapon(TRIGGER_PIN, RELOAD_PIN, manager);
Comm comm(bt, manager, irReceiver);

const byte PLAYER_OFFSET                           = 6;
const byte TEAM_OFFSET                             = 4;

const unsigned int PACKET_TYPE                     = 0x2000;          // 10000000000000, 0 = shot packet, 1 = message packet
const unsigned int PLAYER_MASK                     = 0x1FC0;          // 01111111000000
const unsigned int SECOND_BYTE_MASK                = 0x003F;          // 00000000111111
const unsigned int TEAM_MASK                       = 0x0030;          // 00000000110000
const unsigned int DAMAGE_MASK                     = 0x000F;          // 00000000001111

const static byte MESSAGE_PACKET_SIZE              = 14;

volatile unsigned int infraRedState = 0;
bool processing = false;


void flashStatusLed(){
    digitalWrite(STATUS_LED_PIN, HIGH); 
    delay(200);
    digitalWrite(STATUS_LED_PIN, LOW); 
    delay(200);
    digitalWrite(STATUS_LED_PIN, HIGH); 
    delay(200);
    digitalWrite(STATUS_LED_PIN, LOW);
}

void statusLedHit(){
    digitalWrite(STATUS_LED_PIN, HIGH); 
    delay(1000);
    digitalWrite(STATUS_LED_PIN, LOW); 
}

void infraRedReceive(){
    flashStatusLed();
}

void writeMuzzleFlash(int buttonState) {
    if (buttonState == LOW && weapon.getRounds() > 0){
      digitalWrite(MUZZLE_LED_PIN, HIGH); 
      delay(500);     
    }else{
        digitalWrite(MUZZLE_LED_PIN, LOW);
    }
}

void readButtonStates(){
    int fireButtonState = digitalRead(TRIGGER_PIN);
    int reloadButtonState = digitalRead(RELOAD_PIN);
    writeMuzzleFlash(fireButtonState);
}


void setupPins(){
 pinMode(TRIGGER_PIN, INPUT_PULLUP); //reads 1 when gnd is connected to pin
 pinMode(RELOAD_PIN, INPUT_PULLUP); //reads 1 when gnd is connected to pin
 pinMode(MUZZLE_LED_PIN, OUTPUT);
 pinMode(STATUS_LED_PIN, OUTPUT);
}

void processShotPacket(unsigned long in) {

    byte playerId = (in & PLAYER_MASK) >> PLAYER_OFFSET;
    byte teamId = (in & TEAM_MASK) >> TEAM_OFFSET;
    byte damageCode = in & DAMAGE_MASK;
    
    //only apply damage if its not meeeeee
    if(static_cast<PlayerId>(playerId) != player.getPlayerId()){
        Serial.print(F("Je bent geraakt door: ")); Serial.print(player.getNameById(playerId));
        player.applyDamage(damageCode, teamId, playerId);
    }
}
void chkBluetoothStatus()
{
  static int bt_connected = -1;
  int bt_status;

  bt_status = bt.status();
  if (bt_connected != bt_status) {
    bt_connected = bt_status;
    Serial.println("");
    if (bt_connected) {
      Serial.println(">CONNECTED<");
    } else {
      Serial.println(">DISCONNECTED<");
    }
  }
}

void terminal()
{
  char SerialData;

  while (Serial.available() > 0)
  {
    SerialData = Serial.read();
    if (SerialData == 27)  // ESC
    {
      bt.reset();
    }
    else
    {
      Serial.print(SerialData);  // local echo
      bt.print(SerialData);
    }
  }

  while (bt.available() > 0)
  {
    Serial.print((char)bt.read());
  }
}

/**
 * Main setup
 **/
void setup() {
    Serial.begin(9600);
    bt.begin(9600);
    Serial.println("Serial setup....");
    setupPins();
    Serial.println("Pins all set...");
}

void loop() {
    // if(player.isAlive()){
    //     readButtonStates();
    //     if (irReceiver.getResults()) { 
    //         irReceiver.disableIRIn();
    //         myDecoder.decode();           //Decode it
    //         //myDecoder.dumpResults(true);  //Now print results. Use false for less detail
    //         processShotPacket(myDecoder.value);
    //         player.stun();
    //         delay(500);
    //         irReceiver.enableIRIn();      //Restart receiver
    //     }
    // }
    chkBluetoothStatus();
    //readButtonStates();
    manager.update();
    //terminal();
}


