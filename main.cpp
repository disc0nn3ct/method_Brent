#include <iostream>
#include <algorithm> 
#include <NTL/ZZ.h>

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>
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


// тут x это начальное значение для метода (рандомое)
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

    NTL::ZZ for_test_val;
tryAgain:
    auto begin = std::chrono::steady_clock::now();

    for_test_val = method_Pollard(k, i);

    auto end = std::chrono::steady_clock::now();
    
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    if(for_test_val == 1)
        goto tryAgain;
        
    str1 += std::to_string(elapsed_ms.count()) + ',';



tryAgain1:
    begin = std::chrono::steady_clock::now();

    for_test_val = method_Brent(k, i);

    end = std::chrono::steady_clock::now();
    
    elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    if(for_test_val == 1)
        goto tryAgain1;

    str1 += std::to_string(elapsed_ms.count()) + ',';


tryAgain2:
    begin = std::chrono::steady_clock::now();

    for_test_val = idea_method_Brent(k, i);

    end = std::chrono::steady_clock::now();
    
    elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    if(for_test_val == 1)
        goto tryAgain2;
    
    str1 += std::to_string(elapsed_ms.count()) + ',';

    // std::cout << str1 << std::endl;

    return str1;
}
    


NTL::ZZ num_of_ferma(unsigned long n = 5)
{
    n = pow(2, n);
    NTL::ZZ k; 
    NTL::power(k, 2, n);
    return k+1;

}


NTL::ZZ num_of_mersen(unsigned long n)
{
    NTL::ZZ k; 
    NTL::power(k, 2, n);
    return k-1;
}



void test(NTL::ZZ k, uint num_of_test=10)
{

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


NTL::ZZ number_Kalen(unsigned long n)
{
    NTL::ZZ k; 
    NTL::power(k, 2, n);
    return n*k+1;

}



std::string str_for_test_on_hard(NTL::ZZ k) //  создаю строку с замерами 
{
    std::string str1;
    str1=zToString(k) + ",";

    NTL::ZZ for_test_val = k*k;

// tryAgain:
    auto begin = std::chrono::steady_clock::now();

    for_test_val = method_Brent(for_test_val, NTL::ZZ(-1), NTL::ZZ(15));

    auto end = std::chrono::steady_clock::now();
    
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    if(for_test_val == 1)
    {
        // std::cout<<"NO" <<std::endl;
        return "-1";
    }         
    //     goto tryAgain;
    
    
    str1 += std::to_string(elapsed_ms.count()) + ',';



    return str1;
}



void test_for_hard(uint num_of_test=10)
{
    std::ofstream fout("log1.csv");

    NTL::ZZ k;

    for(int l=2; l < num_of_test; l++)
    {
        k = num_of_mersen(l);
        // std::cout<< k << std::endl;
        std::string str1;
        str1 = str_for_test_on_hard(k);
        if(str1 != "-1")
            fout << "2^" << l << "-1," << str1  << std::endl; 
    }

    fout.close();

}




int main()
{

    NTL::ZZ k, l;
    power(k,2,256);
    power(l,3,256);
    k = l*k;  
    test(k, 100); // Реализована функция, которая заполняет файл log.csv. Замеряет 100 раз функции поиска делителя числа k, разными методами.  

    std::map <NTL::ZZ, unsigned> m;
    my_factorizator(k, m, NTL::ZZ(0));
    std::cout << "Тест на рекурсивный \"Умный\" поиск делителей числа (2^256*3^256) "  << std::endl;
    for (std::map <NTL::ZZ, unsigned>::iterator i=m.begin(); i!=m.end(); ++i)
        std::cout << i->first << ' ' << i->second << std::endl;
    
    
    std::cout << std::endl <<"Попробуем найти делитель: (чисело ферма 9)^2 и замерим время поиска" <<std::endl;
    k = num_of_ferma(9);
    auto begin = std::chrono::steady_clock::now();
    k = method_Brent(k, NTL::ZZ(-1), NTL::ZZ(30));
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);    

    std::cout<< k << " найдено за "<< std::to_string(elapsed_ms.count()) << " nanoseconds"<< std::endl;
 


    std::cout << std::endl <<"Попробуем найти делитель:  (2^31-1)*(2^521-1) и замерим время поиска" <<std::endl;
    power(k, 2, 31);
    k--;
    NTL::ZZ r;
    power(r, 2, 521);
    r--;
    k=k*r;


    begin = std::chrono::steady_clock::now();
    k = method_Brent(k, NTL::ZZ(-1), NTL::ZZ(30));
    end = std::chrono::steady_clock::now();
    elapsed_ms = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);    

    std::cout<< k << " найдено за "<< std::to_string(elapsed_ms.count()) << " nanoseconds"<< std::endl;

    std::cout<< std::endl <<"Запуск Теста на числах Мерсена. > запись в файл log1.csv " << std::endl;

    test_for_hard(1024); // Тест на числах Мерсена. Указываем до какого проверить и записать замеры в файл log1.csv

    std::cout<< "Done! " << std::endl;

    return 0;
}