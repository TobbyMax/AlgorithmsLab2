#include <iostream>
#include <vector>
#include "persistent_segment_tree.h"
#include "rectangle_map.h"

using ui = unsigned int;

void printResults(const std::vector<ui> &results) {
    for (auto &res: results) {
        std::cout << res << " ";
    }
}

int main() {
    auto solver = PersistentSegmentTree();
    int n;
    std::cin >> n;
    auto rectangles = scanRectangles(n);
    solver.prepareRectangles(rectangles);

    int m;
    std::cin >> m;
    auto points = scanPoints(m);
    auto results = solver.getResults(points);
    printResults(results);
    return 0;
}
