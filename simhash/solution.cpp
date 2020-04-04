#include <cstdio>
#include <iostream>
#include <vector>
#include <map>
#define ull unsigned long long
using namespace std;

vector<bool> used;
vector<ull> h;
vector<size_t> groups[4][1LL << 16LL];

map<size_t, size_t> groups_sizes;

ull part(ull x, size_t part) {
    return (x >> (16LL * part)) & ((1LL << 16LL) - 1);
}

size_t collect_group(size_t cur_ind) {
    size_t size = 0;
    for (size_t i = 0; i < 4; ++i) {
        for (auto ind: groups[i][part(h[cur_ind], i)]) {
            if (__builtin_popcountll(h[cur_ind] ^ h[ind]) <= 3) {
		size += !used[ind];
		used[ind] = true;
            }
        }
    }
    return size;
}

int main() {

    freopen("simhash_sorted.txt", "r", stdin);
    freopen("result.csv", "w", stdout);

    ull x;
    while (cin >> x) {
        h.push_back(x);
    }
    used.resize(h.size());
    fill(used.begin(), used.end(), false);

    for (size_t p = 0; p < 4; p++) {
    	for (size_t i = 0; i < h.size(); ++i) {
        	groups[p][part(h[i], p)].push_back(i);
    	}
    }

    for (size_t i = 0; i < h.size(); ) {
        
	size_t size = collect_group(i);
        if (groups_sizes.find(size) == groups_sizes.end())
            groups_sizes[size] = 1;
        else
            groups_sizes[size] += 1;
        
	while (i < used.size() && used[i]) {
            ++i;
        }
    }

    for (auto& it : groups_sizes) {
        cout << it.first << ',' << it.second << "\n";
    }
    
    return 0;
}
