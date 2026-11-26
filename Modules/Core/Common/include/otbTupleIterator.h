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

#ifndef otbTupleIterator_h
#define otbTupleIterator_h

#include "OTBCommonExport.h"
#include "otbMetaProgrammingLibrary.h"

#include <boost/hana.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/ext/std/tuple.hpp>
#include <tuple>
#include <type_traits>

namespace otb
{

/**
 * Kind of _zip iterator_ for iterators of different types.
 * While `otb::ZipIterator<>` works well for a list of same-type iterators, it doesn't support the
 * case where different kind of iterators are zipped together. A solution would have been to have a
 * `otb::ZipIterator<VariantIterator<TIterator...>>`.
 *
 * \tparam TIterator  List of iterator types.
 *
 * \invariant All defined iterators shall share the same region, position...
 * \todo disable Get<IteratorType> when all iterators share the same type.
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \sa `otb::TupleOptionalIterator` and `otb::ZipIterator`
 */
template <typename... TIterator>
class OTBCommon_EXPORT_TEMPLATE TupleOfIterators
{
  using RawFrontIterator = otb::FrontType<TIterator...>;
  using FrontIterator    = otb::remove_cvref_t<RawFrontIterator>;

public:
  /**
   * Construct from a list of iterators.
   * \tparam TITKIterator  ITK iterators
   * \param[in] iterator   List of iterators to track in the tuple.
   *
   * \warning Beware that parameters are forwarded, a rvalue will not be stored in a
   * `TupleIterator<Iterator&>`. Do compile with `-Wdangling-pointer` on.
   */
  template <typename... TITKIterator>
  TupleOfIterators(TITKIterator&&... iterator)
  : m_iterators(std::forward<TITKIterator>(iterator)...)
  {
    static_assert(
        sizeof...(TITKIterator) == sizeof...(TIterator),
        "Number of parameters shall match the number of iterators declared"
    );
  }

  // static_assert TIterator::ImageIteratorDimension is the same for all

  using Self = TupleOfIterators;

  static constexpr unsigned int ImageIteratorDimension = FrontIterator::ImageIteratorDimension;
  using IndexType                                      = typename FrontIterator::IndexType;
  using SizeType                                       = typename FrontIterator::SizeType;
  using OffsetType                                     = typename FrontIterator::OffsetType;
  using RegionType                                     = typename FrontIterator::RegionType;

  TupleOfIterators() = default;

  /** Get the region iterated by the iterator(s).
   * Actually, only check the first iterator defined. The others are expected to watch the same
   * region.
   */
  auto const& GetRegion() const
  {
    return std::get<0>(m_iterators).GetRegion();
  }

  /** Set the region iterated by the iterator(s). */
  void SetRegion(RegionType const& region)
  {
    auto const set_region = [&](auto const& it) { it.SetRegion(region); };
    boost::hana::for_each(m_iterators, set_region);
  }

  /** Moves the iterator(s) to the begin of the region iterated. */
  Self& GoToBegin()
  {
    boost::hana::for_each(m_iterators, [](auto& it) { it.GoToBegin(); });
    return *this;
  }
  /** Moves the iterator(s) to the end of the region iterated. */
  Self& GoToEnd()
  {
    boost::hana::for_each(m_iterators, [](auto& it) { it.GoToEnd(); });
    return *this;
  }

  /** Tells whether the iterator(s) is at the begin of the region iterated.
   * Actually, only check the first iterator defined. The others are expected to be at the same
   * position.
   */
  bool IsAtBegin() const
  {
    return std::get<0>(m_iterators).IsAtBegin();
  }

  /** Tells the iterator(s) is at the end of the region iterated.
   * Actually, only check the first iterator defined. The others are expected to be at the same
   * position.
   */
  bool IsAtEnd() const
  {
    return std::get<0>(m_iterators).IsAtEnd();
  }

  template <std::size_t Index>
  auto GetIndex() const
  {
    return std::get<0>(m_iterators).GetIndex();
  }

  /** Pre-increment the iterator(s).
   * As post-increment is less efficient, it hasn't been provided.
   */
  Self& operator++()
  {
    assert(!IsAtEnd());
    boost::hana::for_each(m_iterators, [](auto& it) { ++it; });
    return *this;
  }
  /** Removed post-increment operator(s).
   * Please use the  preincrement operator!
   */
  Self operator++(int) = delete;

