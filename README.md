# outdoorlasertag
Lasertag game based on arduino and enhanced with a smartphone

**Arduino parts**  
- Arduino Uno
- YX5300 sound module: https://github.com/cefaloide/ArduinoSerialMP3Player  
https://github.com/salvadorrueda/ArduinoSerialMP3Player
- AT-09 4.0 Bluetooth module
- Status led

**Android**  
An android phone with android version 4.3+, and bluetooth  
The android phone is the controller of the game.
It is able to set game time, set gun settings, assign players to a game, and after the game read the game data.

**Game logic v1**  
A game is initiated by a game controller (android)  
A game consists of multiple players and are assigned by the game controller  
A game has a time window which can be set by the game controller.  
A player of a game can hit other players and gains points  
A player of a game can get hit by other players and looses points  
When a game ends, the game data can be shown in the game controller  

**Platform.io IDE **

You can use this project with the platform.io IDE  
http://platformio.org/platformio-ide

You can run the unit test in the PROJECT_ROOT/arduino folder and start it by running `pio test`  
