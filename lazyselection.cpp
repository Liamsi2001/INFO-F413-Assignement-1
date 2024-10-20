#include "lazyselection.hpp"
#include <algorithm> // For std::sort and std::vector
#include <random>    // For random number generation
#include <iostream>  // For logging
#include <cmath>     // For std::pow

LazySelect::LazySelect() : comparison_count(0) {}

void LazySelect::resetComparisonCount() {
    comparison_count = 0;
}

int LazySelect::getComparisonCount() const {
    return comparison_count;
}

void LazySelect::lazySelect(int arr[], int n, int k) {
    std::vector<int> sample;
    sampleAndSort(arr, n, sample); // Step 1: Sample and sort

    int a, b;
    getRange(k, n, a, b, sample); // Step 3: Determine the range [a, b]

    // Determine the ranks r_S(a) and r_S(b) in the entire set S
    int rank_a = 0, rank_b = 0;
    std::vector<int> P; // Set P to hold the qualifying elements

    // Single pass through the array to classify elements
    for (int i = 0; i < n; i++) {
        // Count comparisons only when we're making them
        if (arr[i] < a) {
            comparison_count++; // Comparison with a
            rank_a++;
        }
        if (arr[i] < b) {
            comparison_count++; // Comparison with b
            rank_b++;
        }

        // Case 1: k < n^(1/4), P contains elements <= b
        if (k < std::pow(n, 1.0 / 4.0) && arr[i] <= b) {
            comparison_count++; // Comparison with b
            P.push_back(arr[i]);
        }

        // Case 2: k > n - n^(1/4), P contains elements >= a
        else if (k > n - std::pow(n, 1.0 / 4.0) && arr[i] >= a) {
            comparison_count++; // Comparison with a
            P.push_back(arr[i]);
        }

        // Case 3: n^(1/4) <= k <= n - n^(1/4), P contains elements in [a, b]
        else if (k >= std::pow(n, 1.0 / 4.0) && k <= n - std::pow(n, 1.0 / 4.0) && arr[i] >= a && arr[i] <= b) {
            comparison_count += 2; // Comparisons with a and b
            P.push_back(arr[i]);
        }
    }

    // Do not count comparisons during sorting
    std::sort(P.begin(), P.end()); // Final sort (comparisons not counted)

    // If the size of P is small enough, return the k-th element directly
    if (P.size() <= 4 * std::pow(n, 3.0 / 4.0) + 2) {
        int adjusted_k = k - rank_a;

        if (adjusted_k >= 0 && adjusted_k < P.size()) {
            //std::cout << "Returning the k-th smallest element: " << P[adjusted_k] << std::endl;
            return;
        } else {
            //std::cerr << "Error: Adjusted index out of bounds for P. adjusted_k = " << adjusted_k << std::endl;
            return;
        }
    } else {
        // P is too large; make a recursive call but restrict the range to [a, b]
        //std::cerr << "Resampling with restricted range since P is too large. P.size() = " << P.size() << std::endl;

        // Recursive call only considering the elements within the range [a, b]
        std::vector<int> restrictedArr;
        for (int i = 0; i < n; i++) {
            if (arr[i] >= a && arr[i] <= b) {
                restrictedArr.push_back(arr[i]);
            }
        }
        lazySelect(restrictedArr.data(), restrictedArr.size(), k - rank_a); // Recursive call with reduced array
    }
}


// Sampling and sorting of the sample
void LazySelect::sampleAndSort(int arr[], int n, std::vector<int>& sample) {
    int sampleSize = static_cast<int>(std::pow(n, 3.0 / 4.0));

    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<> distr(0, n - 1);

    sample.reserve(sampleSize);

    for (int i = 0; i < sampleSize; i++) {
        sample.push_back(arr[distr(eng)]); // Randomly choose elements
    }

    std::sort(sample.begin(), sample.end()); // Sort the sample

/*     std::cerr << "Sampled elements (sorted): ";
    for (const auto& elem : sample) {
        std::cerr << elem << " ";
    }
    std::cerr << std::endl; */
}

// Determine the range [a, b] that contains the k-th smallest element
void LazySelect::getRange(int k, int n, int& a, int& b, const std::vector<int>& sortedSample) {
    int x = k * std::pow(n, -1.0 / 4.0);
    int l = std::max(x - static_cast<int>(std::sqrt(n)), 0); // Narrower range for 0
    int h = std::min(x + static_cast<int>(std::sqrt(n)), static_cast<int>(std::pow(n, 3.0 / 4.0)) - 1); // Narrower range for h

    a = sortedSample[l - 1]; // Lower bound
    b = sortedSample[h - 1]; // Upper bound

    //std::cerr << "Selected range: a = " << a << ", b = " << b << " (lowIndex = " << l << ", highIndex = " << h << ")" << std::endl;
}





