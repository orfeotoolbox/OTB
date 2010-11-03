// $Id$
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/lasclassification.hpp>
#include <tut/tut.hpp>
#include <bitset>
#include <sstream>
#include <stdexcept>
#include <string>
#include "common.hpp"

namespace tut
{ 
    struct lasclassification_data
    {
        typedef liblas::Classification::bitset_type bitset_type;
        liblas::Classification m_default;
    };

    typedef test_group<lasclassification_data> tg;
    typedef tg::object to;

    tg test_group_lasclassification("liblas::Classification");

    template<>
    template<>
    void to::test<1>()
    {
        ensure_equals(m_default, bitset_type(0));
        ensure_equals(m_default.GetClass(), 0);
        ensure_not(m_default.IsSynthetic());
        ensure_not(m_default.IsKeyPoint());
        ensure_not(m_default.IsWithheld());
    }

    template<>
    template<>
    void to::test<2>()
    {
        liblas::Classification c0(0);

        ensure_equals(c0, m_default);
        ensure_equals(c0, bitset_type(0));
        ensure_equals(c0.GetClass(), 0);
        ensure_not(c0.IsSynthetic());
        ensure_not(c0.IsKeyPoint());
        ensure_not(c0.IsWithheld());
    }

    template<>
    template<>
    void to::test<3>()
    {
        liblas::Classification c31(0x1F);

        ensure_not(c31 == m_default);
        ensure_equals(c31.GetClass(), 31);
        ensure_not(c31.IsSynthetic());
        ensure_not(c31.IsKeyPoint());
        ensure_not(c31.IsWithheld());
    }

    template<>
    template<>
    void to::test<4>()
    {
        liblas::Classification c255(255);
        ensure_equals(c255, bitset_type(255));
        ensure_equals(c255.GetClass(), 31);
        ensure(c255.IsSynthetic());
        ensure(c255.IsKeyPoint());
        ensure(c255.IsWithheld());
    }

    template<>
    template<>
    void to::test<5>()
    {
        liblas::Classification c(31, false, false, false);
     
        ensure_equals(c.GetClass(), 31);
        ensure_not(c.IsSynthetic());
        ensure_not(c.IsKeyPoint());
        ensure_not(c.IsWithheld());
        ensure_equals(c, bitset_type(std::string("00011111")));
    }

    template<>
    template<>
    void to::test<6>()
    {
        liblas::Classification c(7, true, false, true);
     
        ensure_equals(c.GetClass(), 7);
        ensure_not(c.IsKeyPoint());
        ensure(c.IsWithheld());
        ensure(c.IsSynthetic());
        ensure_equals(c, bitset_type(std::string("10100111")));
    }

    template<>
    template<>
    void to::test<7>()
    {
        try
        {
            liblas::Classification c(255, true, true, true);

            fail("std::out_of_range not thrown but expected");
        }
        catch (std::out_of_range const& e)
        {
            ensure(e.what(), true);
        }
        catch (...)
        {
            fail("unhandled exception expected");
        }
    }

    template<>
    template<>
    void to::test<8>()
    {
        liblas::Classification cpy(m_default);
     
        ensure_equals(cpy, m_default);
    }

    template<>
    template<>
    void to::test<9>()
    {
        liblas::Classification c(7, true, false, true);
        liblas::Classification cpy(c);
     
        ensure_equals(cpy.GetClass(), 7);
        ensure_not(cpy.IsKeyPoint());
        ensure(cpy.IsWithheld());
        ensure(cpy.IsSynthetic());
        ensure_equals(cpy, bitset_type(std::string("10100111")));
        ensure_equals(cpy, c);
    }

    template<>
    template<>
    void to::test<10>()
    {
        liblas::Classification cpy;
        cpy = m_default;
     
        ensure_equals(cpy, m_default);
    }

