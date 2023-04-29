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

class RectangleMap {
private:
    std::vector<std::vector<unsigned int>> rectangleMap;
    std::vector<int> xUnique;
    std::vector<int> yUnique;

    static int compressCoordinate(const std::vector<int> &compressed, int coordinate) {
        return (int) (std::lower_bound(compressed.begin(), compressed.end(), coordinate) - compressed.begin());
    }

    Point compressRectanglePoint(const Point &p) {
        Point mapped;
        mapped.x = compressCoordinate(xUnique, p.x);
        mapped.y = compressCoordinate(yUnique, p.y);
        return mapped;
    }

    void mapRectangles(const std::vector<Rectangle> &rectangles) {
        rectangleMap = std::vector<std::vector<unsigned int>>(yUnique.size());
        for (auto &row: rectangleMap) {
            row = std::vector<unsigned int>(xUnique.size());
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

    void getUniqueCoordinates(const std::vector<Rectangle> &rectangles) {
        std::set<int> x_set;
        std::set<int> y_set;

        for (const auto &r: rectangles) {
            x_set.insert(r.start.x);
            y_set.insert(r.start.y);
            x_set.insert(r.finish.x);
            y_set.insert(r.finish.y);
        }
        xUnique = std::vector<int>();
        xUnique.insert(xUnique.begin(), x_set.begin(), x_set.end());
        yUnique = std::vector<int>();
        yUnique.insert(yUnique.begin(), y_set.begin(), y_set.end());
    }

public:
    void prepareRectangles(const std::vector<Rectangle> &rectangles) {
        getUniqueCoordinates(rectangles);
        mapRectangles(rectangles);
    }

    unsigned int findRectangleCrossing(const Point &p) {
        auto pos_x = std::upper_bound(xUnique.begin(), xUnique.end(), p.x);
        if (pos_x == xUnique.end() || pos_x == xUnique.begin()) {
            return 0;
        }
        auto pos_y = std::upper_bound(yUnique.begin(), yUnique.end(), p.y);
        if (pos_y == yUnique.end() || pos_y == yUnique.begin()) {
            return 0;
        }
        auto y = pos_y - yUnique.begin() - 1;
        auto x = pos_x - xUnique.begin() - 1;
        return rectangleMap[y][x];
    }

    std::vector<unsigned int> getResults(std::vector<Point> points) {
        std::vector<unsigned int> results(points.size());
        for (int i = 0; i < points.size(); ++i) {
            results[i] = findRectangleCrossing(points[i]);
        }
        return results;
    }
};

#endif //ALGOLAB2_RECTANGLE_MAP_H
