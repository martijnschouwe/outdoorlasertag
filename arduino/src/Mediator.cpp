#include "Mediator.h"
#include "Comm.h"
#include "Game.h"
#include "Player.h"
#include "Weapon.h"

Mediator::Mediator() {

}


void Mediator::registerComm(Comm* comm) {
    _comm = comm;
}


void Mediator::registerGame(Game* game) {
    _game = game;
}

void Mediator::registerPlayer(Player* player) {
    _player = player;
}

void Mediator::registerWeapon(Weapon* weapon) {
    _weapon = weapon;
}

void Mediator::update() {
    _comm->update();
    _game->update();
    _player->update();
    _weapon->update();
}

void Mediator::startGameWithDelay() {
    _game->start(Game::COUNTDOWN_DELAY);
}

void Mediator::fire() {
    if (_player->isAlive() && !_player->isStunned() && _weapon->fire()) {
        _comm->sendShot(_weapon->getDamageCode());
    }
}

void Mediator::reload() {
    if (!_player->isStunned()) {
        _weapon->reload();
    }
}


void Mediator::adminKill() {
    _player->kill();
    _game->end();
}

void Mediator::pauseUnpause() {
    _game->pauseUnpause();
}

void Mediator::respawn() {
    _player->respawn();
}

void Mediator::newGameImmediate() {
    _game->start(0);
}

void Mediator::fullAmmo() {
    _weapon->fullAmmo();
}

void Mediator::endGame() {
    _game->end();
}

void Mediator::resetClock() {
    _game->resetClock();
}

void Mediator::explodePlayer() {
    _player->explode();
    _game->end();
}

void Mediator::fullHealth() {
    _player->fullHealth();
}

void Mediator::fullArmor() {
    _player->fullArmour();
}

void Mediator::stun() {
    _player->stun();
}

void Mediator::disarm() {
    _weapon->disarm();
}

void Mediator::addHealth(byte health) {
    _player->addHealth(health);
}

void Mediator::addRounds(byte rounds) {
    _weapon->addRounds(rounds);
}

void Mediator::clipsPickup() {
    _weapon->addClips(_game->getAmmoBoxSize());
}

void Mediator::healthPickup() {

    _player->addHealth(_game->getMediBoxSize());
}

void Mediator::applyDamage(byte damageCode, byte otherTeam) {
    _player->applyDamage(damageCode, otherTeam);
}

PlayerId Mediator::getPlayerId() {
    return _player->getPlayerId();
}

TeamId Mediator::getTeamId() {
    return _player->getTeamId();
}

bool Mediator::isAlive() {
    return _player->isAlive();
}

bool Mediator::isEnd() {
    return _game->isEnd();
}

byte Mediator::getHealth() {
    return _player->getHealth();
}

byte Mediator::getClips() {
    return _weapon->getClips();
}

byte Mediator::getRounds() {
    return _weapon->getRounds();
}

long Mediator::getTimeLeft() {
    return _game->getTimeLeft();
}

String Mediator::getName() {
    return _player->getName();
}

String Mediator::getTeam() {
    return _player->getTeam();
}

bool Mediator::isActive() {
    return _game->isActive();
}
