#ifndef LAZYSELECTION_HPP
#define LAZYSELECTION_HPP

#include <vector>

class LazySelect {
private:
    int comparison_count;
    int kth_element;
public:
    LazySelect();
    int getKthElement() const;
    void lazySelect(int arr[], int n, int k);
    void resetComparisonCount();
    int getComparisonCount() const;
    void sampleAndSort(int arr[], int n, std::vector<int>& sample);
    void getRange(int k, int n, int& a, int& b, const std::vector<int>& sortedSample);
};

#endif // LAZYSELECTION_HPP
