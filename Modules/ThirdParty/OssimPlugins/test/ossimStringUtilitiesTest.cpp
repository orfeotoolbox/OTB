//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
// #pragma GCC diagnostic ignored "-Woverloaded-virtual"
#pragma GCC diagnostic ignored "-Wshadow"
#endif

#define BOOST_TEST_MODULE "ossim String Utilities unit testing"
#define BOOST_TEST_DYN_LINK
#include "ossimStringUtilities.h"
#include <boost/test/unit_test.hpp>
#include <cstdio>
#include <iostream>

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

using namespace ossimplugins;

#if 0
BOOST_AUTO_TEST_CASE(begin_end)
{
   char const        t[] = "123456789";
   std::string const s   = "123456789";

   BOOST_REQUIRE_EQUAL(*rbegin(t), 0);
   BOOST_REQUIRE_EQUAL(size(s), 9u);
   BOOST_REQUIRE_EQUAL(size(t), 10u);
}
#endif

BOOST_AUTO_TEST_CASE(string_view_array)
{
   char const        t[] = "123456789";

   string_view v(t);
   BOOST_CHECK_EQUAL(v.size()    , 9u);
   BOOST_CHECK(! v.empty());
   BOOST_CHECK_EQUAL(*v.begin()  , '1');
   BOOST_CHECK_EQUAL(*(v.end()-1), '9');
   BOOST_CHECK_EQUAL(v.front()   , '1');
   BOOST_CHECK_EQUAL(v.back()    , '9');

   BOOST_CHECK(v.belongs_to(t));
   for (std::size_t i = 0; i!= sizeof(t) ; ++i) {
      for (std::size_t j=i+1; j!=sizeof(t) ; ++j) {
         string_view tmp(&t[i], &t[j]);
         // std::cout << t << "[" << i << ":" << j << "[ -> " << tmp ;
         BOOST_CHECK(tmp.belongs_to(t));
         string_view wh = find(t, tmp);
         // std::cout << "  -> found: " << static_cast<void const*>(wh.data()) << ", " << wh.size() << " chars\n";
         BOOST_CHECK_EQUAL(wh.data(), &t[i]);
         BOOST_CHECK(contains(t,tmp));
      }
   }

   BOOST_CHECK(contains(t, "345"));
   BOOST_CHECK(!contains(t, "35"));
}

BOOST_AUTO_TEST_CASE(string_view_string)
{
   std::string const s   = "123456789";

   string_view v(s);
   BOOST_CHECK_EQUAL(v.size()    , 9u);
   BOOST_CHECK(! v.empty());
   BOOST_CHECK_EQUAL(*v.begin()  , '1');
   BOOST_CHECK_EQUAL(*(v.end()-1), '9');
   BOOST_CHECK_EQUAL(v.front()   , '1');
   BOOST_CHECK_EQUAL(v.back()    , '9');

   const std::size_t len = s.length();
   BOOST_CHECK(v.belongs_to(s));
   for (std::size_t i = 0; i!= len ; ++i) {
      for (std::size_t j=i; j!=len ; ++j) {
         BOOST_CHECK(string_view(&s[i], &s[j]).belongs_to(s));
      }
   }

   BOOST_CHECK(string_view(&s[0], &s[len]).belongs_to(s));
   BOOST_CHECK(!string_view(&s[0], &s[len+1]).belongs_to(s));
   BOOST_CHECK(!string_view(&s[-1], &s[len]).belongs_to(s));
}

BOOST_AUTO_TEST_CASE(string_view_ptr)
{
   char const       *ptr = "123456789";

   string_view v(ptr);
   BOOST_CHECK_EQUAL(v.size()    , 9u);
   BOOST_CHECK(! v.empty());
   BOOST_CHECK_EQUAL(*v.begin()  , '1');
   BOOST_CHECK_EQUAL(*(v.end()-1), '9');
   BOOST_CHECK_EQUAL(v.front()   , '1');
   BOOST_CHECK_EQUAL(v.back()    , '9');

   const std::size_t len = std::strlen(ptr);
   BOOST_CHECK(v.belongs_to(ptr));
   for (std::size_t i = 0; i!= len ; ++i) {
      for (std::size_t j=i; j!=len ; ++j) {
         BOOST_CHECK(string_view(&ptr[i], &ptr[j]).belongs_to(ptr));
      }
   }
}

BOOST_AUTO_TEST_CASE(tests)
{
   char const        t[] = "123456789";
   std::string const s   = "123456789";

   string_view v(t);
   BOOST_CHECK_EQUAL(v, t);
   BOOST_CHECK_EQUAL(v, s);

   BOOST_CHECK(starts_with(v, t));
   BOOST_CHECK(starts_with(v, s));
   BOOST_CHECK(starts_with(s, t));
   BOOST_CHECK(starts_with(t, s));
   BOOST_CHECK(starts_with(t, v));
   BOOST_CHECK(starts_with(s, v));

   BOOST_CHECK(ends_with(v, t));
   BOOST_CHECK(ends_with(v, s));
   BOOST_CHECK(ends_with(s, t));
   BOOST_CHECK(ends_with(t, s));
   BOOST_CHECK(ends_with(t, v));
   BOOST_CHECK(ends_with(s, v));

   BOOST_CHECK(starts_with(s, "1"));
   BOOST_CHECK(starts_with(s, "12"));
   BOOST_CHECK(starts_with(s, "123"));
   BOOST_CHECK(!starts_with(s, "23"));

   BOOST_CHECK(ends_with(s, "9"));
   BOOST_CHECK(ends_with(s, "89"));
   BOOST_CHECK(ends_with(s, "789"));
   BOOST_CHECK(!ends_with(s, "23"));
}



