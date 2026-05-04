#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int n;

    cout << "Enter number of elements: ";
    cin >> n;

    int arr[n];   

    cout << "Enter elements:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    int sum = 0;
    int minVal = arr[0];
    int maxVal = arr[0];

    #pragma omp parallel for reduction(+:sum) reduction(min:minVal) reduction(max:maxVal)
    for (int i = 0; i < n; i++) {
        sum += arr[i];

        if (arr[i] < minVal)
            minVal = arr[i];

        if (arr[i] > maxVal)
            maxVal = arr[i];
    }

    float avg = (float)sum / n;

    cout << "\nSum = " << sum << endl;
    cout << "Min = " << minVal << endl;
    cout << "Max = " << maxVal << endl;
    cout << "Average = " << avg << endl;

    return 0;
}