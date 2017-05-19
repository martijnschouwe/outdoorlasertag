#include "Weapon.h"
#include "Mediator.h"

#define DEBUG                                        // Allow Debug Statements (Serial)

/**
    Initializes the weapon
*/
Weapon::Weapon(byte primaryPin, byte reloadPin, Damage damageCode, short reloadDelay, Cyclic cyclic, FireMode fireMode, SoundSet soundSet, Mediator &mediator) {
    _primaryPin  = primaryPin;
    _reloadPin   = reloadPin;
    _reloading   = false;
    _damageCode  = damageCode;
    _cyclic      = cyclic;
    _fireMode    = fireMode;
    _soundSet    = soundSet;
    _rounds      = ROUNDS_PER_CLIP;
    _clips       = CLIPS;
    calculateFiringDelay(cyclic);
    _shotTimer.setDelay(_firingDelay);
    _reloadTimer.setDelay(reloadDelay);
    _mediator = &mediator;
    _mediator->registerWeapon(this);
}

/**
    Calculates the Firing Delay between each round based on the Cyclic RPM
*/
void Weapon::calculateFiringDelay(Cyclic cyclic) {
    short rpm = 0;
    switch (cyclic) {
        case Cyclic::RPM_250:
            rpm = 250;
            break;
        case Cyclic::RPM_300:
            rpm = 300;
            break;
        case Cyclic::RPM_350:
            rpm = 350;
            break;
        case Cyclic::RPM_400:
            rpm = 400;
            break;
        case Cyclic::RPM_450:
            rpm = 450;
            break;
        case Cyclic::RPM_500:
            rpm = 500;
            break;
        case Cyclic::RPM_550:
            rpm = 550;
            break;
        case Cyclic::RPM_600:
            rpm = 600;
            break;
        case Cyclic::RPM_650:
            rpm = 650;
            break;
        case Cyclic::RPM_700:
            rpm = 700;
            break;
        case Cyclic::RPM_750:
            rpm = 750;
            break;
        case Cyclic::RPM_800:
            rpm = 800;
            break;
    }

    if (_fireMode == FireMode::SemiAuto) {
        _firingDelay = 0;
    } else {
        _firingDelay = round(1000 / (rpm / 60));
    }
}

/**
    Determines whether the weapon has ammo
*/
bool Weapon::hasAmmo() {
    return _rounds > 0;
}

/**
    Fires the weapon, returns true if firing was successful
*/
bool Weapon::fire() {
    if (_shotTimer.canAction() && !isReloading()) {
        if (hasAmmo()) {
            _shotTimer.refresh();
            _rounds--;
            return true;
        } else {
        }
    }
    return false;
}

/**
    Returns the number of clips left
*/
byte Weapon::getClips() {
    return _clips;
}

/**
    Returns the number of rounds left
*/
byte Weapon::getRounds() {
    return _rounds;
}

/**
    Gives the weapon full ammo and clips
*/
void Weapon::fullAmmo() {
    _clips = CLIPS;
    _rounds = ROUNDS_PER_CLIP;
}

/**
    Removes all of the weapon's ammo and clips
*/
void Weapon::disarm() {
    _clips = 0;
    _rounds = 0;
}

/**
    Adds the given amount of rounds to the weapon
    It is possible to have more ammo than ROUNDS_PER_CLIP
*/
void Weapon::addRounds(byte rounds) {
    if (rounds >= 1 && rounds <= 100) {
        _rounds += rounds;
    }
}

/**
    Adds the given amount of clips to the weapon
*/
void Weapon::addClips(byte clips) {
    if (clips >= 1 && clips <= 100) {
        _clips += clips;
    }
}

/**
    Reloads the Weapon, returns true if the reload attempt was successful
    If you reload while a clip isn't completely empty then those
    rounds are lost.
*/
void Weapon::reload() {
    // Reload if you have at least one clip and
    // you're not already reloading
    if (!isReloading()) {
        if (_clips > 0) {
            _reloadTimer.refresh();
            _clips--;
            _reloading = true;
        } else {
        }
    }
}

/**
    Weapon Update Loop
*/
void Weapon::update() {
    if (_mediator->isActive() && _mediator->isAlive()) {
        // When buttons are pressed they are LOW due to using the
        // internal pull up resistors (INPUT_PULLUP)
        if (digitalRead(_primaryPin) == LOW) {
            if ((_fireMode == FireMode::SemiAuto && _release == true) || _fireMode == FireMode::FullAuto) {
                _mediator->fire();
                _release = false;
            }
        } else if (digitalRead(_reloadPin) == LOW) {
            _mediator->reload();
        }
    }

    // Determine if reloading is complete
    if (isReloading() && _reloadTimer.canAction()) {
        _rounds = ROUNDS_PER_CLIP;
        _reloading = false;
    }

    if (digitalRead(_primaryPin) == HIGH) {
        _release = true;
    }
}

/**
    Returns whether the weapon is reloading
*/
bool Weapon::isReloading() {
    return _reloading;
}

/**
    Returns the damageCode of the weapon
*/
Damage Weapon::getDamageCode() {
    return _damageCode;
}
