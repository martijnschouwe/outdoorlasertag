#ifndef WEAPON_H
#define WEAPON_H

#include <Arduino.h>
#include "Timer.h"

class Mediator;

enum class SoundSet {
    MilSim   = 0x00,
    SciFi    = 0x01,
    Silenced = 0x02
};

enum class FireMode {
    SemiAuto  = 0x00,
    Burst     = 0x01,
    FullAuto  = 0x02
};

enum class Cyclic {
    RPM_250  = 0x00,
    RPM_300  = 0x01,
    RPM_350  = 0x02,
    RPM_400  = 0x03,
    RPM_450  = 0x04,
    RPM_500  = 0x05,
    RPM_550  = 0x06,
    RPM_600  = 0x07,
    RPM_650  = 0x08,
    RPM_700  = 0x09,
    RPM_750  = 0x0A,
    RPM_800  = 0x0B
};

enum class Damage {
    DAMAGE1   = B0000,
    DAMAGE2   = B0001,
    DAMAGE4   = B0010,
    DAMAGE5   = B0011,
    DAMAGE7   = B0100,
    DAMAGE10  = B0101,
    DAMAGE15  = B0110,
    DAMAGE17  = B0111,
    DAMAGE20  = B1000,
    DAMAGE25  = B1001,
    DAMAGE30  = B1010,
    DAMAGE35  = B1011,
    DAMAGE40  = B1100,
    DAMAGE50  = B1101,
    DAMAGE75  = B1110,
    DAMAGE100 = B1111
};

class Weapon {
        const byte ROUNDS_PER_CLIP                  = 40;    // Range 1 to 255
        const byte CLIPS                            = 20;    // Range 1 to 200
        const bool MUZZLE_FLASH                     = true;  // Is Muzzle Flash Enabled

        bool _reloading;
        bool _release;
        byte _rounds;
        byte _clips;
        Damage _damageCode;
        SoundSet _soundSet;
        FireMode _fireMode;
        Cyclic _cyclic;
        short _firingDelay;
        short _reloadDelay;
        Timer _shotTimer;
        Timer _reloadTimer;
        Mediator* _mediator;
        byte _primaryPin;
        byte _reloadPin;

    public:
        static const short PRIMARY_RELOAD_DELAY            = 3000;  // Reload Delay of the Primary Weapon
        Weapon(byte primaryPin, byte reloadPin, Damage damageCode, short reloadDelay, Cyclic cyclic, FireMode fireMode, SoundSet soundSet, Mediator &mediator);
        Damage getDamageCode();
        void update();
        void fullAmmo();
        void disarm();
        void addRounds(byte rounds);
        void addClips(byte clips);
        bool hasAmmo();
        bool fire();
        byte getClips();
        byte getRounds();
        void reload();
        bool isReloading();

    private:
        void calculateFiringDelay(Cyclic cyclic);
};

#endif
