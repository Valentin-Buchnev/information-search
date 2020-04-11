#include <iostream>
#include <cstdio>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#include <sys/stat.h>
using namespace std;

template <typename T>
vector<char> encode(const vector<T>& data, size_t count) {
    vector<char> result;
    for (size_t i = 0; i < count; ++i) {
        T cur = data[i];
        while (cur > 0) {
            char to_append = cur % (1 << 7);
            if (cur >= (1 << 7))
                to_append += (1 << 7);
            result.push_back(to_append);
            cur >>= 7;
        }
    }
    return result;
}

template <typename T>
vector<T> decode(const vector<char>& data, size_t length) {
    vector<T> result;
    T cur = 0;
    for (int i = length - 1; i >= 0; --i) {
        if (i != length - 1 && !(data[i] & (1 << 7))) {
            result.push_back(cur);
            cur = 0;
        }
        cur = (cur << 7) + (data[i] & 127);
        if (i == 0 && (data[i] & (1 << 7))) {
            result.push_back(cur);
        }
    }
    reverse(result.begin(), result.end());
    return result;
}

void test_algorithm(size_t N) {
        
    srand(time(NULL));
    
    fprintf(stderr, "testing algorithm...\n");
    
    // generating random data
    fprintf(stderr, "generating data... ");
    vector<int> data;
    for (size_t i = 0; i < N; ++i) {
        data.push_back(rand());
    }
    fprintf(stderr, "done\n");
    
    // encoding data
    fprintf(stderr, "encoding data... ");
    vector<char> encoded_data = encode(data, N);
    fprintf(stderr, "done\n");

    // decoding data
    fprintf(stderr, "decoding data... ");
    vector<int> decoded_data = decode <int>(encoded_data, encoded_data.size());
    fprintf(stderr, "done\n");

    bool flag = true;
    for (size_t i = 0; i < N; ++i) {
        flag &= (data[i] == decoded_data[i]);
    }
    if (flag)
        fprintf(stderr, "test passed!\n");
    else
        fprintf(stderr, "test failed!\n");
}

size_t get_file_size(string name) {
    struct stat stat_buf;
    int rc = stat(name.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1; 
}

void test_compression(size_t N, float p, float& ratio) {
    
    // generating random data

    default_random_engine generator(time(NULL));
    geometric_distribution<int> distribution(p);

    fprintf(stderr, "generating data from geometric distribution... ");
    vector<int> data;
    for (size_t i = 0; i < N; ++i) {
        data.push_back(distribution(generator));
    }
    fprintf(stderr, "done\n");
    
    // encoding data
    fprintf(stderr, "encoding data... ");
    vector<char> encoded_data = encode(data, N);
    fprintf(stderr, "done\n");
    
    float data_size = 4 * N;
    float encoded_data_size = encoded_data.size();
    ratio = encoded_data_size / data_size;
}

int main() {
    test_algorithm(10000);

    size_t N = 100000;

    freopen("compression.csv", "w", stdout);
    size_t MAX_P = 1000;
    for (size_t p = 1; p <= MAX_P; ++p) {
        float ratio=0;
        test_compression(N, p *1. / MAX_P, ratio);
        cout << p * 1. / MAX_P << "," << ratio << "\n";
    }
    return 0;
}
