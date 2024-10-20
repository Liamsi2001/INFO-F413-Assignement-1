#ifndef QUICKSELECTION_HPP
#define QUICKSELECTION_HPP

class QuickSelect {
private:
    int comparison_count;

public:
    QuickSelect();
    void resetComparisonCount();
    int getComparisonCount() const;

    void randomizedQuickselect(int arr[], int low, int high, int k);
    int randomizedPartition(int arr[], int low, int high);
    int getRandomPivot(int low, int high); // Function to get a random pivot index
    void swap(int* a, int* b);
};

#endif // QUICKSELECTION_HPP
