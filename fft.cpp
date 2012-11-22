#include "fft.h"

vector<int> bitReverHash[31];

int bitReverse(int& a, int width){
    int n = width;
    long long bit = 1;

    vector<bool> v;

    int res = 0;
    for(int i=0; i<n; ++i){
        v.push_back(!!(a & bit));
        bit <<= 1;
    }

    bit = 1;
    for(int i=v.size()-1; i>=0; --i){
        if(v[i])
            res |= bit;
        bit<<=1;
    }
    return res;
}

void initBitReverTab(int idx){
    int len = 1<<idx;

    bitReverHash[idx].resize(len);

    for(int i=0; i < len; ++i){
        bitReverHash[idx][i] = bitReverse(i, idx);
    }
}

void initTab(int n){
    for(int i=0; ; ++i){
        initBitReverTab(i);
        if((1<<i) >= n)
            break;
    }
}

void iterFFT(vector<Complex>& y, vector<double>& a){
    int n;
    int c=0;
    for(c=0, n=1; n<a.size(); n<<=1, c++);

    y.resize(n, Complex(0,0));

    for(int i=0; i<n; ++i){
        int idx = bitReverHash[c][i];
        if(idx < a.size())
            y[i] = Complex(a[idx], ZERO);
    }

    for (int m = 1; m<=n; m<<=1){
        Complex wm(polar(1.0, -2*myPI/m));
        for(int k=0; k<n; k+=m){
            Complex w = 1;
            for(int j=0; j<m/2; j++){
                Complex t = w * y[k+j+m/2];
                Complex u = y[j+k];
                y[k+j] = u + t;
                y[k+j+m/2] = u - t;
                w = w * wm;
            }
        }
    }

}

void iterFFTInv(vector<Complex>& b, vector<Complex>& y){

    //  it is guaranteed that y has a size of 2^n
    int n = y.size();
    int c = 0, cc=1;
    for(c=0, cc=1; cc<n; cc<<=1, c++)
        ;
    if(cc != n)
        cout << "nnd\n";

    b.resize(n, Complex(ZERO,ZERO));

    for(int i=0; i<n; ++i){
        int idx = bitReverHash[c][i];
        b[i] = y[idx];
    }
    for (int m = 1; m<=n; m<<=1){
        Complex wm(polar(1.0, 2*myPI/m));
        for(int k=0; k<n; k+=m){
            Complex w = 1;
            for(int j=0; j<m/2; j++){
                Complex t = w * b[k+j+m/2];
                Complex u = b[j+k];
                b[k+j] = u+t;
                b[k+j+m/2] = u-t;
                w = w*wm;
            }
        }
    }
}

void FFT(vector<Complex>& y, vector<double>& a){
    iterFFT(y, a);
}

void FFTInv(vector<Complex>& b, vector<Complex>& y){
    iterFFTInv(b, y);

    Complex deno(b.size());
    for(int i=0; i<b.size(); ++i){
        b[i] /= deno;
    }
}

Complex power(const Complex& p, int n){
    if(n==1)
        return p;

    Complex res = power(p,n/2);
    return n&1 ? res*res*p : res*res;
}

void convol(vector<double>& v, vector<double>& a, vector<double>& b){
    int n1 = a.size();
    int n2 = b.size();
    int n = a.size() + b.size() - 1;

    v.resize(n, ZERO);
    a.resize(n, ZERO);
    b.resize(n, ZERO);

    vector<Complex> ya, yb, tmp, tmp2;

    FFT(ya, a);
    FFT(yb, b);
    tmp.resize(ya.size());

    for(int i=0; i<tmp.size(); ++i){
        tmp[i] = ya[i] * yb[i];
    }

    FFTInv(tmp2, tmp);

    for(int i=0; i<v.size(); ++i){
        v[i] = tmp2[i].real();
    }

    a.resize(n1);
    b.resize(n2);

}

void deconvol(vector<double>& v, vector<double>& a, vector<double>& b){
    int n1 = a.size();
    int n2 = b.size();
    int n = a.size() - b.size() + 1;

    v.resize(n1, ZERO);
    a.resize(n1, ZERO);
    b.resize(n1, ZERO);

    vector<Complex> ya, yb, tmp, tmp2;

    FFT(ya, a);
    FFT(yb, b);

    tmp.resize(ya.size());

    for(int i=0; i<tmp.size(); ++i){
        tmp[i] = ya[i] / yb[i];
    }

    FFTInv(tmp2, tmp);

    for(int i=0; i<v.size(); ++i){
        v[i] = tmp2[i].real();
    }

    a.resize(n1);
    b.resize(n2);
    v.resize(n);
}

void rightShift(vector<double>& p, int dif){
    vector<double> res(dif, 1e-100);
    res.insert(res.end(), p.begin(), p.end());
    p = res;
}

void leftShift(vector<double>& p, int dif){
    p = vector<double>(p.begin()+dif, p.end());
}


//////////////////////////////  End  ////////////////////////////////
