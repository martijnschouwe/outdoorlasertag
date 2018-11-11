#ifndef MANAGER_H
#define MANAGER_H

#include <Arduino.h>
#include "Player.h"
#include "Comm.h"
#include "Weapon.h"

class Comm;
class Player;
class Weapon;

class Manager {
        Comm* _comm;
        Player* _player;
        Weapon* _weapon;
    public:
        Manager();
        void registerComm(Comm* comm);
        void registerPlayer(Player* player);
        void registerWeapon(Weapon* weapon);
        
        void update();
        void startGameWithDelay();
        void fire();
        void reload();

        void adminKill();
        void pauseUnpause();
        void respawn();
        void fullAmmo();
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
        void sendInfraredBurst();
        void receiveIR();

        bool isActive();
        bool isAlive();
        bool isEnd();
        byte getHealth();
        byte getClips();
        byte getRounds();
        long getTimeLeft();
        String getName();
        String getNameById(byte id);
        String getTeam();
        Player* getPlayer();
        Weapon* getWeapon();
        void incrementKills(byte senderId);
        void serialPrintScore();

        //Commands functions
        void GetScores();
        void StopGame();
        void setWeapon(byte clips, byte rounds, byte cyclic);
        void setHealth(byte health);
};

#endif
