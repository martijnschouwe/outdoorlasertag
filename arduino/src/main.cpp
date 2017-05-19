#include <IRremote.h>
#include <SPI.h>
#include "Comm.h"
#include "Game.h"
#include "Player.h"
#include "Timer.h"
#include "Mediator.h"

#ifndef UNIT_TEST  // IMPORTANT LINE!
                                                // Allow Debug Statements (Serial)
#define DEBUG 1
const byte IR_POWER_INDOOR                  = 0x00;
const byte IR_POWER_OUTDOOR                 = 0x01;

// Radio Consts
const byte NODEID                           = 1;
const byte NETWORKID                        = 100;

// Define PINs
//const byte AUDIO_RX_PIN                     = 0;     // Audio Player RX [Teensy 3.2 - RX1]
//const byte AUDIO_TX_PIN                     = 1;     // Audio Player TX [Teensy 3.2 - TX1]
//const byte AUDIO_BUSY_PIN                   = 2;     // Audio Player Busy
//const byte PRIMARY_PIN                      = 3;     // Primary Trigger
//const byte RELOAD_PIN                       = 4;     // Reload Button
//const byte IR_OUTPUT                        = 5;     // IR Output (Fire Shot)  [Hardcoded to PIN 5 by IRremote library]
//const byte MUZZLE_LED_PIN                   = 6;     // Muzzle LED
//const byte RADIO_MOSI_PIN                   = 7;     // RFM69 Radio MOSI [Teensy 3.2 - MOSI Alternate]
//const byte RADIO_MISO_PIN                   = 8;     // RFM69 Radio MISO [Teensy 3.2 - MISO Alternate]
//const byte RADIO_SS_PIN                     = 9;     // RFM69 Radio SS [Teensy 3.2 - SS Alternate]
//const byte RADIO_CLK_PIN                    = 14;    // RFM69 Radio CLK [Teensy 3.2 - CLK Alternate]
//const byte HIT_PIN                          = 10;     // Hit Indicator LED
//const byte PIEZO_PIN                        = 11;     // Piezo Buzzer
//const byte IR_INPUT                         = 12;    // IR Input (Receive Hit)
//const byte DISPLAY_CLK_PIN                  = 18;    // LED Display Clock Pin  [Teensy 3.2 - SDA0 I2C]
//const byte DISPLAY_DAT_PIN                  = 19;    // LED Display Data Pin   [Teensy 3.2 - SCL0 I2C]
const byte AUDIO_RX_PIN                     = 9;     // Audio Player RX [Teensy 3.2 - RX1]
const byte AUDIO_TX_PIN                     = 10;     // Audio Player TX [Teensy 3.2 - TX1]
const byte AUDIO_BUSY_PIN                   = 11;     // Audio Player Busy
const byte PRIMARY_PIN                      = 0;     // Primary Trigger
const byte RELOAD_PIN                       = 2;     // Reload Button
const byte IR_OUTPUT                        = 5;     // IR Output (Fire Shot)  [Hardcoded to PIN 5 by IRremote library]
const byte MUZZLE_LED_PIN                   = 3;     // Muzzle LED
const byte RADIO_MOSI_PIN                   = 7;     // RFM69 Radio MOSI [Teensy 3.2 - MOSI Alternate]
const byte RADIO_MISO_PIN                   = 8;     // RFM69 Radio MISO [Teensy 3.2 - MISO Alternate]
const byte RADIO_SS_PIN                     = 9;     // RFM69 Radio SS [Teensy 3.2 - SS Alternate]
const byte RADIO_CLK_PIN                    = 14;    // RFM69 Radio CLK [Teensy 3.2 - CLK Alternate]
const byte HIT_PIN                          = 4;     // Hit Indicator LED
const byte PIEZO_PIN                        = 6;     // Piezo Buzzer
const byte IR_INPUT                         = 23;    // IR Input (Receive Hit)
const byte DISPLAY_CLK_PIN                  = 18;    // LED Display Clock Pin  [Teensy 3.2 - SDA0 I2C]
const byte DISPLAY_DAT_PIN                  = 19;    // LED Display Data Pin   [Teensy 3.2 - SCL0 I2C]

const short SERIAL_BAUD_RATE                = 9600;  // Only supported UART Baud Rate is 9600

Timer newGameTimer;                                  // Used to determine whether to start a new game
Mediator mediator;
Game game;
Weapon weapon(PRIMARY_PIN, RELOAD_PIN, Damage::DAMAGE1, Weapon::PRIMARY_RELOAD_DELAY, Cyclic::RPM_400, FireMode::FullAuto, SoundSet::MilSim, mediator);
Player player(TeamId::Blue, PlayerId::GIZMO, game.friendlyFireEnabled(), mediator);
Comm comm(NODEID, NETWORKID, mediator);

/**
    Checks if the user has pressed the Primary, Secondary or Reload buttons
    and calls the corresponding method
*/
void checkInput() {
    if (game.isNotStarted()) {
        if (digitalRead(PRIMARY_PIN) == LOW) {
            if (newGameTimer.canAction()) {
                mediator.startGameWithDelay();
            }
        } else {
            newGameTimer.refresh();
        }
    }
}

/**
    Setup Program
*/
void setup() {
#ifdef DEBUG
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println(F("setup"));
#endif

    // INPUT PINS
    // Enable the Interal Pull-Up Resistors
    pinMode(PRIMARY_PIN, INPUT_PULLUP);
    pinMode(RELOAD_PIN, INPUT_PULLUP);

    pinMode(IR_INPUT, INPUT);

    // OUTPUT PINS
    pinMode(HIT_PIN, OUTPUT);
    pinMode(MUZZLE_LED_PIN, OUTPUT);
    pinMode(PIEZO_PIN, OUTPUT);

    game.init(mediator);
    newGameTimer.setDelay(3000);
    // Add a short delay to configure the internal pullup resistors
    delay(100);
}

/**
    Main Application Loop
 **/
void loop() {
    checkInput();
    mediator.update();
}

#endif    // IMPORTANT LINE!
