#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "Timer.h"

class Mediator;

enum class GameState {
    NotStarted,
    CountDown,
    Active,
    End,
    Pause
};

enum class GameMode {
    Deathmatch,
    TeamDeathmatch,
    CaptureTheFlag,
    SearchAndDestroy
};

class Game {
        const int TIME_LIMIT                    = 600000;    // 1000 * 60 * 10 = 10 minutes
        const byte AMMO_BOX_SIZE                = 20;        // Amount of Clips
        const byte MEDI_BOX_SIZE                = 50;        // Amount of HP
        const bool FRIENDLY_FIRE                = false;      // Is Friendly Fire Allowed

        GameState _gameState;
        Timer _countDown;
        unsigned long _startTime;
        long _timeLeft;
        Mediator* _mediator;

    public:
        void init(Mediator &mediator);
        void update();
        bool isActive();
        bool isEnd();
        bool isNotStarted();
        bool isCountDown();
        void resetClock();
        byte getAmmoBoxSize();
        byte getMediBoxSize();
        long getTimeLeft();
        void start(short delay);
        void end();
        bool friendlyFireEnabled();
        void pauseUnpause();
        static const short COUNTDOWN_DELAY             = 5000;
};

#endif
