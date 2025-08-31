#include <iostream>
#include <vector>
#include <bitset>
#include <numeric>
#include <cstdint>
#include <queue>
#include <bit>
#include <algorithm>
class dynamic_bitset {
    using ele_type = unsigned long long;
    const ele_type max_value = std::numeric_limits<ele_type>::max();
    ele_type get_spec_bit(size_t l, size_t r) const {
        return ((r == 63 ? max_value : ((1ull << (r + 1)) - 1)) >> l) << l;
    }
    std::vector<ele_type> ele;
    size_t size;
public:
    explicit 
    dynamic_bitset(size_t size, bool val = false) : 
    size(size), ele(size / 64 + (size % 64 != 0), val ? max_value : 0ull) {}
    dynamic_bitset() : size() {}
    bool operator[](size_t pos) const {
        return (bool)(ele[pos / 64] >> (pos % 64) & 1ull);
    }
    void set(size_t pos, bool val) {
        if (this->operator[](pos) != val) {
            ele[pos / 64] ^= (1ull << (pos % 64));
        }
    }
    size_t get_size() {
        return size;
    }
    void push_back(bool val) {
        if (size % 64 == 0 && size / 64 >= ele.size()) {
            ele.push_back(0ull);
        }
        this->set(size++, val);
    }
    void pop_back() {
        if (size % 64 == 0) {
            ele.pop_back();
        }
        --size;
    }
    void resize(size_t _size) {
        if (size < _size) {
            for (size_t i = size / 64 + 1; i <= _size / 64; ++i) {
                ele.push_back(0ull);
            }
        } else if (size > _size) {
            for (size_t i = size / 64 - 1; i >= _size / 64; --i) {
                ele.pop_back();
            }
        }
        size = _size;
    }
    void range_set(size_t lpos, size_t rpos, bool val) {
        if (lpos < rpos && rpos < size) {
            auto bl = lpos / 64, br = rpos / 64;
            if (val) {
                if (bl < br) {
                    ele[bl] |= get_spec_bit(lpos % 64, 63);
                    for (size_t i = bl + 1; i <= br - 1; --i) {
                        ele[i] = max_value;
                    }
                    ele[br] |= get_spec_bit(0, rpos % 64);
                } else {
                    ele[br] |= get_spec_bit(lpos % 64, rpos % 64);
                }
            } else {
                if (bl < br) {
                    ele[bl] &= ~get_spec_bit(lpos % 64, 63);
                    for (size_t i = bl + 1; i <= br - 1; --i) {
                        ele[i] = 0ull;
                    }
                    ele[br] &= ~get_spec_bit(0, rpos % 64);
                } else {
                    ele[br] &= ~get_spec_bit(lpos % 64, rpos % 64);
                }
            }
        }
    }
    void range_flip(size_t lpos, size_t rpos) {
        if (lpos < rpos && rpos < size) {
            if (lpos / 64 < rpos / 64) {
                ele[lpos / 64] ^= get_spec_bit(lpos % 64, 63);
                for (size_t i = lpos / 64 + 1; i <= rpos / 64 - 1; ++i) {
                    ele[i] ^= max_value;
                }
                ele[rpos / 64] ^= get_spec_bit(0, rpos % 64);
            } else {
                ele[rpos / 64] ^= get_spec_bit(lpos % 64, rpos % 64);
            }
        }
    }
    void operator&=(const dynamic_bitset& rbs) {
        if (size < rbs.size) {
            this->resize(rbs.size);
        }
        auto bl = size / 64, br = rbs.size / 64;
        for (size_t i = 0; i < br; ++i) {
            ele[i] &= rbs.ele[i];
        }
        if (size > rbs.size) {
            ele[br] &= rbs.ele[br] & get_spec_bit(0, rbs.size % 64);
        } else {
            ele[bl] &= rbs.ele[br] & get_spec_bit(0, size % 64);
        }
    }
    void operator|=(const dynamic_bitset& rbs) {
        if (size < rbs.size) {
            this->resize(rbs.size);
        }
        auto bl = size / 64, br = rbs.size / 64;
        for (size_t i = 0; i < br; ++i) {
            ele[i] |= rbs.ele[i];
        }
        if (size > rbs.size) {
            ele[br] |= rbs.ele[br] & get_spec_bit(0, rbs.size % 64);
        } else {
            ele[bl] |= rbs.ele[br] & get_spec_bit(0, size % 64);
        }
    }
    void operator^=(const dynamic_bitset& rbs) {
        if (size < rbs.size) {
            this->resize(rbs.size);
        }
        auto bl = size / 64, br = rbs.size / 64;
        for (size_t i = 0; i < br; ++i) {
            ele[i] ^= rbs.ele[i];
        }
        if (size > rbs.size) {
            ele[br] ^= rbs.ele[br] & get_spec_bit(0, rbs.size % 64);
        } else {
            ele[bl] ^= rbs.ele[br] & get_spec_bit(0, size % 64);
        }
    }
    size_t count() const {
        size_t cnt = 0;
        for (size_t i = 0; i < size / 64; ++i) {
            cnt += std::popcount<ele_type>(ele[i]);
        }
        cnt += std::popcount<ele_type>(ele[size / 64] & get_spec_bit(0, size % 64));
        return cnt;
    }
};
