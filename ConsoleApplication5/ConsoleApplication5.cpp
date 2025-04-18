#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <mutex>
#include <iostream>
// Цвета для текста
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

using namespace sf;
using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
const int MIN_BARS = 10;
const int MAX_BARS = 100;

class SortingVisualizer;

class SortingAlgorithm {
protected:
    SortingVisualizer* visualizer;

public:
    SortingAlgorithm(SortingVisualizer* visualizer) : visualizer(visualizer) {}

    virtual void sort() = 0;
    virtual ~SortingAlgorithm() = default;
};

class SortingVisualizer {
private:
    int NUM_BARS = 50;
    int BAR_WIDTH = WIDTH / NUM_BARS;
    vector<int> bars;
    bool sorting = false;
    bool sorted = false; 
    int swaps = 0, comparisons = 0;
    string sortInfo = "Press B/I/Q/S to Sort";
    mutex barsMutex;
    int currentIndex = -1;

public:
    SortingVisualizer() {
        srand(static_cast<unsigned>(time(nullptr)));
        generateBars();
    }

    void generateBars() {
        lock_guard<mutex> lock(barsMutex);
        bars.resize(NUM_BARS);
        for (int i = 0; i < NUM_BARS; i++) {
            bars[i] = rand() % HEIGHT;
        }
        BAR_WIDTH = WIDTH / NUM_BARS;
        sortInfo = "Press B/I/Q/S to Sort";
        system("cls");
        cout << CYAN << "===============================" << RESET << endl;
        cout << CYAN << "  Course project for OOP class  " << RESET << endl;
        cout << CYAN << "       by Amirhan Ordobaev      " << RESET << endl;
        cout << CYAN << "           SE(eng)-2-23         " << RESET << endl;
        cout << CYAN << "===============================" << RESET << endl;
        cout << CYAN << "   Sorting Visualizer Options  " << RESET << endl;
        cout << CYAN << "===============================" << RESET << endl;
        cout << "Number of bars: " << GREEN << NUM_BARS << RESET << endl;
        cout << YELLOW << "For edit N.o.b. " << RESET << "use" << YELLOW << " up arrow and down arrow " << RESET << endl;
        // Регенерация
        cout << "Press " << RED << "R " << RESET << "for "
            << MAGENTA << "R" << YELLOW << "a" << GREEN << "n"
            << BLUE << "d" << CYAN << "o" << WHITE << "m"
            << RESET << MAGENTA << "i" << YELLOW << "z"
            << GREEN << "e" << RESET << endl;
        cout << RED << "      W " << RESET << "for " << RED << "Worst case" << RESET << endl;
        cout << CYAN << "===============================" << RESET << endl;
        cout << CYAN << "   Sorting Algorithms          " << RESET << endl;
        cout << CYAN << "===============================" << RESET << endl;

        // Квадратичная сложность (O(n^2))
        cout << CYAN << "   Quadratic Time Algorithms  " << RESET << endl;
        cout << RED << "      B " << RESET << "for " << MAGENTA << "Bubble Sort" << RESET << endl;
        cout << RED << "      S " << RESET << "for " << MAGENTA << "Shaker Sort" << RESET << endl;
        cout << RED << "      I " << RESET << "for " << MAGENTA << "Insertion Sort" << RESET << endl;
        cout << RED << "      T " << RESET << "for " << MAGENTA << "Selection Sort" << RESET << endl;

        // Логарифмическая сложность (O(log n))
        cout << CYAN << "   Logarithmic Time Algorithms" << RESET << endl;
        cout << RED << "      Q " << RESET << "for " << MAGENTA << "Quick Sort" << RESET << endl;
        cout << RED << "      M " << RESET << "for " << MAGENTA << "Merge Sort" << RESET << endl;
        cout << RED << "      H " << RESET << "for " << MAGENTA << "Heap Sort" << RESET << endl;
        cout << RED << "      L " << RESET << "for " << MAGENTA << "Shell Sort" << RESET << endl;

        // Линейно-логарифмическая сложность (O(n log n))
        cout << CYAN << "   Another Algorithms" << RESET << endl;
        cout << RED << "      X " << RESET << "for " << MAGENTA << "Radix Sort" << RESET << endl;
        cout << RED << "      G " << RESET << "for " << MAGENTA << "Gnome Sort" << RESET << endl;
        cout << RED << "      U " << RESET << "for " << MAGENTA << "Tim Sort" << RESET << endl;
        cout << RED << "      P " << RESET << "for " << MAGENTA << "Pancake Sort" << RESET << endl;
        cout << RED << "      Z " << RESET << "for " << MAGENTA << "Bucket Sort" << RESET << endl;
        cout << CYAN << "===============================" << RESET << endl;

        sorted = false;  
    }

