#include <iostream>
#include <algorithm> 
#include <NTL/ZZ.h>

#include <map>
#include <string>
#include <fstream>
#include <sstream>
// Алгоритмы возвращают делитель в случае успеха, а в других случаях 1



inline NTL::ZZ f(NTL::ZZ x) { return (x*x+1); }

// Для того, чтобы тесты были объективнее, начальное случайное значение будет передаваться в алгоритм

NTL::ZZ method_Pollard(NTL::ZZ n, NTL::ZZ x = NTL::ZZ(-1),NTL::ZZ num_of_tries = NTL::ZZ(16384)) // Po - algo 
{
    NTL::ZZ y, divisor;
    if(x==-1)
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



NTL::ZZ method_Brent(NTL::ZZ m, NTL::ZZ x = NTL::ZZ(-1), NTL::ZZ num_of_tries = NTL::ZZ(14)) // max_i = 14 это 16384, чтобы в 2-х алгоритмах макс число шагов было одинаково 
{
    if (num_of_tries < 1)
        return NTL::ZZ(1);
    NTL::ZZ z, n, k, t;
    z=0;
    if(x==-1)
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

NTL::ZZ idea_method_Brent(NTL::ZZ m, NTL::ZZ x= NTL::ZZ(-1), NTL::ZZ num_of_tries = NTL::ZZ(14)) 
{   
    // NTL::ZZ x=RandomBnd(m), y=f(x) % m, a=y;
    NTL::ZZ y=f(x) % m, a=y;
    if(x == -1)
        x=RandomBnd(m);
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


NTL::ZZ method_Ferma(NTL::ZZ m) // Только для нечетных
{
    NTL::ZZ x = SqrRoot(m), y = NTL::ZZ(0), r = x*x - y*y - m;
    std::cout << "x = "<< x << " y = " << y << " r = " << r << std::endl;
    for (;;)
    {
            std::cout << "x = "<< x << " y = " << y << " r = " << r << std::endl;

        if (r == 0)
            return x!=y ? x-y : x+y;
        else
            if (r > 0)
            {
                r -= y+y+1;
                ++y;
            }
            else
            {
                r += x+x+1;
                ++x;
            }
    }
}

bool prime(NTL::ZZ n)
{ 
    for(NTL::ZZ i=NTL::ZZ(2);i<=SqrRoot(n);i++)
        if(n%i==0)
            return false;
    return true;
}


std::string zToString(const NTL::ZZ &z) {
    std::stringstream buffer;
    buffer << z;
    return buffer.str();
}

std::string str(NTL::ZZ k, NTL::ZZ i) //  создаю строку с замерами 
{
    std::string str1;
    str1=zToString(k) + ",";


    auto begin = std::chrono::steady_clock::now();

    method_Pollard(k, i);

    auto end = std::chrono::steady_clock::now();
    
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    
    str1 += std::to_string(elapsed_ms.count()) + ',';




    begin = std::chrono::steady_clock::now();

    method_Brent(k, i);

    end = std::chrono::steady_clock::now();
    
    elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    
    str1 += std::to_string(elapsed_ms.count()) + ',';



    begin = std::chrono::steady_clock::now();

    method_Brent(k, i);

    end = std::chrono::steady_clock::now();
    
    elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    str1 += std::to_string(elapsed_ms.count()) + ',';

    // std::cout << str1 << std::endl;

    return str1;
}


void test(uint num_of_test=10)
{
    NTL::ZZ k, l;
    power(k,2,256);
    power(l,3,256);
    k = l*k;  

    NTL::ZZ i=RandomBnd(k);

    std::ofstream fout("log.csv");


    for(int l=0; l<num_of_test; l++)
    {
        fout << str(k, i)  << std::endl;
        i=RandomBnd(k);
    
    }

    fout.close();

}


void my_factorizator(NTL::ZZ m, std::map<NTL::ZZ, unsigned> &res, NTL::ZZ not_used)
{
    if(m!=1)
    {
        if(prime(m) == true)
            ++res[m]; 
        else
        {
        NTL::ZZ div=NTL::ZZ(1);
        div=idea_method_Brent(m);
        if(div==1)
            if(m%2==0)
                div=NTL::ZZ(2);
        if(div==1)
            div=method_Ferma(m);
        my_factorizator(div, res, not_used);
        my_factorizator(m/div, res, not_used);
        }
    }
}



int main()
{
    test(100);



    NTL::ZZ k, l;
    power(k,2,256);
    power(l,3,256);
    k = l*k;  
    std::map <NTL::ZZ, unsigned> m;
    my_factorizator(k, m, NTL::ZZ(0));
    for (std::map <NTL::ZZ, unsigned>::iterator i=m.begin(); i!=m.end(); ++i)
        std::cout << i->first << ' ' << i->second << std::endl;
    
    return 0;
}