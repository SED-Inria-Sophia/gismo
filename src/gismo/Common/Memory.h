/** @file Memory.h

    @brief Provides utility function related to memory management.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): C. Hofreither, A. Mantzaflaris, J. Vogl
*/

#pragma once

#include <gismo/Common/TemplateTools.h>
#include <gismo/Common/DebugAssert.h>  // For gsInfo

#ifdef __MINGW32__
//#include <malloc/malloc.h> //xcode
#include <malloc.h>
#endif

#if  __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
#    include <boost/shared_ptr.hpp>
#    include <boost/weak_ptr.hpp>
#  else
#    include <tr1/memory>
#  endif
#else // libc++ or other
#  include <memory>
#endif

namespace gismo {

/** @namespace gismo::memory

    @brief
    This namespace contains functions related to memory management.

    \ingroup Core
*/
namespace memory
{

/* \brief Adaptor for a shared pointer

usage:
\code
memory::shared_ptr<int> B;
\endcode
*/

#if   __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
using boost::shared_ptr;
using boost::weak_ptr;
#  else
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;
#  endif
#else // libc++ or other
using std::shared_ptr;
using std::weak_ptr;
#endif

/* \brief Adaptor for a unique pointer

usage:
\code
memory::unique_ptr<int> B;
\endcode
*/
#if __cplusplus >= 201103 || _MSC_VER >= 1600
using std::unique_ptr;
using std::nullptr_t;
#else

template <typename T>
class unique_ptr : public std::auto_ptr<T>
{
    typedef std::auto_ptr<T> Base;
    typedef std::auto_ptr_ref<T> unique_ptr_ref;

    //struct Cannot_Convert_Pointer;

public :
    explicit unique_ptr(T* p = 0)  throw() : Base(p) { }

    unique_ptr(const unique_ptr& r) : Base( const_cast<unique_ptr&>(r) ) { }

    unique_ptr(unique_ptr_ref m)  throw() : Base(m) { }

    template<typename U>
    unique_ptr(const unique_ptr<U> & r
               // unique_ptr<typename conditional<is_base_of<U,T>::value, U,
               //                      Cannot_Convert_Pointer >::type>
        )  throw()
    : Base( const_cast<unique_ptr<U>&>(r) ) { }

    ~unique_ptr() throw() { }

    unique_ptr& operator=(const unique_ptr& r)
    {
        Base::operator=( const_cast<unique_ptr&>(r) );
        return *this;
    }