    void drawBars(RenderWindow& window) {
        window.clear();
        {
            lock_guard<mutex> lock(barsMutex);
            for (int i = 0; i < NUM_BARS; i++) {
                RectangleShape bar(Vector2f(BAR_WIDTH - 1, bars[i]));
                bar.setPosition(i * BAR_WIDTH, HEIGHT - bars[i]);

                if (isSorting() && i == getCurrentIndex()) {
                    bar.setFillColor(Color::Red);
                }
                else if (sorted) {
                    bar.setFillColor(Color::Green);
                }
                else {
                    bar.setFillColor(Color::White);
                }

                window.draw(bar);
            }
        }
        window.display();
    }
    void generateWorstCase() {
        lock_guard<mutex> lock(barsMutex);
        bars.resize(NUM_BARS);
        for (int i = 0; i < NUM_BARS; i++) {
            bars[i] = HEIGHT - (i * (HEIGHT / NUM_BARS)); 
        }
        sorted = false;
        sorting = false;
    }

    void updateNumBars(int change) {
        NUM_BARS = min(MAX_BARS, max(MIN_BARS, NUM_BARS + change));
        generateBars();
    }

    bool isSorting() const {
        return sorting;
    }

    string getSortInfo() const {
        return sortInfo;
    }

    vector<int>& getBars() {
        return bars;
    }

    void setSorting(bool value) {
        sorting = value;
    }

    int getCurrentIndex() const {
        return currentIndex;
    }

    void setCurrentIndex(int index) {
        currentIndex = index;
    }

    void incrementComparisons() {
        comparisons++;
    }

    void incrementSwaps() {
        swaps++;
    }

    void setSorted(bool value) {
        sorted = value;  // Устанавливаем флаг, когда сортировка завершена
    }
};

class BubbleSort : public SortingAlgorithm {
public:
    BubbleSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();
            for (size_t i = 0; i < bars.size() - 1 && visualizer->isSorting(); i++) {
                for (size_t j = 0; j < bars.size() - i - 1 && visualizer->isSorting(); j++) {
                    comparisons++;
                    visualizer->setCurrentIndex(j);
                    if (bars[j] > bars[j + 1]) {
                        swap(bars[j], bars[j + 1]);
                        swaps++;
                    }
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
            }
            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Bubble Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);  // Устанавливаем флаг сортировки как завершенное
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Bubble Sort: " << e.what() << endl;
        }
    }
};
class InsertionSort : public SortingAlgorithm {
public:
    InsertionSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();
            for (size_t i = 1; i < bars.size() && visualizer->isSorting(); i++) {
                int key = bars[i];
                int j = i - 1;
                while (j >= 0 && visualizer->isSorting()) {
                    comparisons++;
                    bool needSwap = false;
                    visualizer->setCurrentIndex(j);
                    if (bars[j] > key) {
                        bars[j + 1] = bars[j];
                        swaps++;
                        needSwap = true;
                    }
                    if (!needSwap)
                        break;
                    j--;
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
                bars[j + 1] = key;
            }
            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Insertion Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);  // Устанавливаем флаг сортировки как завершенное
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Insertion Sort: " << e.what() << endl;
        }
    }
};
class QuickSort : public SortingAlgorithm {
public:
    QuickSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void quickSort(int left, int right, int& comparisons, int& swaps) {
        if (left >= right || !visualizer->isSorting()) return;

        auto& bars = visualizer->getBars();
        int pivot = bars[right];
        int i = left;

        for (int j = left; j < right; j++) {
            comparisons++;
            visualizer->incrementComparisons();
            visualizer->setCurrentIndex(j);

            if (bars[j] < pivot) {
                swap(bars[i], bars[j]);
                swaps++;  
                visualizer->incrementSwaps();
                i++;
            }
            this_thread::sleep_for(chrono::milliseconds(10));
        }

        swap(bars[i], bars[right]);
        swaps++;
        visualizer->incrementSwaps();

        quickSort(left, i - 1, comparisons, swaps);
        quickSort(i + 1, right, comparisons, swaps);
    }

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();

