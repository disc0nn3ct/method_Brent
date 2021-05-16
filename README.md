# method_Brent
Метод Брента 



Используется библиотека [NTL](https://github.com/libntl/ntl)

Установка: 
```shell

   % gunzip ntl-xxx.tar.gz
   % tar xf ntl-xxx.tar
   % cd ntl-xxx/src
   % ./configure 
   % make
   % make check
   % sudo make install

```


Компиляция:
```shell
g++ -g -O2 -std=c++11 -pthread -march=native one.cpp -lntl -lgmp -lm
```

