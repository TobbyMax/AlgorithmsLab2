#define BENCH // CONTEST

#ifdef BENCH

#include "benchmarks/time_mesuring.h"

int main() {
    bench();
}

#endif


#ifdef CONTEST

#include <iostream>
#include <algorithm>
#include <vector>
#include "solvers/persistent_segment_tree.h"
#include "solvers/exhaustive_search.h"
#include "solvers/rectangle_map.h"

void printResults(const std::vector<unsigned int> &results) {
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
#endif