# outdoorlasertag
Lasertag game based on arduino and enhanced with a smartphone

**Arduino parts**  
- Arduino uno (probably with wifi) 
- YX5300 sound module: https://github.com/cefaloide/ArduinoSerialMP3Player  
- HC-05 bluetooth module  

**Android**  
An android phone with android version 4+, and bluetooth  
The android phone is the controller of the game.
It is able to set game time, set gun settings, assign players to a game, and after the game read the game data.

**Game logic v1**  
A game is initiated by a game controller (android)
A game consists of multiple players and are assigned by the game controller
A game has a time window which can be set by the game controller.
A player of a game can hit other players and gains points
A player of a game can get hit by other players and looses points
When a game ends, the gae data can be shown in the game controller



