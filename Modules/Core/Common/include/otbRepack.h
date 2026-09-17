/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbRepack_h
#define otbRepack_h

#include <cassert>
#include <tuple>
#include <type_traits>
#include <array>

// Forwarded types
namespace itk
{
template <typename T, unsigned L> class FixedArray;
template <typename T, unsigned L> class Point;
} // itk namespace

template <typename T, unsigned L> class vnl_vector_fixed;

namespace otb
{

/**
 * \ingroup OTBCommon
 * \name Metaprog utilities to repack arrays and numbers into arrays */
//@{

namespace details
{
/*===============================[ size traits ]=============================*/
// Will tell if the size of an array is known at compile time
template <typename T, typename = void>
struct is_size_known1: std::is_arithmetic<T> {};

template <template <typename, unsigned> class T, typename R, unsigned N>
struct is_size_known1<T<R,N>,
         std::enable_if_t<std::is_base_of<itk::FixedArray<R,N>, T<R,N>>::value>
> : std::true_type{};

template <template <typename, unsigned> class T, typename R, unsigned N>
struct is_size_known1<T<R,N>,
         std::enable_if_t<std::is_base_of<vnl_vector_fixed<R,N>, T<R,N>>::value>
> : std::true_type{};

template <template <typename, std::size_t> class T, typename R, std::size_t N>
struct is_size_known1<T<R,N>,
         std::enable_if_t<std::is_base_of<std::array<R,N>, T<R,N>>::value>
> : std::true_type{};

template <typename R, unsigned N>
struct is_size_known1<R (&)[N]> : std::true_type{};
template <typename R, unsigned N>
struct is_size_known1<R [N]> : std::true_type{};

/**
 * Tells whether size of all types in the list is known at compile time.
 * Implemented through usual LISP <head, Tail...> recursion.
 *
 * \tparam T... List of types (arrays, and scalars)
 * \see C++20 `std::is_bounded_array`
 */
template <typename H, typename... Tail>
struct is_size_known_type
: std::integral_constant<bool,
  is_size_known1<std::remove_cv_t<H>>::value
  && is_size_known_type<Tail...>::value>
{};

template <typename H>
struct is_size_known_type<H>
: is_size_known1<std::remove_cv_t<H>>
{};

/**
 * Helper constexpr variable that tells whether the size of all parameters is
 * known at compile-time..
 * \note This traits can be used in all contexts unlike `is_size_known()`.
 * \tparam T... List of types (arrays, and scalars)
 * \param[in]   unused List of arrays and scalars
 *
 * \return whether the sum size is known at compile time
 * \throw None
 */
template <typename... Inputs>
constexpr bool is_size_known_v = is_size_known_type<Inputs...>::value;

/**
 * Helper constexpr function to return if the size of all parameters is known
 * at compile-time..
 * \note This function cannot be used in all contexts, like from another
 * function. Prefer the trait `otb::is_size_known_v` instead.
 * \tparam T... List of types (arrays, and scalars)
 * \param[in]   unused List of arrays and scalars
 *
 * \return whether the sum size is known at compile time
 * \throw None
 */
template <typename... T>
constexpr bool is_size_known(T const&...) noexcept
{ return is_size_known_v<T...>; }

/*===============================[ size traits ]=============================*/
// Will tell the size known at compile time
template <typename T, typename = void>
struct known_size1
: std::integral_constant<std::size_t, 1U> {};

template <template <typename, unsigned> class T, typename R, unsigned N>
struct known_size1<T<R,N>,
                   std::enable_if_t<std::is_base_of<itk::FixedArray<R,N>, T<R,N>>::value>
> : std::integral_constant<std::size_t, N> {};

template <template <typename, unsigned> class T, typename R, unsigned N>
struct known_size1<T<R,N>,
                   std::enable_if_t<std::is_base_of<vnl_vector_fixed<R,N>, T<R,N>>::value>
> : std::integral_constant<std::size_t, N> {};

template <template <typename, std::size_t> class T, typename R, std::size_t N>
struct known_size1<T<R,N>,
  std::enable_if_t<std::is_base_of<std::array<R,N>, T<R,N>>::value>
> : std::integral_constant<std::size_t, N> {};

template <typename R, unsigned N>
struct known_size1<R (&)[N]>
: std::integral_constant<std::size_t, N> {};
template <typename R, unsigned N>
struct known_size1<R [N]>
: std::integral_constant<std::size_t, N> {};

/**
 * Tells the cumulated size of all types in the list.
 * Implemented through usual LISP <head, Tail...> recursion.
 *
 * \tparam T... List of types (arrays, and scalars)
 * \see C++20 `std::is_bounded_array`
 * \pre requires the size of all types to be known at compile-time
 */
template <typename H, typename... Tail>
struct known_size_type
: std::integral_constant<std::size_t,
  known_size1<std::remove_cv_t<H>>::value + known_size_type<Tail...>::value>
{};

template <typename H>
struct known_size_type<H>
: known_size1<std::remove_cv_t<H>>
{};

/**
 * Helper constexpr variable that tells the cumulated size of all parameters.
 * \note This traits can be used in all contexts unlike `cumulated_size()`.
 * \tparam T... List of types (arrays, and scalars)
 * \param[in]   unused List of arrays and scalars
 *
 * \see C++20 `std::is_bounded_array_v`
 * \pre requires the size of all types to be known at compile-time
 */
template <typename... Inputs>
constexpr auto known_size_v = known_size_type<Inputs...>::value;

/*===============================[ size functions ]==========================*/
constexpr unsigned cumulated_size() noexcept { return 0U; }

// Overload for ITK static array type
template <typename Real, unsigned L>
constexpr auto size_one_input(itk::FixedArray<Real, L> const&) noexcept
{ return L; }

// Overload for vnl static array type
template <typename Real, unsigned L>
constexpr auto size_one_input(vnl_vector_fixed<Real, L> const&) noexcept
{ return L; }

template <typename Real, unsigned L>
constexpr auto size_one_input(itk::Point<Real, L> const&) noexcept
{ return L; }

template <typename Input>
inline auto size_one_input(Input const& head_input)
-> decltype(head_input.Size()) // ITK 4 is not noexcept...
{
  return head_input.Size();
}

// Overload for all std array types (static or not)
template <typename Input>
constexpr auto size_one_input(Input const& head_input) noexcept
-> decltype(head_input.size())
{
  return head_input.size();
}

// Overload for C static array type
template <typename Real, unsigned L>
constexpr auto size_one_input(Real const (&)[L]) noexcept
{
  return L;
}

// Overload for numbers
// TODO; need complex
template <typename Input, class = std::enable_if_t<std::is_arithmetic<Input>::value>>
constexpr unsigned size_one_input(Input const&) noexcept
{
  return 1U;
}

/**
 * Main function to extract the cumulated size of a series of arrays and
 * numbers.
 * The functions tries to return a `constexpr` result, but there is no
 * guarantee to it dependending on the context. For a guaranteed `constexpr`
 * value, use `known_size_v` instead.
 * \tparam Inputs  List of arrays and numbers types
 * \param[in] inputs  List of arrays and numbers types
 *
 * \return the cumulated size of the arrays + one per number
 * \throw None, normally
 */
template <typename Input, typename... Inputs>
constexpr unsigned cumulated_size(Input const& head_input, Inputs const& ...inputs)
{
  return size_one_input(head_input) + cumulated_size(inputs...);
}

#if 0
/*===============================[ Detect size type ]========================*/
template <typename T>
struct is_size_t_indexed : std::false_type {};

template <template <typename, std::size_t> class T, typename U, std::size_t N>
struct is_size_t_indexed<T<U, N>> : std::true_type {};

template <typename T>
struct is_unsigned_indexed : std::false_type {};

template <template <typename, unsigned> class T, typename U, unsigned N>
struct is_unsigned_indexed<T<U, N>> : std::true_type {};
#endif

#if 0
template <typename T>
struct is_indexed : std::false_type {};

template <typename Int, template <typename, Int> class T, typename U, Int N>
struct is_indexed<T<U, N>> : std::true_type {};

static_assert(is_indexed<std::array<int, 42>>::value);
static_assert(is_indexed<vnl_vector_fixed<int, 42>>::value);
#endif


/*===============================[ Common underlying type ]==================*/
// Remove extents from all types, and apply std::common_type
template <typename T, typename = void>
struct common_underlying_type1
: std::remove_extent<T> {};

// template <typename R, unsigned N>
  // struct is_size_known1<itk::FixedArray<R,N>> : std::true_type{};

template <template <typename, unsigned> class T, typename R, unsigned N>
struct common_underlying_type1<T<R,N>,
  std::enable_if_t<std::is_base_of<itk::FixedArray<R,N>, T<R,N>>::value>
>
{ using type = R; };

template <template <typename, unsigned> class T, typename R, unsigned N>
struct common_underlying_type1<T<R,N>,
  std::enable_if_t<std::is_base_of<vnl_vector_fixed<R,N>, T<R,N>>::value>
>
{ using type = R; };

template <template <typename, std::size_t> class T, typename R, std::size_t N>
struct common_underlying_type1<T<R,N>,
  std::enable_if_t<std::is_base_of<std::array<R,N>, T<R,N>>::value>
>
{ using type = R; };

/**
 * Trait that extracts the common type between numbers and the underlying types
 * of arrays.
 * Uses `std::common_type_t` eventually between all the underlying types.
 * \tparam T... List of types (arrays, and scalars)
 */
template <typename H, typename... Tail>
struct common_underlying_type
: std::common_type<typename common_underlying_type1<std::remove_cv_t<H>>::type,
                   typename common_underlying_type<Tail...>::type>
{};

template <typename H>
struct common_underlying_type<H>
: common_underlying_type1<std::remove_cv_t<H>>
{};

/**
 * Helper trait that extracts the common type between numbers and the
 * underlying types of arrays.
 * Uses `std::common_type_t` eventually between all the underlying types.
 * \tparam Types... List of types (arrays, and scalars)
 */
template <typename... Types>
using common_underlying_t = typename common_underlying_type<Types...>::type;

/*===============================[ Extract Array Kind ]======================*/
// Helper trait for ITK arrays (unsigned!)
template <template <typename, unsigned> class T>
struct array_generator_4_from_itk_array
{
  using type = T<int,1>;
  template <typename RR, std::size_t NN, typename Fallback>
  using gen_type = T<RR,NN>;
};

// Helper trait for std arrays (size_t!)
template <template <typename, std::size_t> class T>
struct array_generator_4_from_std_array
{
  using type = T<int,1>;
  template <typename RR, std::size_t NN, typename Fallback>
  using gen_type = T<RR,NN>;
};

// Helper trait when array type cannot be deduced
// Conventions used:
// - T==bool when automatic resolution is not possible (e.g. itk::FixedArray +
// std::array)
// - T==void when there is no array in the "inputs"
template <typename T = void>
struct array_generator_4_from_default
{
  using type = T;
  template <typename RR, std::size_t NN, typename Fallback>
  using gen_type = Fallback;
};

// Default case for numbers
template <typename T, typename = void>
struct common_array1 : array_generator_4_from_default<> {};

// Case of ITK arrays
template <template <typename, unsigned> class T, typename R, unsigned N>
struct common_array1<T<R,N>,
  std::enable_if_t<std::is_base_of<itk::FixedArray<R,N>, T<R,N>>::value>
> : array_generator_4_from_itk_array<T> {};

// Case of ITK arrays
template <template <typename, unsigned> class T, typename R, unsigned N>
struct common_array1<T<R,N>,
  std::enable_if_t<std::is_base_of<vnl_vector_fixed<R,N>, T<R,N>>::value>
> : array_generator_4_from_itk_array<T> {};

// Case of std static arrays
template <template <typename, std::size_t> class T, typename R, std::size_t N>
struct common_array1<T<R,N>,
  std::enable_if_t<std::is_base_of<std::array<R,N>, T<R,N>>::value>
> : array_generator_4_from_std_array<std::array> {};

// In this case, automatic resolution is impossible
// We need to use default!
// Let's use "bool" for "cannot be resolved", and "void" for "this is a number
// compatible with any array type"
template <typename R1, typename R2>
struct merge_common_array
: array_generator_4_from_default<bool> {};

#if __cplusplus < 201703L
#if defined(__clang_major__) && __clang_major__ >= 19
// In C++ 14, we require clang < v19
// Either downgrade clang version to at most clang 18, or compile in C++17 at least
#  error otbRepack cannot compile with clang++ 19+ in C++14
#endif

template <template <typename, std::size_t> class T>
struct merge_common_array<T<int, 1>, T<int,1>>
: array_generator_4_from_std_array<T> {};

template <template <typename, std::size_t> class T>
struct merge_common_array<T<int, 1>, void>
: array_generator_4_from_std_array<T> {};

template <template <typename, std::size_t> class T>
struct merge_common_array<void, T<int, 1>>
: array_generator_4_from_std_array<T> {};

template <template <typename, unsigned> class T>
struct merge_common_array<T<int, 1>, T<int,1>>
: array_generator_4_from_itk_array<T> {};

template <template <typename, unsigned> class T>
struct merge_common_array<T<int, 1>, void>
: array_generator_4_from_itk_array<T> {};

template <template <typename, unsigned> class T>
struct merge_common_array<void, T<int, 1>>
: array_generator_4_from_itk_array<T> {};

#else

// C++ 17 code!
template <typename Int, template <typename, Int> class T>
struct array_generator_4_from_static_array
{
  using type = T<int,1>;
  template <typename RR, std::size_t NN, typename Fallback>
  using gen_type = T<RR,NN>;
};

template <typename Int, template <typename, Int> class T, Int N, Int M>
struct merge_common_array<T<int, N>, T<int,M>>
: array_generator_4_from_static_array<Int, T> {};

template <typename Int, template <typename, Int> class T, Int N>
struct merge_common_array<T<int, N>, void>
: array_generator_4_from_static_array<Int, T> {};

template <typename Int, template <typename, Int> class T, Int N>
struct merge_common_array<void, T<int, N>>
: array_generator_4_from_static_array<Int, T> {};

#endif

template <>
struct merge_common_array<void, void>
: array_generator_4_from_default<> {};

/**
 * Generative traits that provides a builder typedef for generating a static
 * array type of the same kind (`itk::FixedArray`, `std::array`,
 * `vnl_vector_fixed`...) static array in the input types.
 *
 * \tparam Types... List of types (arrays, and scalars)
 * \return a generic `gen_type` typedef
 * \pre All types should be statically bound arrays, or numbers.
 */
template <typename H, typename... Tail>
struct common_array
: merge_common_array<typename common_array1<std::remove_cv_t<H>>::type,
                     typename common_array<Tail...>::type>
{};

template <typename H>
struct common_array<H>
: common_array1<std::remove_cv_t<H>>
{};

template <typename... Types>
using common_array_t = typename common_array<Types...>::type;

/**
 * Generative traits that provides a builder typedef for generating a static
 * array type of the same kind (`itk::FixedArray`, `std::array`,
 * `vnl_vector_fixed`...) of all the static array in the input types.
 *
 * \tparam RR       Numeric type of the elements for  the auto-deduced array
 *                  (`double`, `float`...)
 * \tparam NN       Size for the auto-deduced array
 * \tparam Fallback Fallback array type if sum array type cannot be deduced
 * \tparam Types... List of types (arrays, and scalars)
 * \return a generic `gen_type` typedef
 * \pre All types should be statically bound arrays, or numbers.
 */
template <typename RR, std::size_t NN, typename Fallback, typename... Types>
using common_array_g = typename common_array<Types...>::template gen_type<RR,NN,Fallback>;

/*===============================[ Repacking functions ]=====================*/

// function for all array types
template <typename OutArray, typename Input>
inline auto repack_input_array(
    OutArray& output, unsigned out_idx,
    Input const& head_input, unsigned N)
{
  // assert(out_idx+N <= output.Size());

  for (unsigned i = 0U; i < N ; ++i) {
    // assert(out_idx < output.Size());
    output[out_idx++] = head_input[i];
  }
  return out_idx;
}

// overload for array like type
template <typename OutArray, typename Input, class = std::enable_if_t<!std::is_arithmetic<Input>::value>>
inline auto repack_one_input(
    OutArray& output, unsigned out_idx,
    Input const& head_input)
{
  return repack_input_array(output, out_idx, head_input, size_one_input(head_input));
}

// overload for numbers
// TODO: need complex
template <typename OutArray, typename Input, class = std::enable_if_t<std::is_arithmetic<Input>::value>>
inline unsigned repack_one_input(
    OutArray& output, unsigned out_idx,
    Input const& head_input)
{
  // assert(out_idx < output.Size());
  output[out_idx++] = head_input;
  return out_idx;
}

// overload for terminal case
template <typename OutArray>
inline void repack_internal(OutArray& output, unsigned out_idx)
{
  std::ignore = output;
  std::ignore = out_idx;
  // assert(out_idx <= output.Size());
}

// main entry point that splits head and tail...
template <typename OutArray, typename Input, typename... Inputs>
inline void repack_internal(
    OutArray& output, unsigned out_idx,
    Input const& head_input, Inputs const& ...inputs)
{
  out_idx = repack_one_input(output, out_idx, head_input);
  repack_internal(output, out_idx, inputs...);
}

/*===============================[ Dispatching static VS dynamic arrays ]====*/
// specialization for statically bound arrays
template <typename OutArray>
auto make_array(std::true_type const&, unsigned)
{ return OutArray{}; }

// specialization for unbound/dynamic arrays
template <typename OutArray>
auto make_array(std::false_type const&, unsigned N)
{ return OutArray(N); }


// The dispatching function
template <typename OutArray, typename... Inputs>
inline OutArray repack(
    std::true_type /*is_size_static*/,
    Inputs const& ...inputs)
{
  auto output = make_array<OutArray>(
      is_size_known_type<OutArray>{},
      cumulated_size(inputs...));

  using Output_type = decltype(output);
  static_assert(! (is_size_known_v<Output_type> && is_size_known_v<Inputs...>)
      ||          (known_size_v<Output_type> >= known_size_v<Inputs...>),
      "Destination not big enough");
  details::repack_internal(output, 0, inputs...);
  return output;
}


}  // ===============================================[ namespace (otb::)details

/**
 * Helper function to repackage numbers or arrays into bigger arrays.
 * Function to help us work around the absence of aggregate initialization, or
 * initializer-list enabled constructors in ITK 4.x array types.
 *
 * Instead of
 * \code
 * itk::Point<double, 2> lat_lon = ....;
 * itk::Point<double, 3> lat_lon_hgt;
 * lat_lon_hgt[0] = lat_lon[0];
 * lat_lon_hgt[1] = lat_lon[1]; // no risk of mismatching indices after copy-paste
 * lat_lon_hgt[0] = z;
 * \endcode
 *
 * we can directly write
 * \code
 * itk::Point<double, 2> lat_lon = ....;
 * auto  lat_lon_hgt = repack<itk::Point<double, 3>>(lat_lon, z);
 * \endcode
 *
 * \tparam OutArray  Expected to be an ITK like array with a capital `Size()`
 *                   function.
 * \tparam Inputs    List of numbers or array that follow either ITK or C++
 *                   standard naming conventions
 * \param[in] inputs  list of numbers or arrays smaller than the output type
 *
 * \return A new array of the type specified with all inputs repacked
 * \throw None
 * \pre the size of the flattened inputs shall be < to the size of the output
 * \note the code generated will be as efficient as the manual assignment.
 */
template <typename OutArray, typename... Inputs>
inline OutArray repack(Inputs const&... inputs)
{
  return details::repack<OutArray>(std::true_type{}, inputs...);
}

/**
 * Helper function to repackage numbers or statically bound arrays into bigger
 * statically bound arrays.
 * Function to help us work around the absence of aggregate initialization, or
 * intializer-list enabled constructors in ITK 4.x array types.
 *
 * Instead of
 * \code
 * itk::Point<double, 2> lat_lon = ....;
 * itk::Point<double, 3> lat_lon_hgt;
 * lat_lon_hgt[0] = lat_lon[0];
 * lat_lon_hgt[1] = lat_lon[1]; // no risk of mismatching indices after copy-paste
 * lat_lon_hgt[0] = z;
 * \endcode
 *
 * we can directly write
 * \code
 * itk::Point<double, 2> lat_lon = ....;
 * auto  lat_lon_hgt = auto_repack(lat_lon, z);
 * \endcode
 * Unlike `repack()` where we must explicitly tell the destination array type,
 * this function will try to automagically deduce it.
 *
 * If all arrays types used as parameters are of the same type, this type will
 * be used, but with:
 * - the type of underlying elements defined from the common type of all inputs
 * - the size of the output array automatically deduced from the inputs.
 *
 * If mixed array types are used (e.g `itk::FixedArray` and `std::array`), then
 * the fallback type (`itk::FixedArray` by default) will be used.
 *
 * \tparam OutArray  Fallback output array type in case all `inputs` are not of
 *                   the same type.
 * \tparam Inputs    List of numbers or array that follow either ITK or C++
 *                   standard naming conventions
 * \param[in] inputs List of numbers or statically bound arrays
 *
 * \return A new array with all inputs repacked
 * \pre all input arrays shall be statically bound at compile-time.
 * \throw None, normally
 */
template <template <typename, unsigned> class OutArray = itk::FixedArray, typename... Inputs>
inline auto auto_repack(Inputs const&... inputs)
{
  static_assert(details::is_size_known_type<Inputs...>::value, "Cannot repack dynamics sized inputs");
  using T = details::common_underlying_t<Inputs...>;
  static_assert(std::is_arithmetic<T>::value, "Underlying type shall be a numeric type");
  constexpr auto N    = details::known_size_v<Inputs...>;
  using OutArray_type = details::common_array_g<T, N, OutArray<T, N>, Inputs...>;
  return details::repack<OutArray_type>(std::true_type{}, inputs...);
}

//@}

} // otb namespace

#endif // otbRepack_h