template <typename RangeType, std::size_t NbParts>
void check_split(RangeType const& parts, char const* (&expected)[NbParts])
{
   typedef RangeType                           range_type;
   typedef typename range_type::const_iterator const_iterator;

   const_iterator it = parts.begin();
   const_iterator end = parts.end();

   std::size_t n=0;
   for (; it != end && n!=NbParts ; ++it, ++n) {
      // std::cout << "iteration #" << n << " -> " << it << std::endl;
      BOOST_CHECK(it->begin() <= it->end());
      BOOST_CHECK_EQUAL(*it, expected[n]);
   }
   // std::cout << "End reached: " << it << std::endl;
   BOOST_CHECK_EQUAL(n, NbParts);
   BOOST_CHECK_EQUAL(it, end);
}

BOOST_AUTO_TEST_CASE(split1) {
   {
      char const* expected[] = { "abc", "def", "xy" };
      check_split(split_on( "abc,def,xy", ','), expected);
   }
   {
      char const* expected[] = { "", "abc", "z", "", "", "def", "xy", "", "" };
      check_split(split_on( ",abc,z,,,def,xy,,", ','), expected);
   }
}


BOOST_AUTO_TEST_CASE(to_int) {
   BOOST_CHECK_EQUAL(to<int>("12", "UT"), 12);
   BOOST_CHECK_EQUAL(to<int>("0", "UT"), 0);
   BOOST_CHECK_EQUAL(to<int>("13576", "UT"), 13576);
   BOOST_CHECK_EQUAL(to<int>("+12", "UT"), 12);
   BOOST_CHECK_EQUAL(to<int>("+0", "UT"), 0);
   BOOST_CHECK_EQUAL(to<int>("+13576", "UT"), 13576);
   BOOST_CHECK_EQUAL(to<int>("-12", "UT"), -12);
   BOOST_CHECK_EQUAL(to<int>("-0", "UT"), 0);
   BOOST_CHECK_EQUAL(to<int>("-13576", "UT"), -13576);

   BOOST_CHECK_EQUAL(to_with_default<int>("abc", 42), 42);
   BOOST_CHECK_EQUAL(to_with_default<int>("12h", 42), 42);
   BOOST_CHECK_EQUAL(to_with_default<int>("0x12", 42), 42);
   BOOST_CHECK_THROW(to<int>("abc", "UT"), std::runtime_error);
   BOOST_CHECK_THROW(to<int>("12h", "UT"), std::runtime_error);
   BOOST_CHECK_THROW(to<int>("0x12", "UT"), std::runtime_error);

   BOOST_CHECK_EQUAL(to<unsigned int>("12", "UT"), 12u);
   BOOST_CHECK_EQUAL(to<int>("-1", "UT"), -1);
   BOOST_CHECK_THROW(to<unsigned int>("-1", "UT"), std::runtime_error);
}
BOOST_AUTO_TEST_CASE(to_double) {
   BOOST_CHECK_CLOSE(to<double>("12", "UT"),       12,       0.00001);
   BOOST_CHECK_CLOSE(to<double>("0", "UT"),        0,        0.00001);
   BOOST_CHECK_CLOSE(to<double>("13576", "UT"),    13576,    0.00001);
   BOOST_CHECK_CLOSE(to<double>("+12", "UT"),      12,       0.00001);
   BOOST_CHECK_CLOSE(to<double>("+0", "UT"),       0,        0.00001);
   BOOST_CHECK_CLOSE(to<double>("+13576", "UT"),   13576,    0.00001);
   BOOST_CHECK_CLOSE(to<double>("-12", "UT"),      -12,      0.00001);
   BOOST_CHECK_CLOSE(to<double>("-0", "UT"),       0,        0.00001);
   BOOST_CHECK_CLOSE(to<double>("-13576", "UT"),   -13576,   0.00001);

   BOOST_CHECK_CLOSE(to<double>("12.1", "UT"),     12.1,     0.00001);
   BOOST_CHECK_CLOSE(to<double>("0.1", "UT"),      0.1,      0.00001);
   BOOST_CHECK_CLOSE(to<double>("13576.1", "UT"),  13576.1,  0.00001);
   BOOST_CHECK_CLOSE(to<double>("+12.1", "UT"),    12.1,     0.00001);
   BOOST_CHECK_CLOSE(to<double>("+0.1", "UT"),     0.1,      0.00001);
   BOOST_CHECK_CLOSE(to<double>("+13576.1", "UT"), 13576.1,  0.00001);
   BOOST_CHECK_CLOSE(to<double>("-12.1", "UT"),    -12.1,    0.00001);
   BOOST_CHECK_CLOSE(to<double>("-0.1", "UT"),     -0.1,     0.00001);
   BOOST_CHECK_CLOSE(to<double>("-13576.1", "UT"), -13576.1, 0.00001);

   // invalid double conversions return 0 as per ossim way of doind things
   BOOST_CHECK_THROW(to<double>("abc", "UT"),  std::runtime_error);
   BOOST_CHECK_THROW(to<double>("12h", "UT"),  std::runtime_error);
   BOOST_CHECK_THROW(to<double>("0x12", "UT"), std::runtime_error);
   BOOST_CHECK_CLOSE(to_with_default<double>("abc", 42),  42, 0.00001);
   BOOST_CHECK_CLOSE(to_with_default<double>("12h", 42),  42, 0.00001);
   BOOST_CHECK_CLOSE(to_with_default<double>("0x12", 42), 42, 0.00001);

   BOOST_CHECK(ossim::isnan(to<double>("nan", "UT")));
}
