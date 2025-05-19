#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <boost/thread.hpp>
#include <algorithm>
#include <chrono>

using namespace std;

struct Item {
    double value;
    double weight;

    double val_wei_ratio() const {
        return value / weight;
    }
};

void selection_sort(vector<Item>& items) {
    size_t n = items.size();
    for (size_t i = 0; i < n - 1; i++) {
        size_t max_index = i;
        for (size_t j = i + 1; j < n; j++) {
            if (items[j].val_wei_ratio() > items[max_index].val_wei_ratio()) {
                max_index = j;
            }
        }
        if (max_index != i) {
            swap(items[i], items[max_index]);
        }
    }
}

void threaded_sort_segment(const vector<Item>& input_items, size_t start, size_t end, vector<Item>& sorted_out) {
    vector<Item> segment(input_items.begin() + start, input_items.begin() + end);
    selection_sort(segment);
    sorted_out = std::move(segment);
}

int main() {
    chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();

    ifstream input("ks_10000_0");

    int n;
    double W;
    input >> n >> W;

    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        input >> items[i].value >> items[i].weight;
    }
    input.close();

    unsigned int num_threads = boost::thread::hardware_concurrency();
    if (num_threads == 0 || num_threads > static_cast<unsigned int>(n)) num_threads = 2;

    size_t thread_size = n / num_threads;
    vector<vector<Item>> sorted_chunks(num_threads);
    vector<boost::thread> threads;

    for (unsigned int i = 0; i < num_threads; i++) {
        size_t start_idx = i * thread_size;
        size_t end_idx = (i == num_threads - 1) ? n : (start_idx + thread_size);
        threads.emplace_back(threaded_sort_segment, cref(items), start_idx, end_idx, ref(sorted_chunks[i]));
    }

    for (boost::thread& t : threads) {
        t.join();
    }

    vector<Item> merged;
    for (size_t i = 0; i < sorted_chunks.size(); ++i) {
        merged.insert(merged.end(), sorted_chunks[i].begin(), sorted_chunks[i].end());
    }

    selection_sort(merged);

    double res = 0.0, max_weight = W;

    for (size_t i = 0; i < merged.size(); ++i) {
        const Item& item = merged[i];
        if (max_weight > 0) {
            if (item.weight <= max_weight) {
                res += item.value;
                max_weight -= item.weight;
            } else {
                res += item.val_wei_ratio() * max_weight;
                max_weight = 0;
            }
        }
    }

    cout << fixed << setprecision(4);
    cout << "Max value: " << res << endl;

    chrono::high_resolution_clock::time_point end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> work_time = end_time - start_time;

    cout << "Work time: " << work_time.count() << " sec" << endl;

    return 0;
}