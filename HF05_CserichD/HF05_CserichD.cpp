//Cserich David
#include <iostream>
#include <cmath>

#define SIZE 720
#define PREC 10
#define VALTYPE double

using namespace std;

template<int N, int I, int J, int K>
class SineSeries {
public:
    enum { go = (K+1 != J) };
    
    static inline float accumulate()
    {
        return 1-(I*2*M_PI/N)*(I*2*M_PI/N)/(2*K+2)/(2*K+3) *
        SineSeries<N*go,I*go,J*go,(K+1)*go>::accumulate();
    }
};
template<>
class SineSeries<0,0,0,0> {
public:
    static inline float accumulate()
    { return 1; }
};

template<int N, int I>
class Sine {
public:
    static inline VALTYPE sin()
    {
        return (I*2*M_PI/N) * SineSeries<N,I,PREC,0>::accumulate();
    }
};

template<int I>
class loop {
private:
    enum { go = (I-1) != 0 };
public:
    static inline void f(VALTYPE *sin_t) {
        *sin_t = Sine<SIZE,I>::sin();
        loop< go ? (I-1) : 0 >::f(++sin_t);
    }
};

template<>
class loop<0> {
public:
    static inline void f(VALTYPE *sin_t)
    { }
};

template<int N, int I>
class loop_inc{
private:
    enum { go = (I + 1) != N };
public:
    static inline void f(VALTYPE *sin_t) {
        *sin_t = Sine<SIZE,I>::sin();
        loop_inc< SIZE,(go ? (I + 1) : SIZE) >::f(++sin_t);
    }
};
template<>
class loop_inc<SIZE,SIZE> {
public:
    static inline void f(VALTYPE *sin_t)
    { }
};

VALTYPE sin_t[SIZE];

bool sin_lookup(
                const VALTYPE &alpha,
                int &alpha_lo_idx,
                int &alpha_hi_idx,
                VALTYPE &alpha_lo,
                VALTYPE &alpha_hi,
                VALTYPE &sin_lo,
                VALTYPE &sin_hi
                ){
    alpha_lo_idx=(int)((alpha)/360*SIZE)%SIZE;
    alpha_lo=alpha_lo_idx*360.0/SIZE;
    
    bool alpha_match=(alpha==alpha_lo);
    
    alpha_hi_idx=alpha_match?
alpha_lo_idx:
    (alpha_lo_idx+1)%SIZE;
    alpha_hi=alpha_hi_idx*360.0/SIZE;
    
    sin_lo=sin_t[alpha_lo_idx];
    sin_hi=sin_t[alpha_hi_idx];
    return alpha_match;
}

bool arcsin_lookup(
                   const VALTYPE &val,
                   int &val_lo_idx,
                   int &val_hi_idx,
                   VALTYPE &val_lo,
                   VALTYPE &val_hi,
                   VALTYPE &alpha_lo,
                   VALTYPE &alpha_hi
                   ){
    for (val_hi_idx=0;val_hi_idx<=SIZE/4 && sin_t[val_hi_idx]<=val;val_hi_idx++);
    for (val_lo_idx=SIZE/4;val_lo_idx>=0 && sin_t[val_lo_idx]>=val;val_lo_idx--);
    if (!(val_hi_idx==val_lo_idx || val_hi_idx==val_lo_idx+1)){
        return false;
    }
    val_lo=360.0*val_lo_idx/SIZE;
    val_hi=360.0*val_hi_idx/SIZE;
    alpha_lo=sin_t[val_lo_idx];
    alpha_hi=sin_t[val_hi_idx];
    return true;
}

int main(){
    
    loop_inc<SIZE,0>::f(sin_t);
    
    VALTYPE alpha,val;
    VALTYPE alpha_lo,alpha_hi;
    VALTYPE sin_lo,sin_hi;
    VALTYPE val_lo,val_hi;
    int alpha_lo_idx,alpha_hi_idx;
    int val_lo_idx,val_hi_idx;
    
    cout<<"alpha (deg)?";
    cin>>alpha;
    
    bool alpha_match=sin_lookup(alpha,
                                alpha_lo_idx,alpha_hi_idx,alpha_lo,alpha_hi,
                                sin_lo,sin_hi);
    cout<<"sin("<<alpha_lo<<")="<<sin_lo<<endl;
    if (!alpha_match){
        cout<<"sin("<<alpha_hi<<")="<<sin_hi<<endl;
    }
    
    cout<<"sin(alpha)?";
    cin>>val;
    if (arcsin_lookup(val,val_lo_idx,val_hi_idx,val_lo,val_hi,alpha_lo,alpha_hi)){
        cout<<"arcsin("<<val_lo<<")="<<alpha_lo<<endl;
        cout<<"arcsin("<<val_hi<<")="<<alpha_hi<<endl;
    }
    return 0;
}
