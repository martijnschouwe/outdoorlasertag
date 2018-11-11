#include "Weapon.h"
#include "Manager.h"

#define DEBUG                                        // Allow Debug Statements (Serial)

/**
    Initializes the weapon
*/
Weapon::Weapon(byte primaryPin, byte reloadPin, Manager &manager) {
    _primaryPin  = primaryPin;
    _reloadPin   = reloadPin;
    _reloading   = false;
    _damageCode  = DAMAGE;
    _cyclic      = CYCLIC;
    _fireMode    = int(FireMode::FullAuto);
    _rounds      = ROUNDS_PER_CLIP;
    _clips       = CLIPS;
    _reloadDelay = DEFAULT_RELOAD_DELAY;
    calculateFiringDelay(_cyclic);
    _shotTimer.setDelay(firingDelay);
    _reloadTimer.setDelay(_reloadDelay);
    _manager = &manager;
    _manager->registerWeapon(this);
}

/**
    Calculates the Firing Delay between each round based on the Cyclic RPM
*/
void Weapon::calculateFiringDelay(byte cyclic) {
    short rpm = 0;
    

    if (_fireMode == int(FireMode::SemiAuto)) {
        _firingDelay = 0;
    } else {
        _firingDelay = round(1000 / (rpm / 60));
    }
    Serial.println("Firing delay set to ");Serial.print(_firingDelay);
}

byte Weapon::getDamageCode(){
    return _damageCode;
}

/**
    Determines whether the weapon has ammo
*/
bool Weapon::hasAmmo() {
    return _rounds > 0;
}

bool Weapon::hasClips() {
    return _clips > 0;
}

/**
    Fires the weapon, returns true if firing was successful
*/
bool Weapon::fire() {
    if (_shotTimer.canAction() && !isReloading()) {
        if (hasAmmo()) {
            _shotTimer.refresh();
            _rounds--;
            Serial.println("Rounds left: ");Serial.print(_rounds);
            return true;
        } else {
            Serial.println("Empty Clip....reload first");
           // _manager->play(SoundFX::EmptyChamber);
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

void Weapon::setRounds(byte rounds) {
    _rounds = rounds;
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
    if (!isReloading() && getRounds() == 0) {
        Serial.println("Empty Clip....reloading");
        Serial.println("Clips left:");Serial.print(_clips);
        if (_clips > 0) {
            _reloadTimer.refresh();
            _clips--;
            Serial.println("Clips left after reload:");Serial.print(_clips);
            _reloading = true;
           // _mediator->play(SoundFX::StartReload);
        } else {
            //_mediator->play(SoundFX::Buzz);
        }
    }
    else{
         Serial.println("Reload: No need to reload....");
    }
}

/**
    Weapon Update Loop
*/
void Weapon::update() {
    if (_manager->isActive() && _manager->isAlive()) {
        // When buttons are pressed they are LOW due to using the
        // internal pull up resistors (INPUT_PULLUP)
        if (digitalRead(_primaryPin) == LOW) {
            if ((_fireMode == byte(FireMode::SemiAuto) && _release == true) || _fireMode == byte(FireMode::FullAuto)) {
                _manager->fire();
                _release = false;
            }
        } else if (digitalRead(_reloadPin) == LOW) {
            _manager->reload();
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