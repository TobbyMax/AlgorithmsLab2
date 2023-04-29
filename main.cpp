#include <iostream>
#include <vector>
#include "persistent_segment_tree.h"

using ui = unsigned int;

void printResults(const std::vector<ui>& results) {
    for (auto& res : results) {
        std::cout << res << " ";
    }
}

int main() {
    auto solver = PersistentSegmentTree();
    int n;
    std::cin >> n;
    std::vector<Rectangle> rectangles = scanRectangles(n);
    solver.prepareRectangles(rectangles);
    int m;
    std::cin >> m;
    auto results = solver.getResults(m);
    printResults(results);
    return 0;
}
