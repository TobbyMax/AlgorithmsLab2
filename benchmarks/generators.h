//
// Created by ageev on 30.04.2023.
//

#ifndef ALGOLAB2_GENERATORS_H
#define ALGOLAB2_GENERATORS_H

#include "../solvers/rectangle.h"

#define P_X 131
#define P_Y 113

unsigned long long powMod(unsigned long long base, unsigned long long exp, unsigned long long modulus) {
    if (modulus == 1) {
        return 0;
    }
    base %= modulus;
    unsigned long long result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exp >>= 1;
    }
    return result;
}

std::vector<Rectangle> generateRectangles(int n) {
    std::vector<Rectangle> rectangles(n);
    for (int i = 0; i < n; ++i) {
        rectangles[i] = Rectangle({10*i, 10*i}, {10*(2*n-i), 10*(2*n-i)});
    }
    return rectangles;
}

int getPointCoordinate(int seed, int i, int n) {
    return (int)powMod(seed * i, 31, 20 * n);
}

std::vector<Point> generatePoints(int n) {
    std::vector<Point> points(n);
    for (int i = 0; i < n; ++i) {
        points[i] = Point(getPointCoordinate(P_X, i, n),
                          getPointCoordinate(P_Y, i, n));
    }
    return points;
}

#endif //ALGOLAB2_GENERATORS_H
