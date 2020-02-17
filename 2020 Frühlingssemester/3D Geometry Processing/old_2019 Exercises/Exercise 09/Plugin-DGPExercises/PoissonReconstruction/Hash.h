#ifndef HASH_INCLUDED
#define HASH_INCLUDED
#ifdef WIN32

  #if _MSC_VER >= 1900
    #include <unordered_map>
    #define hash_map std::unordered_map
  #else
    #include <hash_map>
    using stdext::hash_map;
  #endif

#else // !WIN32

  #if (__GNUC__ * 10000  + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)>= 40300 //gcc >= 4.3
    #include <tr1/unordered_map>
    #define hash_map std::tr1::unordered_map

  #else //GCC_VERSION < 40300
    #include <ext/hash_map>
    using namespace __gnu_cxx;

    namespace __gnu_cxx
    {  
      template<> struct hash<long long> {
        size_t operator()(long long __x) const { return __x; }
      };
      template<> struct hash<const long long> {
        size_t operator()(const long long __x) const { return __x; }
      };
  
  
      template<> struct hash<unsigned long long> {
        size_t operator()(unsigned long long __x) const { return __x; }
      };
      template<> struct hash<const unsigned long long> {
        size_t operator()(const unsigned long long __x) const { return __x; }
      };
    }
  #endif // gcc >= 4.3
#endif // WIN32
#endif // HASH_INCLUDED

