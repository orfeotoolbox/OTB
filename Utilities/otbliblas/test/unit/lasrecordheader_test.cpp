// $Id: lasrecordheader_test.cpp 712 2008-05-14 22:47:43Z hobu $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/lasrecordheader.hpp>
#include <liblas/cstdint.hpp>
#include <tut/tut.hpp>
#include <string>

namespace tut
{ 
    struct lasrecordheader_data
    {
        liblas::LASVLR m_default;

        void test_default(liblas::LASVLR const& h)
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

    typedef test_group<lasrecordheader_data> tg;
    typedef tg::object to;

    tg test_group_lasrecordheader("liblas::LASVLR");

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
        liblas::LASVLR hdr_copy(m_default);
        test_default(hdr_copy);
    }

    // Test assignment operator
    template<>
    template<>
    void to::test<3>()
    {
        liblas::LASVLR hdr_copy;
        test_default(hdr_copy);

        hdr_copy = m_default;
        test_default(hdr_copy);
    }

    // Test equal function
    template<>
    template<>
    void to::test<4>()
    {
        liblas::LASVLR hdr;
        ensure("two default headers not equal", m_default.equal(hdr));

        liblas::LASVLR hdr_copy(m_default);
        ensure("copy of default header not equal", hdr.equal(m_default));
    }

    // Test equal-to operator
    template<>
    template<>
    void to::test<5>()
    {
        liblas::LASVLR hdr;
        ensure("two default headers not equal", m_default == hdr);

        liblas::LASVLR hdr_copy(m_default);
        ensure("copy of default header not equal", hdr == m_default);
    }

    // Test not-equal-to operator
    template<>
    template<>
    void to::test<6>()
    {
        liblas::LASVLR hdr;
        ensure_not("two default headers not equal", m_default != hdr);

        liblas::LASVLR hdr_copy(m_default);
        ensure_not("copy of default header not equal", hdr != m_default);
    }
}

