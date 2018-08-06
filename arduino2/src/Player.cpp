#include "Timer.h"
#include "Player.h"
#define DEBUG                                        // Allow Debug Statements (Serial)

/**
    Initializes the player
*/
Player::Player(TeamId teamId, PlayerId playerId, bool friendlyFire) {
    _teamId              = teamId;
    _playerId            = playerId;
    _teamId              = teamId;
    _playerId            = playerId;
    _health              = MAX_HEALTH;
    _armour              = MAX_ARMOUR;
    _stunned             = false;
    _friendlyFireEnabled = friendlyFire;
    _playerState         = PlayerState::Alive;
    _kills               = 0;
}

/**
    Applies damage to players of the same team if FRIENDLY_FIRE is enabled
    otherwise the damage is ignored
*/
void Player::applyDamage(byte damageCode, byte otherTeam) {
    byte damage = 0;
    switch (damageCode) {
        case int(Damage::DAMAGE1):
            damage = 1;
            break;
        case int(Damage::DAMAGE2):
            damage = 2;
            break;
        case int(Damage::DAMAGE4):
            damage = 4;
            break;
        case int(Damage::DAMAGE5):
            damage = 5;
            break;
        case int(Damage::DAMAGE7):
            damage = 7;
            break;
        case int(Damage::DAMAGE10):
            damage = 10;
            break;
        case int(Damage::DAMAGE15):
            damage = 15;
            break;
        case int(Damage::DAMAGE17):
            damage = 17;
            break;
        case int(Damage::DAMAGE20):
            damage = 20;
            break;
        case int(Damage::DAMAGE25):
            damage = 25;
            break;
        case int(Damage::DAMAGE30):
            damage = 30;
            break;
        case int(Damage::DAMAGE35):
            damage = 35;
            break;
        case int(Damage::DAMAGE40):
            damage = 40;
            break;
        case int(Damage::DAMAGE50):
            damage = 50;
            break;
        case int(Damage::DAMAGE75):
            damage = 75;
            break;
        case int(Damage::DAMAGE100):
            damage = 100;
            break;
    }

#ifdef DEBUG
    Serial.print(F("applyDamage(")); Serial.print(damage, DEC); Serial.print(F(", ")); Serial.print(otherTeam, DEC); Serial.println(F(")"));
#endif
    // Always apply damage if you not from the same team otherwise check if
    // Friendly Fire is enabled first
    if (isAlive() && ((int(_teamId) == otherTeam && _friendlyFireEnabled) || int(_teamId) != otherTeam)) {
#ifdef DEBUG
        Serial.print(F("Health: ")); Serial.println(_health, DEC);
#endif
        _health -= damage;
        if (_health <= 0) {
            kill();
        }
    }
}

/**
    Returns whether the player is alive
*/
bool Player::isAlive() {
    return _playerState == PlayerState::Alive;
}

/**
    Returns the players Team ID
*/
TeamId Player::getTeamId() {
    return _teamId;
}

/**
    Returns the player's ID
*/
PlayerId Player::getPlayerId() {
    return _playerId;
}

/**
    Adds health to the player
    Any values other than 1 to 100 are ignored
*/
void Player::addHealth(byte health) {
#ifdef DEBUG
    Serial.print(F("addHealth(")); Serial.print(health, BIN); Serial.println(F(")"));
#endif
    if (health >= 1 && health <= 100) {
        _health += health;
    }
}

/**
    Returns the player's health
*/
short Player::getHealth() {
    return _health;
}

/**
    Reset the players health to its initial value
*/
void Player::fullHealth() {
#ifdef DEBUG
    Serial.println(F("fullHealth()"));
#endif
    _health = MAX_HEALTH;
}

/**
    Reset the players armour to its initial value
*/
void Player::fullArmour() {
#ifdef DEBUG
    Serial.println(F("fullArmour()"));
#endif
    _armour = MAX_ARMOUR;
}

/**
    Respawns a dead player
*/
void Player::respawn() {
#ifdef DEBUG
    Serial.println(F("respawn()"));
#endif
    if (!isAlive()) {
        _playerState = PlayerState::Alive;
        _health = MAX_HEALTH;
    }
}

/**
    Kills the player
*/
void Player::kill() {
    _health = 0;
    _playerState = PlayerState::Dead;
    Serial.println(F("Your Dead!!"));
}

void Player::explode() {
    _health = 0;
    _playerState = PlayerState::Dead;
}

/**
    Prevent the player doing anything for a few moments
*/
void Player::stun() {
#ifdef DEBUG
    Serial.println(F("stunPlayer()"));
#endif
    _stunned = true;
    _stunnedTimer.refresh();
}

