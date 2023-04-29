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

    static int compressCoordinate(const std::vector<int>& compressed, int coordinate) {
        return (int) (std::lower_bound(compressed.begin(), compressed.end(), coordinate) - compressed.begin());
    }

    Point compressRectanglePoint(const Point &p) {
        Point mapped;
        mapped.x = compressCoordinate(xCompressed, p.x);
        mapped.y = compressCoordinate(yCompressed, p.y);
        return mapped;
    }

    void mapRectangles(const std::vector<Rectangle> &rectangles) {
        rectangleMap = std::vector<std::vector<ui>>(yCompressed.size());
        for (auto &row: rectangleMap) {
            row = std::vector<ui>(xCompressed.size());
        }
        for (auto &r: rectangles) {
            Point start = compressRectanglePoint(r.start);
            Point finish = compressRectanglePoint(r.finish);

            for (auto i = start.y; i < finish.y; i++) {
                for (auto j = start.x; j < finish.x; j++) {
                    rectangleMap[i][j] += 1;
                }
            }
        }
    }

    void compressCoordinates(const std::vector<Rectangle> &rectangles) {
        std::set<int> x_set;
        std::set<int> y_set;

        for (const auto &r: rectangles) {
            x_set.insert(r.start.x);
            y_set.insert(r.start.y);
            x_set.insert(r.finish.x);
            y_set.insert(r.finish.y);
        }
        xCompressed = std::vector<int>();
        xCompressed.insert(xCompressed.begin(), x_set.begin(), x_set.end());
        yCompressed = std::vector<int>();
        yCompressed.insert(yCompressed.begin(), y_set.begin(), y_set.end());
    }

public:
    void prepareRectangles(const std::vector<Rectangle> &rectangles) {
        compressCoordinates(rectangles);
        mapRectangles(rectangles);
    }

    ui findRectangleCrossing(const Point &p) {
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
        for (auto &res: results) {
            Point p;
            std::cin >> p.x >> p.y;
            res = findRectangleCrossing(p);
        }
        return results;
    }
};

#endif //ALGOLAB2_RECTANGLE_MAP_H