            auto start = chrono::high_resolution_clock::now();
            quickSort(0, bars.size() - 1, comparisons, swaps);
            auto end = chrono::high_resolution_clock::now();

            stringstream ss;
            ss << "Quick Sort: "
                << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Quick Sort: " << e.what() << endl;
        }
    }
};
class ShakerSort : public SortingAlgorithm {
public:
    ShakerSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();
            int left = 0;
            int right = bars.size() - 1;
            while (left < right && visualizer->isSorting()) {
                for (int i = left; i < right && visualizer->isSorting(); i++) {
                    comparisons++;
                    visualizer->setCurrentIndex(i);
                    if (bars[i] > bars[i + 1]) {
                        swap(bars[i], bars[i + 1]);
                        swaps++;
                    }
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
                right--;

                for (int i = right; i > left && visualizer->isSorting(); i--) {
                   comparisons++;
                    visualizer->setCurrentIndex(i);
                    if (bars[i] < bars[i - 1]) {
                        swap(bars[i], bars[i - 1]);
                        swaps++;
                    }
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
                left++;
            }
            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Shaker Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);  // Устанавливаем флаг сортировки как завершенное
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Shaker Sort: " << e.what() << endl;
        }
    }
};
class SelectionSort : public SortingAlgorithm {
public:
    SelectionSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();

            for (int i = 0; i < bars.size() - 1 && visualizer->isSorting(); i++) {
                int minIdx = i;
                for (int j = i + 1; j < bars.size() && visualizer->isSorting(); j++) {
                    comparisons++;
                    visualizer->setCurrentIndex(j);
                    if (bars[j] < bars[minIdx])
                        minIdx = j;
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
                if (i != minIdx) {
                    swap(bars[i], bars[minIdx]);
                    swaps++;
                }
            }

            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Selection Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Selection Sort: " << e.what() << endl;
        }
    }
};
class HeapSort : public SortingAlgorithm {
private:
    void heapify(vector<int>& bars, int n, int i, int& swaps, int& comparisons) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < n) {
            comparisons++;
            if (bars[left] > bars[largest])
                largest = left;
        }

        if (right < n) {
            comparisons++;
            if (bars[right] > bars[largest])
                largest = right;
        }

        if (largest != i) {
            swap(bars[i], bars[largest]);
            swaps++;
            heapify(bars, n, largest, swaps, comparisons);
        }
    }

public:
    HeapSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();

            for (int i = bars.size() / 2 - 1; i >= 0 && visualizer->isSorting(); i--)
                heapify(bars, bars.size(), i, swaps, comparisons);

            for (int i = bars.size() - 1; i > 0 && visualizer->isSorting(); i--) {
                swap(bars[0], bars[i]);
                swaps++;
                heapify(bars, i, 0, swaps, comparisons);
                this_thread::sleep_for(chrono::milliseconds(10));
            }

            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Heap Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Heap Sort: " << e.what() << endl;
        }
    }
};
class MergeSort : public SortingAlgorithm {
private:
    void merge(vector<int>& bars, int left, int mid, int right, int& comparisons) {
        vector<int> L(bars.begin() + left, bars.begin() + mid + 1);
        vector<int> R(bars.begin() + mid + 1, bars.begin() + right + 1);

        int i = 0, j = 0, k = left;
        while (i < L.size() && j < R.size()) {
            comparisons++;
            if (L[i] <= R[j])
                bars[k++] = L[i++];
            else
                bars[k++] = R[j++];
        }
        while (i < L.size()) bars[k++] = L[i++];
        while (j < R.size()) bars[k++] = R[j++];
    }