/**
    Player's Update Loop
*/
void Player::update() {
    if (isAlive()) {
        if (isStunned() && _stunnedTimer.canAction()) {
            _stunned = false;
        }
    }
}

/**
    Returns whether the player is disabled
*/
bool Player::isStunned() {
    return _stunned;
}

String Player::getTeam() {
    switch (_teamId) {
        case TeamId::Red:
            return "RED";
        case TeamId::Blue:
            return "BLU";
        case TeamId::Yellow:
            return "YEL";
        case TeamId::Green:
            return "GRN";
        default:
            return "";
    }
}

String Player::getName() {
    switch (_playerId) {
        case PlayerId::JESSE:
            return "JESSE";
        case PlayerId::JAIDEN:
            return "JAIDEN";
        case PlayerId::SENN:
            return "SENN";
        case PlayerId::SARGE:
            return "SARGE";
        case PlayerId::ANGEL:
            return "ANGEL";
        case PlayerId::COSMO:
            return "COSMO";
        case PlayerId::GECKO:
            return "GECKO";
        case PlayerId::BLAZE:
            return "BLAZE";
        case PlayerId::CAMO:
            return "CAMO";
        case PlayerId::FURY:
            return "FURY";
        case PlayerId::FLASH:
            return "FLASH";
        case PlayerId::GIZMO:
            return "GIZMO";
        case PlayerId::HOMER:
            return "HOME";
        case PlayerId::STORM:
            return "STORM";
        case PlayerId::HABIT:
            return "HABIT";
        case PlayerId::CLICK:
            return "CLICK";
        case PlayerId::RONIN:
            return "RONIN";
        case PlayerId::LUCKY:
            return "LUCKY";
        case PlayerId::RADAR:
            return "RADAR";
        case PlayerId::BLADE:
            return "BLADE";
        case PlayerId::NINJA:
            return "NINJA";
        case PlayerId::MAGIC:
            return "MAGIC";
        case PlayerId::GONZO:
            return "GONZO";
        case PlayerId::COBRA:
            return "COBRA";
        case PlayerId::PAPPY:
            return "PAPPY";
        case PlayerId::RAMBO:
            return "RAMBO";
        case PlayerId::SNAKE:
            return "SNAKE";
        case PlayerId::AUDIE:
            return "AUDIE";
        case PlayerId::STING:
            return "STING";
        case PlayerId::ZEENA:
            return "ZEENA";
        case PlayerId::BUGSY:
            return "BUGSY";
        case PlayerId::VIPER:
            return "VIPER";
        case PlayerId::JEWEL:
            return "JEWEL";
        case PlayerId::GENIE:
            return "GENIE";
        case PlayerId::LOGAN:
            return "LOGAN";
        case PlayerId::RAZOR:
            return "RAZOR";
        case PlayerId::SLICK:
            return "SLICK";
        case PlayerId::VENOM:
            return "VENOM";
        case PlayerId::ROCKY:
            return "ROCKY";
        case PlayerId::SABER:
            return "SABER";
        case PlayerId::CRUSH:
            return "CRUSH";
        case PlayerId::TITAN:
            return "TITAN";
        case PlayerId::ORBIT:
            return "ORBIT";
        case PlayerId::VIXEN:
            return "VIXEN";
        case PlayerId::TANK:
            return "TANK";
        case PlayerId::ROGUE:
            return "ROGUE";
        case PlayerId::SHEIK:
            return "SHEIK";
        case PlayerId::GIZMO2:
            return "GIZMO2";
        case PlayerId::SIREN:
            return "SIREN";
        case PlayerId::DOZER:
            return "DOZER";
        case PlayerId::MICRO:
            return "MICRO";
        case PlayerId::LGT_MG:
            return "LGTMG";
        case PlayerId::HVY_MG:
            return "HVYMG";
        case PlayerId::ZOOKA:
            return "ZOOKA";
        case PlayerId::ROCKT:
            return "ROCKT";
        case PlayerId::GRNDE:
            return "GRNDE";
        case PlayerId::CLYMR:
            return "CLYMR";
        case PlayerId::MINE:
            return "MINE";
        case PlayerId::BOMB:
            return "BOMB";
        case PlayerId::NUKE:
            return "NUKE";
        default:
            return "";
    }
}

String Player::getNameById(byte id) {
    switch (id) {
        case 0x00:
            return "JESSE";
        case 0x01:
            return "JAIDEN";
        case 0x02:
            return "SENN";
        default:
            return "";
    }
}

byte Player::getKills() {
    return _kills;
}

void Player::incrementKills() {
    _kills++;
}
