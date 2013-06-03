/*
  KFModulator.h - An Arduino library for modulating voltage levels in keyframe-style
  Created by Roland Prähofer, May 17th, 2013
  This software is public domain.
  For peaceful application only
 */

#include "Arduino.h"
#include "KFModulator.h"

KFModulator::KFModulator() {
    //Serial.begin(9600);
    _loop = false;
    _running = false;
    _nrOfKFrames = 10;
    _kFrames = (unsigned long*) malloc(sizeof (unsigned long) *_nrOfKFrames);
    _kFrames[0] = 0;
    _kFrames[1] = 500;
    _kFrames[2] = 1000;
    _kfValues = (int*) malloc(sizeof (int) *_nrOfKFrames);
    _kfValues[0] = 0;
    _kfValues[1] = 255;
    _kfValues[2] = 0;
    _modDuration = 0;
    _startedAt = 0;
    _loopTime = 0;

}

/* set keyframes */
void KFModulator::setKFrames(unsigned long *kf, int nr) {
    if (this->isRunning()) {
        _running = false;
    }
    if (_kFrames) {
        free(_kFrames);
    }
    _nrOfKFrames = nr;
    _kFrames = (unsigned long*) malloc(sizeof (unsigned long) *nr);
    memcpy(&_kFrames, &kf, nr);
//    int i;
//    for (i = 0; i < nr; i++) {
//        _kFrames[i] = kf[i];
//    }
}

/* set values for the corresponding keyframes */
void KFModulator::setKFValues(int *values, int nr) {
    if (this->isRunning()) {
        _running = false;
    }
    if (_kfValues) {
        free(_kfValues);
    }
    _nrOfKFrames = nr;
    _kfValues = (int*) malloc(sizeof (int) *nr);
    memcpy(&_kfValues, &values, nr);
//    int i;
//    for (i = 0; i < nr; i++) {
//        _kfValues[i] = values[i];
//    }
}

/* start modulation */
void KFModulator::start(unsigned long ts) {
    if (!_kFrames | !_kfValues) {
        return;
    }
    _running = true;
    _startedAt = ts;
    _loopTime = ts; // will be updated after each loop
    _modDuration = this->_kFrames[_nrOfKFrames - 1] - this->_kFrames[0];
}

/* update, call from loop(), returns interpolated value */
int KFModulator::update(unsigned long now) {
    if (!_running) {
        return 0;
    }
    // time since last loop start
    int passed = (now - _loopTime);

    // stop, if now is beyond _modDuration and loop is false
    if (passed >= _modDuration) {
        if (_loop) {
            _loopTime = now;
        } else {
            _running = false;
        }
        return _kfValues[_nrOfKFrames - 1]; // set to last element
    }
    // check between which two frames we are
    int i = 0;
    for (i = 0; i < (_nrOfKFrames - 1); i++) {
        if (_kFrames[i + 1] >= passed) {
            break;
        }
    }
    // Interpolate between kFrames
    // Get the fraction of passedSinceLastFrame to lastToNextFrame
    float fraction = (float) (passed - _kFrames[i]) / (float) (_kFrames[i + 1] - _kFrames[i]);
    return (int) _kfValues[i] + ((float) fraction * (float) (_kfValues[i + 1] - _kfValues[i]));
}

/* setLoop - turn loop on and off */
void KFModulator::setLoop(bool b) {
    _loop = b;
}

/* check, if modulation is running */
bool KFModulator::isRunning() {
    return _running;
}

