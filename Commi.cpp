#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace std::chrono;

struct Coord {
    double x, y;
};

double Distance(const Coord& a, const Coord& b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

vector<int> nearestNeighbor(const vector<Coord>& locations) {
    int total = locations.size();
    vector<int> route;
    vector<bool> visit(total, false);
    route.push_back(0);
    visit[0] = true;

    for (int i = 1; i < total; ++i) {
        int last = route.back();
        int next = -1;
        double minDist = 1e9;

        for (int j = 0; j < total; ++j) {
            if (!visit[j]) {
                double dist = Distance(locations[last], locations[j]);
                if (dist < minDist) {
                    minDist = dist;
                    next = j;
                }
            }
        }

        route.push_back(next);
        visit[next] = true;
    }

    return route;
}

void swapSegment(vector<int>& route, int start, int end) {
    reverse(route.begin() + start, route.begin() + end + 1);
}

double PathLength(const vector<int>& route, const vector<Coord>& locations) {
    double length = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        length += Distance(locations[route[i]], locations[route[i + 1]]);
    }
    length += Distance(locations[route.back()], locations[route[0]]);
    return length;
}

vector<int> TwoOpt(const vector<Coord>& locations) {
    vector<int> route = nearestNeighbor(locations);
    bool improved = true;

    while (improved) {
        improved = false;
        for (size_t i = 1; i < route.size() - 1; ++i) {
            for (size_t j = i + 1; j < route.size(); ++j) {
                vector<int> newRoute = route;
                swapSegment(newRoute, i, j);

                if (PathLength(newRoute, locations) < PathLength(route, locations)) {
                    route = newRoute;
                    improved = true;
                }
            }
        }
    }

    return route;
}

void modifyThreeOpt(vector<int>& route, int a, int b, int c) {
    reverse(route.begin() + a, route.begin() + b);
    reverse(route.begin() + b, route.begin() + c);
}

vector<int> ThreeOpt(const vector<Coord>& locations, vector<int>& route) {
    bool improved = true;

    while (improved) {
        improved = false;
        for (size_t i = 1; i < route.size() - 2; ++i) {
            for (size_t j = i + 1; j < route.size() - 1; ++j) {
                for (size_t k = j + 1; k < route.size(); ++k) {
                    vector<int> newRoute = route;
                    modifyThreeOpt(newRoute, i, j, k);

                    if (PathLength(newRoute, locations) < PathLength(route, locations)) {
                        route = newRoute;
                        improved = true;
                    }
                }
            }
        }
    }

    return route;
}

int main() {
    ifstream fileInput("tsp_5_1");

    int n;
    fileInput >> n;
    vector<Coord> locs(n);

    for (int i = 0; i < n; ++i) {
        fileInput >> locs[i].x >> locs[i].y;
    }
    fileInput.close();
    

    high_resolution_clock::time_point startTwoOpt = high_resolution_clock::now();
    vector<int> path_2 = TwoOpt(locs);
    high_resolution_clock::time_point endTwoOpt = high_resolution_clock::now();

    duration<double> time_2 = duration_cast<duration<double> >(endTwoOpt - startTwoOpt);

    cout << PathLength(path_2, locs) << endl;
    for (size_t i = 0; i < path_2.size(); i++) {
        cout << path_2[i] << " ";
    }
    cout << "\n" << fixed << setprecision(6) << time_2.count() << " s\n\n";

    high_resolution_clock::time_point startThreeOpt = high_resolution_clock::now();
    vector<int> path_3 = ThreeOpt(locs, path_2);
    high_resolution_clock::time_point endThreeOpt = high_resolution_clock::now();
    duration<double> time_3 = duration_cast<duration<double> >(endThreeOpt - startThreeOpt);

    cout << PathLength(path_3, locs) << endl;
    for (size_t i = 0; i < path_3.size(); i++) {
        cout << path_3[i] << " ";
    }
    cout << "\n" << fixed << setprecision(6) << time_3.count() << " s\n";

    return 0;
}