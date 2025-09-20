namespace Trie01 {
    int tree[MAXN * MAXBIT][2], total, root[MAXN], count[MAXN * MAXBIT];
    void insert(int id, int x) {
        root[id] = ++total;
        int cur = root[id];
        int precur = root[id - 1];
        for (int i = MAXBIT; i >= 0; --i) {
            int now = (x >> i) & 1;
            tree[cur][now] = ++total;
            tree[cur][!now] = tree[precur][!now];
            cur = tree[cur][now];
            precur = tree[precur][now];
            count[cur] = count[precur] + 1;
        }
    }
    int query(int l, int r, int x) {
        int res = 0;
        int curr = root[r], curl = root[l - 1];
        for (int i = MAXBIT; i >= 0; --i) {
            int now = (x >> i) & 1;
            if (count[tree[curr][!now]] - count[tree[curl][!now]] > 0) {
                res |= (1 << i);
                curr = tree[curr][!now];
                curl = tree[curl][!now];
            } else {
                curr = tree[curr][now];
                curl = tree[curl][now];
            }
        }
        return res;
    }
}