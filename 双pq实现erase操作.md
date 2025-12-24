```cpp
template<typename T, typename Cmp = std::less<>>
class multiset {
    std::priority_queue<T, std::vector<T>, Cmp> que, delque;
public:
    void push(T x) {
        que.push(x);
    }
    void erase(T x) {
        delque.push(x);
    }
    T top() {
        while (!que.empty() && !delque.empty() && que.top() == delque.top()) {
            que.pop();
            delque.pop();
        }
        return que.top();
    }
    void pop() {
        while (!que.empty() && !delque.empty() && que.top() == delque.top()) {
            que.pop();
            delque.pop();
        }
        que.pop();
    }
};
```