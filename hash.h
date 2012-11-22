#ifndef HASH_H
#define HASH_H
#include<tr1/functional>
template <class T>
inline void hash_combine(std::size_t &seed, const T &v) {
    std::tr1::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed >> 2);
}

namespace std{
    namespace tr1{
        template<typename T, class Comp, class Alloc>
        struct hash<std::set<T, Comp, Alloc> > {
            inline std::size_t operator()(const std::set<T, Comp, Alloc> & s) const {
                return my_range_hash(s.begin(), s.end());
            }
        };
    }
}

template <typename C> struct ContainerHasher{
    typedef typename C::value_type value_type;
    inline size_t operator()(const C& c) const {
        size_t seed = 0;
        for(typename C::const_iterator it = c.begin(), end = c.end(); it != end; it++) {
            hash_combine<value_type>(seed, *it);
        }
        return seed;
    }
};

/*
struct eqstr {
    bool operator() (const set<int>& pat1, const set<int>& pat2) {
        bool result = true;
        set<int>::const_iterator it = pat1.begin();
        for(; it!=pat1.end(); it++) {
            if(pat2.find(*it) == pat2.end()) {
                result = false;
                break;
            }
        }
        return result;
    }
};
*/
#endif
