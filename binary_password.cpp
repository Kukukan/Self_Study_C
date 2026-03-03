#include <iostream>
#include <climits>
using namespace std;

int countBits(long long n) {
    int count = 0;
    while (n) {
        if (n & 1) count++;
        n >>= 1;
    }
    return count;
}

long long findPrevious(long long x) {
    int k = countBits(x);
    long long result = x - 1;
    
    while (result > 0 && countBits(result) != k) {
        result--;
    }
    
    return result > 0 ? result : 0;
}

long long findNext(long long x) {
    int k = countBits(x);
    long long result = x + 1;
    
    while (result <= LLONG_MAX && countBits(result) != k) {
        result++;
    }
    
    if (result > LLONG_MAX) return 0;
    return result;
}

int main() {
    long long X;
    cin >> X;
    
    cout << findPrevious(X) << " " << findNext(X) << endl;
    
    return 0;
}