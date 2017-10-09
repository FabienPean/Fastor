#ifndef SIMD_VECTOR_DOUBLE_H
#define SIMD_VECTOR_DOUBLE_H

#include "simd_vector_base.h"

namespace Fastor {

// AVX VERSION
//--------------------------------------------------------------------------------------------------
#ifdef __AVX__

template <>
struct SIMDVector<double, 256> {
    static constexpr FASTOR_INDEX Size = get_vector_size<double>::size;
    static constexpr FASTOR_INLINE FASTOR_INDEX size() {return get_vector_size<double>::size;}
    static constexpr int unroll_size(FASTOR_INDEX size) {return (static_cast<int>(size) - static_cast<int>(Size));}

    FASTOR_INLINE SIMDVector() : value(_mm256_setzero_pd()) {}
    FASTOR_INLINE SIMDVector(double num) : value(_mm256_set1_pd(num)) {}
    FASTOR_INLINE SIMDVector(__m256d regi) : value(regi) {}
    FASTOR_INLINE SIMDVector(const SIMDVector<double> &a) : value(a.value) {}
    FASTOR_INLINE SIMDVector(const double *data) : value(_mm256_load_pd(data)) {}
    FASTOR_INLINE SIMDVector(double *data) : value(_mm256_load_pd(data)) {}

    FASTOR_INLINE SIMDVector<double> operator=(double num) {
        value = _mm256_set1_pd(num);
        return *this;
    }
    FASTOR_INLINE SIMDVector<double> operator=(__m256d regi) {
        value = regi;
        return *this;
    }
    FASTOR_INLINE SIMDVector<double> operator=(const SIMDVector<double> &a) {
        value = a.value;
        return *this;
    }

    FASTOR_INLINE void load(const double *data, bool Aligned=true) {
        if (Aligned)
            value =_mm256_load_pd(data);
        else
            value = _mm256_loadu_pd(data);
    }
    FASTOR_INLINE void store(double *data, bool Aligned=true) {
        if (Aligned)
            _mm256_store_pd(data,value);
        else
            _mm256_storeu_pd(data,value);
    }

    FASTOR_INLINE double operator[](FASTOR_INDEX i) const { return reinterpret_cast<const double*>(&value)[i]; }
    FASTOR_INLINE double operator()(FASTOR_INDEX i) const { return reinterpret_cast<const double*>(&value)[i]; }

    FASTOR_INLINE void set(double num) {
        value = _mm256_set1_pd(num);
    }
    FASTOR_INLINE void set(double num0, double num1, double num2, double num3) {
        value = _mm256_set_pd(num0,num1,num2,num3);
    }
    FASTOR_INLINE void set_sequential(double num0) {
        value = _mm256_setr_pd(num0,num0+1.0,num0+2.0,num0+3.0);
    }
    FASTOR_INLINE void broadcast(const double *data) {
        value = _mm256_broadcast_sd(data);
    }

    // In-place operators
    FASTOR_INLINE void operator+=(double num) {
        value = _mm256_add_pd(value,_mm256_set1_pd(num));
    }
    FASTOR_INLINE void operator+=(__m256d regi) {
        value = _mm256_add_pd(value,regi);
    }
    FASTOR_INLINE void operator+=(const SIMDVector<double> &a) {
        value = _mm256_add_pd(value,a.value);
    }

    FASTOR_INLINE void operator-=(double num) {
        value = _mm256_sub_pd(value,_mm256_set1_pd(num));
    }
    FASTOR_INLINE void operator-=(__m256d regi) {
        value = _mm256_sub_pd(value,regi);
    }
    FASTOR_INLINE void operator-=(const SIMDVector<double> &a) {
        value = _mm256_sub_pd(value,a.value);
    }

    FASTOR_INLINE void operator*=(double num) {
        value = _mm256_mul_pd(value,_mm256_set1_pd(num));
    }
    FASTOR_INLINE void operator*=(__m256d regi) {
        value = _mm256_mul_pd(value,regi);
    }
    FASTOR_INLINE void operator*=(const SIMDVector<double> &a) {
        value = _mm256_mul_pd(value,a.value);
    }

