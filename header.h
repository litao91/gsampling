#pragma warning(disable:4267) // conver size_t to int
#pragma warning(disable:4018) // compare size_t with int
#pragma warning(disable:4996) // usage of <cstring> functions

#ifndef HEADER_H
#define HEADER_H

#include<cstdio>
#include<cstring>
#include<vector>
#include<string>
#include<map>
#include<set>
#include<queue>
#include<cmath>
#include<algorithm>
#include<iostream>
#include<sstream>
#include<fstream>
#include<ctime>
#include<iomanip>
#include<complex>
#include<ext/hash_map> // for Linux
#include<ext/hash_set> // for Linux
//#include<hash_map> // for MS Visual Studio on Win
//#include<hash_set> // for MS Visual Studio on Win
using namespace std;
//using namespace stdext; // for MS Visual Studio on Win
using namespace __gnu_cxx; // for Linux

typedef double Prob;

const Prob epsln = 1e-12;
const Prob ZERO = 1e-30;
typedef long long int64;
const double myPI = atan2(0.0,-1.0);

typedef std::complex <double> Complex;

class Timer
{
private:
    clock_t m_start, m_stop;
public:
    Timer() { }
    void start() { m_start = clock(); }
    void stop() { m_stop = clock(); }
    double interval() {
        return (double)(m_stop - m_start) / CLOCKS_PER_SEC;
    }
};


/*///// the timer below requires #include<windows.h> /////
class Timer
{
private:
    LARGE_INTEGER m_t1, m_t2, m_tc;
    bool m_fstart, m_fstop;
public:
    Timer(){
        m_fstart = 0;
        m_fstop = 0;
        QueryPerformanceFrequency(&m_tc);
    }
    void start(){
        m_fstart = 1;
        QueryPerformanceCounter(&m_t1);
    }
    void stop(){
        m_fstop = 1;
        QueryPerformanceCounter(&m_t2);
    }
    double interval(){
        if(!m_fstart || !m_fstop){
            cerr << "not enough info to measure" << endl;
            return 0;
        }
        return double(m_t2.QuadPart - m_t1.QuadPart) / double(m_tc.QuadPart);
    }
};
*/
#endif
