```cpp
auto static_convec_hull(std::vector<Pd> &pts) {
    int n = pts.size();
    std::sort(pts.begin(), pts.end());
    std::vector<int> stack;
    std::vector<bool> vis(n);
    stack.push_back(0);
    for (int i = 1; i < n; ++i) {
        while (stack.size() >= 2) {
            int s1 = stack.back(), s2 = stack[stack.size() - 2];
            if (cross((pts[s1] - pts[s2]), (pts[i] - pts[s1])) > 0) { //如果需要保留凸包边上的点，此处改为>=，同下
                break;
            }
            vis[s1] = false;
            stack.pop_back();
        }
        vis[i] = true;
        stack.push_back(i);
    }
    int down_sz = stack.size();
    for (int i = n - 2; i >= 0; --i) {
        if (vis[i]) continue;
        while (stack.size() > down_sz) {
            int s1 = stack.back(), s2 = stack[stack.size() - 2];
            if (cross((pts[s1] - pts[s2]), (pts[i] - pts[s1])) > 0) {
                break;
            }
            vis[s1] = false;
            stack.pop_back();
        }
        vis[i] = true;
        stack.push_back(i);
    }
    return stack;
}
```