    template<>
    template<>
    void to::test<11>()
    {
        liblas::Classification c(7, true, false, true);
        liblas::Classification cpy = c;
     
        ensure_equals(cpy.GetClass(), 7);
        ensure_not(cpy.IsKeyPoint());
        ensure(cpy.IsWithheld());
        ensure(cpy.IsSynthetic());
        ensure_equals(cpy, bitset_type(std::string("10100111")));
        ensure_equals(cpy, c);
    }


    template<>
    template<>
    void to::test<12>()
    {
        liblas::Classification c;

        c.SetClass(0);
        ensure_equals(c.GetClass(), 0);

        c.SetClass(31);
        ensure_equals(c.GetClass(), 31);

        ensure(c != m_default);
    }

    template<>
    template<>
    void to::test<13>()
    {
        liblas::Classification c;

        c.SetSynthetic(true);
        ensure(c.IsSynthetic());
        ensure(c != m_default);

        c.SetSynthetic(false);
        ensure_not(c.IsSynthetic());
        ensure_equals(c, m_default);

        c.SetSynthetic(true);
        ensure(c.IsSynthetic());
        ensure(c != m_default);

        c.SetSynthetic(false);
        ensure_not(c.IsSynthetic());
        ensure_equals(c, m_default);

        ensure_equals(c.GetClass(), 0);
    }

    template<>
    template<>
    void to::test<14>()
    {
        liblas::Classification c;

        c.SetKeyPoint(true);
        ensure(c.IsKeyPoint());
        ensure(c != m_default);

        c.SetKeyPoint(false);
        ensure_not(c.IsKeyPoint());
        ensure_equals(c, m_default);

        c.SetKeyPoint(true);
        ensure(c.IsKeyPoint());
        ensure(c != m_default);

        c.SetKeyPoint(false);
        ensure_not(c.IsKeyPoint());
        ensure_equals(c, m_default);

        ensure_equals(c.GetClass(), 0);
    }

    template<>
    template<>
    void to::test<15>()
    {
        liblas::Classification c;

        c.SetWithheld(true);
        ensure(c.IsWithheld());
        ensure(c != m_default);

        c.SetWithheld(false);
        ensure_not(c.IsWithheld());
        ensure_equals(c, m_default);

        c.SetWithheld(true);
        ensure(c.IsWithheld());
        ensure(c != m_default);

        c.SetWithheld(false);
        ensure_not(c.IsWithheld());
        ensure_equals(c, m_default);

        ensure_equals(c.GetClass(), 0);
    }

    template<>
    template<>
    void to::test<16>()
    {
        liblas::Classification c;

        c.SetKeyPoint(true);
        ensure(c.IsKeyPoint());
        ensure(c != m_default);

        c.SetWithheld(true);
        ensure(c.IsWithheld());
        ensure(c.IsKeyPoint());
        ensure(c != m_default);
        
        c.SetSynthetic(true);
        ensure(c.IsWithheld());
        ensure(c.IsKeyPoint());
        ensure(c.IsSynthetic());
        ensure(c != m_default);

        ensure_equals(c.GetClass(), 0);
    }

    template<>
    template<>
    void to::test<17>()
    {
        liblas::Classification c;

        c.SetKeyPoint(true);
        c.SetSynthetic(true);
        c.SetWithheld(true);
        ensure(c.IsWithheld());
        ensure(c.IsKeyPoint());
        ensure(c.IsSynthetic());
        ensure_not(c == m_default);
        ensure_equals(c.GetClass(), 0);

        c.SetKeyPoint(false);
        c.SetSynthetic(false);
        c.SetWithheld(false);
        ensure_not(c.IsWithheld());
        ensure_not(c.IsKeyPoint());
        ensure_not(c.IsSynthetic());
        ensure_equals(c.GetClass(), 0);

        liblas::Classification::bitset_type bits1(c);
        liblas::Classification::bitset_type bits2(m_default);
        ensure_equals(c, m_default);
    }

