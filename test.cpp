#include "quickselection.hpp"
#include "lazyselection.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <random> // For std::mt19937 and std::uniform_int_distribution
#include <ctime>

void generateSortedArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = i; // Fill the array with sorted values from 0 to n-1
    }
}


void generateRandomArray(int arr[], int n) {
    std::vector<int> temp(n);
    
    // Fill the vector with unique values
    for (int i = 1; i <= n; i++) {
        temp[i] = i; // Generate unique values from 0 to n-1
    }

    // Shuffle the array to randomize the order
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::shuffle(temp.begin(), temp.end(), eng); // Shuffle the values
    
    // Copy the shuffled values to arr
    for (int i = 0; i < n; i++) {
        arr[i] = temp[i]; // Fill with unique random integers
    }
}

void testLargeQuickSelect(int size) {
    LazySelect qs;
    int* arr = new int[size];
    generateSortedArray(arr, size);

    int medianIndex = size / 4 - 1; // Median index
    qs.resetComparisonCount();
    
    // Measure execution time
    clock_t start = clock();
    qs.lazySelect(arr, size, medianIndex);
    clock_t end = clock();

    std::cout << "Array Size: " << size << ", Total Comparisons: " << qs.getComparisonCount();
    std::cout << ", Execution Time: " << double(end - start) / CLOCKS_PER_SEC * 1000000 << " us" << std::endl;

    delete[] arr; // Clean up memory
}

int main() {
    srand(time(0)); // Seed for random number generation

    // Test with increasing sizes
    for (int size : {10000}) {
        testLargeQuickSelect(size);
    }

    return 0;
}
