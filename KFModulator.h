/*
  KFModulator.h - An Arduino library for modulating voltage levels in keyframe-style
  Created by Roland Prähofer, May 17th, 2013
  This software is public domain.
  For peaceful application only
 */

#ifndef KFModulator_h
#define KFModulator_h

#include "Arduino.h"

class KFModulator {
public:
    KFModulator();
    /* set keyframes */
    void setKFrames(unsigned long *kf,int nr);
    /* set values for the corresponding keyframes */
    void setKFValues(int *values,int nr);
    /* start modulation */
    void start(unsigned long ts);
    /* update, call from loop(), returns interpolated value */
    int update(unsigned long now);
    /* setIsLoop - turn loop on and off*/
    void setLoop(bool b);
    /* check, if modulation is running */
    bool isRunning();
private:
    unsigned long *kFrames;
    int *kfValues;
    bool loop;
    bool running;
    int nrOfKFrames;
    unsigned long modDuration;
    unsigned long startedAt;
    unsigned long loopTime;
};
#endif