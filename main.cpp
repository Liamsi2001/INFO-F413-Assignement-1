#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <chrono>  // For timing
#include <algorithm> // For std::shuffle
#include <random> // For std::mt19937 and std::uniform_int_distribution
#include "quickselection.hpp" // Include QuickSelect header
#include "lazyselection.hpp"   // Include LazySelect header

void generateRandomArray(int arr[], int n) {
    std::vector<int> temp(n);
    
    // Fill the vector with unique values
    for (int i = 0; i < n; i++) {
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

int main() {
    srand(time(0));  // Seed the random number generator

    // Define sizes and corresponding k values (first, 25%, median) explicitly
    // Define sizes following the geometric progression from 10,000, doubling each time
    std::vector<int> sizes;
    int current_size = 10000;
    while (current_size <= 10000000) {
        sizes.push_back(current_size);
        current_size *= 2; // Multiply by 2 for the geometric progression
    }
    std::vector<std::pair<std::string, int>> k_targets = {{"First", 1}, {"25%", 0}, {"Median", 0}};

    QuickSelect quickselect; // Create a QuickSelect instance
    LazySelect lazySelect;   // Create a LazySelect instance
    int runs = 100;          // Number of runs to average

    int total_time_quickselect = 0; // Total time for QuickSelect
    int total_time_lazyselect = 0;   // Total time for LazySelect

    // Open files to store results
    std::ofstream quickSelectOut("quick_selection_data.csv");
    quickSelectOut << "ArraySize,Run,Kth,EmpiricalComparisons,ExpectedComparisons,RunningTime(us)\n";  // CSV header

    std::ofstream lazySelectOut("lazy_selection_data.csv");
    lazySelectOut << "ArraySize,Run,Kth,EmpiricalComparisons,ExpectedComparisons,RunningTime(us)\n";  // CSV header

    // Run tests for different array sizes
    for (int n : sizes) {
        // Update k values for this array size
        k_targets[1].second = n / 4;       // 25% element
        k_targets[2].second = n / 2;       // Median element

        // Total comparisons and time for QuickSelect
        long total_comparisons_q[3] = {0};  // To store total comparisons for each k
        long long total_time_q[3] = {0};    // To store total running time for each k

        // Total comparisons and time for LazySelect
        long total_comparisons_l[3] = {0};  // To store total comparisons for each k
        long long total_time_l[3] = {0};    // To store total running time for each k

        for (int r = 0; r < runs; r++) {
            int* arr = new int[n];
            generateRandomArray(arr, n);

            // Perform QuickSelect for each k value
            for (int i = 0; i < k_targets.size(); i++) {
                // QuickSelect part
                quickselect.resetComparisonCount();
                auto start_q = std::chrono::high_resolution_clock::now();
                quickselect.randomizedQuickselect(arr, 0, n-1, k_targets[i].second);
                auto end_q = std::chrono::high_resolution_clock::now();
                int comparisons_q = quickselect.getComparisonCount();
                total_comparisons_q[i] += comparisons_q;

                // Measure time in microseconds
                long long time_q = std::chrono::duration_cast<std::chrono::microseconds>(end_q - start_q).count();
                total_time_q[i] += time_q;

                // Output each run's result for QuickSelect
                quickSelectOut << n << "," << r + 1 << "," << k_targets[i].first << "," << comparisons_q << ","
                               << 3.386 * n << "," << time_q << "\n";


            }

            // Perform LazySelect for each k value
            for (int i = 0; i < k_targets.size(); i++) {
                // LazySelect part
                lazySelect.resetComparisonCount();
                auto start_l = std::chrono::high_resolution_clock::now();
                lazySelect.lazySelect(arr, n, k_targets[i].second); // Get the k-th smallest element
                auto end_l = std::chrono::high_resolution_clock::now();
                int comparisons_l = lazySelect.getComparisonCount();
                total_comparisons_l[i] += comparisons_l;

                // Measure time in microseconds
                long long time_l = std::chrono::duration_cast<std::chrono::microseconds>(end_l - start_l).count();
                total_time_l[i] += time_l;

                // Output each run's result for LazySelect
                lazySelectOut << n << "," << r + 1 << "," << k_targets[i].first << "," << comparisons_l << ","
                              << 2 * n << "," << time_l << "\n";
            }

            delete[] arr;
        }

        // After all runs, calculate and output the average comparisons and times for QuickSelect
        for (int i = 0; i < k_targets.size(); i++) {
            double avg_comps_q = total_comparisons_q[i] / (double)runs;
            long long avg_time_q = total_time_q[i] / runs;
            quickSelectOut << n << ",Average," << k_targets[i].first << "," << avg_comps_q << ","
                           << 3.386 * n << "," << avg_time_q << "\n";
        }

        // After all runs, calculate and output the average comparisons and times for LazySelect
        for (int i = 0; i < k_targets.size(); i++) {
            double avg_comps_l = total_comparisons_l[i] / (double)runs;
            long long avg_time_l = total_time_l[i] / runs;
            lazySelectOut << n << ",Average," << k_targets[i].first << "," << avg_comps_l << ","
                           << 2 * n << "," << avg_time_l << "\n";
        }

        total_time_quickselect += total_time_q[0] + total_time_q[1] + total_time_q[2];
        total_time_lazyselect += total_time_l[0] + total_time_l[1] + total_time_l[2];
    }

    // Output total time for all runs
    total_time_quickselect /= 1000000; // Convert to seconds
    total_time_lazyselect /= 1000000;   // Convert to seconds
    std::cout << "Total time for QuickSelect: " << total_time_quickselect << " seconds\n";
    std::cout << "Total time for LazySelect: " << total_time_lazyselect << " seconds\n";

    quickSelectOut.close();
    lazySelectOut.close();
    return 0;
}
