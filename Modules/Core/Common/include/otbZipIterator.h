/*
 * Copyright(C) 2005-2020 Centre National d'Etudes Spatiales(CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https: //www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbZipIterator_h
#define otbZipIterator_h

#include "otbSpan.h"
#include "itkMacro.h"
#include <type_traits>
#include <vector>
#include <cassert>

namespace otb
{
namespace internals
{

struct ConstTag {};
struct MutableTag {};

/**
 * Wrapper to present list of iterators as a single iterator.
 *
 * \invariant This class shall not be inherited.
 * \invariant All sub iterators are always synchronised (same region, size...)
 * \invariant `!m_iterators.empty()`
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 */
template <typename TImageIterator, typename ConstOrMutable>
class ZipIterator
{
public:
  /**\name ITK Constants and Typedefs */
  //@{
  /** Iterator type for zipped iterators. */
  using ImageIteratorType     = TImageIterator;

  /** Image type alias support */
  using ImageType             = typename TImageIterator::ImageType;

  /** Dimension of the image the iterator walks.  This constant is needed so
   * functions that are templated over image iterator type (as opposed to
   * being templated over pixel type and dimension) can have compile time
   * access to the dimension of the image that the iterator walks. */
  static constexpr unsigned int ImageIteratorDimension = ImageIteratorType::ImageIteratorDimension;

  using Self                  = ZipIterator;

  /** Run-time type information (and related methods). */
  itkTypeMacroNoParent(ZipIterator);

  /** Index type alias support */
  using IndexType             = typename ImageIteratorType::IndexType;

  /** Size type alias support */
  using SizeType              = typename ImageIteratorType::SizeType;

  /** Offset type alias support */
  using OffsetType            = typename ImageIteratorType::OffsetType;

  /** Region type alias support */
  using RegionType            = typename ImageIteratorType::RegionType;

  /** PixelContainer type alias support. Used to refer to the container for
   * the pixel data. While this was already typdef'ed in the superclass
   * it needs to be redone here for this subclass to compile properly with gcc. */
  using PixelContainer        = typename ImageIteratorType::PixelContainer;
  using PixelContainerPointer = typename PixelContainer::Pointer;

  /** Internal Pixel Type */
  using InternalPixelType     = typename ImageIteratorType::InternalPixelType;

  /** External Pixel Type */
  using PixelType             = typename ImageIteratorType::PixelType;

  /**  Accessor type that convert data between internal and external
   *  representations. */
  using AccessorType          = typename ImageIteratorType::AccessorType;
  using AccessorFunctorType   = typename ImageIteratorType::AccessorFunctorType;
  //@}

  /**\name Constructions & Destruction
   *
   * This class follows the rule of 0/5
   */
  //@{
  ZipIterator           ()                   = default;
  ~ZipIterator          ()                   = default;
  ZipIterator           (ZipIterator const&) = default;
  ZipIterator           (ZipIterator     &&) = default;
  ZipIterator& operator=(ZipIterator const&) = default;
  ZipIterator& operator=(ZipIterator     &&) = default;

  /** Convertion constructor.
   * Converts from mutable to const iterator
   * \see https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
   */
  template <bool IsConst_ = std::is_same<ConstOrMutable, ConstTag>::value, class = std::enable_if<IsConst_>>
  ZipIterator(ZipIterator<ImageIteratorType, MutableTag> const& rhs)
    : m_iterators(rhs.m_iterators())
    {}

  /** Convertion move constructor.
   * Move converts from mutable to const iterator
   * \see https://quuxplusone.github.io/blog/2018/12/01/const-iterator-antipatterns/
   */
  template <bool IsConst_ = std::is_same<ConstOrMutable, ConstTag>::value, class = std::enable_if<IsConst_>>
  ZipIterator(ZipIterator<ImageIteratorType, MutableTag> && rhs)
    : m_iterators(move(rhs.m_iterators()))
    {}

  /**
   * Init Constructor.
   * Constructs a `ZipIterator` from a list of Images and a Region.
   * \param[in,out] images  List of images
   * \param[in]     region  Region to iterate over
   * \pre There should be at least one image
   */
  ZipIterator(Span<ImageType * const> images, RegionType const& region)
  {
    assert(! images.empty());
    m_iterators.reserve(images.size());

    for (auto & im: images)
      m_iterators.emplace_back(im, region);
  }

