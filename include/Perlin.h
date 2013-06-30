#ifndef PERLIN_H
#define PERLIN_H
#include <cmath>
#include <cstdlib>

namespace Perlin
{
    float Noise1(int x);
    float Noise2(int x, int y);
    float SmoothNoise_1(int x);
    float SmoothNoise_2(int x, int y);
    float Cosine_Interpolate(float a, float b, float x);
    float InterpolatedNoise_1(float x);
    float InterpolatedNoise_2(float x, float y);
    float PerlinNoise_1D(float x, float persistence=0.5,int octave=6);
    float PerlinNoise_2D(float x, float y,float persistence=0.5,int octave=4);
}

#endif
