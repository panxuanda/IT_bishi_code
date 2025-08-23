/*给你两个正整数x,y，定义一个整数如果是特别的数，那么它一定满足以下条件至少其一:
·它是 x 的整数倍。
·它是 y 的整数倍。
·它是 x 的因子。
·它是 y 的因子。
给你一个整数 n 请你求出 1~ n 中有多少个整数是特别的数。
输入描述
输入一行三个整数 n,x,y(1 < n,x,y < 10^9)如题意描述。
输出描述
输出一个整数表示1~  n中特别的数的个数。
输入：
10 3 5
输出：
6
*/
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll gcdll(ll a, ll b){
    while(b){ ll t = a % b; a = b; b = t; }
    return a;
}

vector<ll> divisors(ll v){
    vector<ll> res;
    for(ll i = 1; i * i <= v; ++i){
        if(v % i == 0){
            res.push_back(i);
            if(i != v / i) res.push_back(v / i);
        }
    }
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, x, y;
    if(!(cin >> n >> x >> y)) return 0;

    ll g = gcdll(x, y);
    // compute lcm carefully (fits in 64-bit as x,y<=1e9)
    __int128 l = ( (__int128)x / g ) * (__int128)y;
    ll lcm;
    if(l > ( (__int128)1<<62 )) lcm = (long long)l; // safe cast (l <= 1e18 here)
    else lcm = (long long)l;

    ll count = n / x + n / y - (lcm==0 ? 0 : n / lcm);

    // enumerate divisors of x and y
    vector<ll> dx = divisors(x);
    vector<ll> dy = divisors(y);

    unordered_set<ll> added; // track extra divisors we've counted to avoid double count
    added.reserve(dx.size() + dy.size() + 4);

    for(ll d : dx){
        if(d > n) continue;
        if(d % x == 0 || d % y == 0) continue;
        if(added.insert(d).second) ++count;
    }

    for(ll d : dy){
        if(d > n) continue;
        if(d % x == 0 || d % y == 0) continue;
        if(added.insert(d).second) ++count;
    }

    cout << count << '\n';
    return 0;
}
