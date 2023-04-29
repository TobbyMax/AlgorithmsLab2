//
// Created by ageev on 29.04.2023.
//

#ifndef ALGOLAB2_RECTANGLE_H
#define ALGOLAB2_RECTANGLE_H

#include "point.h"

struct Rectangle {
    Point start;
    Point finish;

    Rectangle() : start(0, 0), finish(0, 0) {}
    Rectangle(Point p1, Point p2) : start(p1), finish(p2) {}
    Rectangle(int x1, int y1, int x2, int y2) : start(x1, y1), finish(x2, y2) {}
    Rectangle(Rectangle& r) : start(r.start), finish(r.finish) {}
    Rectangle(Rectangle const &r) : start(r.start), finish(r.finish) {}
};

#endif //ALGOLAB2_RECTANGLE_H
