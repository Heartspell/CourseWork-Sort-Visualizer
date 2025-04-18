#include "SortAlgorithms.h"
#include<mutex>
#include<functional>
#include<algorithm>
SortAlgorithms::SortAlgorithms(vector<int>& bars)
    : bars(bars), sorting(false), swaps(0) {}

bool SortAlgorithms::isSorting() const {
    return sorting;
}

void SortAlgorithms::updateInfo(const string& message) {
    cout << message << endl;
}

void SortAlgorithms::drawBars(RenderWindow& window) {
    window.clear();
    lock_guard<mutex> lock(barsMutex);
    for (size_t i = 0; i < bars.size(); i++) {
        RectangleShape bar(Vector2f(800.0f / bars.size(), bars[i]));
        bar.setPosition(i * (800.0f / bars.size()), 600 - bars[i]);
        bar.setFillColor(Color::White);
        window.draw(bar);
    }
    window.display();
}

void SortAlgorithms::bubbleSort(RenderWindow& window) {
    sorting = true;
    swaps = 0;
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 0; i < bars.size() - 1 && sorting; i++) {
        for (size_t j = 0; j < bars.size() - i - 1 && sorting; j++) {
            lock_guard<mutex> lock(barsMutex);
            if (bars[j] > bars[j + 1]) {
                swap(bars[j], bars[j + 1]);
                swaps++;
            }
            drawBars(window);
            this_thread::sleep_for(chrono::milliseconds(10));
        }
    }
    sorting = false;
    auto end = chrono::high_resolution_clock::now();
    updateInfo("Bubble Sort: " + to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms, Swaps: " + to_string(swaps));
}

void SortAlgorithms::insertionSort(RenderWindow& window) {
    sorting = true;
    swaps = 0;
    auto start = chrono::high_resolution_clock::now();
    for (size_t i = 1; i < bars.size() && sorting; i++) {
        int key = bars[i];
        int j = i - 1;
        while (j >= 0 && sorting) {
            if (bars[j] > key) {
                bars[j + 1] = bars[j];
                swaps++;
            }
            else {
                break;
            }
            j--;
            drawBars(window);
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        bars[j + 1] = key;
    }
    sorting = false;
    auto end = chrono::high_resolution_clock::now();
    updateInfo("Insertion Sort: " + to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms, Swaps: " + to_string(swaps));
}

void SortAlgorithms::quickSort(RenderWindow& window) {
    sorting = true;
    swaps = 0;
    auto start = chrono::high_resolution_clock::now();

    function<void(int, int)> quickSortHelper = [&](int left, int right) {
        if (left >= right || !sorting) return;
        int pivot = bars[right];
        int i = left;
        for (int j = left; j < right; j++) {
            if (bars[j] < pivot) {
                swap(bars[i], bars[j]);
                swaps++;
                i++;
            }
            drawBars(window);
            this_thread::sleep_for(chrono::milliseconds(50));
        }
        swap(bars[i], bars[right]);
        swaps++;
        quickSortHelper(left, i - 1);
        quickSortHelper(i + 1, right);
        };

    quickSortHelper(0, bars.size() - 1);
    sorting = false;
    auto end = chrono::high_resolution_clock::now();
    updateInfo("Quick Sort: " + to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms, Swaps: " + to_string(swaps));
}

void SortAlgorithms::stalinSort(RenderWindow& window) {
    sorting = true;
    swaps = 0;
    auto start = chrono::high_resolution_clock::now();
    vector<int> newBars;
    newBars.push_back(bars[0]);

    for (size_t i = 1; i < bars.size(); i++) {
        if (bars[i] >= newBars.back()) {
            newBars.push_back(bars[i]);
        }
        else {
            swaps++;
        }
    }
    bars = newBars;
    sorting = false;
    auto end = chrono::high_resolution_clock::now();
    updateInfo("Stalin Sort: " + to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms, Swaps: " + to_string(swaps));
}

void SortAlgorithms::radixSort(RenderWindow& window) {
    sorting = true;
    swaps = 0;
    auto start = chrono::high_resolution_clock::now();

    // Find maximum number to determine the number of digits
    int max = *max_element(bars.begin(), bars.end());

    // Perform counting sort for every digit
    for (int exp = 1; max / exp > 0 && sorting; exp *= 10) {
        vector<int> output(bars.size());
        vector<int> count(10, 0);

        for (int i = 0; i < bars.size(); i++) {
            count[(bars[i] / exp) % 10]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = bars.size() - 1; i >= 0; i--) {
            output[count[(bars[i] / exp) % 10] - 1] = bars[i];
            count[(bars[i] / exp) % 10]--;
        }

        bars = output;
        drawBars(window);
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    sorting = false;
    auto end = chrono::high_resolution_clock::now();
    updateInfo("Radix Sort: " + to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms, Swaps: " + to_string(swaps));
}

void SortAlgorithms::heapSort(RenderWindow& window) {
    sorting = true;
    swaps = 0;
    auto start = chrono::high_resolution_clock::now();

    int n = bars.size();

    // Build max heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(n, i);
    }

    // One by one extract elements
    for (int i = n - 1; i >= 0; i--) {
        swap(bars[0], bars[i]);
        swaps++;
        heapify(i, 0);
        drawBars(window);
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    sorting = false;
    auto end = chrono::high_resolution_clock::now();
    updateInfo("Heap Sort: " + to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms, Swaps: " + to_string(swaps));
}

void SortAlgorithms::heapify(int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && bars[left] > bars[largest]) {
        largest = left;
    }
    if (right < n && bars[right] > bars[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(bars[i], bars[largest]);
        swaps++;
        heapify(n, largest);
    }
}

void SortAlgorithms::shellSort(RenderWindow& window) {
    sorting = true;
    swaps = 0;
    auto start = chrono::high_resolution_clock::now();

    int n = bars.size();
    int gap = n / 2;

    while (gap > 0 && sorting) {
        shellSortStep(gap);
        gap /= 2;
        drawBars(window);
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    sorting = false;
    auto end = chrono::high_resolution_clock::now();
    updateInfo("Shell Sort: " + to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms, Swaps: " + to_string(swaps));
}

void SortAlgorithms::shellSortStep(int gap) {
    for (int i = gap; i < bars.size(); i++) {
        int temp = bars[i];
        int j = i;
        while (j >= gap && bars[j - gap] > temp) {
            bars[j] = bars[j - gap];
            swaps++;
            j -= gap;
        }
        bars[j] = temp;
    }
}

void SortAlgorithms::shakerSort(RenderWindow& window) {
    sorting = true;
    swaps = 0;
    auto start = chrono::high_resolution_clock::now();

    int left = 0;
    int right = bars.size() - 1;

    while (left < right&& sorting) {
        for (int i = left; i < right; i++) {
            if (bars[i] > bars[i + 1]) {
                swap(bars[i], bars[i + 1]);
                swaps++;
            }
            drawBars(window);
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        right--;

        for (int i = right; i > left; i--) {
            if (bars[i] < bars[i - 1]) {
                swap(bars[i], bars[i - 1]);
                swaps++;
            }
            drawBars(window);
            this_thread::sleep_for(chrono::milliseconds(10));
        }
        left++;
    }

    sorting = false;
    auto end = chrono::high_resolution_clock::now();
    updateInfo("Shaker Sort: " + to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count()) + "ms, Swaps: " + to_string(swaps));
}
