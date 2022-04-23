#ifndef POINT_BUFFER_H
#define POINT_BUFFER_H


// #include <consts.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>

class Point
{
  public:
     Point(float x, float y, float z);
     Point();
     float x = 0;
     float y = 0;
     float z = 0;
};

class PointBuffer {
    public:
        Point* pop();
        bool addPoint(Point pnt);
        void addPointsFromList(const char* input);
        int32_t num_points = 0;
        PointBuffer();

    private:
        static const uint32_t NUM_POINTS_MAX = 1000;
        Point points[NUM_POINTS_MAX];
        uint32_t idx_start = 0; // points to first entry
        uint32_t idx_end = 0; // points one after last entry
};

#endif