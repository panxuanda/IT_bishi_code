#include <bits/stdc++.h>
using namespace std;

const int MOD = 998244353;
const int MAXN = 1e6 + 5; // 根据实际测试数据范围调整

vector<long long> fact(MAXN), inv_fact(MAXN);
vector<long long> a(MAXN), s(MAXN), t(MAXN);
vector<long long> A(MAXN), B(MAXN);

long long mod_pow(long long a, long long b) {
    long long res = 1;
    while (b) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}

void precompute(int max_n) {
    // 预处理阶乘和逆阶乘
    fact[0] = 1;
    for (int i = 1; i <= max_n; ++i) {
        fact[i] = fact[i-1] * i % MOD;
    }
    inv_fact[max_n] = mod_pow(fact[max_n], MOD-2);
    for (int i = max_n-1; i >= 0; --i) {
        inv_fact[i] = inv_fact[i+1] * (i+1) % MOD;
    }

    // 初始化基础序列
    a[0] = 1;
    s[0] = a[0] * inv_fact[0] % MOD; // s[0] = a[0]/0!
    t[0] = (0+1) * a[0] % MOD * inv_fact[0] % MOD; // t[0] = sum (k+1)a(k)/k! 到k=0

    for (int n = 1; n <= max_n; ++n) {
        // 计算a[n] = fact[n-1] * t[n-1]
        a[n] = fact[n-1] * t[n-1] % MOD;

        // 计算s[n] = s[n-1] + a[n]/n!
        s[n] = (s[n-1] + a[n] * inv_fact[n] % MOD) % MOD;

        // 计算t[n] = t[n-1] + (n+1)*a[n]/n!
        long long term = (n+1) * a[n] % MOD;
        term = term * inv_fact[n] % MOD;
        t[n] = (t[n-1] + term) % MOD;

        // 计算A[n] = n! * s[n-1]
        A[n] = fact[n] * s[n-1] % MOD;

        // 计算B[n]
        if (n == 1) {
            B[n] = 2; // 特殊情况
        } else {
            long long part1 = (1LL * n * (n-1)) % MOD;
            part1 = part1 * a[n-1] % MOD;
            long long part2 = 2 * A[n] % MOD;
            B[n] = (part1 + part2) % MOD;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int T;
    cin >> T;
    vector<int> cases(T);
    int max_n = 0;
    for (int i = 0; i < T; ++i) {
        cin >> cases[i];
        max_n = max(max_n, cases[i]);
    }

    precompute(max_n);

    for (int n : cases) {
        if (n == 0) {
            cout << "0\n";
            continue;
        }
        long long an = a[n];
        long long An = A[n];
        long long Bn = B[n];

        // 计算分子: (Bn - An) * an - An^2
        long long numerator = (Bn - An + MOD) % MOD;
        numerator = numerator * an % MOD;
        long long An_sq = An * An % MOD;
        numerator = (numerator - An_sq + MOD) % MOD;

        // 计算分母: an^2 的逆元
        long long denominator = an * an % MOD;
        long long inv_denominator = mod_pow(denominator, MOD-2);

        long long var = numerator * inv_denominator % MOD;
        cout << var << '\n';
    }

    return 0;
}
