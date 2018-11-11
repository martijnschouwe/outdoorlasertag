#include "Manager.h"
#include "Comm.h"
#include "Player.h"

Manager::Manager() {
}

void Manager::registerComm(Comm* comm) {
    _comm = comm;
}

void Manager::registerPlayer(Player* player) {
    _player = player;
}

void Manager::registerWeapon(Weapon* weapon) {
    _weapon = weapon;
}

void Manager::update() {
    _comm->update();
    _player->update();
    _weapon->update();
}

Player* Manager::getPlayer(){
    return _player;
}

Weapon* Manager::getWeapon(){
    return _weapon;
}

void Manager::fire() {
    if (_player->isAlive() && !_player->isStunned() && _weapon->fire()) {
        Serial.println("Firing shot....");
        _comm->sendInfraredBurst();
    }
}

void Manager::reload() {
    if (!_player->isStunned()) {
        _weapon->reload();
    }
}

void Manager::adminKill() {
    _player->kill();
}

void Manager::serialPrintScore(){
    _comm->serialPrintScore(_player->getHits());
}

void Manager::respawn() {
    _player->respawn();
}

void Manager::fullAmmo() {
    //_weapon->fullAmmo();
}

void Manager::explodePlayer() {
    _player->explode();
}

void Manager::fullHealth() {
    _player->fullHealth();
}

void Manager::fullArmor() {
    _player->fullArmour();
}

void Manager::stun() {
    _player->stun();
}

void Manager::addHealth(byte health) {
    _player->addHealth(health);
}

void Manager::addRounds(byte rounds) {
    //_weapon->addRounds(rounds);
}

void Manager::clipsPickup() {
    //_weapon->addClips(_game->getAmmoBoxSize());
}

void Manager::healthPickup() {
   // _player->addHealth(_game->getMediBoxSize());
}

void Manager::applyDamage(byte damageCode, byte otherTeam) {
    //_player->applyDamage(damageCode, otherTeam);
}

PlayerId Manager::getPlayerId() {
    return _player->getPlayerId();
}

TeamId Manager::getTeamId() {
    return _player->getTeamId();
}

bool Manager::isAlive() {
    return _player->isAlive();
}

byte Manager::getHealth() {
    return _player->getHealth();
}

byte Manager::getClips() {
    return _weapon->getClips();
}

byte Manager::getRounds() {
   return _weapon->getRounds();
}

String Manager::getName() {
    return _player->getName();
}

String Manager::getTeam() {
    return _player->getTeam();
}

String Manager::getNameById(byte id){
    return _player->getNameById(id);
}

//Commands functions
void Manager::GetScores(){
    _comm->serialPrintScore(_player->getHits());
}

void Manager::StopGame(){


}
void Manager::setWeapon(byte clips, byte rounds, byte cyclic){
    _weapon->setClips(clips);
    _weapon->setRounds(round);
    _weapon->setCyclic(cyclic);
}

void Manager::setHealth(byte health){
}

bool Manager::isActive(){
    return true;
}