    template<>
    template<>
    void to::test<18>()
    {
        liblas::Classification c;

        c.SetKeyPoint(true);
        c.SetClass(1);
        c.SetSynthetic(true);
        c.SetWithheld(true);
        ensure(c.IsWithheld());
        ensure(c.IsKeyPoint());
        ensure(c.IsSynthetic());
        ensure_equals(c.GetClass(), 1);
        ensure_not(c == m_default);
        
        c.SetKeyPoint(false);
        c.SetSynthetic(false);
        c.SetClass(0);
        c.SetWithheld(false);
        ensure_not(c.IsWithheld());
        ensure_not(c.IsKeyPoint());
        ensure_not(c.IsSynthetic());
        ensure_equals(c.GetClass(), 0);

        liblas::Classification::bitset_type bits1(c);
        liblas::Classification::bitset_type bits2(m_default);
        ensure_equals(c, m_default);
    }

    template<>
    template<>
    void to::test<19>()
    {
        std::string const sbits("00000000");

        liblas::Classification c;

        std::ostringstream oss;
        oss << c;
        ensure_equals(oss.str(), sbits);
    }

    template<>
    template<>
    void to::test<20>()
    {
        std::string const sbits("00000011");

        liblas::Classification c;
        c.SetClass(3);

        std::ostringstream oss;
        oss << c;
        ensure_equals(oss.str(), sbits);
    }

    template<>
    template<>
    void to::test<21>()
    {
        std::string const sbits("10000001");

        liblas::Classification c;
        
        c.SetWithheld(true);
        c.SetClass(1);

        std::ostringstream oss;
        oss << c;
        ensure_equals(oss.str(), sbits);
    }

    template<>
    template<>
    void to::test<22>()
    {
        std::string const sbits("10110000");

        liblas::Classification c;
        
        c.SetClass(16);
        c.SetSynthetic(true);
        c.SetKeyPoint(false);
        c.SetWithheld(true);

        std::ostringstream oss;
        oss << c;
        ensure_equals(oss.str(), sbits);
    }

    template<>
    template<>
    void to::test<23>()
    {
        std::string const sbits("00000000");
        liblas::Classification::bitset_type bits(sbits);
        
        liblas::Classification c(bits);
        ensure_equals(c, bits);

        std::ostringstream oss;
        oss << c;
        ensure_equals(oss.str(), sbits);
    }

    template<>
    template<>
    void to::test<24>()
    {
        std::string const sbits("00000011");
        liblas::Classification::bitset_type bits(sbits);

        liblas::Classification c(bits);
        ensure_equals(c, bits);

        std::ostringstream oss;
        oss << c;
        ensure_equals(oss.str(), sbits);
    }

    template<>
    template<>
    void to::test<25>()
    {
        std::string const sbits("10000001");
        liblas::Classification::bitset_type bits(sbits);

        liblas::Classification c(bits);
        ensure_equals(c, bits);

        std::ostringstream oss;
        oss << c;
        ensure_equals(oss.str(), sbits);
    }

    template<>
    template<>
    void to::test<26>()
    {
        std::string const sbits("10110000");
        liblas::Classification::bitset_type bits(sbits);

        liblas::Classification c(bits);
        ensure_equals(c, bits);

        std::ostringstream oss;
        oss << c;
        ensure_equals(oss.str(), sbits);
    }

    template<>
    template<>
    void to::test<27>()
    {
        std::string const cn("Created, never classified");
        ensure_equals(m_default.GetClassName(), cn);
    }

    template<>
    template<>
    void to::test<28>()
    {
        std::string const cn("Low Point (noise)");
        m_default.SetClass(7);
        ensure_equals(m_default.GetClassName(), cn);
    }

    template<>
    template<>
    void to::test<29>()
    {
        std::string const cn("Reserved for ASPRS Definition");
        m_default.SetClass(31);
        ensure_equals(m_default.GetClassName(), cn);
    }

    template<>
    template<>
    void to::test<30>()
    {
        try
        {
            m_default.SetClass(32);
            fail("std::out_of_range not thrown but expected");
        }
        catch (std::out_of_range const& e)
        {
            ensure(e.what(), true);
        }
        catch (...)
        {
            fail("unhandled exception expected");
        }
    }
}
