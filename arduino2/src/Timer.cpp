#include "Timer.h"

void Timer::setDelay(short delay) {
    _delay = delay;
}

bool Timer::canAction() {
    return millis() > _lastAction + _delay;
}

void Timer::refresh() {
    _lastAction = millis();
}