  /**\name ScanLine Iterator Interface */
  //@{
  /** Moves iterator(s) to next line. */
  Self& NextLine()
  {
    boost::hana::for_each(m_iterators, [](auto& it) { it.NextLine(); });
    return *this;
  }
  /** Moves iterator(s) to the beginning of the current line. */
  Self& GoToBeginOfLine()
  {
    boost::hana::for_each(m_iterators, [](auto& it) { it.GoToBeginOfLine(); });
    return *this;
  }
  /** Moves iterator(s) to the end of the current line. */
  Self& GoToEndOfLine()
  {
    boost::hana::for_each(m_iterators, [](auto& it) { it.GoToEndOfLine(); });
    return *this;
  }
  /** Tells whether the iterator(s) is a the begin of a line.
   * Actually, only check the first iterator defined. The others are expected to be at the same
   * position.
   */
  bool IsAtBeginOfLine() const
  {
    return std::get<0>(m_iterators).IsAtBeginOfLine();
  }
  /** Tells whether the iterator(s) is a the end of a line.
   * Actually, only check the first iterator defined. The others are expected to be at the same
   * position.
   */
  bool IsAtEndOfLine() const
  {
#if 0
    return std::get<0>(m_iterators).IsAtEndOfLine();
#else
    // Const qualifier has been added to ScanLineIterator::IsAtEndOfLine in ITK
    // 5.1 => Use const_cast in the mean time...
    return const_cast<FrontIterator&>(std::get<0>(m_iterators)).IsAtEndOfLine();
#endif
  }
  //@}

  /**\name Pixel accessor Interface */
  //@{
  /**
   * Return pixel value from iterator type.
   * \tparam WhichIterator  Unique type identitying the iterator in the tuple.
   * \warning this overload cannot be used when several iterators have the same type -- which may
   *          happen when each iterator type comes from a different template parameters of the OTB
   *          filter where it's used.
   *
   * \return the exact value/reference to the underlying pixel
   * \throw std::logic_error if not iterator is set. Actually `[[unreachable]]` would be more exact
   *                         given the class invariant.
   */
  template <typename WhichIterator>
  decltype(auto) Get() const
  {
    auto&& iterator = std::get<WhichIterator>(m_iterators);
    return iterator->Get();
  }

  /**
   * Return pixel value from its index.
   * \tparam Index compiled-time used to identify the actual iterator used.
   * \note use this overload cannot be used when several iterators have the same type -- which may
   *       happen when each iterator type comes from a different template parameters of the OTB
   *       filter where it's used.
   *
   * \return the exact value/reference to the underlying pixel
   * \throw std::logic_error if not iterator is set. Actually `[[unreachable]]` would be more exact
   *                         given the class invariant.
   *
   * \note don't forget to call the function with `it.template Get<0>()` when defining (template)
   *       generic filter as the function is a _dependent template name_.
   */
  template <std::size_t Index>
  decltype(auto) Get() const
  {
    auto&& iterator = std::get<Index>(m_iterators);
    return iterator->Get();
  }

  /**
   * Set pixel value from iterator type.
   * \tparam WhichIterator  Unique type identitying the iterator in the tuple.
   * \tparam TValue         Type on data that needs to be convertible to what `WhichIterator::Set()`
   *                        expects.
   * \warning this overload cannot be used when several iterators have the same type -- which may
   *          happen when each iterator type comes from a different template parameters of the OTB
   *          filter where it's used.
   * \param[in] value  Value to assign to the pixel
   *
   * \throw Neutral This function will forward whatever `WhichIterator::Set()` may throw
   */
  template <typename WhichIterator, typename TValue>
  void Set(TValue const& value)
  {
    auto&& iterator = std::get<WhichIterator>(m_iterators);
    iterator->Set(value);
  }

  /**
   * Set pixel value from its index.
   * \tparam Index  Compiled-time used to identify the actual iterator used.
   * \tparam TValue Type on data that needs to be convertible to what `WhichIterator::Set()`
   *                expects.
   * \note use this overload cannot be used when several iterators have the same type -- which may
   *       happen when each iterator type comes from a different template parameters of the OTB
   *       filter where it's used.
   * \param[in] value  Value to assign to the pixel
   *
   * \throw Neutral This function will forward whatever `m_iterators<Index>::Set()` may throw
   */
  template <std::size_t Index, typename TValue>
  void Set(TValue const& value)
  {
    auto&& iterator = std::get<Index>(m_iterators);
    iterator->Set(value);
  }
  //@}
private:
  std::tuple<TIterator...> m_iterators;
};

/**
 * Helper function to create `TupleOfIterators`.
 * Workaround lack of CTAD in C++14
 *
 * \tparam TIterator    Auto-deduced effective type of the iterators.
 * \param[in] iterator  List of may-be-iterators to track in the tuple.
 *
 * \return An instance of `otb::TupleOfIterators<>`
 */
template <typename... TIterator>
inline
auto make_iterator_tuple(TIterator && ... iterator)
{
  return TupleOfIterators<otb::remove_cvref_t<TIterator>...>(std::forward<TIterator>(iterator)...);
}

template <typename... TIterator>
inline
auto wrap_iterators_in_tuple(TIterator && ... iterator)
{
  return TupleOfIterators<TIterator&...>(std::forward<TIterator>(iterator)...);
}
}  // otb namespace

#endif  // otbTupleIterator_h
