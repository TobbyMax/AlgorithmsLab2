//
// Created by ageev on 29.04.2023.
//

#ifndef ALGOLAB2_PERSISTENT_SEGMENT_TREE_H
#define ALGOLAB2_PERSISTENT_SEGMENT_TREE_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include "rectangle.h"

using ui = unsigned int;

class PersistentSegmentTree {
private:
    struct Node;

    std::vector<int> xCompressed;
    std::vector<int> yCompressed;
    std::vector<std::shared_ptr<Node>> states;

    struct RectangleModifier {
        int x, y1, y2;
        int modifier;

        RectangleModifier() : x(0), y1(0), y2(0), modifier(0) {}

        RectangleModifier(int _x, int _y1, int _y2, int mod) : x(_x), y1(_y1), y2(_y2), modifier(mod) {}
    };

    class ModifierComparator {
    public:
        bool operator()(RectangleModifier rm1, RectangleModifier rm2) {
            if (rm1.x == rm2.x) {
                return rm1.modifier < rm2.modifier;
            }
            return rm1.x < rm2.x;
        }
    };

    struct Node {
        ui modifier;
        ui leftBound;
        ui rightBound;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(ui l, ui r) : leftBound(l), rightBound(r) {
            modifier = 0;
            left = nullptr;
            right = nullptr;
        }

        Node() : Node(0, 0) {}

        Node(Node const &node) : Node(node.leftBound, node.rightBound) {
            modifier = node.modifier;
            left = node.left;
            right = node.right;
        }
    };

    std::shared_ptr<Node> createTree(ui l, ui r) {
        auto root = std::make_shared<Node>(l, r);
        if (l == r) {
            return root;
        }
        ui mid = (l + r) / 2;
        root->left = createTree(l, mid);
        root->right = createTree(mid + 1, r);
        return root;
    }

    std::shared_ptr<Node> modify(const std::shared_ptr<Node> &cur, int l, int r, int mod) {
        if (cur->leftBound >= r || cur->rightBound < l) {
            return cur;
        }
        if (cur->leftBound >= l && cur->rightBound < r) {
            auto newRoot = std::make_shared<Node>(*cur);
            newRoot->modifier += mod;
            return newRoot;
        } else {
            auto newRoot = std::make_shared<Node>(*cur);
            newRoot->left = modify(cur->left, l, r, mod);
            newRoot->right = modify(cur->right, l, r, mod);
            return newRoot;
        }
    }

    ui get(const std::shared_ptr<Node> &cur, long long j) {
        ui sum = 0;
        if (cur->right != nullptr && cur->right->leftBound <= j) {
            sum += get(cur->right, j);
        } else if (cur->left != nullptr && cur->left->rightBound >= j) {
            sum += get(cur->left, j);
        }
        sum += cur->modifier;
        return sum;
    }

    static int compressCoordinate(const std::vector<int> &compressed, int coordinate) {
        return (int) (std::lower_bound(compressed.begin(), compressed.end(), coordinate) - compressed.begin());
    }

    Point compressRectanglePoint(const Point &p) {
        Point mapped;
        mapped.x = compressCoordinate(xCompressed, p.x);
        mapped.y = compressCoordinate(yCompressed, p.y);
        return mapped;
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

    std::vector<RectangleModifier> getRectangleModifiers(const std::vector<Rectangle> &rectangles) {
        std::vector<RectangleModifier> modifiers(rectangles.size() * 2);
        int i = 0;
        for (const auto &r: rectangles) {
            Point start = compressRectanglePoint(r.start);
            Point finish = compressRectanglePoint(r.finish);
            modifiers[2 * i] = RectangleModifier(start.x, start.y, finish.y, 1);
            modifiers[2 * i + 1] = RectangleModifier(finish.x, start.y, finish.y, -1);
            i++;
        }
        std::sort(modifiers.begin(), modifiers.end(), ModifierComparator());
        return modifiers;
    }

    void getTreeStates(const std::vector<Rectangle> &rectangles) {
        std::vector<RectangleModifier> modifiers = getRectangleModifiers(rectangles);

        states = std::vector<std::shared_ptr<Node>>(xCompressed.size());
        std::shared_ptr<Node> initState;
        if (yCompressed.empty()) {
            initState = nullptr;
        } else {
            initState = createTree(0, yCompressed.size() - 1);
        }
        ui modifierIndex = 0, stateIndex = 0;
        for (int x = 0; x < xCompressed.size(); ++x) {
            std::shared_ptr<Node> curState;
            if (stateIndex == 0) {
                curState = initState;
            } else {
                curState = states[stateIndex - 1];
            }
            while (modifiers[modifierIndex].x == x) {
                curState = modify(curState, modifiers[modifierIndex].y1,
                                  modifiers[modifierIndex].y2, modifiers[modifierIndex].modifier);
                modifierIndex++;
            }
            states[stateIndex++] = curState;
        }
    }

public:
    void prepareRectangles(const std::vector<Rectangle> &rectangles) {
        compressCoordinates(rectangles);
        getTreeStates(rectangles);
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
        return get(states[x], y);
    }

    std::vector<ui> getResults(std::vector<Point> points) {
        std::vector<ui> results(points.size());
        for (int i = 0; i < points.size(); ++i) {
            results[i] = findRectangleCrossing(points[i]);
        }
        return results;
    }
};

#endif //ALGOLAB2_PERSISTENT_SEGMENT_TREE_H
