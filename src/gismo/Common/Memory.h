/** @file gsMemory.h

    @brief Provides utility function related to memory management.

    This file is part of the G+Smo library.

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.

    Author(s): C. Hofreither, A. Mantzaflaris, J. Vogl
*/

#pragma once

// Include config for build configuration
#include "gismo/Common/Config.h"
#include "gismo/Common/Export.h"

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

#if  __cplusplus < 201103 && defined( __GLIBCXX__ )
#  if defined(__INTEL_COMPILER)
     template <class T>
     struct shared_ptr : public boost::shared_ptr<T>
     {
         shared_ptr() : boost::shared_ptr<T>() { }
         template<class U>
         shared_ptr(U * u) : boost::shared_ptr<T>(u) { }
         template<class U>
         shared_ptr(const boost::shared_ptr<U> & u) : boost::shared_ptr<T>(u) { }
     };

     template <class T>
     struct weak_ptr : public boost::weak_ptr<T>
     {
         weak_ptr() : boost::weak_ptr<T>() { }
         template<class U>
         weak_ptr(const boost::shared_ptr<U> & u) : boost::weak_ptr<T>(u) { }
     };

     using boost::make_shared;
#  else
     template<class T> struct shared_ptr : public std::tr1::shared_ptr<T>
     {
         shared_ptr() : std::tr1::shared_ptr<T>() { }
         template<class U>
         shared_ptr(U * u) : std::tr1::shared_ptr<T>(u) { }
         template<class U>
         shared_ptr(const std::tr1::shared_ptr<U> & u) : std::tr1::shared_ptr<T>(u) { }
     };

     template<class T> struct weak_ptr : public std::tr1::weak_ptr<T>
     {
         weak_ptr() : std::tr1::weak_ptr<T>() { }
         template<class U>
         weak_ptr(const std::tr1::shared_ptr<U> & u) : std::tr1::weak_ptr<T>(u) { }
     };

     template<class T>
     shared_ptr<T> make_shared()
     {
         return shared_ptr<T>( new T() );
     }

     template<class T, class A1>
     shared_ptr<T> make_shared(const A1& a1)
     {
         return shared_ptr<T>( new T(a1) );
     }

     template<class T, class A1, class A2>
     shared_ptr<T> make_shared(const A1& a1, const A2& a2)
     {
         return shared_ptr<T>( new T(a1, a2) );
     }

     template<class T, class A1, class A2, class A3>
     shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3)
     {
         return shared_ptr<T>( new T(a1, a2, a3) );
     }

     template<class T, class A1, class A2, class A3, class A4>
     shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
     {
         return shared_ptr<T>( new T(a1, a2, a3, a4) );
     }

     template<class T, class A1, class A2, class A3, class A4, class A5>
     shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
     {
         return shared_ptr<T>( new T(a1, a2, a3, a4, a5) );
     }

     template<class T, class A1, class A2, class A3, class A4, class A5, class A6>
     shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
     {
         return shared_ptr<T>( new T(a1, a2, a3, a4, a5, a6) );
     }
#  endif
#else
     using std::shared_ptr;
     using std::weak_ptr;
     using std::make_shared;
     using std::unique_ptr;
     using std::make_unique;
#endif


/// Returns the amount of data (in bytes) available in the system's memory
GISMO_EXPORT size_t available();

/// Returns the total amount of data (in bytes) in the system's memory
GISMO_EXPORT size_t total();

} // namespace memory

} // namespace gismo