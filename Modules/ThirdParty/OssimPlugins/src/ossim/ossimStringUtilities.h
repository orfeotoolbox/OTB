//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$
#ifndef ossimStringUtilities_h
#define ossimStringUtilities_h

#include "ossimPluginConstants.h"

#ifdef nan
#  undef nan
#endif

#if defined(__GNUC__) || defined(__clang__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
// #  pragma GCC diagnostic ignored "-Woverloaded-virtual"
// #  pragma GCC diagnostic ignored "-Wshadow"
#endif
#include <ossim/base/ossimCommon.h> // nan
#include <ossim/base/ossimString.h>
#if defined(__GNUC__) || defined(__clang__)
#  pragma GCC diagnostic pop
#endif

#include <algorithm>
#include <iterator>
#include <cassert>
#include <cstring>
#include <ostream>
// #include <iostream>
#include <sstream>
#include <typeinfo>
#include <stdexcept>
#include <cctype>
#include <cstdarg>
#include <string>

namespace ossimplugins {

namespace details {
   /** Predicate to test whether two characters are equals, independently of
    * their case.
    */
   struct i_equal {
      bool operator()(char lhs, char rhs) const {
         return std::tolower(lhs) == std::tolower(rhs);
      }
   };
}

/**\name String view + functions */
//@{
/** String view class.
 * Class inspired by C++17 `std::string_view` and `gsl::string_span`.
 *
 * This idea is to bring efficiency and correctness to C++ functions that take
 * strings. A function that receives a `string_view` can receive any kind of
 * string flavour and guarantee 0-copy.
 *
 * The only side effect to be aware of: no terminal-0 is to be expected. i.e. a
 * `string_view` data cannot be passed freely to a function with a C API that
 * expect a null-terminated string.
 *
 * @invariant there is no guarantee `data()[size()] == '\0'`. It may happen
 * though depending on the constructor used.
 */
struct string_view
{
   typedef char const                         value_type;
   typedef char const&                        reference;
   typedef char const&                        const_reference;
   typedef char const*                        const_pointer;
   typedef char const*                        pointer;
   typedef std::size_t                        size_type;

private:
   /** Strict view iterator.*/
   struct char_iterator
   {
      typedef char const                         value_type;
      typedef char const&                        reference;
      typedef char const*                        pointer;
      typedef std::size_t                        size_type;
      typedef std::ptrdiff_t                     difference_type;
      typedef std::random_access_iterator_tag    iterator_category;

      explicit char_iterator(const_pointer p) : m_ptr(p) {}
      char_iterator & operator++() { ++m_ptr; return *this; }
      char_iterator   operator++(int) { char_iterator tmp(*this); ++m_ptr; return tmp; }
      char_iterator & operator--() { --m_ptr; return *this; }
      char_iterator   operator--(int) { char_iterator tmp(*this); --m_ptr; return tmp; }
      const_reference operator*() const { return *m_ptr; }

      char_iterator & operator+=(difference_type off) { m_ptr += off; return *this; }
      char_iterator & operator-=(difference_type off) { m_ptr -= off; return *this; }

      friend char_iterator operator+(char_iterator lhs, difference_type off) { return lhs += off; }
      friend char_iterator operator+(difference_type off, char_iterator rhs) { return rhs += off; }
      friend char_iterator operator-(char_iterator lhs, difference_type off) { return lhs -= off; }
      friend char_iterator operator-(difference_type off, char_iterator rhs) { return rhs -= off; }

      friend difference_type operator-(char_iterator lhs, char_iterator rhs) { return lhs.m_ptr - rhs.m_ptr; }

      friend bool operator==(char_iterator lhs, char_iterator rhs) {
         return lhs.m_ptr == rhs.m_ptr;
      }
      friend bool operator!=(char_iterator lhs, char_iterator rhs) {
         return lhs.m_ptr != rhs.m_ptr;
      }
      friend bool operator<(char_iterator lhs, char_iterator rhs) {
         return lhs.m_ptr < rhs.m_ptr;
      }
   private:
      friend bool operator<=(char_iterator lhs, char_iterator rhs) {
         return lhs.m_ptr <= rhs.m_ptr;
      }
   private:
      const_pointer m_ptr;
   };

public:
   // typedef char_iterator                        iterator;
   typedef char_iterator                        const_iterator;
   // typedef std::reverse_iterator<char_iterator> reverse_iterator;
   typedef std::reverse_iterator<char_iterator> const_reverse_iterator;

