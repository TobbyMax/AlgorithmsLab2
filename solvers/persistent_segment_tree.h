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


class PersistentSegmentTree {
private:
    struct Node;

    std::vector<int> xUnique;
    std::vector<int> yUnique;
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
        unsigned int modifier;
        unsigned int leftBound;
        unsigned int rightBound;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(unsigned int l, unsigned int r) : leftBound(l), rightBound(r) {
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

    std::shared_ptr<Node> createTree(unsigned int l, unsigned int r) {
        auto root = std::make_shared<Node>(l, r);
        if (l == r) {
            return root;
        }
        unsigned int mid = (l + r) / 2;
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

    unsigned int get(const std::shared_ptr<Node> &cur, long long j) {
        unsigned int sum = 0;
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
        mapped.x = compressCoordinate(xUnique, p.x);
        mapped.y = compressCoordinate(yUnique, p.y);
        return mapped;
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

        states = std::vector<std::shared_ptr<Node>>(xUnique.size());
        std::shared_ptr<Node> initState;
        if (yUnique.empty()) {
            initState = nullptr;
        } else {
            initState = createTree(0, yUnique.size() - 1);
        }
        unsigned int modifierIndex = 0, stateIndex = 0;
        for (int x = 0; x < xUnique.size(); ++x) {
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
        getUniqueCoordinates(rectangles);
        getTreeStates(rectangles);
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
        return get(states[x], y);
    }

    std::vector<unsigned int> getResults(std::vector<Point> points) {
        std::vector<unsigned int> results(points.size());
        for (int i = 0; i < points.size(); ++i) {
            results[i] = findRectangleCrossing(points[i]);
        }
        return results;
    }
};

#endif //ALGOLAB2_PERSISTENT_SEGMENT_TREE_H
