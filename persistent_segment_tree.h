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

    struct Node {
        ui modifier;
        ui leftBound;
        ui rightBound;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(ui l, ui r) : leftBound(l), rightBound(r){
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

        std::shared_ptr<Node> modify(std::shared_ptr<Node>& cur, int l, int r, int mod) {
            if (leftBound >= r || rightBound < l) {
                return cur;
            }
            if (leftBound >= l && rightBound < r) {
                auto newRoot = std::make_shared<Node>(*cur);
                newRoot->modifier += mod;
                return newRoot;
            } else {
                auto newRoot = std::make_shared<Node>(*cur);
                newRoot->left = left->modify(left, l, r, mod);
                newRoot->right = right->modify(right, l, r, mod);
                return newRoot;
            }
        }

        ui get(long long j) {
            ui sum = 0;
            if (right != nullptr && right->leftBound <= j) {
                sum += right->get(j);
            } else if (left != nullptr && left->rightBound >= j){
                sum += left->get(j);
            }
            sum += modifier;
            return sum;
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

public:
    struct RectangleModifier {
        int x, y1, y2;
        int modifier;
        RectangleModifier() : x(0), y1(0), y2(0), modifier(0) {}
        RectangleModifier(int _x, int _y1, int _y2, int mod) : x(_x), y1(_y1), y2(_y2), modifier(mod) {}
    };
    class Comparator {
    public:
        bool operator() (RectangleModifier rm1, RectangleModifier rm2) {
            if (rm1.x == rm2.x) {
                return rm1.modifier < rm2.modifier;
            }
            return rm1.x < rm2.x;
        }
    };

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
        std::vector<RectangleModifier> modifiers(rectangles.size() * 2);
        i = 0;
        for (const auto& r : rectangles) {
            Point start, finish;
            start.x = (int)(std::lower_bound(xCompressed.begin(), xCompressed.end(), r.start.x) - xCompressed.begin());
            start.y = (int)(std::lower_bound(yCompressed.begin(), yCompressed.end(), r.start.y) - yCompressed.begin());
            finish.x = (int)(std::lower_bound(xCompressed.begin(), xCompressed.end(), r.finish.x) - xCompressed.begin());
            finish.y = (int)(std::lower_bound(yCompressed.begin(), yCompressed.end(), r.finish.y) - yCompressed.begin());
            modifiers[2*i] = RectangleModifier(start.x, start.y, finish.y, 1);
            modifiers[2*i + 1] = RectangleModifier(finish.x, start.y, finish.y, -1);
            i++;
        }

        std::sort(modifiers.begin(), modifiers.end(), Comparator());
        states = std::vector<std::shared_ptr<Node>>(x_set.size());
        std::shared_ptr<Node> initState;
        if (y_set.empty()){
            initState = nullptr;
        }
        else {
            initState = createTree(0, y_set.size() - 1);
        }
        int j = 0;
        ui index = 0;
        for (int x = 0; x < xCompressed.size(); ++x) {
            std::shared_ptr<Node> curState;
            if (j == 0) {
                curState = initState;
            } else {
                curState = states[j - 1];
            }

            while (modifiers[index].x == x) {
                curState = std::shared_ptr<Node>(curState->modify(curState, modifiers[index].y1, modifiers[index].y2, modifiers[index].modifier));
                index++;
            }
            states[j] = curState;
            j++;
        }
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
        return states[x]->get(y);
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

#endif //ALGOLAB2_PERSISTENT_SEGMENT_TREE_H
