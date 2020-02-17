#ifndef SING_H_
#define SING_H_

#include <cstdint>
#include <vector>
#include <complex>
#include "sing_vectors.h"
#include "sing_pointers.h"
#include "sing_string.h"

#define SING_STORAGE(name) name, 0, sizeof(name)/sizeof(name[0])

namespace sing {

    template<class T>
    inline T pow2(T base) {
        return(base * base);
    }

    //
    // all uint to int comparisons
    //
    inline bool ismore(uint64_t op1, int64_t op2) {
        return(op2 < 0 || op1 > (uint64_t)op2);
    }
    inline bool isless(uint64_t op1, int64_t op2) {
        return(op2 > 0 && op1 < (uint64_t)op2);
    }
    inline bool ismore_eq(uint64_t op1, int64_t op2) {
        return(op2 < 0 || op1 >= (uint64_t)op2);
    }
    inline bool isless_eq(uint64_t op1, int64_t op2) {
        return(op2 >= 0 && op1 <= (uint64_t)op2);
    }
    inline bool iseq(uint64_t op1, int64_t op2) {
        return(op2 >= 0 && op1 == (uint64_t)op2);
    }

    inline bool ismore(uint64_t op1, int32_t op2) {
        return(op2 < 0 || op1 >(uint64_t)op2);
    }
    inline bool isless(uint64_t op1, int32_t op2) {
        return(op2 > 0 && op1 < (uint64_t)op2);
    }
    inline bool ismore_eq(uint64_t op1, int32_t op2) {
        return(op2 < 0 || op1 >= (uint64_t)op2);
    }
    inline bool isless_eq(uint64_t op1, int32_t op2) {
        return(op2 >= 0 && op1 <= (uint64_t)op2);
    }
    inline bool iseq(uint64_t op1, int32_t op2) {
        return(op2 >= 0 && op1 == (uint64_t)op2);
    }

    inline bool ismore(uint32_t op1, int32_t op2) {
        return(op2 < 0 || op1 >(uint32_t)op2);
    }
    inline bool isless(uint32_t op1, int32_t op2) {
        return(op2 > 0 && op1 < (uint32_t)op2);
    }
    inline bool ismore_eq(uint32_t op1, int32_t op2) {
        return(op2 < 0 || op1 >= (uint32_t)op2);
    }
    inline bool isless_eq(uint32_t op1, int32_t op2) {
        return(op2 >= 0 && op1 <= (uint32_t)op2);
    }
    inline bool iseq(uint32_t op1, int32_t op2) {
        return(op2 >= 0 && op1 == (uint32_t)op2);
    }

    template<class T, class T2>
    T pow(T base, T2 exp) {
        if (base == 0) {
            if (exp == 0) {
                throw(std::domain_error("result of 0^0 is undefined"));
            }
            if (exp <= 0) {
                throw(std::overflow_error("result of 0^(anything negative) is infinite"));
            }
            return(0);
        }
        if (base < 0) {         // so if T is unsigned this is never executed
            if (base == -1) {   // this would be true for all-1s unsigned values
                return((exp & 1) == 0 ? 1 : -1);
            }
        }
        if (base == 1 || exp == 0) {
            return(1);
        }
        if (exp < 0) {
            return(0);
        } else if (exp == 1) {
            return(base);
        } else if (exp > 63) {
            throw(std::overflow_error("result of exponentiation overflows"));
        }

        T result = 1;
        T cur_power = base;
        while (exp > 0) {
            if (exp & 1) {
                result *= cur_power;
            }
            cur_power *= cur_power;
            exp >>= 1;
        }
        return(result);
    }

    inline std::complex<float> c_d2f(std::complex<double> in) {
        return(std::complex<float>((float)in.real(), (float)in.imag()));
    }

    inline std::complex<double> c_f2d(std::complex<float> in) {
        return(std::complex<double>(in.real(), in.imag()));
    }

    int64_t string2int(const char *instring);
    uint64_t string2uint(const char *instring);
    double string2double(const char *instring);
    std::complex<float> string2complex64(const char *instring);
    std::complex<double> string2complex128(const char *instring);

    inline int64_t string2int(const string &instring) { return(string2int(instring.c_str())); }
    inline uint64_t string2uint(const string &instring) { return(string2uint(instring.c_str())); }
    inline double string2double(const string &instring) { return(string2double(instring.c_str())); }
    inline std::complex<float> string2complex64(const string &instring) { return(string2complex64(instring.c_str())); }
    inline std::complex<double> string2complex128(const string &instring) { return(string2complex128(instring.c_str())); }

    string tostring(int value);
    string tostring(long long value);
    string tostring(unsigned int value);
    string tostring(unsigned long long value);
    //string tostring(float value);
    string tostring(double value);
    string tostring(std::complex<float> value);
    string tostring(std::complex<double> value);
    string tostring(bool value);

    string add_strings(int count, ...);
    string format(const char *format, ...);

}   // namespace

#endif