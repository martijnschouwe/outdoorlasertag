#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
        short _delay = 0;
        unsigned long _lastAction = 0;

    public:
        void setDelay(short delay);
        bool canAction();
        void refresh();
};

#endif