   /** Construtor from a standard string.
    *@post `data()[size()] == '\0'`
    */
   string_view(std::string const& s)
      : m_size(s.size()), m_first(s.c_str())
      {}
   /** Construtor from an ossim string.
    *@post `data()[size()] == '\0'`
    */
   string_view(ossimString const& s)
       : m_size(s.size()), m_first(s.c_str())
       {}
   /** Construtor from an array of characters.
    * @post `data()[size()] == '\0'` <= the array is built with this kind of
    * syntax: `char const a[]="foobar";`
    */
   template <std::size_t N>
   string_view(char const (&array)[N])
      : m_size(N), m_first(&array[0])
      {}
   /** Construtor from a pointer to a 0-terminated raw string.
    * @post `data()[size()] == '\0'`
    */
   string_view(char const* ptr)
      : m_size(std::strlen(ptr)), m_first(ptr)
      {}
#if 0
   string_view(char const* first, char const* last)
      : m_size(last-first), m_first(first)
      {
         assert(first <= last);
      }
#endif
   /** Construtor from an input iterator and a size.
    * @post there is no guarantee `data()[size()] == '\0'`.
    */
   template <typename II>
   string_view(II first, size_type size_)
      : m_size(size_), m_first(first)
      {}

   /** Construtor from a pair of input interators.
    * @post there is no guarantee `data()[size()] == '\0'`.
    */
   template <typename II>
   string_view(II first, II last)
      : m_size(last-first), m_first(first)
      {
         assert(first <= last);
      }

   /** Builds a peculiar empty view to points to nullptr. */
   static string_view null() { return string_view(build_empty());}

   /** Tells whether a view is within another view.
    * This functions doesn't test whether the characters from a view are found
    * in the same relative positions in another view.
    */
   bool belongs_to(char const* first, char const* last) const {
      return first <= &*begin() && &*end() <= last;
   }
   template <std::size_t N>
   bool belongs_to(char const (&array)[N]) const {
      // N-1 because of terminal 0
      return belongs_to(&array[0], &array[N-1]);
   }
   bool belongs_to(char const* ptr) const {
      return belongs_to(&ptr[0], &ptr[std::strlen(ptr)]);
   }
   template <typename String>
   bool belongs_to(String const& s) const{
      return belongs_to(&*s.begin(), &*s.end());
   }

   size_type               size   () const { return m_size; }
   bool                    empty  () const { return m_size == 0; }

   // iterator                begin  ()       { return iterator(m_first); }
   // iterator                end    ()       { return iterator(m_first+m_size); }
   const_iterator          begin  () const { return m_first; }
   const_iterator          end    () const { return m_first+m_size; }
   const_iterator          cbegin () const { return m_first; }
   const_iterator          cend   () const { return m_first+m_size; }

   // reverse_iterator        rbegin ()       { return reverse_iterator(m_first+m_size); }
   // reverse_iterator        rend   ()       { return reverse_iterator(m_first); }
   const_reverse_iterator  rbegin () const { return const_reverse_iterator(m_first+m_size); }
   const_reverse_iterator  rend   () const { return const_reverse_iterator(m_first); }
   const_reverse_iterator  crbegin() const { return const_reverse_iterator(m_first+m_size); }
   const_reverse_iterator  crend  () const { return const_reverse_iterator(m_first); }

   /** Indexed access.
    * @pre `p < size(),` checked with an assertion.
    */
   const_reference operator[](size_type p) const {
      assert(p < size());
      return *(m_first+p);
   }

   /** Returns first character of the view.
    * @pre `!empty()`, checked with an assertion.
    */
   const_reference front() const { assert(!empty()); return *begin(); }
   /** Returns last character of the view.
    * @pre `!empty()`, checked with an assertion.
    */
   const_reference back () const { assert(!empty()); return *(end()-1); }

   /** Returns a pointer to the first character of the view.
    * @warning there is no guarantee `data()[size()] == '\0'`
    */
   const_pointer   data () const { return &*m_first; }

