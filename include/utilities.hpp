#pragma once

#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <random>

#define eps 1e-6
#define INIT_RADIUS 1e-4
#define INF_FLOAT 1e38
#define MAX_TRACE_DEPTH 15
const double PI = acos(-1);

#define thread_local __thread

#include <time.h>

using namespace std;

#define RAND() drand48()

inline float clamp(float x)
{
    return x > 0 ? x : 0;
}

inline float sqr(float x)
{
    return x * x;
}

inline Vector3f generateRandomPoint()
{
    Vector3f p;
    do
    {
        p = 2.0 * Vector3f(RAND(), RAND(), RAND()) - Vector3f(1,1,1);
    } while (p.length() >= 1.0);
    return p;
}