  // static_assert(std::is_copy_constructible<Self>::value, "Requires copy construction");
  // static_assert(std::is_trivially_copy_constructible<Self>::value, "Requires trivial copy construction");
  //@}

  /**\name Comparison */
  //@{
  friend bool operator==(ZipIterator const& lhs, ZipIterator const& rhs)
  {
    assert(!lhs.m_iterators.empty());
    assert(lhs.m_iterators.size() == rhs.m_iterators.size());

    return lhs.m_iterators.front() == rhs.m_iterators.front();
  }
  friend bool operator!=(ZipIterator const& lhs, ZipIterator const& rhs)
  { return ! (lhs == rhs); }

  friend bool operator<=(ZipIterator const& lhs, ZipIterator const& rhs)
  {
    assert(!lhs.m_iterators.empty());
    assert(lhs.m_iterators.size() == rhs.m_iterators.size());

    return lhs.m_iterators.front() <= rhs.m_iterators.front();
  }
  friend bool operator<(ZipIterator const& lhs, ZipIterator const& rhs)
  {
    assert(!lhs.m_iterators.empty());
    assert(lhs.m_iterators.size() == rhs.m_iterators.size());

    return lhs.m_iterators.front() < rhs.m_iterators.front();
  }
  friend bool operator>=(ZipIterator const& lhs, ZipIterator const& rhs)
  { return ! (lhs < rhs); }
  friend bool operator>(ZipIterator const& lhs, ZipIterator const& rhs)
  { return ! (lhs <= rhs); }
  //@}

  /**\name ITK iterator interface */
  //@{
  // What about GetIndex() and SetIndex ?
  /** Fetch the region iterated by the iterator. */
  auto const& GetRegion() const
  {
    assert(!m_iterators.empty());
    return m_iterators.front().GetRegion();
  }

  /** Set the region iterated by the iterator. */
  void SetRegion(RegionType const& region)
  {
    for (auto & it : m_iterators)
      it.SetRegion(region);
  }

  /** Moves the iterator to the begin of the region iterated. */
  Self& GoToBegin() {
    for (auto & it : m_iterators)
      it.GoToBegin();
    return *this;
  }
  /** Moves the iterator to the end of the region iterated. */
  Self& GoToEnd() {
    for (auto & it : m_iterators)
      it.GoToEnd();
    return *this;
  }

  /** Tells whether the iterator is at the begin of the region iterated. */
  bool IsAtBegin() const {
    assert(!m_iterators.empty());
    return m_iterators.front().IsAtBegin();
  }

  /** Tells the iterator is at the end of the region iterated. */
  bool IsAtEnd() const {
    assert(!m_iterators.empty());
    return m_iterators.front().IsAtEnd();
  }

  /** Pre-increment the iterator.
   * As post-increment is less efficient, it hasn't been provided.
   */
  Self& operator++() {
    assert(!IsAtEnd());
    for (auto & it : m_iterators)
      ++it;
    return *this;
  }
  /** Removed post-increment operator.
   * Please use the  preincrement operator!
   */
  Self operator++(int) = delete;

  /** Internal typedef to the type holding the list of ITK iterators. */
  using ImageIteratorList_t = std::vector<ImageIteratorType>;
  /** Internal Pixel Proxy Type returned by `ZipIterator`.
   * \author Luc Hermitte (CS Group)
   * \copyright CNES
   * \ingroup OTBCommon
   */
  struct PixelListProxy
  {
    /** Init constructor.
     * @param[in] iterators List of ITK iterators
     */
    explicit PixelListProxy(ImageIteratorList_t const& iterators) noexcept
      : m_iterators(iterators)
      {}
    bool empty()   const noexcept {return m_iterators.empty();}
    auto size()    const noexcept {return m_iterators.size();}

    /** Internal C++ iterator over the components of the Pixel Proxy.
     * \author Luc Hermitte (CS Group)
     * \copyright CNES
     * \ingroup OTBCommon
     */
    struct iterator__
    {
      using difference_type   = typename ImageIteratorList_t::difference_type;
      using value_type        = decltype(typename ImageIteratorList_t::const_iterator{}->Get());
      using pointer           = value_type*;
      using reference         = value_type&;
      using iterator_category = std::forward_iterator_tag;