    FASTOR_INLINE void operator/=(double num) {
        value = _mm256_div_pd(value,_mm256_set1_pd(num));
    }
    FASTOR_INLINE void operator/=(__m256d regi) {
        value = _mm256_div_pd(value,regi);
    }
    FASTOR_INLINE void operator/=(const SIMDVector<double> &a) {
        value = _mm256_div_pd(value,a.value);
    }
    // end of in-place operators

    FASTOR_INLINE SIMDVector<double> shift(FASTOR_INDEX i) {
        SIMDVector<double> out;
        if (i==1)
            out.value = _mm256_shift1_pd(value);
        else if (i==2)
            out.value = _mm256_shift2_pd(value);
        else if (i==3)
            out.value = _mm256_shift3_pd(value);
        return out;
    }
    FASTOR_INLINE double sum() {return _mm256_sum_pd(value);}
    FASTOR_INLINE double product() {return _mm256_prod_pd(value);}
    FASTOR_INLINE SIMDVector<double> reverse() {
        SIMDVector<double> out;
        out.value = _mm256_reverse_pd(value);
        return out;
    }
    FASTOR_INLINE double minimum() {return _mm256_hmin_pd(value);}
    FASTOR_INLINE double maximum() {return _mm256_hmax_pd(value);}

    FASTOR_INLINE double dot(const SIMDVector<double> &other) {
        return _mm_cvtsd_f64(_mm256_dp_pd(value,other.value));
    }