    template<typename U>
    unique_ptr& operator=(const unique_ptr<U> & r)
    {
        Base::operator=( const_cast<unique_ptr<U>&>(r) );
        return *this;
    }
};

#endif

/* \brief Adaptor for a function pointer

usage:
\code
memory::function<bool(real_t)> B;
\endcode
*/
#if __cplusplus >= 201103 || _MSC_VER >= 1600
using std::function;
#else

//todo
template<typename _Signature>
struct function;

template<typename _Res, typename _ArgTypes>
struct function<_Res(_ArgTypes)>
{

};

#endif

/* \brief Adaptor for make_shared

usage:
\code
memory::make_shared<int>(3)
\endcode
*/
template<typename T>
shared_ptr<T> make_shared()
{
#if   __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
    return shared_ptr<T>(new T);
#  else
    return shared_ptr<T>(new T);
#  endif
#else // libc++ or other
    return std::make_shared<T>();
#endif
}

template<typename T, typename Arg1>
shared_ptr<T> make_shared(Arg1 && arg1)
{
#if   __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
    return shared_ptr<T>(new T(arg1));
#  else
    return shared_ptr<T>(new T(arg1));
#  endif
#else // libc++ or other
    return std::make_shared<T>(arg1);
#endif
}

template<typename T, typename Arg1, typename Arg2>
shared_ptr<T> make_shared(Arg1 && arg1, Arg2 && arg2)
{
#if   __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
    return shared_ptr<T>(new T(arg1, arg2));
#  else
    return shared_ptr<T>(new T(arg1, arg2));
#  endif
#else // libc++ or other
    return std::make_shared<T>(arg1, arg2);
#endif
}

template<typename T, typename Arg1, typename Arg2, typename Arg3>
shared_ptr<T> make_shared(Arg1 && arg1, Arg2 && arg2, Arg3 && arg3)
{
#if   __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
    return shared_ptr<T>(new T(arg1, arg2, arg3));
#  else
    return shared_ptr<T>(new T(arg1, arg2, arg3));
#  endif
#else // libc++ or other
    return std::make_shared<T>(arg1, arg2, arg3);
#endif
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
shared_ptr<T> make_shared(Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4)
{
#if   __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
    return shared_ptr<T>(new T(arg1, arg2, arg3, arg4));
#  else
    return shared_ptr<T>(new T(arg1, arg2, arg3, arg4));
#  endif
#else // libc++ or other
    return std::make_shared<T>(arg1, arg2, arg3, arg4);
#endif
}

template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
shared_ptr<T> make_shared(Arg1 && arg1, Arg2 && arg2, Arg3 && arg3, Arg4 && arg4, Arg5 && arg5)
{
#if   __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
    return shared_ptr<T>(new T(arg1, arg2, arg3, arg4, arg5));
#  else
    return shared_ptr<T>(new T(arg1, arg2, arg3, arg4, arg5));
#  endif
#else // libc++ or other
    return std::make_shared<T>(arg1, arg2, arg3, arg4, arg5);
#endif
}

/* \brief Adaptor for make_unique

usage:
\code
memory::make_unique<int>(3)
\endcode
*/
#if __cplusplus >= 201402L
using std::make_unique;
#else

template<class T> struct _Unique_if {
    typedef unique_ptr<T> _Single_object;
};

template<class T> struct _Unique_if<T[]> {
    typedef unique_ptr<T[]> _Unknown_bound;
};

template<class T, size_t N> struct _Unique_if<T[N]> {
    typedef void _Known_bound;
};

template<class T, class... Args>
    typename _Unique_if<T>::_Single_object
    make_unique(Args&&... args) {
        return unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

template<class T>
    typename _Unique_if<T>::_Unknown_bound
    make_unique(size_t n) {
        typedef typename std::remove_extent<T>::type U;
        return unique_ptr<T>(new U[n]());
    }

template<class T, class... Args>
    typename _Unique_if<T>::_Known_bound
    make_unique(Args&&...) = delete;
#endif

/* \brief Allocator for aligned data

usage:
\code
memory::aligned_allocator<real_t> alloc(32)
\endcode
*/

template < typename T, std::size_t Alignment = 16 >
class aligned_allocator
{
public:

    // The following will be the same for virtually all allocators.
    typedef T * pointer;
    typedef const T * const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;
    typedef std::size_t size_type;
    typedef ptrdiff_t difference_type;

    T * address(T& r) const
    {
        return &r;
    }

    const T * address(const T& s) const
    {
        return &s;
    }

    std::size_t max_size() const
    {
        // The following has been carefully written to be independent of
        // the definition of size_t and to avoid signed/unsigned warnings.
        return (static_cast<std::size_t>(0) - static_cast<std::size_t>(1)) / sizeof(T);
    }


    // The following must be the same for all allocators.
    template < typename U >
    struct rebind
    {
        typedef aligned_allocator < U, Alignment > other;
    } ;

    bool operator!=(const aligned_allocator& other) const
    {
        return !(*this == other);
    }

    void construct(T * const p, const T& t) const
    {
        void * const pv = static_cast<void *>(p);

        new (pv) T(t);
    }

    void destroy(T * const p) const
    {
        p->~T();
    }

    // Returns true if and only if storage allocated from *this
    // can be deallocated from other, and vice versa.
    // Always returns true for stateless allocators.
    bool operator==(const aligned_allocator& other) const
    {
        return true;
    }


    // Default constructor, copy constructor, rebinding constructor, and destructor.
    // Empty for stateless allocators.
    aligned_allocator() { }

    aligned_allocator(const aligned_allocator&) { }

    template < typename U > aligned_allocator(const aligned_allocator < U, Alignment >&) { }

    ~aligned_allocator() { }


    // The following will be different for each allocator.
    T * allocate(const std::size_t n) const
    {
        // The return value of allocate(0) is unspecified.
        // Mallocator returns NULL in order to avoid depending
        // on malloc(0)'s implementation-defined behavior
        // (the implementation can define malloc(0) to return NULL,
        // in which case the bad_alloc check below would fire).
        // All allocators can return NULL in this case.
        if (n == 0) {
            return NULL;
        }

        // All allocators should contain an integer overflow check.
        // The Standardization Committee recommends that std::length_error
        // be thrown in the case of integer overflow.
        if (n > max_size())
        {
            throw std::length_error("aligned_allocator<T>::allocate() - Integer overflow.");
        }

        // Mallocator wraps malloc().
        void * const pv = aligned_malloc(n * sizeof(T), Alignment);

        // Allocators should throw std::bad_alloc in the case of memory allocation failure.
        if (pv == NULL)
        {
            throw std::bad_alloc();
        }

        return static_cast<T *>(pv);
    }

    void deallocate(T * const p, const std::size_t n) const
    {
        aligned_free(p);
    }


    // The following will be the same for all allocators that ignore hints.
    template < typename U >
    T * allocate(const std::size_t n, const U * const) const
    {
        return allocate(n);
    }


    // Allocators are not required to be assignable, so
    // all allocators should have a private unimplemented
    // assignment operator. Note that this will trigger the
    // off-by-default (enabled under /Wall) warning C4626
    // "assignment operator could not be generated because a
    // base class assignment operator is inaccessible" within
    // the STL headers, but that warning is useless.
private:

    aligned_allocator& operator=(const aligned_allocator&);

    static inline void* aligned_malloc(size_t size, size_t alignment)
    {
        void* res = 0;
        void* ptr = malloc(size + alignment);
        if (ptr != 0)
        {
            res = reinterpret_cast<void*>((reinterpret_cast<size_t>(ptr) & ~(size_t(alignment - 1))) + alignment);
            *(reinterpret_cast<void**>(res) - 1) = ptr;
        }
        return res;
    }

    static inline void aligned_free(void* ptr)
    {
        if (ptr != 0)
            free(*(reinterpret_cast<void**>(ptr) - 1));
    }
};

/// \brief Deleter function that does not delete an object pointer
template <typename T> void null_deleter(T *) {}

/// \brief Deleter function that prints debug information
struct debug_deleter
{
    template <typename T> void operator()(T * ptr)
    {
        gsInfo<<" debug_deleter: delete "<< ptr << std::flush;
        delete ptr;
        gsInfo<<".\n";
    }
};

/// Takes a T* and wraps it in a shared_ptr. Useful for avoiding
/// memory leaks.
///
/// This has a move semantics: the shared_ptr object takes
/// the ownership.
template <typename T>
inline shared_ptr<T> make_shared(T *x) { return shared_ptr<T>(x); }

/// \brief Creates a shared pointer which does not eventually delete
/// the underlying raw pointer. Useful to refer to objects which
/// should not be destroyed.
///
/// The caller keeps the ownership.
template <typename T>
inline shared_ptr<T> make_shared_not_owned(const T *x)
{ return shared_ptr<T>(const_cast<T*>(x), null_deleter<T>); }

/// \brief Converts an uPtr \a p to an uPtr
/// of class \a toC and gives it back as return value.
template<class toC, typename from>
inline unique_ptr<toC> convert_ptr(from p)
{ return unique_ptr<toC>( dynamic_cast<toC*>(p.release()) ); }

/// Takes a vector of smart pointers and returns the corresponding raw pointers.
template <typename T>
inline std::vector<T*> get_raw(const std::vector< unique_ptr<T> >& cont)
{
    std::vector<T*> result;
    for (typename std::vector< unique_ptr<T> >::const_iterator it = cont.begin(); it != cont.end(); ++it)
        result.push_back(const_cast<T*>( (*it).get() ));
    return result;
}

/// Takes a vector of smart pointers and returns the corresponding raw pointers.
template <typename T>
inline std::vector<T*> get_raw(const std::vector< shared_ptr<T> >& cont)
{
    std::vector<T*> result;
    for (typename std::vector< shared_ptr<T> >::const_iterator it = cont.begin(); it != cont.end(); ++it)
        result.push_back(const_cast<T*>( (*it).get() ));
    return result;
}

/// Takes a vector of smart pointers, releases them and returns the corresponding raw pointers.
template <typename T>
inline std::vector<T*> release(std::vector< unique_ptr<T> >& cont)
{
    std::vector<T*> result;
    for (typename std::vector< unique_ptr<T> >::iterator it = cont.begin(); it != cont.end(); ++it)
        result.push_back( (*it).release() );
    cont.clear();
    return result;
}

} // end namespace memory

/**
    Alias for std::move, to be used instead of std::move for backward
    c++98 compatibility and MSVC before 2015

    Based on swapping and copy elision.
*/
template <typename S> inline S give(S & x)
{ S t; t.swap(x); return t; }

template <typename T> inline
memory::unique_ptr<T> give(memory::unique_ptr<T> & x)
{ return memory::unique_ptr<T>(x.release()); }

template <typename T> inline
memory::shared_ptr<T> give(memory::shared_ptr<T> & x)
{ memory::shared_ptr<T> result = x; x.reset(); return result; }

// Small, dynamically sized arrays on the stack, for POD types.
// Only use this if the size is guaranteed not to be more than a few
// hundred bytes! Be warned: overflow occurs without any warning
#if defined(gsGmp_ENABLED) || defined(gsMpfr_ENABLED)
 #define STACK_ARRAY( T, name, sz )    T name[sz];
#else
// Note: VLAs(following line) can be buggy on some compilers/versions,
// also not nececarily on the stack
// #define STACK_ARRAY( T, name, sz )    T name[sz];
#define STACK_ARRAY( T, name, sz )    T * name = (T*) alloca ( (sz) * sizeof(T) );
#endif

/// \brief Clones all pointers in the range [\a start \a end) and stores new
/// raw pointers in iterator \a out.
template <typename It, typename ItOut>
void cloneAll(It start, It end, ItOut out)
{
    for (It i = start; i != end; ++i)
        *out++ = dynamic_cast<typename std::iterator_traits<ItOut>::value_type>((*i)->clone().release());
}

/// \brief Clones all pointers in the container \a in and stores them as raw
/// pointers in container \a out
template <typename ContIn, typename ContOut>
void cloneAll(const ContIn& in, ContOut& out)
{
    out.resize(in.size());
    cloneAll(in.begin(), in.end(), out.begin());
}

/// \brief Frees all pointers in the range [\a begin \a end)
template <typename It>
void freeAll(It begin, It end)
{
    for (It it = begin; it != end; ++it)
    {
        delete (*it);
        *it = NULL;
    }
}

/// \brief Frees all pointers in the container \a Cont
template <typename Cont>
void freeAll(Cont& cont)
{
    for (typename Cont::iterator it = cont.begin(); it != cont.end(); ++it)
        delete (*it);
    cont.clear();
}

/// \brief Constructs a vector of pointers from a vector of objects
template<typename obj> inline
std::vector<obj*> asVectorPtr(const std::vector<obj> & matv)
{
    std::vector<obj*> result;
    const size_t d = matv.size();
    result.reserve(d);
    for ( size_t i = 0; i!=d; ++i)
        result.push_back( const_cast<obj*>(&matv[i]) );
    return result;
}

/// \brief Casts a vector of pointers
template <typename Base, typename Derived>
std::vector<Base*> castVectorPtr(std::vector<Derived*> pVec)
{
    std::vector<Base*> result(pVec.size());
    std::copy(pVec.begin(), pVec.end(), result.begin() );
    return result;
}

/// \brief Returns true if all instances of \a Base cast to \a Derived
template <typename Derived, typename Base>
bool checkVectorPtrCast(std::vector<Base*> pVec)
{
    for (typename std::vector<Base*>::iterator it = pVec.begin(); it != pVec.end(); ++it)
        if ( ! dynamic_cast<Derived*>(*it) )
            return false;
    return true;
}

/**
   \brief Small wrapper for std::copy mimicking memcpy (or
   std::copy_n) for a raw pointer destination, copies \a n positions
   starting from \a begin into \a result. The latter is expected to
   have been allocated in advance
*/
template <class T, class U>
inline void copy_n(T begin, const size_t n, U* result)
{
    std::copy(begin, begin+n,
#   ifdef _MSC_VER
              // Take care of C4996 warning
              //stdext::checked_array_iterator<U*>(result,n));
              stdext::unchecked_array_iterator<U*>(result));
#   else
    result);
// Note: in C++11 there is:
// std::copy_n(begin, n, result);
#   endif
}

namespace util
{
/**
   \brief Small wrapper for std::copy mimicking std::copy for a raw
   pointer destination, copies \a n positions starting from \a begin
   into \a result. The latter is expected to have been allocated in
   advance
*/
template <class T, class U>
inline void copy(T begin, T end, U* result)
{
    std::copy(begin, end,
#   ifdef _MSC_VER
              // Take care of C4996 warning
              //stdext::checked_array_iterator<U*>(result,n));
              stdext::unchecked_array_iterator<U*>(result));
#   else
    result);
#   endif
}

}

} // end namespace gismo