    void mergeSort(vector<int>& bars, int left, int right, int& comparisons) {
        if (left >= right) return;
        int mid = left + (right - left) / 2;
        mergeSort(bars, left, mid, comparisons);
        mergeSort(bars, mid + 1, right, comparisons);
        merge(bars, left, mid, right, comparisons);
    }

public:
    MergeSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();

            mergeSort(bars, 0, bars.size() - 1, comparisons);

            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Merge Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Merge Sort: " << e.what() << endl;
        }
    }
};
class ShellSort : public SortingAlgorithm {
public:
    ShellSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();

            for (int gap = bars.size() / 2; gap > 0 && visualizer->isSorting(); gap /= 2) {
                for (int i = gap; i < bars.size() && visualizer->isSorting(); i++) {
                    int temp = bars[i];
                    int j;
                    for (j = i; j >= gap && bars[j - gap] > temp; j -= gap) {
                        bars[j] = bars[j - gap];
                        swaps++;
                        comparisons++;
                    }
                    bars[j] = temp;
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
            }

            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Shell Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Shell Sort: " << e.what() << endl;
        }
    }
};
class PancakeSort : public SortingAlgorithm {
private:
    int findMaxIndex(const vector<int>& bars, int n) {
        int maxIndex = 0;
        for (int i = 1; i < n; i++) {
            if (bars[i] > bars[maxIndex]) {
                maxIndex = i;
            }
        }
        return maxIndex;
    }

    void flip(vector<int>& bars, int index) {
        int start = 0;
        while (start < index) {
            swap(bars[start], bars[index]);
            start++;
            index--;
        }
    }
public:
    PancakeSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();

            for (int i = bars.size(); i > 1; i--) {
                int maxIndex = findMaxIndex(bars, i);
                if (maxIndex != i - 1) {
                    if (maxIndex != 0) {
                        flip(bars, maxIndex); 
                        swaps++;
                    }
                    flip(bars, i - 1);
                    swaps++;
                }
                comparisons += (i - 1);
                this_thread::sleep_for(chrono::milliseconds(10));
            }

            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Pancake Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Pancake Sort: " << e.what() << endl;
        }
    }
};
class RadixSort : public SortingAlgorithm {
public:
    RadixSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}
private:
    void countSort(vector<int>& bars, int exp, int& swaps, int& comparisons) {
        int n = bars.size();
        vector<int> output(n);
        int count[10] = { 0 };

        for (int i = 0; i < n; i++) {
            comparisons++;
            count[(bars[i] / exp) % 10]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            output[count[(bars[i] / exp) % 10] - 1] = bars[i];
            count[(bars[i] / exp) % 10]--;
        }

        for (int i = 0; i < n; i++) {
            bars[i] = output[i];
        }

        swaps += n;
        this_thread::sleep_for(chrono::milliseconds(10));
    }
public:
    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();

            int maxVal = *max_element(bars.begin(), bars.end());  
            int exp = 1;  
            while (maxVal / exp > 0) {
                countSort(bars, exp, swaps, comparisons);
                exp *= 10;
            }

            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Radix Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Radix Sort: " << e.what() << endl;
        }
    }


};
class GnomeSort : public SortingAlgorithm {
public:
    GnomeSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();
            int index = 0;
            while (index < bars.size()) {
                if (index == 0) {
                    index++;
                }
                if (bars[index] >= bars[index - 1]) {
                    comparisons++;
                    index++;
                }
                else {
                    swap(bars[index], bars[index - 1]);
                    swaps++;
                    comparisons++;
                    visualizer->setCurrentIndex(index);
                    index--;
                }
                this_thread::sleep_for(chrono::milliseconds(10));
            }

            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Gnome Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true); 
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Gnome Sort: " << e.what() << endl;
        }
    }
};
class TimSort {
private:
    SortingVisualizer* visualizer;
    const int RUN = 32;  // Размер "группы" для Insertion Sort

    // Вставочная сортировка для подмассива
    void insertionSort(vector<int>& bars, int left, int right) {
        for (int i = left + 1; i <= right; i++) {
            int key = bars[i];
            int j = i - 1;
            while (j >= left && bars[j] > key) {
                bars[j + 1] = bars[j];
                j--;
            }
            bars[j + 1] = key;
        }
    }

