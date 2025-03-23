#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

struct Item {
    int value, weight;
};

struct Node {
    int level, profit, weight;
    double bound;
};

struct Compare {
    bool operator()(const Node &a, const Node &b) {
        return a.bound < b.bound;
    }
};

double computeBound(const Node &u, int W, const vector<Item> &items, int n) {
    if (u.weight >= W) return 0;

    double profitBound = u.profit;
    int totalWeight = u.weight, j = u.level + 1;

    while (j < n && totalWeight + items[j].weight <= W) {
        totalWeight += items[j].weight;
        profitBound += items[j].value;
        j++;
    }

    if (j < n) {
        profitBound += (W - totalWeight) * (double(items[j].value) / items[j].weight);
    }

    return profitBound;
}

int knapsack(int W, vector<Item> &items) {
    int n = items.size();
    sort(items.begin(), items.end(), [](const Item &a, const Item &b) {
        return double(a.value) / a.weight > double(b.value) / b.weight;
    });

    int maxProfit = 0;

    priority_queue<Node, vector<Node>, Compare> pq;
    Node u{}, v{};

    u.level = -1;
    u.profit = 0;
    u.weight = 0;

    u.bound = computeBound(u, W, items, n);
    pq.push(u);

    while (!pq.empty()) {
        u = pq.top();
        pq.pop();

        if (u.bound > maxProfit) {
            v.level = u.level + 1;

            if (v.level < n) {
                v.weight = u.weight + items[v.level].weight;
                v.profit = u.profit + items[v.level].value;

                if (v.weight <= W && v.profit > maxProfit)
                    maxProfit = v.profit;

                v.bound = computeBound(v, W, items, n);
                if (v.bound > maxProfit)
                    pq.push(v);

                v.weight = u.weight;
                v.profit = u.profit;

                v.bound = computeBound(v, W, items, n);

                if (v.bound > maxProfit) {
                    pq.push(v);
                }
                    
            }
        }
    }

    return maxProfit;
}

int main() {
    auto time = high_resolution_clock::now();

    ifstream input("ks_300_0");

    int N, W;
    input >> N >> W;
    vector<Item> items(N);

    for (auto &item : items)
        input >> item.value >> item.weight;

    input.close();

    auto start = high_resolution_clock::now();
    int result = knapsack(W, items);
    auto end = high_resolution_clock::now();

    auto totalEnd = high_resolution_clock::now();

    cout << result << endl;
    cout << "Время работы алгоритма: " << duration_cast<microseconds>(end - start).count() << " мкс" << endl;
    cout << "Полное время выполнения: " << duration_cast<microseconds>(totalEnd - time).count() << " мкс" << endl;

    return 0;
}