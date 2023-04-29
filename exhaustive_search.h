//
// Created by ageev on 29.04.2023.
//

#ifndef ALGOLAB2_EXHAUSTIVE_SEARCH_H
#define ALGOLAB2_EXHAUSTIVE_SEARCH_H

#include <vector>
#include <iostream>
#include "point.h"
#include "rectangle.h"

using ui = unsigned int;

class ExhaustiveSearch {
private:
    std::vector<Rectangle>& rectangles;
public:
    void prepareRectangles(const std::vector<Rectangle>& _rectangles) {
        this->rectangles = _rectangles;
    }
    ui findRectangleCrossing(const Point& p) {
        ui result = 0;
        for (const auto& rect : rectangles) {
            if (rect.start.x <= p.x && rect.start.y <= p.y && rect.finish.x > p.x && rect.finish.y > p.y) {
                result++;
            }
        }
        return result;
    }
    std::vector<ui> getResults(int m) {
        std::vector<ui> results(m);
        for (auto& res : results) {
            Point p;
            std::cin >> p.x >> p.y;
            res = findRectangleCrossing(rectangles, p);
        }
        return results;
    }
};

#endif //ALGOLAB2_EXHAUSTIVE_SEARCH_H
