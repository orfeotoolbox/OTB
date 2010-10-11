// $Id: lasvariablerecord_test.cpp 712 2008-05-14 22:47:43Z hobu $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/lasvariablerecord.hpp>
#include <liblas/cstdint.hpp>
#include <tut/tut.hpp>
#include <string>

namespace tut
{ 
    struct lasvariablerecord_data
    {
        liblas::LASVariableRecord m_default;

        void test_default(liblas::LASVariableRecord const& h)
        {
            ensure_equals("wrong default reserved bytes",
                h.GetReserved(), liblas::uint16_t());

            ensure_equals("wrong default record identifier",
                h.GetRecordId(), liblas::uint16_t());

            ensure_equals("wrong default record length",
                h.GetRecordLength(), liblas::uint16_t());

            ensure_equals("wrong default user identifier",
                h.GetUserId(true).c_str(), std::string());

            ensure_equals("wrong default description",
                h.GetDescription(true).c_str(), std::string());
        }
    };

    typedef test_group<lasvariablerecord_data> tg;
    typedef tg::object to;

    tg test_group_lasvariablerecord("liblas::LASVariableRecord");

    // Test default constructor
    template<>
    template<>
    void to::test<1>()
    {
        test_default(m_default);
    }

    // Test copy constructor
    template<>
    template<>
    void to::test<2>()
    {
        liblas::LASVariableRecord hdr_copy(m_default);
        test_default(hdr_copy);
    }

    // Test assignment operator
    template<>
    template<>
    void to::test<3>()
    {
        liblas::LASVariableRecord hdr_copy;
        test_default(hdr_copy);

        hdr_copy = m_default;
        test_default(hdr_copy);
    }

    // Test equal function
    template<>
    template<>
    void to::test<4>()
    {
        liblas::LASVariableRecord hdr;
        ensure("two default headers not equal", m_default.equal(hdr));

        liblas::LASVariableRecord hdr_copy(m_default);
        ensure("copy of default header not equal", hdr.equal(m_default));
    }

    // Test equal-to operator
    template<>
    template<>
    void to::test<5>()
    {
        liblas::LASVariableRecord hdr;
        ensure("two default headers not equal", m_default == hdr);

        liblas::LASVariableRecord hdr_copy(m_default);
        ensure("copy of default header not equal", hdr == m_default);
    }

    // Test not-equal-to operator
    template<>
    template<>
    void to::test<6>()
    {
        liblas::LASVariableRecord hdr;
        ensure_not("two default headers not equal", m_default != hdr);

        liblas::LASVariableRecord hdr_copy(m_default);
        ensure_not("copy of default header not equal", hdr != m_default);
    }
}