   /** Removes the n first characters.
    * @pre `n <= size()`, checked with an assertion
    * @post size is n characters more little
    * @post `begin()` iterator is n characters right.
    */
   void remove_prefix(std::size_t n) { assert(n <= m_size); m_first += n; m_size -= n; }
   /** Removes the n last characters.
    * @pre `n <= size()`, checked with an assertion
    * @post size is n characters more little
    * @post `end()` iterator is n characters left.
    */
   void remove_suffix(std::size_t n) { assert(n <= m_size); m_size -= n; }
private:
   struct build_empty{};
   string_view(build_empty) : m_size(0), m_first(NULL) {}

   std::size_t    m_size;
   const_iterator m_first;
};

inline
bool operator==(string_view const& lhs, string_view const& rhs)
{
   return lhs.size() == rhs.size()
      && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}
inline
bool operator!=(string_view const& lhs, string_view const& rhs)
{
   return ! (lhs == rhs);
}

inline
std::ostream & operator<<(std::ostream & os, const string_view & v)
{
   std::copy(v.begin(), v.end(), std::ostream_iterator<char>(os));
   return os ;
}

inline
std::string operator+(string_view const& lhs, string_view const& rhs)
{
   std::string res(lhs.begin(), lhs.end());
   res.append(rhs.begin(), rhs.end());
   return res;
}

/** Tells whether two views are exactly identical. */
inline
bool is_same_view(string_view const& lhs, string_view const& rhs)
{
   return lhs.begin() == rhs.begin() && lhs.size() == rhs.size();
}

/** Tells whether the haystack string starts with the needle string. */
inline
bool starts_with(string_view const& haystack, string_view const& needle)
{
   return needle.size() <= haystack.size()
      && std::equal(needle.begin(), needle.end(), haystack.begin());
}

/** Tells whether the haystack string starts with the needle string. */
inline
bool ends_with(string_view const& haystack, string_view const& needle)
{
   return needle.size() <= haystack.size()
      && std::equal(needle.rbegin(), needle.rend(), haystack.rbegin());
}

/** Tels whether two strings are equal, independently of their case. */
inline
bool iequal(string_view const& lhs, string_view const& rhs)
{
   return lhs.size() == rhs.size()
      && std::equal(lhs.begin(), lhs.end(), rhs.begin(), details::i_equal());
}

/** Searches needle string within the haystack string. */
inline
string_view find(string_view const& haystack, string_view const& needle)
{
   assert(!haystack.empty());
   assert(!needle.empty());
   const string_view::size_type size = needle.size();
   if (haystack.size() < size) {
      // std::cout << "haystack size < needle size!!!";
      return string_view::null();
   }
   string_view::const_iterator const end = haystack.end() - size +1;
   // std::cout << "comp up to:"
         // << static_cast<void const*>(&*end) << "(" << (*end) << ")"
         // ;
   string_view::const_iterator       it  = haystack.begin();
   for ( ; it != end ; ++it) {
      string_view v(it, size);
      assert(v.size() == size);
      assert(v.belongs_to(haystack));
      // std::cout << "  check "
         // << static_cast<void const*>(v.data())<<"["<<v.size()<<"]  VS "
         // << static_cast<void const*>(needle.data())<<"["<<needle.size()<<"]"
         // ;
      if (v == needle) {
         return v;
      }
   }
   assert(it+size-1 == haystack.end());
   return string_view::null();
}

/** Tells whether the haystack string contains the needle string. */
inline
bool contains(string_view const& haystack, string_view const& needle)
{
   return ! is_same_view(find(haystack, needle), string_view::null());
}

/**
 * Efficient to-lower string transformation.
 * This version copies the input string in lower case into the destination
 * char-array.
 * \param[in]  v    input string
 * \param[out] dest output destination
 *
 * \pre If the input string is longer than the output destination, extraneous
 * characters will be trimmed.
 * \post No terminal 0 will be added.
 * \return a `string_view` around the `dest` variable, but restricted to the
 * actual string produced.
 * \throw None
 */
template <std::size_t N> inline string_view to_lower_to(string_view const& v, char (&dest)[N])
{
   std::size_t i=0;
   std::size_t n = std::min(N, v.size());
   for (; i!=n && v[i] != '\0' ; ++i)
   {
      dest[i] = std::tolower(v[i]);
   }
   // dest[std::min(i, N-1)] = '\0'; // ensure a terminal-0
   return string_view(&dest[0], i);
}

/**
 * Efficient to-upper string transformation.
 * This version copies the input string in upper case into the destination
 * char-array.
 * \param[in]  v    input string
 * \param[out] dest output destination
 *
 * \pre If the input string is longer than the output destination, extraneous
 * characters will be trimmed.
 * \post No terminal 0 will be added.
 * \return a `string_view` around the `dest` variable, but restricted to the
 * actual string produced.
 * \throw None
 */
template <std::size_t N> inline string_view to_upper_to(string_view const& v, char (&dest)[N])
{
   std::size_t i=0;
   std::size_t n = std::min(N, v.size());
   for (; i!=n && v[i] != '\0' ; ++i)
   {
      dest[i] = std::toupper(v[i]);
   }
   // dest[std::min(i, N-1)] = '\0'; // ensure a terminal-0
   return string_view(&dest[0], i);
}

//@}

/**\name Splitting functions */
//@{

/** Split parts iterator.
 * Not meant to be used directly.
 * @tparam Splitter split algorithm used.
 * @see `part_range`
 * @see `split_on()`
 */
template <typename Splitter>
struct part_iterator
{
   typedef string_view &                      reference;
   typedef string_view const&                 const_reference;
   typedef string_view const*                 const_pointer;
   typedef std::forward_iterator_tag    iterator_category;

