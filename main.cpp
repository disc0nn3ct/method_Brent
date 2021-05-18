#include <iostream>
#include <algorithm> 
#include <NTL/ZZ.h>

// Алгоритмы возвращают делитель в случае успеха, а в других случаях 1



inline NTL::ZZ f(NTL::ZZ x) { return (x*x+1); }




NTL::ZZ method_Pollard(NTL::ZZ n, NTL::ZZ num_of_tries = NTL::ZZ(16384)) 
{
    NTL::ZZ x, y, divisor;
    x = RandomBnd(n);	
    y=x;
    divisor = 1;
    while ( (divisor == 1 || divisor == n) && (num_of_tries > 0) ) {
        y = f(y) % n;
        x = f(f(x) % n) % n;
        divisor = NTL::GCD(NTL::abs(x-y), n);
        num_of_tries--;
    }
    if (divisor == n)
    	return NTL::ZZ(1);
    return divisor;
}



NTL::ZZ method_Brent(NTL::ZZ m, NTL::ZZ num_of_tries = NTL::ZZ(14)) // max_i = 14 это 16384, чтобы в 2-х алгоритмах макс число шагов было одинаково 
{
    if (num_of_tries < 1)
        return NTL::ZZ(1);
    NTL::ZZ z, x, n, k, t;
    z=0;
    x=RandomBnd(m);
    n=0; k=0; t=1;
    
    to_3:
    x = f(x) % m;
    n++;
    NTL::ZZ p;
    p= NTL::GCD(m, NTL::abs(z-x));
    if( m>p && p>1)
    {
        return p;
    }
    if (n==t)
    {
        z=x;
        k++;
        t=t*2;
    }
    if (k > num_of_tries)
        return NTL::ZZ(1);
    else 
        goto to_3;
}

NTL::ZZ idea_method_Brent(NTL::ZZ m, NTL::ZZ num_of_tries = NTL::ZZ(14)) 
{   
    NTL::ZZ x=RandomBnd(m), y=f(x) % m, a=y;

    for (unsigned i=0, series_len=1; i<num_of_tries; i++, series_len*=2)
        {
            NTL::ZZ g = NTL::GCD(y-x, m);
            for (unsigned len=0; len<series_len && (g==1 || g==m); len++)
            {
                y = f(y) % m;
                g = NTL::GCD(NTL::abs(y-x), m);
            }
            x = a;
            a = y;
            if (g != 1 && g != m)
                return g;
        }
    return NTL::ZZ(1);
}







int main()
{
    NTL::ZZ k, l;
    
    power(k,2,256);

    power(l,3,256);
    k = l*k;
    std::cout << " pollard  "  << method_Pollard(k) << std::endl; // 4294967297
    std::cout << "Bernt " <<  method_Brent(k) << std::endl; // 4294967297
    std::cout << "Idea Bernt " <<  idea_method_Brent(k) << std::endl; // 4294967297

    return 0;
}