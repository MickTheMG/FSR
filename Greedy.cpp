#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace chrono;

struct Item {
    double value;
    double weight;

    double val_wei_ratio() const {
        return value / weight;
    }
};

int main() {

    auto start = chrono::high_resolution_clock::now();
    ifstream input("file-name");

    int n;
    double W;
    input >> n >> W;

    vector<Item> items(n);

    for (int i = 0; i < n; i++) {
        input >> items[i].value >> items[i].weight;
    }

    input.close();

    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (items[j].val_wei_ratio() > items[max_idx].val_wei_ratio()) {
                max_idx = j;
            }
        }
        if (max_idx != i) {
            swap(items[i], items[max_idx]);
        }
    }

    double res = 0.0, max_weight = W;

    for (const auto &item : items) {
        if (max_weight <= 0)
            break;

        if (item.weight <= max_weight) {
            res += item.value;
            max_weight -= item.weight;
        } 
        else {
            res += item.val_wei_ratio() * max_weight;
            max_weight = 0;
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto work_time = duration_cast<microseconds>(end - start).count();

    cout << "Work time: " << work_time << endl;

    cout << fixed << setprecision(4);
    cout << "Max weight: " << res << endl;

    return 0;
}
