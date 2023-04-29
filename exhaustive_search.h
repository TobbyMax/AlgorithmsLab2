//
// Created by ageev on 29.04.2023.
//

#ifndef ALGOLAB2_EXHAUSTIVE_SEARCH_H
#define ALGOLAB2_EXHAUSTIVE_SEARCH_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "point.h"
#include "rectangle.h"

using ui = unsigned int;

class ExhaustiveSearch {
private:
    std::vector<Rectangle> rectangles;
public:
    void prepareRectangles(const std::vector<Rectangle> &_rectangles) {
        this->rectangles = _rectangles;
    }

    ui findRectangleCrossing(const Point &p) {
        ui result = 0;
        for (const auto &rect: rectangles) {
            if (rect.start.x <= p.x && rect.start.y <= p.y && rect.finish.x > p.x && rect.finish.y > p.y) {
                result++;
            }
        }
        return result;
    }

    std::vector<ui> getResults(std::vector<Point> points) {
        std::vector<ui> results(points.size());
        for (int i = 0; i < points.size(); ++i) {
            results[i] = findRectangleCrossing(points[i]);
        }
        return results;
    }
};

#endif //ALGOLAB2_EXHAUSTIVE_SEARCH_H
