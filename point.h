//
// Created by ageev on 29.04.2023.
//

#ifndef ALGOLAB2_POINT_H
#define ALGOLAB2_POINT_H


struct Point {
    int x;
    int y;

    Point() : x(0), y(0) {}
    Point(int x_, int y_) : x(x_), y (y_) {}
    Point(Point& p) : Point(p.x, p.y){}
    Point(Point const &p) : Point(p.x, p.y) {}
};



#endif //ALGOLAB2_POINT_H
