#include <vector>
#include <functional>

template <typename T>
class sparseTable {
    std::vector<std::array<T, 30>> table;
    std::vector<T> lg;
    T (*func)(const T, const T);
public:
    sparseTable(const std::vector<T>& arr, T (*func)(const T, const T)) : func(func), table(arr.size()), lg(arr.size()) {
        lg[1] = 0;
        int n = arr.size() - 1;
        for (int i = 2; i <= n; ++i) {
            lg[i] = lg[i / 2] + 1;
        }
        for (int i = 1; i <= n; ++i) {
            table[i][0] = arr[i];
        }
        for (int j = 1; j <= lg[n]; ++j) {
            for (int i = 1; i + (1 << j) - 1 <= n; ++i) {
                table[i][j] = func(table[i][j - 1], table[i + (1 << (j - 1))][j - 1]);
            }
        }
    }
    T query(int l, int r) {
        int k = lg[r - l + 1];
        return func(table[l][k], table[r - (1 << k) + 1][k]);
    }
};
template <typename T>
class sparseTable {
    std::vector<T> table[30];
    std::vector<T> lg;
    T (*func)(const T, const T);
public:
    sparseTable(const std::vector<T>& arr, T (*func)(const T, const T)) : func(func), lg(arr.size()) {
        for (auto& v : table) v.resize(arr.size());
        lg[1] = 0;
        int n = arr.size() - 1;
        for (int i = 2; i <= n; ++i) {
            lg[i] = lg[i / 2] + 1;
        }
        for (int i = 1; i <= n; ++i) {
            table[0][i] = arr[i];
        }
        for (int j = 1; j <= lg[n]; ++j) {
            for (int i = 1; i + (1 << j) - 1 <= n; ++i) {
                table[j][i] = func(table[j - 1][i], table[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
    T query(int l, int r) {
        int k = lg[r - l + 1];
        return func(table[k][l], table[k][r - (1 << k) + 1]);
    }
};