   // build last
   part_iterator(string_view const& global_string, Splitter s)
      : m_global(global_string)
      , m_splitter(s)
      , m_crt(m_splitter(global_string))
      {}
   struct theend {};
   part_iterator(string_view const& global_string, Splitter s, theend)
      : m_global(global_string)
      , m_splitter(s)
      , m_crt(string_view::null())
      {}

   part_iterator & operator++() {
      assert(m_crt.belongs_to(m_global) && "Invariant check failure");
      assert(! is_same_view(m_crt, string_view::null()) && "Don't iterate past end");
      if (m_crt.end() == m_global.end()) {
         m_crt = string_view::null();
      } else {
         string_view within(m_crt.end()+m_splitter.next_start(), m_global.end());
         m_crt = m_splitter(within);
      }
      return *this;
   }
   part_iterator operator++(int) {
      part_iterator tmp(*this);
      ++(*this);
      return *this;
   }
   const_reference operator*() const {
      return m_crt;
   }
   const_pointer operator->() const {
      return &m_crt;
   }

   friend bool operator==(part_iterator const& lhs, part_iterator const& rhs) {
      // std::cout << "<<"<<lhs<<">> ==?== <<"<<rhs<<">>" << std::endl;
      assert(is_same_view(lhs.m_global, rhs.m_global));
      return is_same_view(lhs.m_crt, rhs.m_crt); // TODO: too many tests
   }
   friend bool operator!=(part_iterator const& lhs, part_iterator const& rhs) {
      return ! (lhs == rhs);
   }
#if defined(BOOST_TEST_MODULE)
   friend std::ostream & operator<<(std::ostream & os, const part_iterator & v)
   {
      return os << "split iterator on `" << v.m_crt
         << "'("<<static_cast<void const*>(&*v.m_crt.begin()) <<"-"<<v.m_crt.size()<<")"<<", within `"
         << v.m_global << "'";
   }
#endif
private:
   string_view m_global;
   Splitter    m_splitter;
   string_view m_crt;
};

namespace details {
   inline std::size_t len(char) { return 1; }
   template <std::size_t N> inline std::size_t len(char const (&)[N])
   { assert(N>0); return N-1; }
   inline std::size_t len(char const* ptr) { return std::strlen(ptr); }
   template <typename String> inline std::size_t len(String const& s) { return s.length(); }
} // details namespace

/** Split parts range.
 * Not meant to be used directly.
 * @tparam Splitter split algorithm used.
 * @see `part_iterator`
 * @see `split_on()`
 */
template <typename Splitter>
struct part_range
{
   part_range(string_view const& global_string, Splitter s)
      : m_first(global_string, s)
      , m_last(global_string, s, typename const_iterator::theend())
      {}
   typedef part_iterator<Splitter> const_iterator;
   const_iterator const& begin() const { return m_first; }
   const_iterator const& end  () const { return m_last; }
private:
   const_iterator const m_first;
   const_iterator const m_last;
};

/** Split algorithm that splits on single characters.
 * Not meant to be used directly
 * @see `split_on()`
 */
struct splitter_on_delim
{
   splitter_on_delim(char delim) : m_delimiter(delim) {}
   string_view operator()(string_view const& within) const {
      string_view res(within.begin(),
            std::find(within.begin(), within.end(), m_delimiter)
            );
      return res ;
   }
   std::size_t next_start() const { return details::len(m_delimiter); }
private:
   char m_delimiter;
};

/** Split entry point.
 * @tparam String Type of the string split (`ossimString`, `std::string`,
 * `string_view`, ...)
 * @param[in] str string to be split
 * @param[in] delim character delimiter used to split.
 * @return a range of the parts found in `str`.
 *
 * Example of use:
 * \code
   typedef part_range<splitter_on_delim>       range_type;
   typedef typename range_type::const_iterator const_iterator;
   const range_type parts = split_on(",abc,z,,,def,xy,,", ',')
   const_iterator it = parts.begin();
   const_iterator end = parts.end();
   for ( ; it != end ; ++it) {
       doStuff(*it);
   }
 * \endcode
 */
template <typename String>
part_range<splitter_on_delim> split_on(String const& str, char delim) {
   return part_range<splitter_on_delim>(str, splitter_on_delim(delim));
}
//@}

/**\name string to number conversion */
//@{

namespace details
{
   template <typename T> struct name {
      static char const* value() { return typeid(T).name(); }
   };

