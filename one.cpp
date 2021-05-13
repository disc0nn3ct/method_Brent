#include <iostream>
#include <algorithm> 


#include <cmath>

typedef long long ll;

// (x+1)*(x+1)
inline ll f(ll x) { return (x*x+1); }



ll method_Pollard(ll n, ll seed = 1) {
    ll x = seed, y = seed;
    ll divisor = 1;
    while (divisor == 1 || divisor == n) {
        y = f(y) % n;
        x = f(f(x) % n) % n;
        divisor = std::__gcd(std::abs(x-y), n);
        // std::cout  << "y = " << y << " x = "  << x << " divisor = " << divisor << std::endl;

    }
    return divisor;
}



ll method_Brent(ll m, uint c)
{
    std::cout << "Будет произведено " << " c = " << c << ", " << pow(2, c) << " шагов " << std::endl;    
    if (c == 0)
        return 0;
    ll z=0,x=rand() % m;
    int n=0, k=0, t=1;
    to_3:
    std::cout  << "x = " << x << " t = " << t << " n = " << n << " k = " << k <<" z = " << z << std::endl;    
    x = f(x) % m;
    n++;
////
    ll p;
    p= std::__gcd(m, std::abs(z-x));
    if( m>p && p>1)
    {
        std::cout << " =========================== p = " << p <<  std::endl;
        return p;
    }
    std::cout << " =========================== p = " << p <<  std::endl;
////    
    if (n==t)
    {
        z=x;
        k++;
        t=t*2;
    }
    if (k > c)
        return -1;
    else 
        goto to_3;
    // ll p;
    // p= std::__gcd(m, std::abs(z-x));
    // std::cout << " =========================== " << p <<  std::endl;
    // if( m>p && p>1)
    //     return p;
    // else 
    //     goto to_3;
}


int main()
{
    long long k;
    // std::cin >> k;
    // k = 4294967297;
    // k = 112;
    k =4;
    // std::cout << method_Pollard(k) << std::endl; // 4294967297
    std::cout << "======" << std::endl;
    std::cout << method_Brent(k, 5) << std::endl; // 4294967297

    return 0;
}