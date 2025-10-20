#include <iostream>
#include <queue>
#include <algorithm>
#include <cmath>
struct Query {
    int l, r, id;
};
int main() {
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int n;
    std::cin >> n;
    int block_size = std::ceil(std::sqrt(n));
    std::vector<int> a(n + 1), mp(n + 1), app(n + 1);
    for (int i = 1; i <= n; ++i) {
        std::cin >> a[i];
    }
    auto cpy_a = a;
    std::sort(cpy_a.begin() + 1, cpy_a.end());
    for (int i = 1; i <= n; ++i) {
        mp[i] = std::lower_bound(cpy_a.begin() + 1, cpy_a.end(), a[i]) - cpy_a.begin();
    }
    std::vector<Query> query(n);
    std::vector<int> ans(n);
    for (int cnt = 0; auto &q : query) {
        std::cin >> q.l >> q.r;
        q.id = cnt++;
    }
    std::ranges::sort(query, [&](auto i, auto j) {
        int bil = (i.l - 1) / block_size, bjl = (j.l - 1) / block_size;
        return bil == bjl ? i.r < j.r : bil < bjl;
    });
    auto get_bl_num = [&](int x) {
        return (x - 1) / block_size + 1;
    };
    
    int l = 1, r = 1, now_ans = 0;
    int now_bl = 1;
    app[mp[1]] = 1;
    std::vector<int> appt(n + 1);
    auto move = [&](int x) {
        ++app[mp[x]];
        if (app[now_ans] < app[mp[x]] || (app[now_ans] == app[mp[x]] && now_ans > mp[x])) {
            now_ans = mp[x];
        }
    };
    auto cancel = [&](int backup) {
        for (; l < backup; ++l) {
            --app[mp[l]];
        }
    };
    auto simple_query = [&](int l, int r) {
        int ans = 0;
        for (int i = l; i <= r; ++i) {
            ++appt[mp[i]];
            if (appt[ans] < appt[mp[i]] || (appt[ans] == appt[mp[i]] && ans > mp[i])) {
                ans = mp[i];
            }
        }  
        for (int i = l; i <= r; ++i) {
            --appt[mp[i]];
        }
        return cpy_a[ans];
    };
    auto init = [&]() {
        for (int i = l; i <= r; ++i) {
            --app[mp[i]];
        }
        now_ans = 0;
        l = std::min(now_bl * block_size, n) + 1;
        r = std::min(now_bl * block_size, n);
    };
    init();
    for (const auto &q : query) {
        auto bll = get_bl_num(q.l), brr = get_bl_num(q.r);
        if (bll > now_bl) {
            now_bl = bll;
            init();
        }
        if (bll == brr) {
            ans[q.id] = simple_query(q.l, q.r);
        } else {
            while (r < q.r) move(++r);
            int backup_ans = now_ans;
            while (l > q.l) move(--l);
            ans[q.id] = cpy_a[now_ans];
            cancel(std::min(now_bl * block_size, n) + 1);
            now_ans = backup_ans;
        }
    }
    for (const auto p : ans) {
        std::cout << p << '\n';
    }
    return 0;
}
/*
对原序列进行分块，对询问按以左端点所属块编号升序为第一关键字，右端点升序为第二关键字的方式排序。
按顺序处理询问：
如果询问左端点所属块 B 和上一个询问左端点所属块的不同，那么将莫队区间的左端点初始化为 B 的右端点加 1, 将莫队区间的右端点初始化为 𝐵
的右端点；
如果询问的左右端点所属的块相同，那么直接扫描区间回答询问；
如果询问的左右端点所属的块不同：
如果询问的右端点大于莫队区间的右端点，那么不断扩展右端点直至莫队区间的右端点等于询问的右端点；
不断扩展莫队区间的左端点直至莫队区间的左端点等于询问的左端点；
回答询问；
撤销莫队区间左端点的改动，使莫队区间的左端点回滚到 B 的右端点加 1
*/