    // Слияние двух отсортированных подмассивов
    void merge(vector<int>& bars, int left, int mid, int right) {
        vector<int> temp(right - left + 1);
        int i = left, j = mid + 1, k = 0;
        while (i <= mid && j <= right) {
            if (bars[i] <= bars[j]) {
                temp[k++] = bars[i++];
            }
            else {
                temp[k++] = bars[j++];
            }
        }
        while (i <= mid) {
            temp[k++] = bars[i++];
        }
        while (j <= right) {
            temp[k++] = bars[j++];
        }
        for (int i = 0; i < k; i++) {
            bars[left + i] = temp[i];
        }
    }

    // Timsort
    void timSort(vector<int>& bars) {
        int n = bars.size();
        for (int i = 0; i < n; i += RUN) {
            insertionSort(bars, i, min((i + RUN - 1), (n - 1)));
        }
        for (int size = RUN; size < n; size = 2 * size) {
            for (int left = 0; left < n; left += 2 * size) {
                int mid = min(n - 1, left + size - 1);
                int right = min((left + 2 * size - 1), (n - 1));
                if (mid < right) {
                    merge(bars, left, mid, right);
                }
            }
        }
    }

public:
    TimSort(SortingVisualizer* visualizer) : visualizer(visualizer) {}

    void sort() {
        try {
            visualizer->setSorting(true);
            int comparisons = 0, swaps = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();
            timSort(bars);
            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Timsort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Comparisons: " << comparisons << ", Swaps: " << swaps;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Timsort: " << e.what() << endl;
        }
    }
};
class BucketSort : public SortingAlgorithm {
public:
    BucketSort(SortingVisualizer* visualizer) : SortingAlgorithm(visualizer) {}

    void sort() override {
        try {
            visualizer->setSorting(true);
            int swaps = 0, comparisons = 0;
            auto& bars = visualizer->getBars();
            auto start = chrono::high_resolution_clock::now();

            if (bars.empty()) return; // Если массив пуст, то ничего не делаем

            // Находим максимальный и минимальный элементы
            int maxVal = *max_element(bars.begin(), bars.end());
            int minVal = *min_element(bars.begin(), bars.end());

            // Количество корзин
            int numBuckets = bars.size();
            // Создаем корзины
            vector<std::vector<int>> buckets(numBuckets);

            // Распределяем элементы по корзинам
            for (int i = 0; i < bars.size(); i++) {
                int bucketIndex = (bars[i] - minVal) * (numBuckets - 1) / (maxVal - minVal);
                buckets[bucketIndex].push_back(bars[i]);
                comparisons++;
                visualizer->setCurrentIndex(i);
                this_thread::sleep_for(chrono::milliseconds(10));
            }
            for (auto& bucket : buckets) {
                std::sort(bucket.begin(), bucket.end());
                swaps += bucket.size();  // Количество операций свопов
                comparisons += bucket.size();  // Количество сравнений
            }

            // Собираем элементы из всех корзин
            int idx = 0;
            for (int i = 0; i < numBuckets; i++) {
                for (auto val : buckets[i]) {
                    bars[idx++] = val;
                    comparisons++;
                    visualizer->setCurrentIndex(idx - 1);
                    this_thread::sleep_for(chrono::milliseconds(10));
                }
            }

            auto end = chrono::high_resolution_clock::now();
            stringstream ss;
            ss << "Bucket Sort: " << chrono::duration_cast<chrono::milliseconds>(end - start).count()
                << "ms, Swaps: " << swaps << ", Comparisons: " << comparisons;
            visualizer->setSorting(false);
            visualizer->setSorted(true);
            cout << ss.str() << endl;
        }
        catch (exception& e) {
            cout << "An error occurred during Bucket Sort: " << e.what() << endl;
        }
    }
};

class SortingManager {
private:
    SortingVisualizer& visualizer;

public:
    SortingManager(SortingVisualizer& visualizer) : visualizer(visualizer) {}