   namespace policy
   {
      template <typename T> struct throw_on_error {
         throw_on_error(string_view const& context)
            : m_context(context)
            {}
         T const& on_error(string_view const& v) const
         {
            throw std::runtime_error("Cannot decode "+v+" as "+name<T>::value()+" while " + m_context);
         }
      private:
         string_view const m_context;
      };
      template <typename T> struct default_on_error {
         default_on_error(T const& def) : m_default(def) {}
         T const& on_error(string_view const&) const
         {
            return m_default;
         }
      private:
         T const& m_default;
      };
   }// namespace policy

    /**
     * \brief Internal generic string to integer conversion.
     * Tries to convert the input string into a integer type. If the
     * string doesn't represent an integer value, `false` is returned.
     * \tparam Int Integral type (In a perfect world, we'd used `enable_if` &
     * co to restrict the code to integral types)
     * \param[in] v  input string
     * \param[out] result As its name suggest
     * \return Whether the operation is a success
     * \throw None
     * \sa `to<>()`
     * \sa `to_with_default<>()`
     * \sa `to_uinteger()`
     * \sa `to_float()`
     * \sa `to_bool()`
     */
    template <typename Int> inline bool to_integer(string_view const& v, Int & result)
    {
       // string_view::data() isn't compatible with strtol => we emulate it

       // TODO: handle HEX, OCT, BIN, locales?
       string_view::const_iterator it  = v.begin();
       string_view::const_iterator end = v.end();

       bool is_negative = false;
       Int res = 0;
       if (it != end) {
          switch (*it) {
             case '-': is_negative = true; /*[[fallthrough]]*/
             case '+': ++it;
          }
          for ( ; it != end ; ++it) {
             // only support arabic digits
             if (!std::isdigit(*it)) {
                return false;
             }
             res  = 10 * res + *it - '0';
          }
       }
       result = is_negative ? -res : res;
       return true;
    }

    /**
     * \brief Internal generic string to unsigned integer conversion.
     * Tries to convert the input string into a integer type. If the
     * string doesn't represent an integer value, `false` is returned.
     * \tparam Int Integral type (In a perfect world, we'd used `enable_if` &
     * co to restrict the code to integral types)
     * \param[in] v  input string
     * \param[out] result As its name suggest
     * \return Whether the operation is a success
     * \throw None
     * \sa `to<>()`
     * \sa `to_with_default<>()`
     * \sa `to_integer()`
     * \sa `to_float()`
     * \sa `to_bool()`
     */
    template <typename Int> inline bool to_uinteger(string_view const& v, Int & result)
    {
       // string_view::data() isn't compatible with strtol => we emulate it

       // TODO: handle HEX, OCT, BIN, locales?
       string_view::const_iterator it  = v.begin();
       string_view::const_iterator end = v.end();

       Int res = 0;
       if (it != end) {
          for ( ; it != end ; ++it) {
             // only support arabic digits
             if (!std::isdigit(*it)) {
                return false;
             }
             res  = 10 * res + *it - '0';
          }
       }
       result = res;
       return true;
    }

