#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include "rectangle.h"

using ui = unsigned int;

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

std::vector<Rectangle> scanRectangles(ui n) {
    std::vector<Rectangle> rectangles(n);

    for (auto& r : rectangles) {
        int x1, y1, x2, y2;
        std::cin >> x1 >> y1 >> x2 >> y2;
        r = Rectangle{x1, y1, x2, y2};
    }
    return rectangles;
}

void printResults(const std::vector<ui>& results) {
    for (auto& res : results) {
        std::cout << res << " ";
    }
}

std::vector<std::vector<ui>> mapRectangles (const std::vector<Rectangle>& rectangles,
                                            const std::vector<int>& x_compressed, const std::vector<int>& y_compressed) {
    std::vector<std::vector<ui>> rect_map(y_compressed.size());
    for (auto& row : rect_map) {
        row = std::vector<ui>(x_compressed.size());
    }
    for (auto& r : rectangles) {
        Point start, finish;
        start.x = (int)(std::lower_bound(x_compressed.begin(), x_compressed.end(), r.start.x) - x_compressed.begin());
        start.y = (int)(std::lower_bound(y_compressed.begin(), y_compressed.end(), r.start.y) - y_compressed.begin());
        finish.x = (int)(std::lower_bound(x_compressed.begin(), x_compressed.end(), r.finish.x) - x_compressed.begin());
        finish.y = (int)(std::lower_bound(y_compressed.begin(), y_compressed.end(), r.finish.y) - y_compressed.begin());

        for (auto i = start.y; i < finish.y; i++) {
            for (auto j = start.x; j < finish.x; j++) {
                rect_map[i][j] += 1;
            }
        }
    }
    return rect_map;
}

ui findRectangleCountMap(const std::vector<std::vector<ui>>& rect_map,
                         const std::vector<int>& x_compressed, const std::vector<int>& y_compressed, const Point& p) {
    auto pos_x = std::upper_bound(x_compressed.begin(), x_compressed.end(), p.x);
    if (pos_x == x_compressed.end() || pos_x == x_compressed.begin()) {
        return 0;
    }
    auto pos_y = std::upper_bound(y_compressed.begin(), y_compressed.end(), p.y);
    if (pos_y == y_compressed.end() || pos_y == y_compressed.begin()) {
        return 0;
    }
    auto y = pos_y - y_compressed.begin() - 1;
    auto x = pos_x - x_compressed.begin() - 1;
    return rect_map[y][x];
}

