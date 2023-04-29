//
// Created by ageev on 29.04.2023.
//

#ifndef ALGOLAB2_RECTANGLE_MAP_H
#define ALGOLAB2_RECTANGLE_MAP_H
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <algorithm>
#include "rectangle.h"

using ui = unsigned int;

class RectangleMap {
private:
    std::vector<std::vector<ui>> rectangleMap;
    std::vector<int> xCompressed;
    std::vector<int> yCompressed;
public:
    std::vector<std::vector<ui>> mapRectangles (const std::vector<Rectangle>& rectangles) {
        std::vector<std::vector<ui>> rect_map(yCompressed.size());
        for (auto& row : rect_map) {
            row = std::vector<ui>(xCompressed.size());
        }
        for (auto& r : rectangles) {
            Point start, finish;
            start.x = (int)(std::lower_bound(xCompressed.begin(), xCompressed.end(), r.start.x) - xCompressed.begin());
            start.y = (int)(std::lower_bound(yCompressed.begin(), yCompressed.end(), r.start.y) - yCompressed.begin());
            finish.x = (int)(std::lower_bound(xCompressed.begin(), xCompressed.end(), r.finish.x) - xCompressed.begin());
            finish.y = (int)(std::lower_bound(yCompressed.begin(), yCompressed.end(), r.finish.y) - yCompressed.begin());

            for (auto i = start.y; i < finish.y; i++) {
                for (auto j = start.x; j < finish.x; j++) {
                    rect_map[i][j] += 1;
                }
            }
        }
        return rect_map;
    }
    void prepareRectangles(const std::vector<Rectangle>& rectangles) {
        std::set<int> x_set;
        std::set<int> y_set;

        for (const auto& r : rectangles) {
            x_set.insert(r.start.x);
            y_set.insert(r.start.y);
            x_set.insert(r.finish.x);
            y_set.insert(r.finish.y);
        }
        xCompressed = std::vector<int>(x_set.size());
        int i = 0;
        for (auto x : x_set) {
            xCompressed[i++] = x;
        }
        yCompressed = std::vector<int>(y_set.size());
        i = 0;
        for (auto y : y_set) {
            yCompressed[i++] = y;
        }
        rectangleMap = mapRectangles(rectangles);
    }

    ui findRectangleCrossing(const Point& p) {
        auto pos_x = std::upper_bound(xCompressed.begin(), xCompressed.end(), p.x);
        if (pos_x == xCompressed.end() || pos_x == xCompressed.begin()) {
            return 0;
        }
        auto pos_y = std::upper_bound(yCompressed.begin(), yCompressed.end(), p.y);
        if (pos_y == yCompressed.end() || pos_y == yCompressed.begin()) {
            return 0;
        }
        auto y = pos_y - yCompressed.begin() - 1;
        auto x = pos_x - xCompressed.begin() - 1;
        return rectangleMap[y][x];
    }

    std::vector<ui> getResults(int m) {
        std::vector<ui> results(m);
        for (auto& res : results) {
            Point p;
            std::cin >> p.x >> p.y;
            res = findRectangleCrossing(p);
        }
        return results;
    }
};

#endif //ALGOLAB2_RECTANGLE_MAP_H
