
#pragma once
#ifndef ZENCONSTANTS_H_INCLUDED
#define ZENCONSTANTS_H_INCLUDED


#include <boost/math/constants/constants.hpp>
#include <atomic>

const static float ZEN_MAGIC_FLOAT_FAILURE = -98765.4321f;

/* float representation of PI*/
const static float ZEN_PI_F = 3.1415927;

/* Double representation of PI*/
const static double ZEN_PI_D = 3.141592653589793;

/* Long Double representation of PI*/
const static long double ZEN_PI_LD = 3.141592653589793238L;

/* Float representation of PI/2 (Pi divided by 2)*/
const static float ZEN_PI_OVER_2F = (3.1415927 / 2.0);

/* Double representation of PI/2 (Pi divided by 2)*/
const static double ZEN_PI_OVER_2D = (3.141592653589793 / 2.0);

/* Long Double representation of PI/2 (Pi divided by 2)*/
const static long double ZEN_PI_OVER_2LD = (3.141592653589793238L / 2.0);

using TimeValue = long long int;
using AtomicTimeValue = std::atomic<long long int>;

#define S(inText) String(inText)

#endif // ZENCONSTANTS_H_INCLUDED