    /**
     * \brief Internal generic string to float conversion.
     * Tries to convert the input string into a floating point type. If the
     * string doesn't represent a floating point value, `false` is returned.
     * \tparam FloatType floating point type (`float`, `double`, `long double`)
     * \param[in] v  input string
     * \param[out] result As its name suggest. The special `"nan"` string is
     * converted to `ossim::nan()`.
     * \return Whether the operation is a success
     * \throw None
     * \sa `to<>()`
     * \sa `to_with_default<>()`
     * \sa `to_integer()`
     * \sa `to_bool()`
     */
    template <typename FloatType> inline bool to_float(string_view const& v, FloatType & result)
    {
       // std::cout << "to_float<" << name<FloatType>::value() << ">(" << v << ")\n";
       if (contains(v, "nan")) {
          result = ::ossim::nan();
          return true;
       }
       FloatType res = FloatType(); // 0-construction
       if (!v.empty()) {
          std::stringstream ss;
          if (! (ss << v && (ss >> res >> std::ws).eof()))  {
             return false;
          }
       }
       result = res;
       return true;
    }

    /**
     * \brief Internal generic string to bool conversion.
     * Tries to convert the input string into a boolean value. If the
     * string doesn't represent a boolean value, `false` is returned.
     * \tparam T expected to be bool, but ignored actually.
     * \param[in] v  input string
     * \param[out] result As its name suggest.
     * \return Whether the operation is a success
     * \throw None
     * \sa `to<>()`
     * \sa `to_with_default<>()`
     * \sa `to_integer()`
     * \sa `to_float()`
     * \internal This function is template because of the `OSSIM_GENERATE_CONV` macro.
     */
    template <typename T> inline bool to_bool(string_view const& v, bool & result)
    {
       char b[10]; // more than enough for "1", "true", "on", "yes", "0", "false", ...
       const string_view vlow = to_lower_to(v, b);
       if (v == "1" || vlow == "true" || vlow == "on" || vlow == "yes")
       {
          result = true;
          return true;
       } else if (v == "0" || vlow == "false" || vlow == "off" || vlow == "no")
       {
          result = false;
          return true;
       } else
       {
          return false;
       }
    }

    inline unsigned int decode_uint(string_view & v)
    {
        unsigned int res = 0;
        for ( ; !v.empty() ; v.remove_prefix(1))
        {
            // only support arabic digits
            if (!std::isdigit(v.front()))
                break;
            res  = 10 * res + v.front() - '0';
        }
        return res;
    }

