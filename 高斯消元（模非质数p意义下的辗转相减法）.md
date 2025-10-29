```cpp
#include<bits/stdc++.h>  
using namespace std;  
#define int long long  
int p;  
int Gs(vector<vector<int>> G){  
    int n=G.size()-1;  
    int res=1,w=1;  
    for(int i=1;i<=n;i++){  
        for(int j=i+1;j<=n;j++){  
            while(G[i][i]){  
                int div=G[j][i]/G[i][i];  
                for(int k=i;k<=n;k++){  
                    G[j][k]=(G[j][k]-div*G[i][k]%p+p)%p;  
                }  
                swap(G[i],G[j]);w=-w;  
            }  
            swap(G[i],G[j]);w=-w;  
        }  
    }  
    for(int i=1;i<=n;i++){  
        res*=G[i][i];  
        res%=p;  
    }  
    res*=w;  
    return (res+p)%p;  
}
```