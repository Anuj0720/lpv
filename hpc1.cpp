#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

// -------------------- PRINT FUNCTION --------------------
void printArray(const vector<int>& arr) {
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;
}

// -------------------- BUBBLE SORT --------------------

// Sequential Bubble Sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Parallel Bubble Sort (Odd-Even Method)
void parallelBubbleSort(vector<int>& arr) {
    int n = arr.size();

    for (int i = 0; i < n; i++) {

        // Even phase
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }

        // Odd phase
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// -------------------- MERGE SORT --------------------

void merge(vector<int>& arr, int s, int m, int e) {

    vector<int> L(arr.begin() + s, arr.begin() + m + 1);
    vector<int> R(arr.begin() + m + 1, arr.begin() + e + 1);

    int i = 0, j = 0, k = s;

    while (i < L.size() && j < R.size()) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {w
            arr[k++] = R[j++];
        }
    }

    while (i < L.size()) arr[k++] = L[i++];
    while (j < R.size()) arr[k++] = R[j++];
}

vector<int> mergeSort(vector<int>& arr, int s, int e) {
    if (e - s + 1 <= 1) return arr;

    int m = (s + e) / 2;

    mergeSort(arr, s, m);
    mergeSort(arr, m + 1, e);

    merge(arr, s, m, e);

    return arr;
}

// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int s, int e, int depth) {
    if (e - s + 1 <= 1) return;

    int m = (s + e) / 2;

    if (depth <= 0) {
        mergeSort(arr, s, e);
    } else {
        #pragma omp parallel sections
        {
            #pragma omp section
            parallelMergeSort(arr, s, m, depth - 1);

            #pragma omp section
            parallelMergeSort(arr, m + 1, e, depth - 1);
        }
    }

    merge(arr, s, m, e);
}

// -------------------- MAIN --------------------

int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    vector<int> arr(n);

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    vector<int> a1 = arr;
    vector<int> a2 = arr;
    vector<int> a3 = arr;
    vector<int> a4 = arr;

    double start, end;

    // Sequential Bubble
    start = omp_get_wtime();
    bubbleSort(a1);
    end = omp_get_wtime();
    cout << "\nSequential Bubble Sort Time: " << (end - start) * 1000000 << endl;
    cout << "Sorted Array (Sequential Bubble): ";
    printArray(a1);

    // Parallel Bubble
    start = omp_get_wtime();
    parallelBubbleSort(a2);
    end = omp_get_wtime();
    cout << "\nParallel Bubble Sort Time: " << end - start << endl;
    cout << "Sorted Array (Parallel Bubble): ";
    printArray(a2);

    // Sequential Merge
    start = omp_get_wtime();
    mergeSort(a3, 0, n - 1);
    end = omp_get_wtime();
    cout << "\nSequential Merge Sort Time: " << (end - start) * 1000000 << endl;
    cout << "Sorted Array (Sequential Merge): ";
    printArray(a3);

    // Parallel Merge
    start = omp_get_wtime();
    parallelMergeSort(a4, 0, n - 1, 3);
    end = omp_get_wtime();
    cout << "\nParallel Merge Sort Time: " << end - start << endl;
    cout << "Sorted Array (Parallel Merge): ";
    printArray(a4);

    return 0;
}