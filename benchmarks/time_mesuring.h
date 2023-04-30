//
// Created by ageev on 30.04.2023.
//

#ifndef ALGOLAB2_TIME_MESURING_H
#define ALGOLAB2_TIME_MESURING_H

#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include "../solvers/persistent_segment_tree.h"
#include "../solvers/exhaustive_search.h"
#include "../solvers/rectangle_map.h"
#include "generators.h"

#define FIXED 1000
#define TESTS_COUNT 10
#define MAX_N 512*512

enum Algorithm {Exhaustive_Search,	Rectangle_Map,	Persistent_Segment_Tree};


long long measure_time(const std::function<void(void)> &method) {
    auto t0 = std::chrono::high_resolution_clock::now();
    method();
    auto t1 = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
}

void bench() {
    auto psTree = PersistentSegmentTree();
    auto rectMap = RectangleMap();
    auto fullSearch = ExhaustiveSearch();

    std::ofstream prepResults("..\\data\\raw_results\\prep_results.txt");
    std::ofstream findResults("..\\data\\raw_results\\find_results.txt");
    std::ofstream findFixedResults("..\\data\\raw_results\\find_fixed_results.txt");
    std::ofstream sumResults("..\\data\\raw_results\\sum_results.txt");
    std::ofstream sumFixedResults("..\\data\\raw_results\\sum_fixed_results.txt");

    prepResults << "n\tExhaustiveSearch\tRectangleMap\tPersistentSegmentTree\n";
    findResults << "n\tExhaustiveSearch\tRectangleMap\tPersistentSegmentTree\n";
    findFixedResults << "n\tExhaustiveSearch\tRectangleMap\tPersistentSegmentTree\n";
    sumResults << "n\tExhaustiveSearch\tRectangleMap\tPersistentSegmentTree\n";
    sumFixedResults << "n\tExhaustiveSearch\tRectangleMap\tPersistentSegmentTree\n";

    for (int n = 1; n < MAX_N; n *= 2) {
        std::cout << "\nSize: " << n << std::endl;

        auto rectangles = generateRectangles(n);
        auto points = generatePoints(n);
        auto pointsFixed = generatePoints(FIXED);

        std::vector<unsigned long long> sumPrep(3);
        std::vector<unsigned long long> sumFind(3);
        std::vector<unsigned long long> timePrep(3);
        std::vector<unsigned long long> timeFind(3);
        sumFind[Exhaustive_Search] = sumFind[Rectangle_Map] = sumFind[Persistent_Segment_Tree] = 0;
        sumPrep[Exhaustive_Search] = sumPrep[Rectangle_Map] = sumPrep[Persistent_Segment_Tree] = 0;

        std::vector<unsigned long long> sumFindFixed(3);
        std::vector<unsigned long long> timeFindFixed(3);
        sumFindFixed[Exhaustive_Search] = sumFindFixed[Rectangle_Map] = sumFindFixed[Persistent_Segment_Tree] = 0;
        sumPrep[Exhaustive_Search] = sumPrep[Rectangle_Map] = sumPrep[Persistent_Segment_Tree] = 0;

        // Делаем 10 запусков для точности значений времени
        for (int i = 0; i < TESTS_COUNT; ++i) {
            std::cout << i << " ";
            psTree = PersistentSegmentTree();
            rectMap = RectangleMap();
            fullSearch = ExhaustiveSearch();

//            sumPrep[Exhaustive_Search] += measure_time([&fullSearch, &rectangles]() {
            fullSearch.prepareRectangles(rectangles);
//            });
            sumFind[Exhaustive_Search] += measure_time([&fullSearch, &points]() {
                fullSearch.getResults(points);
            });
            sumFindFixed[Exhaustive_Search] += measure_time([&fullSearch, &pointsFixed]() {
                fullSearch.getResults(pointsFixed);
            });

            if (n <= 2048) {
                sumPrep[Rectangle_Map] += measure_time([&rectMap, &rectangles]() {
                    rectMap.prepareRectangles(rectangles);
                });
                sumFind[Rectangle_Map] += measure_time([&rectMap, &points]() {
                    rectMap.getResults(points);
                });
                sumFindFixed[Rectangle_Map] += measure_time([&rectMap, &pointsFixed]() {
                    rectMap.getResults(pointsFixed);
                });
            }

            sumPrep[Persistent_Segment_Tree] += measure_time([&psTree, &rectangles]() {
                psTree.prepareRectangles(rectangles);
            });
            sumFind[Persistent_Segment_Tree] += measure_time([&psTree, &points]() {
                psTree.getResults(points);
            });
            sumFindFixed[Persistent_Segment_Tree] += measure_time([&psTree, &pointsFixed]() {
                psTree.getResults(pointsFixed);
            });
        }
        // Вычисляем среднее время за 10 запусков
        for (auto i = 0; i < 3; i++) {
            timePrep[i] = sumPrep[i] / TESTS_COUNT;
            timeFind[i] = sumFind[i] / TESTS_COUNT;
            timeFindFixed[i] = sumFindFixed[i] / TESTS_COUNT;
        }
        // Записываем данные в текстовый файл
        prepResults << n << "\t";
        findResults << n << "\t";
        findFixedResults << n << "\t";
        sumResults << n << "\t";
        sumFixedResults << n << "\t";
        for (auto i = 0; i < 3; i++) {
            prepResults << timePrep[i] << "\t";
            findResults << timeFind[i] << "\t";
            findFixedResults << timeFindFixed[i] << "\t";
            sumResults << timePrep[i] + timeFind[i] << "\t";
            sumFixedResults << timePrep[i] + timeFindFixed[i] << "\t";
        }
        prepResults << "\n";
        findResults << "\n";
        findFixedResults << "\n";
        sumResults << "\n";
        sumFixedResults << "\n";
    }
}

#endif //ALGOLAB2_TIME_MESURING_H
