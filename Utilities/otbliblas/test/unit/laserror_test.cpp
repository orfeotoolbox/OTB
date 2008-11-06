// $Id: laserror_test.cpp 745 2008-05-21 00:50:09Z mloskot $
//
// (C) Copyright Mateusz Loskot 2008, mateusz@loskot.net
// Distributed under the BSD License
// (See accompanying file LICENSE.txt or copy at
// http://www.opensource.org/licenses/bsd-license.php)
//
#include <liblas/laserror.hpp>
#include <tut/tut.hpp>
#include <string>

namespace tut
{ 
    struct laserror_data
    {
        int const code;
        std::string const msg;
        std::string const method;

        laserror_data()
            : code(101), msg("Test message"), method("foo")
        {}

        private:
            // non-copyable type
            laserror_data(laserror_data const&);
            laserror_data& operator=(laserror_data const&);
    };

    typedef test_group<laserror_data> tg;
    typedef tg::object to;

    tg test_group_laserror("liblas::LASError");

    // Test constructor
    template<>
    template<>
    void to::test<1>()
    {
        liblas::LASError err(code, msg, method);

        ensure_equals(err.GetCode(), code);
        ensure_equals(err.GetMessage(), msg);
        ensure_equals(err.GetMethod(), method);
    }

    // Test copy constructor
    template<>
    template<>
    void to::test<2>()
    {
        liblas::LASError err(code, msg, method);
        liblas::LASError copy(err);

        ensure_equals(copy.GetCode(), code);
        ensure_equals(copy.GetMessage(), msg);
        ensure_equals(copy.GetMethod(), method);
    }

    // Test assignment operator
    template<>
    template<>
    void to::test<3>()
    {
        liblas::LASError copy(0, "", "");;

        {
            liblas::LASError err(code, msg, method);
            copy = err;
        }

        ensure_equals(copy.GetCode(), code);
        ensure_equals(copy.GetMessage(), msg);
        ensure_equals(copy.GetMethod(), method);
    }
}

