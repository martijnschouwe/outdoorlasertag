
#ifndef WEAPON_H
#define WEAPON_H

#include "Timer.h"
#include "Manager.h"

class Weapon {

        enum class FireMode {
            SemiAuto  = 0x00,
            Burst     = 0x01,
            FullAuto  = 0x02
        };
        /* Rounds, clips, damage and cyclic have default values but can be overridden by the android app
        **/
        const byte ROUNDS_PER_CLIP                  = 40;    // Default
        const byte CLIPS                            = 20;    // Default
        const byte CYCLIC                           = 250;   //Default
        const byte DAMAGE                           = B0101; //Default
        static const short DEFAULT_RELOAD_DELAY     = 3000;  // Reload Delay of the Primary Weapon
        const byte firingDelay                      = 240;

        byte _rounds;
        byte _clips;
        byte _cyclic;
        bool _reloading;
        bool _release;
        byte _damageCode;
        byte _fireMode;
        byte _primaryPin;
        byte _reloadPin;

        short _firingDelay;
        short _reloadDelay;
        Timer _shotTimer;
        Timer _reloadTimer;
        Manager* _manager;

    public:
        Weapon(byte primaryPin, byte reloadPin, Manager &manager);
        static const short PRIMARY_RELOAD_DELAY            = 3000;  // Reload Delay of the Primary Weapon

        void setDamageCode(byte level);
        byte getDamageCode();
        void setClips(byte clips);
        void addClips(byte clips);
        void setRounds(byte rounds);
        void addRounds(byte rounds);
        void setCyclic(byte cyclic);
        void fullAmmo();
        bool hasAmmo();
        bool fire();
        bool hasClips();
        byte getClips();
        byte getRounds();
        void reload();
        bool isReloading();
        void update();
        void disarm();

    private:
        void calculateFiringDelay(byte cyclic);
};

#endif
