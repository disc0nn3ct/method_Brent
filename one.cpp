#include <iostream>
#include <algorithm> 



#include <NTL/ZZ.h>

#include <cmath>



// (x+1)*(x+1)
inline NTL::ZZ f(NTL::ZZ x) { return (x*x+1); }

// template <class T> T f(T x) { return x*x+1; }


template <class T, class T_power> 
T super_pow(T a, T_power c)
{


}

template <class T> 
T method_Pollard(T n, T number_of)
{

} 






NTL::ZZ method_Pollard(NTL::ZZ n, NTL::ZZ seed = 1) {
    NTL::ZZ x = seed, y = seed;
    NTL::ZZ divisor = 1;
    while (divisor == 1 || divisor == n) {
        y = f(y) % n;
        x = f(f(x) % n) % n;
        divisor = std::__gcd(std::abs(x-y), n);
        // std::cout  << "y = " << y << " x = "  << x << " divisor = " << divisor << std::endl;

    }
    return divisor;
}



NTL::ZZ method_Brent(NTL::ZZ m, uint c)
{
    std::cout << "Будет произведено " << " c = " << c << ", " << pow(2, c) << " шагов " << std::endl;    
    if (c == 0)
        return 0;
    NTL::ZZ z=0,x=rand() % m;
    int n=0, k=0, t=1;
    to_3:
    std::cout  << "x = " << x << " t = " << t << " n = " << n << " k = " << k <<" z = " << z << std::endl;    
    x = f(x) % m;
    n++;
////
    NTL::ZZ p;
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
        return 1;
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
    NTL::ZZ k;
    // std::cin >> k;
    // k = 4294967297;
    // k = 112;
    k =4;
    // std::cout << method_Pollard(k) << std::endl; // 4294967297
    std::cout << "======" << std::endl;
    // std::cout << method_Brent(k, 5) << std::endl; // 4294967297

    NTL::ZZ t, r, l;
	t=2;
	r=-1; l=3;
    std::cout << "PER " << abs(t-l) << "  === " << r << std::endl;
    return 0;
}