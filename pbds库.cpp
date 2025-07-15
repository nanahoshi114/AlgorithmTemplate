#include <bits/extc++.h>
using i64 = long long;
struct multiset {
    __gnu_pbds::tree<std::pair<i64, int>, __gnu_pbds::null_type, std::less<>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update> st;
    int cnt = 0;
    multiset() {
        st.insert({-1, 0});
    }
    void insert(i64 x) {
        st.insert({x, ++cnt});
    }
    void erase(i64 x) {
        st.erase(st.lower_bound({x, 0}));
    }
    i64 find_by_order(int x) {
        return st.find_by_order(x)->first;
    }
    int order_of_key(i64 x) {
        return st.order_of_key({x, 0});
    }
};