    void notify(const string& event) {
        if (event == "BubbleSort") {
            BubbleSort* bubbleSort = new BubbleSort(&visualizer);
            thread(&BubbleSort::sort, bubbleSort).detach();
        }
        else if (event == "InsertionSort") {
            InsertionSort* insertionSort = new InsertionSort(&visualizer);
            thread(&InsertionSort::sort, insertionSort).detach();
        }
        else if (event == "QuickSort") {
            QuickSort* quickSort = new QuickSort(&visualizer);
            thread(&QuickSort::sort, quickSort).detach();
        }
        else if (event == "ShakerSort") {
            ShakerSort* shakerSort = new ShakerSort(&visualizer);
            thread(&ShakerSort::sort, shakerSort).detach();
        }
        else if (event == "SelectionSort") {
            SelectionSort* selectionSort = new SelectionSort(&visualizer);
            thread(&SelectionSort::sort, selectionSort).detach();
        }
        else if (event == "HeapSort") {
            HeapSort* heapSort = new HeapSort(&visualizer);
            thread(&HeapSort::sort, heapSort).detach();
        }
        else if (event == "MergeSort") {
            MergeSort* mergeSort = new MergeSort(&visualizer);
            thread(&MergeSort::sort, mergeSort).detach();
        }
        else if (event == "ShellSort") {
            ShellSort* shellSort = new ShellSort(&visualizer);
            thread(&ShellSort::sort, shellSort).detach();
        }
        else if (event == "PancakeSort") {
            PancakeSort* pancakeSort = new PancakeSort(&visualizer);
            thread(&PancakeSort::sort, pancakeSort).detach();
        }
        else if (event == "RadixSort") {
            RadixSort* radixSort = new RadixSort(&visualizer);
            thread(&RadixSort::sort, radixSort).detach();
        }
        else if (event == "GnomeSort") {
            GnomeSort* gnomeSort = new GnomeSort(&visualizer);
            thread(&GnomeSort::sort, gnomeSort).detach();
        }
        else if (event == "TimSort") {
            TimSort* timSort = new TimSort(&visualizer);
            thread(&TimSort::sort, timSort).detach();
        }
        else if (event == "BucketSort") {
            BucketSort* bucketSort = new BucketSort(&visualizer);
            thread(&BucketSort::sort, bucketSort).detach();
        }
        
    }
};

int main() {
    RenderWindow window(VideoMode(WIDTH, HEIGHT), "Sorting Visualizer by Amirhan Ordobaev");
    SortingVisualizer visualizer;

    SortingManager manager(visualizer);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {

                // Управление сортировками
                if (event.key.code == Keyboard::B) {
                    manager.notify("BubbleSort");
                }
                else if (event.key.code == Keyboard::I) {
                    manager.notify("InsertionSort");
                }
                else if (event.key.code == Keyboard::Q) {
                    manager.notify("QuickSort");
                }
                else if (event.key.code == Keyboard::S) {
                    manager.notify("ShakerSort");
                }
                else if (event.key.code == Keyboard::T) { 
                    manager.notify("SelectionSort");
                }
                else if (event.key.code == Keyboard::H) { 
                    manager.notify("HeapSort");
                }
                else if (event.key.code == Keyboard::M) { 
                    manager.notify("MergeSort");
                }
                else if (event.key.code == Keyboard::L) { 
                    manager.notify("ShellSort");
                }
                else if (event.key.code == Keyboard::P) {
                    manager.notify("PancakeSort");
                }
                else if (event.key.code == Keyboard::X) {
                    manager.notify("RadixSort");
                }
                else if (event.key.code == Keyboard::G) {
                    manager.notify("GnomeSort");
                }
                else if (event.key.code == Keyboard::U) {
                    manager.notify("TimSort");
                }
                else if (event.key.code == Keyboard::Z) {
                    manager.notify("BucketSort");
                }

                // Управление количеством баров
                else if (event.key.code == Keyboard::Up) {
                    visualizer.updateNumBars(5);  // Увеличиваем количество баров
                }
                else if (event.key.code == Keyboard::Down) {
                    visualizer.updateNumBars(-5);  // Уменьшаем количество баров
                }

                // Сброс и регенерация баров
                else if (event.key.code == Keyboard::R) {
                    visualizer.generateBars();  // Регенерация баров
                }
                else if (event.key.code == Keyboard::W) {
                    visualizer.generateWorstCase();  // Генерация худшего случая
                }

            }
        }

        visualizer.drawBars(window);
    }

    return 0;
}

