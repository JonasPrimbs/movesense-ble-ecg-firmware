#pragma once
/******************************************************************************

        Copyright (c) Suunto Oy 2012.
        All rights reserved.

******************************************************************************/
/**
        @file interpolator.h
*/
#include "common/compiler/genType.h"
#include "oswrapper/kernel.h"

enum EaseParam {
    INTERP_LINEAR,
    INTERP_EASE_IN,
    INTERP_EASE_OUT,
};

typedef struct Interpolation {
    EaseParam ease;
    uint32 duration;
    uint32 starttime;
    bool active;
} Interpolation;

void interpolator_start(Interpolation* pInterpolation, uint32 currentTime, uint32 durationMs,
                              EaseParam ease);
float interpolator_get_t(Interpolation* pInterpolation, uint32 currentTime);
bool interpolator_is_active(const Interpolation* pInterpolation);
float interpolate(float a, float b, float t);

#ifdef CPP_API

class Interpolator
{
public:
    //	Interpolator() {}
    virtual ~Interpolator() {}

    virtual void start(uint32 durationMs, EaseParam ease = INTERP_LINEAR)
    {
        interpolator_start(&mInterpolation, getCurrentTime(), durationMs, ease);
        animStart();
    }

    bool update()
    {
        float t = interpolator_get_t(&mInterpolation, getCurrentTime());

        animUpdate(t);

        bool active = interpolator_is_active(&mInterpolation);
        if (!active)
            animStop();

        return active;
    }

protected:
    virtual void animStart() {}
    virtual void animStop() {}
    virtual void animUpdate(float t) { PARAM_NOTUSED(t); }

    Interpolation mInterpolation;
    virtual uint32 getCurrentTime() { return kernelGetTickCountMs(); }
};

#endif
