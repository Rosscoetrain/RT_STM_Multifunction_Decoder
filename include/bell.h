/*
 * @file bell.h
 */

#ifndef BELL_H
#define BELL_H

class RailroadBell {
private:
    uint8_t _pin;
    uint32_t _freq;
    uint32_t _interval;
    uint32_t _strikeDuration;
    unsigned long _lastStrikeTime;
    bool _isRinging;
    bool _enabled;

public:
    RailroadBell(uint8_t pin, uint32_t freq, uint32_t interval) 
        : _pin(pin), _freq(freq), _interval(interval), 
          _strikeDuration(120), _lastStrikeTime(0), 
          _isRinging(false), _enabled(false) {}

    void begin() {
        pinMode(_pin, OUTPUT);
        noTone(_pin);
    }

    void setEnabled(bool state) {
        _enabled = state;
        if (!_enabled) {
            noTone(_pin); 
            _isRinging = false;
        }
    }

    bool isEnabled() const { return _enabled; }

    void update() {
        if (!_enabled) return;

        unsigned long currentTime = millis();

        if (!_isRinging && (currentTime - _lastStrikeTime >= _interval)) {
            tone(_pin, _freq);
            _isRinging = true;
            _lastStrikeTime = currentTime;
        }

        if (_isRinging && (currentTime - _lastStrikeTime >= _strikeDuration)) {
            noTone(_pin);
            _isRinging = false;
        }
    }
};

#endif
