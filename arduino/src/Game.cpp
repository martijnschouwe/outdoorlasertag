#include "Game.h"
#include "Mediator.h"

#define DEBUG                                        // Allow Debug Statements (Serial)

/**
    Initializes game variables
*/
void Game::init(Mediator &mediator) {
#ifdef DEBUG
    Serial.println(F("configureGame"));
#endif
    _startTime = 0;
    _timeLeft = TIME_LIMIT;
    _gameState = GameState::NotStarted;
    _countDown.setDelay(COUNTDOWN_DELAY);
    _mediator = &mediator;
    _mediator->registerGame(this);
}

/**
    Returns the number of clips in the Ammo Box
*/
byte Game::getAmmoBoxSize() {
    return AMMO_BOX_SIZE;
}

/**
    Returns the amount of health the Medi Box heals
*/
byte Game::getMediBoxSize() {
    return MEDI_BOX_SIZE;
}

/**
    Returns the time left in the game
*/
long Game::getTimeLeft() {
    return _timeLeft;
}

/**
    Game update loop
*/
void Game::update() {
    if (isCountDown() && _countDown.canAction()) {
        _gameState = GameState::Active;
        _startTime = millis();
    }

    if (isActive()) {
        _timeLeft = TIME_LIMIT - (millis() - _startTime);
        if (_timeLeft <= 0) {
            _gameState = GameState::End;
        }
    }
}

/**
    Reset a players clock to TIME_LIMIT
*/
void Game::resetClock() {
#ifdef DEBUG
    Serial.println(F("resetClock()"));
#endif
    _startTime = millis();
}

/**
    Starts the game
*/
void Game::start(short delay) {
    _countDown.refresh();
    _gameState = GameState::CountDown;
}

/**
    Ends the game
*/
void Game::end() {
    _gameState = GameState::End;
}

/**
    Returns true if the game is still active
*/
bool Game::isActive() {
    return _gameState == GameState::Active;
}

bool Game::isEnd() {
    return _gameState == GameState::End;
}

bool Game::isNotStarted() {
    return _gameState == GameState::NotStarted;
}

bool Game::isCountDown() {
    return _gameState == GameState::CountDown;
}

/**
    Returns true if friendly fire is enabled
*/
bool Game::friendlyFireEnabled() {
    return FRIENDLY_FIRE;
}

void Game::pauseUnpause() {
    if (_gameState == GameState::Active) {
        _gameState = GameState::Pause;
    } else if (_gameState == GameState::Pause) {
        _gameState = GameState::Active;
    }
}