std::vector<ui> getResultsMap(int m, const std::vector<std::vector<ui>>& rect_map,
                              const std::vector<int>& x_compressed, const std::vector<int>& y_compressed) {
    std::vector<ui> results(m);
    for (auto& res : results) {
        Point p;
        std::cin >> p.x >> p.y;
        res = findRectangleCountMap(rect_map, x_compressed, y_compressed, p);
    }
    return results;
}


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

    Node* modify(int l, int r, int mod) {
        if (leftBound > r || rightBound < l) {
            return this;
        }
        if (leftBound >= l && rightBound <= r) {
            auto newRoot = new Node(*this);
            newRoot->modifier += mod;
            return newRoot;
        } else {
            auto newRoot = new Node(*this);
            newRoot->left = std::shared_ptr<Node>(left->modify(l, r, mod));
            newRoot->right = std::shared_ptr<Node>(right->modify(l, r, mod));
            return newRoot;
        }
    }

    std::shared_ptr<Node> modifish(int l, int r, int mod) {
        if (leftBound > r || rightBound < l) {
            return std::make_shared<Node>(*this);
        }
        if (leftBound >= l && rightBound <= r) {
            auto newRoot = std::make_shared<Node>(*this);
            newRoot->modifier += mod;
            return newRoot;
        } else {
            auto newRoot = std::make_shared<Node>(*this);
            newRoot->left = left->modifish(l, r, mod);
            newRoot->right = right->modifish(l, r, mod);
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

ui findRectangleCountTree(const std::vector<std::shared_ptr<Node>>& states,
                         const std::vector<int>& x_compressed, const std::vector<int>& y_compressed, const Point& p) {
    auto pos_x = std::upper_bound(x_compressed.begin(), x_compressed.end(), p.x);
    if (pos_x == x_compressed.end() || pos_x == x_compressed.begin()) {
        return 0;
    }
    auto pos_y = std::upper_bound(y_compressed.begin(), y_compressed.end(), p.y);
    if (pos_y == y_compressed.end() || pos_y == y_compressed.begin()) {
        return 0;
    }
    auto y = pos_y - y_compressed.begin() - 1;
    auto x = pos_x - x_compressed.begin() - 1;
    return states[x]->get(y);
}

std::vector<ui> getResultsTree(int m,  const std::vector<std::shared_ptr<Node>>& states,
                              const std::vector<int>& x_compressed, const std::vector<int>& y_compressed) {
    std::vector<ui> results(m);
    for (auto& res : results) {
        Point p;
        std::cin >> p.x >> p.y;
        res = findRectangleCountTree(states, x_compressed, y_compressed, p);
    }
    return results;
}

int main() {
//    // Алгоритм полного перебора
//    int n;
//    std::cin >> n;
//    std::vector<Rectangle> rectangles = scanRectangles(n);
//    int m;
//    std::cin >> m;
//    std::vector<ui> results = getResults(m, rectangles);
//    printResults(results);

//    // Алгоритм с мапой
//    int n;
//    std::cin >> n;
//    std::vector<Rectangle> rectangles = scanRectangles(n);
//    std::set<int> x_set;
//    std::set<int> y_set;
//
//    for (const auto& r : rectangles) {
//        x_set.insert(r.start.x);
//        y_set.insert(r.start.y);
//        x_set.insert(r.finish.x);
//        y_set.insert(r.finish.y);
//    }
//    auto xs = std::vector<int>(x_set.size());
//    int i = 0;
//    for (auto x : x_set) {
//        xs[i++] = x;
//    }
//    auto ys = std::vector<int>(y_set.size());
//    i = 0;
//    for (auto y : y_set) {
//        ys[i++] = y;
//    }
//    auto rect_map = mapRectangles(rectangles, xs, ys);
//    for (auto row : rect_map) {
//        for (auto val : row) {
//            std::cout << val << " ";
//        }
//        std::cout << std::endl;
//    }
//    int m;
//    std::cin >> m;
//    std::vector<ui> results = getResultsMap(m, rect_map, xs, ys);
//    printResults(results);
//    return 0;

    int n;
    std::cin >> n;
    std::vector<Rectangle> rectangles = scanRectangles(n);
    std::set<int> x_set;
    std::set<int> y_set;

    for (const auto& r : rectangles) {
        x_set.insert(r.start.x);
        y_set.insert(r.start.y);
        x_set.insert(r.finish.x);
        y_set.insert(r.finish.y);
    }
    auto xs = std::vector<int>(x_set.size());
    ui j = 0;
    for (auto x : x_set) {
        xs[j++] = x;
    }
    auto ys = std::vector<int>(y_set.size());
    j = 0;
    for (auto y : y_set) {
        ys[j++] = y;
    }
    std::vector<RectangleModifier> modifiers(rectangles.size() * 2);
    int i = 0;
    for (const auto& r : rectangles) {
        Point start, finish;
        start.x = (int)(std::lower_bound(xs.begin(), xs.end(), r.start.x) - xs.begin());
        start.y = (int)(std::lower_bound(ys.begin(), ys.end(), r.start.y) - ys.begin());
        finish.x = (int)(std::lower_bound(xs.begin(), xs.end(), r.finish.x) - xs.begin());
        finish.y = (int)(std::lower_bound(ys.begin(), ys.end(), r.finish.y) - ys.begin());
        modifiers[2*i] = RectangleModifier(start.x, start.y, finish.y, 1);
        modifiers[2*i + 1] = RectangleModifier(finish.x, start.y, finish.y, -1);
        i++;
    }

    std::sort(modifiers.begin(), modifiers.end(), Comparator());
    std::vector<std::shared_ptr<Node>> states(x_set.size());
    std::shared_ptr<Node> initState;
    if (y_set.empty()){
        initState = nullptr;
    }
    else {
        initState = createTree(0, y_set.size() - 1);
    }
    j = 0;
    ui index = 0;
    for (int x = 0; x < xs.size(); ++x) {
        std::shared_ptr<Node> curState;
        if (j == 0) {
            curState = initState;
        } else {
            curState = states[j - 1];
        }

        while (modifiers[index].x == x) {
            curState = std::shared_ptr<Node>(curState->modifish(modifiers[index].y1, modifiers[index].y2, modifiers[index].modifier));
            index++;
        }
        states[j] = curState;
        j++;
    }


    int m;
    std::cin >> m;
    auto results = getResultsTree(m, states, xs, ys);
    printResults(results);
    return 0;
}
