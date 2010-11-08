// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/lasbounds.hpp>
#include <liblas/guid.hpp>
#include <tut/tut.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include "common.hpp"

namespace tut
{ 
    struct lasbounds_data
    {
        liblas::Bounds<double> m_default;
        typedef std::vector< liblas::Range<double> > Ranges;
    };

    typedef test_group<lasbounds_data> tg;
    typedef tg::object to;

    tg test_group_lasbounds("liblas::Bounds");


    liblas::Range<double> xrng(0, 100);
    liblas::Range<double> yrng(0, 200);
    liblas::Range<double> zrng(0, 300);


    // Test default constructor
    template<>
    template<>
    void to::test<1>()
    {
        Ranges rngs;
        rngs.push_back(xrng);
        rngs.push_back(yrng);
        rngs.push_back(zrng);
        
        typedef liblas::Bounds<double> b_t;
        b_t b(rngs);
        ensure_equals(b.minx(), b_t::value_type(0));
        ensure_equals(b.maxx(), b_t::value_type(100));
        ensure_equals(b.miny(), b_t::value_type(0));
        ensure_equals(b.maxy(), b_t::value_type(200));
        ensure_equals(b.minz(), b_t::value_type(0));
        ensure_equals(b.maxz(), b_t::value_type(300));
    }

    template<>
    template<>
    void to::test<2>()
    {
        ensure(yrng.contains(xrng));
        ensure(zrng.contains(yrng));
        
    }

    template<>
    template<>
    void to::test<3>()
    {
        Ranges rngs;
        rngs.push_back(xrng);
        rngs.push_back(yrng);
        rngs.push_back(zrng);
        
        liblas::Bounds<double> b(rngs);  
        liblas::Bounds<double> b2(b);
        ensure_equals(b2.minx(), b.minx());
        ensure_equals(b2.miny(), b.miny());
        ensure_equals(b2.minz(), b.minz());
        ensure_equals(b2.maxx(), b.maxx());
        ensure_equals(b2.maxy(), b.maxy());
        ensure_equals(b2.maxz(), b.maxz());
        
    }

    template<>
    template<>
    void to::test<4>()
    {
        liblas::Range<double> r;
        ensure(r.empty());
    }
}
