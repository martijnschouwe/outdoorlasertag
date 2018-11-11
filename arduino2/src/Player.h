
#ifndef PLAYER_H
#define PLAYER_H
#include <StandardCplusplus.h>
#include <BluetoothSerial.h>
#include "Timer.h"
#include <vector>
#include <Arduino.h>
#include "Hit.h"
#include "TeamId.h"

class Manager;

enum class PlayerState {
    Alive,
    Dead
};

using namespace std;

class Player {
        const byte MAX_HEALTH                    = 100;   // Maximum Players Health
        const byte MAX_ARMOUR                    = 10;    // Maximum Armour
        const short STUNNED_DELAY                = 5000;

        TeamId _teamId;
        PlayerId _playerId;
        int _health;
        int _armour;
        PlayerState _playerState;
        bool _friendlyFireEnabled;
        bool _stunned;
        Timer _stunnedTimer;
        byte _kills;
        Manager* _manager;    
        vector<Hit> hits;
    public:
        Player(TeamId teamId, PlayerId playerId, bool friendlyFire, Manager &manager);
        void update();
        void applyDamage(byte damage, byte otherTeam, byte playerId);
        void respawn();
        void kill();
        void explode();
        void stun();
        String getName();
        String getTeam();

        void addHealth(byte health);
        void fullHealth();
        void fullArmour();

        bool isAlive();
        bool isStunned();

        short getHealth();
        TeamId getTeamId();
        PlayerId getPlayerId();
        String getNameById(byte id);
        byte getKills();
        void incrementKills();
        vector<Hit> getHits();
};

#endif