#include <cstdio>
#include <cctype>
char fast_getchar() {
	static char buf[1 << 20];
	static char *pos = buf, *end = buf;
	if (pos == end) {
		end = buf + fread(buf, 1, sizeof(buf), stdin);
		pos = buf;
		if (pos == end) return EOF;
	}
	return *pos++;
}
template <typename T>
T read(){
    T sum = 0, fl = 1;
    int ch = getchar();
    for(; !isdigit(ch); ch = getchar()){
        if(ch == '-') fl = -1;
    }
    for(; isdigit(ch); ch = getchar()){
        sum = sum * 10 + ch - '0';
    }
    return sum * fl;
}
struct istream{
	template<typename T>
	istream operator>>(T &x){
		x = 0;
		T fl = 1;
		int ch = getchar();
		for(; !isdigit(ch); ch = getchar()){
			if(ch == '-') fl = -1;
		}
		for(; isdigit(ch); ch = getchar()){
			x = x * 10 + ch - '0';
		}
		return *this;
	}	
}cin;