#pragma once
#ifndef SORTALGORITHMS_H
#define SORTALGORITHMS_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include<mutex>
using namespace std;
using namespace sf;

class SortAlgorithms {
public:
    SortAlgorithms(vector<int>& bars);
    void bubbleSort(RenderWindow& window);
    void insertionSort(RenderWindow& window);
    void quickSort(RenderWindow& window);
    void stalinSort(RenderWindow& window);
    void radixSort(RenderWindow& window);
    void heapSort(RenderWindow& window);
    void shellSort(RenderWindow& window);
    void shakerSort(RenderWindow& window);
    
    bool isSorting() const;

private:
    vector<int>& bars;
    bool sorting;
    int swaps;
    mutex barsMutex;
    void drawBars(RenderWindow& window);
    void updateInfo(const string& message);
    void heapify(int n, int i);
    void shellSortStep(int gap);
};

#endif // SORTALGORITHMS_H
