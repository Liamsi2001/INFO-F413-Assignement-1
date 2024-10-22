#include "quickselection.hpp"
#include <cstdlib>
#include <algorithm>
#include <random>
#include <iostream>

QuickSelect::QuickSelect() : comparison_count(0) {}

void QuickSelect::resetComparisonCount() {
    comparison_count = 0;
}

int QuickSelect::getComparisonCount() const {
    return comparison_count;
}

// Randomized QuickSelect with optimizations for early exit and smaller recursive depth for equal elements
void QuickSelect::randomizedQuickselect(int arr[], int low, int high, int k) {
    if (low < high) {
        int pi = randomizedPartition(arr, low, high);

        if (pi == k) return; // Found the k-th smallest element
        else if (pi < k) randomizedQuickselect(arr, pi + 1, high, k); // Search right
        else randomizedQuickselect(arr, low, pi - 1, k); // Search left
    }
}

// Optimized partitioning with reduced comparisons
int QuickSelect::randomizedPartition(int arr[], int low, int high) {
    int pivotIndex = getRandomPivot(low, high);
    swap(&arr[pivotIndex], &arr[high]); // Move pivot to end
    int pivot = arr[high]; 
    int i = low - 1;
    bool sorted = true; // Track if array is already sorted
    
    for (int j = low; j < high; j++) {
        comparison_count++; // Increment comparison count
        if (arr[j] <= pivot) {
            if (arr[j] != arr[j - 1] && j > low) { // Check if the array is not already sorted
                sorted = false;
            }
            i++;
            if (i != j) { // Avoid unnecessary swaps (i and j are same when no swap is needed)
                swap(&arr[i], &arr[j]);
            }
        } else if (sorted && arr[j] > pivot) {
            break; // Early exit if array is sorted
        }
    }
    swap(&arr[i + 1], &arr[high]); // Place pivot in correct position

    return i + 1; // Return pivot index
}

// Random pivot selection
int QuickSelect::getRandomPivot(int low, int high) {
    std::random_device rd; 
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(low, high); // Uniform distribution
    return distr(eng); // Return random index
}

void QuickSelect::swap(int* a, int* b) {
    if (a != b) { // Only swap if the two elements are different
        int temp = *a;
        *a = *b;
        *b = temp;
    }
}
