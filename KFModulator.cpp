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
    loop = false;
    running = false;
    nrOfKFrames = 10;
    kFrames = (unsigned long*) malloc(sizeof (unsigned long) *nrOfKFrames);
    kFrames[0] = 0;
    kFrames[1] = 500;
    kFrames[2] = 1000;
    kfValues = (int*) malloc(sizeof (int) *nrOfKFrames);
    kfValues[0] = 0;
    kfValues[1] = 255;
    kfValues[2] = 0;
    modDuration = 0;
    startedAt = 0;
    loopTime = 0;

}

/* set keyframes */
void KFModulator::setKFrames(unsigned long *kf, int nr) {
    if (this->isRunning()) {
        running = false;
    }
    if (kFrames) {
        free(kFrames);
    }
    nrOfKFrames = nr;
    kFrames = (unsigned long*) malloc(sizeof (unsigned long) *nr);
    int i;
    for (i = 0; i < nr; i++) {
        kFrames[i] = kf[i];
    }
}

/* set values for the corresponding keyframes */
void KFModulator::setKFValues(int *values, int nr) {
    if (this->isRunning()) {
        running = false;
    }
    if (kfValues) {
        free(kfValues);
    }
    nrOfKFrames = nr;
    kfValues = (int*) malloc(sizeof (int) *nr);
    int i;
    for (i = 0; i < nr; i++) {
        kfValues[i] = values[i];
    }
}

/* start modulation */
void KFModulator::start(unsigned long ts) {
    if (!kFrames | !kfValues) {
        return;
    }
    running = true;
    startedAt = ts;
    loopTime = ts; // will be updated after each loop
    modDuration = this->kFrames[nrOfKFrames - 1] - this->kFrames[0];
}

/* update, call from loop(), returns interpolated value */
int KFModulator::update(unsigned long now) {
    if (!running) {
        return 0;
    }
    // time since last loop start
    int passed = (now - loopTime);

    // stop, if now is beyond modDuration and loop is false
    if (passed >= modDuration) {
        if (loop) {
            loopTime = now;
        } else {
            running = false;
        }
        return kfValues[nrOfKFrames - 1]; // set to last element
    }
    // check between which two frames we are
    int i = 0;
    for (i = 0; i < (nrOfKFrames - 1); i++) {
        if (kFrames[i + 1] >= passed) {
            break;
        }
    }
    // Interpolate between kFrames
    // Get the fraction of passedSinceLastFrame to lastToNextFrame
    float fraction = (float) (passed - kFrames[i]) / (float) (kFrames[i + 1] - kFrames[i]);
    return (int) kfValues[i] + ((float) fraction * (float) (kfValues[i + 1] - kfValues[i]));
}

/* setLoop - turn loop on and off */
void KFModulator::setLoop(bool b) {
    loop = b;
}

/* check, if modulation is running */
bool KFModulator::isRunning() {
    return this->running;
}