   template <typename T> struct converter
   {
      static bool convert(string_view const& v, T & res)
      {
         // std::cout << "default converter<" << name<T>::value() << "> for " << v << "\n";
         std::stringstream ss;
         return (ss << v && (ss >> res >> std::ws).eof());
      }
   };
#define OSSIM_GENERATE_CONV(internal_to, type) \
   template <> struct name<type> { \
      static char const* value() { return #type; } \
   }; \
   template <> struct converter<type> {\
      static bool convert(string_view const& v, type & res) \
      { return internal_to<type>(v, res); } \
   };
   // Note: specialization doesn't support default arguments, but default argument
   // T() will still work.

   OSSIM_GENERATE_CONV(to_bool,     bool);
   OSSIM_GENERATE_CONV(to_integer,  char);
   OSSIM_GENERATE_CONV(to_uinteger, unsigned char);
   OSSIM_GENERATE_CONV(to_integer,  signed char);
   OSSIM_GENERATE_CONV(to_uinteger, unsigned short);
   OSSIM_GENERATE_CONV(to_integer,  short);
   OSSIM_GENERATE_CONV(to_integer,  int);
   OSSIM_GENERATE_CONV(to_uinteger, unsigned int);
   OSSIM_GENERATE_CONV(to_integer,  long);
   OSSIM_GENERATE_CONV(to_uinteger, unsigned long);
#if defined(HAS_LONG_LONG) // TODO: add this configure option
   OSSIM_GENERATE_CONV(to_integer,  long long);
   OSSIM_GENERATE_CONV(to_uinteger, unsigned long long);
#endif
   OSSIM_GENERATE_CONV(to_float,    float);
   OSSIM_GENERATE_CONV(to_float,    double);
   OSSIM_GENERATE_CONV(to_float,    long double);
#undef OSSIM_GENERATE_CONV

} // ossimplugins::details namespace

/**
 * \brief Generic string to whatever conversion -- failure means exception.
 * Tries to decode a value from a string.
 * \tparam  T Type of the value decoded from the input string
 * \param[in] v  input string to convert
 * \param[in] context  context message for the exception thrown
 *
 * \return The value decoded
 * \throw std::runtime_error if a value of type `T` cannot be decoded from the
 * string.
 * \note For efficiency reasons, the function has been specialized for integral
 * floating point, and string types.
 * \note That alternative to `ossimString::toXxxx()` function has been defined
 * to support generic programing in `get()` and `add()` helper functions for
 * keyword list.
 * \see `to_with_default()` for the version that never fails, but returns a
 * default value otherwise (being `T()` by default).
 * \warning libc++ has an incorrect implementation of `eof()` and will not
 * correctly handle the cases where the input string cannot be converted.
 */
template <typename T>
inline
T to(string_view const& v, string_view const& context)
{
   using namespace details;
   // std::cout << "to<" << name<T>::value() << ">(" << v << ")\n";
   T res;
   if (converter<T>::convert(v, res))
   {
      return res;
   }
   return policy::throw_on_error<T>(context).on_error(v);
}

/**
 * \brief Generic string to whatever conversion -- failure is hidden.
 * Tries to decode a value from a string.
 * \tparam  T Type of the value decoded from the input string
 * \param[in] v  input string to convert
 * \param[in] def  default value returned in the conversion isn't possible
 *
 * \return The value decoded
 * \return `def` if no value of type `T` can be decoded from `v`.
 * \throw None
 * \note For efficiency reasons, the function has been specialized for integral
 * floating point, and string types.
 * \note That alternative to `ossimString::toXxxx()` function has been defined
 * to support generic programing in `get()` and `add()` helper functions for
 * keyword list.
 * \see `to()` for the version that fails by throwing exceptions.
 * \warning libc++ has an incorrect implementation of `eof()` and will not
 * correctly handle the cases where the input string cannot be converted.
 */
template <typename T>
inline
T to_with_default(string_view const& v, T const& def = T())
{
   using namespace details;
   // std::cout << "to_with_default<" << name<T>::value() << ">(" << v << ")\n";
   T res;
   if (converter<T>::convert(v, res))
   {
      return res;
   }
   return policy::default_on_error<T>(def).on_error(v);
}

template <> inline std::string to<std::string>(string_view const& v, string_view const& /*context*/)
{ return std::string(v.begin(), v.end()); }

// Overload for fast conversion non to string.
// TODO: add string_view -> which ever type of string
inline std::string const& to(std::string const& v, string_view const& /*context*/) { return v; }
inline std::string const& to_with_default(std::string const& v, std::string const& /* default*/) { return v; }
inline ossimString const& to(ossimString const& v, string_view const& /*context*/) { return v; }
inline ossimString const& to_with_default(ossimString const& v, ossimString const& /* default*/) { return v; }
inline string_view const& to(string_view const& v, string_view const& /*context*/) { return v; }
inline string_view const& to_with_default(string_view const& v, string_view const& /* default*/) { return v; }

// template <> inline double to<double>(ossimplugins::string_view const& v)
// { return details::to_float<double>(v); }
//@}

OSSIM_PLUGINS_DLL int s_printf(char *str, std::size_t size, const char *format, ...);
OSSIM_PLUGINS_DLL int vs_printf(char *str, std::size_t size, const char *format, std::va_list ap);

template <std::size_t size>
inline
int s_printf(char (&str)[size], const char *format, ...) {
    std::va_list ap;
    va_start(ap, format);
    const int res = vs_printf(str, size, format, ap);
    va_end(ap);
    return res;
}


} // ossimplugins namespace


#endif // ossimStringUtilities_h
