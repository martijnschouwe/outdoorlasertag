#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <Arduino.h>
#include "Player.h"

class Comm;
class Game;
class Player;
class Weapon;

class Mediator {
        Comm* _comm;
        Game* _game;
        Player* _player;
        Weapon* _weapon;

    public:
        Mediator();
        void registerComm(Comm* comm);
        void registerGame(Game* game);
        void registerPlayer(Player* player);
        void registerWeapon(Weapon* weapon);
        void update();
        void startGameWithDelay();
        void fire();
        void reload();
        void play(byte in);

        void adminKill();
        void pauseUnpause();
        void respawn();
        void newGameImmediate();
        void fullAmmo();
        void endGame();
        void resetClock();
        void explodePlayer();
        void fullHealth();
        void fullArmor();
        void stun();
        void disarm();
        void addHealth(byte health);
        void addRounds(byte rounds);
        void clipsPickup();
        void healthPickup();
        void applyDamage(byte damageCode, byte otherTeam);
        PlayerId getPlayerId();
        TeamId getTeamId();
        void setPlayerName(String name);
        bool isActive();
        bool isAlive();
        bool isEnd();
        byte getHealth();
        byte getClips();
        byte getRounds();
        long getTimeLeft();
        String getName();
        String getTeam();

};

#endif