      explicit iterator__(typename ImageIteratorList_t::const_iterator ref)
        : reference_to_value(ref){}
      friend bool operator==(iterator__ const& lhs, iterator__ const& rhs) noexcept
      { return lhs.reference_to_value == rhs.reference_to_value;}
      friend bool operator!=(iterator__ const& lhs, iterator__ const& rhs) noexcept
      { return ! (lhs == rhs);}
      iterator__ & operator++() noexcept {
        ++reference_to_value;
        return *this;
      }
      iterator__ & operator--() noexcept {
        --reference_to_value;
        return *this;
      }
      iterator__ operator+(std::ptrdiff_t offset) const noexcept{
        return iterator__{reference_to_value + offset};
      }
      iterator__ operator-(std::ptrdiff_t offset) const noexcept{
        return iterator__{reference_to_value - offset};
      }
      decltype(auto) operator*() const {
        return reference_to_value->Get();
      }

    private:
      typename ImageIteratorList_t::const_iterator reference_to_value;
    };

    auto begin()         noexcept { return iterator__(m_iterators.begin()); }
    auto end()           noexcept { return iterator__{m_iterators.end()}; }
    auto begin()   const noexcept { return iterator__{m_iterators.begin()}; }
    auto end()     const noexcept { return iterator__{m_iterators.end()}; }
    auto cbegin()  const noexcept { return iterator__{m_iterators.cbegin()}; }
    auto cend()    const noexcept { return iterator__{m_iterators.cend()}; }

    decltype(auto) operator[](std::size_t idx) const {
      assert(idx < size());
      return m_iterators[idx].Get();
    }
    decltype(auto) operator[](std::size_t idx)       {
      assert(idx < size());
      return m_iterators[idx].Get();
    }

    decltype(auto) front() const {
      assert(!empty());
      return m_iterators.front().Get();
    }
    decltype(auto) front()       {
      assert(!empty());
      return m_iterators.front().Get();
    }
    decltype(auto) back() const {
      assert(!empty());
      return m_iterators.back().Get();
    }
    decltype(auto) back()       {
      assert(!empty());
      return m_iterators.back().Get();
    }
  private:
    ImageIteratorList_t const& m_iterators;
  };

  /** Fetches the value of the current pixel.
   * \return an iterable proxy over the pixel made of all images.
   */
  PixelListProxy Get() const {
    return PixelListProxy{m_iterators};
  }

  //@}

  /**\name Mutable Iterator Interface */
  //@{
  template <typename MultiCompPixelType>
  void Set(MultiCompPixelType const& p)
  {
    assert(p.size() == m_iterators.size());
    for (std::size_t i = 0; i!=m_iterators.size(); ++i)
    {
      m_iterators[i].Set(p[i]);
    }
  }
  // PixelType & Value(); -- cannot be defined and still preserve direct access
  // to memory => we don't provide it.

  // ImageType * GetImages();
  //@}

  /**\name ScanLine Iterator Interface */
  //@{
  /** Moves iterator to next line. */
  Self& NextLine() {
    for (auto & it : m_iterators)
      it.NextLine();
    return *this;
  }
  /** Moves iterator to the beginning of the current line. */
  Self& GoToBeginOfLine() {
    for (auto & it : m_iterators)
      it.GoToBeginOfLine();
    return *this;
  }
  /** Moves iterator to the end of the current line. */
  Self& GoToEndOfLine() {
    for (auto & it : m_iterators)
      it.GoToEndOfLine();
    return *this;
  }
  /** Tells whether the iterator is a the end of a line.
   * \pre `!m_iterators.empty()`
   */
  bool IsAtEndOfLine() const {
    assert(!m_iterators.empty());
    // Const qualifier has been added to ScanLineIterator::IsAtEndOfLine in ITK
    // 5.1 => Use const_cast in the mean time...
    return const_cast<typename ImageIteratorList_t::value_type &>(m_iterators.front()).IsAtEndOfLine();
  }
  //@}

private:
  ImageIteratorList_t m_iterators;
};

} // otb::internal namespace

/** Typedef for a ZipIterator.
 * ZipIterator presents a single iterator over a list of image iterators.
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 * \see `otb::internals::ZipIterator<>`
 */
template <typename TImageIterator>
using ZipIterator      = internals::ZipIterator<TImageIterator, internals::MutableTag>;
/** Typedef for a ZipConstIterator.
 * ZipConstIterator presents a single iterator over a list of const image iterators.
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \ingroup OTBCommon
 * \see `otb::internals::ZipIterator<>`
 */
template <typename TImageIterator>
using ZipConstIterator = internals::ZipIterator<TImageIterator, internals::ConstTag>;

} // otb namespace

#endif  // otbZipIterator_h