    __m256d value;
};


FASTOR_HINT_INLINE std::ostream& operator<<(std::ostream &os, SIMDVector<double> a) {
#ifdef FASTOR_INTEL
    // ICC crashes without a copy
    const __m256d v = a.value;
#else
    const auto &v = a.value;
#endif
	const double* value = reinterpret_cast<const double*>(&v);
    os << "[" << value[0] <<  " " << value[1] << " " << value[2] << " " << value[3] << "]\n";
    return os;
}

FASTOR_INLINE SIMDVector<double> operator+(const SIMDVector<double> &a, const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_add_pd(a.value,b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double> operator+(const SIMDVector<double> &a, double b) {
    SIMDVector<double> out;
    out.value = _mm256_add_pd(a.value,_mm256_set1_pd(b));
    return out;
}
FASTOR_INLINE SIMDVector<double> operator+(double a, const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_add_pd(_mm256_set1_pd(a),b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double> operator+(const SIMDVector<double> &b) {
    return b;
}

FASTOR_INLINE SIMDVector<double> operator-(const SIMDVector<double> &a, const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_sub_pd(a.value,b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double> operator-(const SIMDVector<double> &a, double b) {
    SIMDVector<double> out;
    out.value = _mm256_sub_pd(a.value,_mm256_set1_pd(b));
    return out;
}
FASTOR_INLINE SIMDVector<double> operator-(double a, const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_sub_pd(_mm256_set1_pd(a),b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double> operator-(const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_neg_pd(b.value);
    return out;
}

FASTOR_INLINE SIMDVector<double> operator*(const SIMDVector<double> &a, const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_mul_pd(a.value,b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double> operator*(const SIMDVector<double> &a, double b) {
    SIMDVector<double> out;
    out.value = _mm256_mul_pd(a.value,_mm256_set1_pd(b));
    return out;
}
FASTOR_INLINE SIMDVector<double> operator*(double a, const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_mul_pd(_mm256_set1_pd(a),b.value);
    return out;
}

FASTOR_INLINE SIMDVector<double> operator/(const SIMDVector<double> &a, const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_div_pd(a.value,b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double> operator/(const SIMDVector<double> &a, double b) {
    SIMDVector<double> out;
    out.value = _mm256_div_pd(a.value,_mm256_set1_pd(b));
    return out;
}
FASTOR_INLINE SIMDVector<double> operator/(double a, const SIMDVector<double> &b) {
    SIMDVector<double> out;
    out.value = _mm256_div_pd(_mm256_set1_pd(a),b.value);
    return out;
}

FASTOR_INLINE SIMDVector<double> rcp(const SIMDVector<double> &a) {
    SIMDVector<double> out;
    // This is very inaccurate for double precision
    out.value = _mm256_cvtps_pd(_mm_rcp_ps(_mm256_cvtpd_ps(a.value)));
    return out;

    // // For making it more accurate using Newton Raphson use this
    // __m128d xmm0 = _mm256_cvtps_pd(_mm_rcp_ps(_mm256_cvtpd_ps(a.value)));
    // xmm0 = _mm256_mul_pd(xmm0,_mm256_sub_pd(VTWOPD,_mm256_mul_pd(x,xmm0)));
    // out.value = _mm256_mul_pd(xmm0,_mm256_sub_pd(VTWOPD,_mm256_mul_pd(x,xmm0)));
    // return out;
    
}

FASTOR_INLINE SIMDVector<double> sqrt(const SIMDVector<double> &a) {
    SIMDVector<double> out;
    out.value = _mm256_sqrt_pd(a.value);
    return out;
}

FASTOR_INLINE SIMDVector<double> rsqrt(const SIMDVector<double> &a) {
    SIMDVector<double> out;
   // This is very inaccurate for double precision
    out.value = _mm256_cvtps_pd(_mm_rsqrt_ps(_mm256_cvtpd_ps(a.value)));
    return out;
}

FASTOR_INLINE SIMDVector<double> abs(const SIMDVector<double> &a) {
    SIMDVector<double> out;
    out.value = _mm256_abs_pd(a.value);
    return out;
}

#endif







// SSE VERSION
//------------------------------------------------------------------------------------------------------------

#ifdef __SSE4_2__

template <>
struct SIMDVector<double, 128> {
    static constexpr FASTOR_INDEX Size = get_vector_size<double,128>::size;
    static constexpr FASTOR_INLINE FASTOR_INDEX size() {return get_vector_size<double,128>::size;}
    static constexpr int unroll_size(FASTOR_INDEX size) {return (static_cast<int>(size) - static_cast<int>(Size));}

    FASTOR_INLINE SIMDVector() : value(_mm_setzero_pd()) {}
    FASTOR_INLINE SIMDVector(double num) : value(_mm_set1_pd(num)) {}
    FASTOR_INLINE SIMDVector(__m128d regi) : value(regi) {}
    FASTOR_INLINE SIMDVector(const SIMDVector<double,128> &a) : value(a.value) {}
    FASTOR_INLINE SIMDVector(const double *data) : value(_mm_load_pd(data)) {}
    FASTOR_INLINE SIMDVector(double *data) : value(_mm_load_pd(data)) {}

    FASTOR_INLINE SIMDVector<double,128> operator=(double num) {
        value = _mm_set1_pd(num);
        return *this;
    }
    FASTOR_INLINE SIMDVector<double,128> operator=(__m128d regi) {
        value = regi;
        return *this;
    }
    FASTOR_INLINE SIMDVector<double,128> operator=(const SIMDVector<double,128> &a) {
        value = a.value;
        return *this;
    }

    FASTOR_INLINE void load(const double *data, bool Aligned=true) {
        if (Aligned)
            value =_mm_load_pd(data);
        else
            value = _mm_loadu_pd(data);
    }
    FASTOR_INLINE void store(double *data, bool Aligned=true) {
        if (Aligned)
            _mm_store_pd(data,value);
        else
            _mm_storeu_pd(data,value);
    }

    FASTOR_INLINE double operator[](FASTOR_INDEX i) const { return reinterpret_cast<const double*>(&value)[i]; }
    FASTOR_INLINE double operator()(FASTOR_INDEX i) const { return reinterpret_cast<const double*>(&value)[i]; }

    FASTOR_INLINE void set(double num) {
        value = _mm_set1_pd(num);
    }
    FASTOR_INLINE void set(double num0, double num1) {
        value = _mm_set_pd(num0,num1);
    }
    FASTOR_INLINE void set_sequential(double num0) {
        value = _mm_setr_pd(num0,num0+1.0);
    }
    FASTOR_INLINE void broadcast(const double *data) {
        value = _mm_load1_pd(data);
    }

    // In-place operators
    FASTOR_INLINE void operator+=(double num) {
        value = _mm_add_pd(value,_mm_set1_pd(num));
    }
    FASTOR_INLINE void operator+=(__m128d regi) {
        value = _mm_add_pd(value,regi);
    }
    FASTOR_INLINE void operator+=(const SIMDVector<double,128> &a) {
        value = _mm_add_pd(value,a.value);
    }

    FASTOR_INLINE void operator-=(double num) {
        value = _mm_sub_pd(value,_mm_set1_pd(num));
    }
    FASTOR_INLINE void operator-=(__m128d regi) {
        value = _mm_sub_pd(value,regi);
    }
    FASTOR_INLINE void operator-=(const SIMDVector<double,128> &a) {
        value = _mm_sub_pd(value,a.value);
    }

    FASTOR_INLINE void operator*=(double num) {
        value = _mm_mul_pd(value,_mm_set1_pd(num));
    }
    FASTOR_INLINE void operator*=(__m128d regi) {
        value = _mm_mul_pd(value,regi);
    }
    FASTOR_INLINE void operator*=(const SIMDVector<double,128> &a) {
        value = _mm_mul_pd(value,a.value);
    }

    FASTOR_INLINE void operator/=(double num) {
        value = _mm_div_pd(value,_mm_set1_pd(num));
    }
    FASTOR_INLINE void operator/=(__m128d regi) {
        value = _mm_div_pd(value,regi);
    }
    FASTOR_INLINE void operator/=(const SIMDVector<double,128> &a) {
        value = _mm_div_pd(value,a.value);
    }
    // end of in-place operators

    FASTOR_INLINE SIMDVector<double,128> shift(FASTOR_INDEX i) {
        SIMDVector<double,128> out;
        FASTOR_ASSERT(i==1,"INCORRECT SHIFT INDEX");
            out.value = _mm_shift1_pd(value);
        return out;
    }
    FASTOR_INLINE double sum() {return _mm_sum_pd(value);}
    FASTOR_INLINE double product() {return _mm_prod_pd(value);}
    FASTOR_INLINE SIMDVector<double,128> reverse() {
        SIMDVector<double,128> out;
        out.value = _mm_reverse_pd(value);
        return out;
    }
    FASTOR_INLINE double minimum() {return _mm_hmin_pd(value);}
    FASTOR_INLINE double maximum() {return _mm_hmax_pd(value);}

    FASTOR_INLINE double dot(const SIMDVector<double,128> &other) {
#ifdef __SSE4_1__
        return _mm_cvtsd_f64(_mm_dp_pd(value,other.value,0xff));
#else
        return _mm_sum_pd(_mm_mul_pd(value,other.value));
#endif
    }

    __m128d value;
};


FASTOR_HINT_INLINE std::ostream& operator<<(std::ostream &os, SIMDVector<double,128> a) {
    // ICC crashes without a copy
    const __m128d v = a.value;
	const double* value = reinterpret_cast<const double*>(&v);
    os << "[" << value[0] <<  " " << value[1] << "]\n";
    return os;
}

FASTOR_INLINE SIMDVector<double,128> operator+(const SIMDVector<double,128> &a, const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_add_pd(a.value,b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator+(const SIMDVector<double,128> &a, double b) {
    SIMDVector<double,128> out;
    out.value = _mm_add_pd(a.value,_mm_set1_pd(b));
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator+(double a, const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_add_pd(_mm_set1_pd(a),b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator+(const SIMDVector<double,128> &b) {
    return b;
}

FASTOR_INLINE SIMDVector<double,128> operator-(const SIMDVector<double,128> &a, const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_sub_pd(a.value,b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator-(const SIMDVector<double,128> &a, double b) {
    SIMDVector<double,128> out;
    out.value = _mm_sub_pd(a.value,_mm_set1_pd(b));
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator-(double a, const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_sub_pd(_mm_set1_pd(a),b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator-(const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_neg_pd(b.value);
    return out;
}

FASTOR_INLINE SIMDVector<double,128> operator*(const SIMDVector<double,128> &a, const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_mul_pd(a.value,b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator*(const SIMDVector<double,128> &a, double b) {
    SIMDVector<double,128> out;
    out.value = _mm_mul_pd(a.value,_mm_set1_pd(b));
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator*(double a, const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_mul_pd(_mm_set1_pd(a),b.value);
    return out;
}

FASTOR_INLINE SIMDVector<double,128> operator/(const SIMDVector<double,128> &a, const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_div_pd(a.value,b.value);
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator/(const SIMDVector<double,128> &a, double b) {
    SIMDVector<double,128> out;
    out.value = _mm_div_pd(a.value,_mm_set1_pd(b));
    return out;
}
FASTOR_INLINE SIMDVector<double,128> operator/(double a, const SIMDVector<double,128> &b) {
    SIMDVector<double,128> out;
    out.value = _mm_div_pd(_mm_set1_pd(a),b.value);
    return out;
}

FASTOR_INLINE SIMDVector<double,128> rcp(const SIMDVector<double,128> &a) {
    SIMDVector<double,128> out;
    // This is very inaccurate for double precision
    out.value = _mm_cvtps_pd(_mm_rcp_ps(_mm_cvtpd_ps(a.value)));
    return out;

    /*
    // For making it more accurate using Newton Raphson use this
    __m128d xmm0 = _mm_cvtps_pd(_mm_rcp_ps(_mm_cvtpd_ps(a.value)));
    xmm0 = _mm_mul_pd(xmm0,_mm_sub_pd(TWOPD,_mm_mul_pd(x,xmm0)));
    out.value = _mm_mul_pd(xmm0,_mm_sub_pd(TWOPD,_mm_mul_pd(x,xmm0)));
    return out;
    */
}

FASTOR_INLINE SIMDVector<double,128> sqrt(const SIMDVector<double,128> &a) {
    SIMDVector<double,128> out;
    out.value = _mm_sqrt_pd(a.value);
    return out;
}

FASTOR_INLINE SIMDVector<double,128> rsqrt(const SIMDVector<double,128> &a) {
    SIMDVector<double,128> out;
    // This is very inaccurate for double precision
    out.value = _mm_cvtps_pd(_mm_rsqrt_ps(_mm_cvtpd_ps(a.value)));
    return out;
}

FASTOR_INLINE SIMDVector<double,128> abs(const SIMDVector<double,128> &a) {
    SIMDVector<double,128> out;
    out.value = _mm_abs_pd(a.value);
    return out;
}


#endif



// SCALAR VERSION
//------------------------------------------------------------------------------------------------------------
template <>
struct SIMDVector<double, 64> {
    static constexpr FASTOR_INDEX Size = 1;
    static constexpr FASTOR_INLINE FASTOR_INDEX size() {return 1;}
    static constexpr int unroll_size(FASTOR_INDEX size) {return (static_cast<int>(size) - 1);}

    FASTOR_INLINE SIMDVector() : value(0) {}
    FASTOR_INLINE SIMDVector(double num) : value(num) {}
    FASTOR_INLINE SIMDVector(const SIMDVector<double,64> &a) : value(a.value) {}
    FASTOR_INLINE SIMDVector(const double *data) : value(*data) {}
    FASTOR_INLINE SIMDVector(double *data) : value(*data) {}

    FASTOR_INLINE SIMDVector<double,64> operator=(double num) {
        value = num;
        return *this;
    }
    FASTOR_INLINE SIMDVector<double,64> operator=(const SIMDVector<double,64> &a) {
        value = a.value;
        return *this;
    }

    FASTOR_INLINE void load(const double *data, bool ) {
        value = *data;
    }
    FASTOR_INLINE void store(double *data, bool ) {
        data[0] = value;
    }

    FASTOR_INLINE void load(const double *data) {
        value = *data;
    }
    FASTOR_INLINE void store(double *data) {
        data[0] = value;
    }

    FASTOR_INLINE double operator[](FASTOR_INDEX) const {return value;}
    FASTOR_INLINE double operator()(FASTOR_INDEX) const {return value;}

    FASTOR_INLINE void set(double num) {
        value = num;
    }

    FASTOR_INLINE void set_sequential(double num) {
        value = num;
    }

    FASTOR_INLINE void broadcast(const double *data) {
        value = *data;
    }

    // In-place operators
    FASTOR_INLINE void operator+=(double num) {
        value += num;
    }
    FASTOR_INLINE void operator+=(const SIMDVector<double,64> &a) {
        value += a.value;
    }

    FASTOR_INLINE void operator-=(double num) {
        value -= num;
    }
    FASTOR_INLINE void operator-=(const SIMDVector<double,64> &a) {
        value -= a.value;
    }

    FASTOR_INLINE void operator*=(double num) {
        value *= num;
    }
    FASTOR_INLINE void operator*=(const SIMDVector<double,64> &a) {
        value *= a.value;
    }

    FASTOR_INLINE void operator/=(double num) {
        value /= num;
    }
    FASTOR_INLINE void operator/=(const SIMDVector<double,64> &a) {
        value /= a.value;
    }
    // end of in-place operators

    FASTOR_INLINE SIMDVector<double,64> shift(FASTOR_INDEX) {
        return *this;
    }
    FASTOR_INLINE double sum() {return value;}
    FASTOR_INLINE double product() {return value;}
    FASTOR_INLINE SIMDVector<double,64> reverse() {
        return *this;
    }
    FASTOR_INLINE double minimum() {return value;}
    FASTOR_INLINE double maximum() {return value;}

    FASTOR_INLINE double dot(const SIMDVector<double,64> &other) {
        return value*other.value;
    }

    double value;
};


FASTOR_HINT_INLINE std::ostream& operator<<(std::ostream &os, SIMDVector<double,64> a) {
    os << "[" << a.value << "]\n";
    return os;
}

FASTOR_INLINE SIMDVector<double,64> operator+(const SIMDVector<double,64> &a, const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = a.value+b.value;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator+(const SIMDVector<double,64> &a, double b) {
    SIMDVector<double,64> out;
    out.value = a.value+b;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator+(double a, const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = a+b.value;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator+(const SIMDVector<double,64> &b) {
    return b;
}

FASTOR_INLINE SIMDVector<double,64> operator-(const SIMDVector<double,64> &a, const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = a.value-b.value;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator-(const SIMDVector<double,64> &a, double b) {
    SIMDVector<double,64> out;
    out.value = a.value-b;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator-(double a, const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = a-b.value;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator-(const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = -b.value;
    return out;
}

FASTOR_INLINE SIMDVector<double,64> operator*(const SIMDVector<double,64> &a, const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = a.value*b.value;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator*(const SIMDVector<double,64> &a, double b) {
    SIMDVector<double,64> out;
    out.value = a.value*b;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator*(double a, const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = a*b.value;
    return out;
}

FASTOR_INLINE SIMDVector<double,64> operator/(const SIMDVector<double,64> &a, const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = a.value/b.value;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator/(const SIMDVector<double,64> &a, double b) {
    SIMDVector<double,64> out;
    out.value = a.value/b;
    return out;
}
FASTOR_INLINE SIMDVector<double,64> operator/(double a, const SIMDVector<double,64> &b) {
    SIMDVector<double,64> out;
    out.value = a/b.value;
    return out;
}

FASTOR_INLINE SIMDVector<double,64> rcp(const SIMDVector<double,64> &a) {
    return 1.0/a.value;
}

FASTOR_INLINE SIMDVector<double,64> sqrt(const SIMDVector<double,64> &a) {
    return std::sqrt(a.value);
}

FASTOR_INLINE SIMDVector<double,64> rsqrt(const SIMDVector<double,64> &a) {
    return 1.0/std::sqrt(a.value);
}

FASTOR_INLINE SIMDVector<double,64> abs(const SIMDVector<double,64> &a) {
    return std::abs(a.value);
}


}
#endif
