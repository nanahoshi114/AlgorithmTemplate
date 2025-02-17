#include <iostream>
#include <deque>
using namespace std;
const int M = 1e6 + 5;
struct Node{
    int ind, val;
};
int arr[M];
int main(){
    cin.tie(0)->sync_with_stdio(0); cout.tie(0);
    int n, k;
    cin >> n >> k;
    deque<Node> dq;
    for(int i = 1; i <= n; i++){
        cin >> arr[i];
    }
    for(int i = 1; i <= n; i++){
        while(!dq.empty() && (dq.back().val > arr[i])){
            dq.pop_back();
        }
        while(!dq.empty() && dq.front().ind <= i - k){
            dq.pop_front();
        }
        dq.push_back({i, arr[i]});
        if(i >= k) cout << dq.front().val << ' ';
    }
    dq.clear();
    cout << '\n';
    for(int i = 1; i <= n; i++){
        while(!dq.empty() && (dq.back().val < arr[i])){
            dq.pop_back();
        }
        while(!dq.empty() && dq.front().ind <= i - k){
            dq.pop_front();
        }
        dq.push_back({i, arr[i]});
        if(i >= k) cout << dq.front().val << ' ';
    }
    cout << 1;
    return 0;
}