#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>
#include "Weapon.h"

class Mediator;

enum class PlayerState {
    Alive,
    Dead
};

enum class TeamId {
    Red     = B00,
    Blue    = B01,
    Yellow  = B10,
    Green   = B11
};

enum class PlayerId {
    EAGLE   = 0x00,
    JOKER   = 0x01,
    RAVEN   = 0x02,
    SARGE   = 0x03,
    ANGEL   = 0x04,
    COSMO   = 0x05,
    GECKO   = 0x06,
    BLAZE   = 0x07,
    CAMO    = 0x08,
    FURY    = 0x09,
    FLASH   = 0x0A,
    GIZMO2  = 0x0B,
    HOMER   = 0x0C,
    STORM   = 0x0D,
    HABIT   = 0x0E,
    CLICK   = 0x0F,
    RONIN   = 0x10,
    LUCKY   = 0x11,
    RADAR   = 0x12,
    BLADE   = 0x13,
    NINJA   = 0x14,
    MAGIC   = 0x15,
    GONZO   = 0x16,
    COBRA   = 0x17,
    PAPPY   = 0x18,
    RAMBO   = 0x19,
    SNAKE   = 0x1A,
    AUDIE   = 0x1B,
    STING   = 0x1C,
    ZEENA   = 0x1D,
    BUGSY   = 0x1E,
    VIPER   = 0x1F,
    JEWEL   = 0x20,
    GENIE   = 0x21,
    LOGAN   = 0x22,
    RAZOR   = 0x23,
    SLICK   = 0x24,
    VENOM   = 0x25,
    ROCKY   = 0x26,
    SABER   = 0x27,
    CRUSH   = 0x28,
    TITAN   = 0x29,
    ORBIT   = 0x2A,
    VIXEN   = 0x2B,
    TANK    = 0x2C,
    ROGUE   = 0x2D,
    SHEIK   = 0x2E,
    GIZMO   = 0x2F,
    SIREN   = 0x30,
    DOZER   = 0x31,
    MICRO   = 0x32,
    LGT_MG  = 0x33,
    HVY_MG  = 0x34,
    ZOOKA   = 0x35,
    ROCKT   = 0x36,
    GRNDE   = 0x37,
    CLYMR   = 0x38,
    MINE    = 0x39,
    BOMB    = 0x3A,
    NUKE    = 0x3B
};

class Player {
        const byte MAX_HEALTH                    = 100;   // Maximum Players Health
        const byte MAX_ARMOUR                    = 10;    // Maximum Armour
        const short STUNNED_DELAY                = 5000;

        TeamId _teamId;
        PlayerId _playerId;
        String _playerName;
        int _health;
        int _armour;
        PlayerState _playerState;
        bool _friendlyFireEnabled;
        bool _stunned;
        Timer _stunnedTimer;
        Mediator* _mediator;

    public:
        Player(TeamId teamId, PlayerId playerId, bool friendlyFire, Mediator &mediator);
        void update();
        void applyDamage(byte damage, byte otherTeam);
        void respawn();
        void kill();
        void explode();
        void stun();
        String getName();
        void setPlayerName(String name);
        String getTeam();
        String getStringName();

        void addHealth(byte health);
        void fullHealth();
        void fullArmour();

        bool isAlive();
        bool isStunned();

        short getHealth();
        TeamId getTeamId();
        PlayerId getPlayerId();
};

#endif
