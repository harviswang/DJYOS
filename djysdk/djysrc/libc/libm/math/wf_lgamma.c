/* wf_lgamma.c -- float version of w_lgamma.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 *
 */

#include "stdint.h"
#include "fdlibm.h"
#include <errno.h>

extern s32 gamma_signgam;

float lgammaf(float x)
{
#ifdef _IEEE_LIBM
    return __ieee754_lgammaf_r(x,&gamma_signgam);
#else
        float y;
    struct exception exc;
        y = __ieee754_lgammaf_r(x,&gamma_signgam);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!finitef(y)&&finitef(x)) {
#ifndef HUGE_VAL
#define HUGE_VAL inf
        double inf = 0.0;

        SET_HIGH_WORD(inf,0x7ff00000);  /* set inf to infinite */
#endif
        exc.name = "lgammaf";
        exc.err = 0;
        exc.arg1 = exc.arg2 = (double)x;
            if (_LIB_VERSION == _SVID_)
               exc.retval = HUGE;
            else
               exc.retval = HUGE_VAL;
        if(floorf(x)==x&&x<=(float)0.0) {
        /* lgammaf(-integer) */
        exc.type = SING;
        if (_LIB_VERSION == _POSIX_)
           Djy_SaveLastError(EDOM);
        else if (!matherr(&exc)) {
           Djy_SaveLastError(EDOM);
        }

            } else {
        /* lgammaf(finite) overflow */
        exc.type = OVERFLOW;
                if (_LIB_VERSION == _POSIX_)
           Djy_SaveLastError(ERANGE);
                else if (!matherr(&exc)) {
                   Djy_SaveLastError(ERANGE);
        }
            }
        if (exc.err != 0)
           Djy_SaveLastError(exc.err);
            return (float)exc.retval;
        } else
            return y;
#endif
}

#ifdef _DOUBLE_IS_32BITS

double lgamma(double x)
{
    return (double) lgammaf((float) x);
}

#endif /* defined(_DOUBLE_IS_32BITS) */
