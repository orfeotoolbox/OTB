// $Id: lasspatialreference_test.cpp 1102 2009-03-16 14:57:12Z hobu $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/lasspatialreference.hpp>
#include <liblas/guid.hpp>
#include <tut/tut.hpp>
#include <string>
#include <stdexcept>
#include "common.hpp"

namespace tut
{ 
    struct lasspatialreference_data
    {
        liblas::SpatialReference m_default;
    };

    typedef test_group<lasspatialreference_data> tg;
    typedef tg::object to;

    tg test_group_lasspatialreference("liblas::SpatialReference");

    // Test default constructor
    template<>
    template<>
    void to::test<1>()
    {
        ensure_equals(m_default.GetProj4(), "");
        ensure_equals(m_default.GetWKT(), "");
    }

}

