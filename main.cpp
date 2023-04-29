#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include "persistent_segment_tree.h"
#include "exhaustive_search.h"
#include "rectangle_map.h"

#define P_X 131
#define P_Y 113
#define TESTS_COUNT 5

enum Algorithm {Exhaustive_Search,	Rectangle_Map,	Persistent_Segment_Tree};

void printResults(const std::vector<unsigned int> &results) {
    for (auto &res: results) {
        std::cout << res << " ";
    }
}

std::vector<Rectangle> generateRectangles(int n) {
    std::vector<Rectangle> rectangles(n);
    for (int i = 0; i < n; ++i) {
        rectangles[i] = Rectangle({10*i, 10*i}, {10*(2*n-i), 10*(2*n-i)});
    }
    return rectangles;
}

std::vector<Point> generatePoints(int n) {
    std::vector<Point> points(n);
    for (size_t i = 0; i < n; ++i) {
        points[i] = Point((int)((unsigned long long) std::pow(P_X*i, 31) % (20 * n)),
                          (int)((unsigned long long) std::pow(P_Y*i, 31) % (20 * n)));
    }
    return points;
}

void bench() {
    auto psTree = PersistentSegmentTree();
    auto rectMap = RectangleMap();
    auto fullSearch = ExhaustiveSearch();

    auto t0 = std::chrono::high_resolution_clock::now();
    auto t1 = std::chrono::high_resolution_clock::now();

    std::ofstream prepResults("..\\prep_results.txt");
    std::ofstream findResults("..\\find_results.txt");

    prepResults << "n\tExhaustiveSearch\tRectangleMap\tPersistentSegmentTree\n";
    findResults << "n\tExhaustiveSearch\tRectangleMap\tPersistentSegmentTree\n";

    for (int n = 1; n < 512*512; n *= 2) {
        std::cout << "\nSize: " << n << std::endl;

        auto rectangles = generateRectangles(n);
        auto points = generatePoints(n);

        std::vector<unsigned long long> sumPrep(3);
        std::vector<unsigned long long> sumFind(3);
        std::vector<unsigned long long> timePrep(3);
        std::vector<unsigned long long> timeFind(3);
        sumFind[Exhaustive_Search] = sumFind[Rectangle_Map] = sumFind[Persistent_Segment_Tree] = 0;
        sumPrep[Exhaustive_Search] = sumPrep[Rectangle_Map] = sumPrep[Persistent_Segment_Tree] = 0;

        // Делаем 1000 запусков для точности значений времени
        for (int i = 0; i < TESTS_COUNT; ++i) {
            std::cout << i << " ";
            psTree = PersistentSegmentTree();
            rectMap = RectangleMap();
            fullSearch = ExhaustiveSearch();

            t0 = std::chrono::high_resolution_clock::now();
            fullSearch.prepareRectangles(rectangles);
            t1 = std::chrono::high_resolution_clock::now();
            sumPrep[Exhaustive_Search] += std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();

            t0 = std::chrono::high_resolution_clock::now();
            fullSearch.getResults(points);
            t1 = std::chrono::high_resolution_clock::now();
            sumFind[Exhaustive_Search] += std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
            if (n <= 2048) {
                t0 = std::chrono::high_resolution_clock::now();
                rectMap.prepareRectangles(rectangles);
                t1 = std::chrono::high_resolution_clock::now();
                sumPrep[Rectangle_Map] += std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();

                t0 = std::chrono::high_resolution_clock::now();
                rectMap.getResults(points);
                t1 = std::chrono::high_resolution_clock::now();
                sumFind[Rectangle_Map] += std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
            }

            t0 = std::chrono::high_resolution_clock::now();
            psTree.prepareRectangles(rectangles);
            t1 = std::chrono::high_resolution_clock::now();
            sumPrep[Persistent_Segment_Tree] += std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();

            t0 = std::chrono::high_resolution_clock::now();
            psTree.getResults(points);
            t1 = std::chrono::high_resolution_clock::now();
            sumFind[Persistent_Segment_Tree] += std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
        }
        // Вычисляем среднее время за 1000 запусков
        for (auto i = 0; i < 3; i++) {
            timePrep[i] = sumPrep[i] / TESTS_COUNT;
            timeFind[i] = sumFind[i] / TESTS_COUNT;
        }
        // Записываем данные в текстовый файл
        prepResults << n << "\t";
        findResults << n << "\t";
        for (auto i = 0; i < 3; i++) {
            prepResults << timePrep[i] << "\t";
            findResults << timeFind[i] << "\t";
        }
        prepResults << "\n";
        findResults << "\n";
    }
}

int main() {
    bench();
//    auto solver = PersistentSegmentTree();
//    int n;
//    std::cin >> n;
//    auto rectangles = scanRectangles(n);
//    solver.prepareRectangles(rectangles);
//
//    int m;
//    std::cin >> m;
//    auto points = scanPoints(m);
//    auto results = solver.getResults(points);
//    printResults(results);
//    return 0;
}
