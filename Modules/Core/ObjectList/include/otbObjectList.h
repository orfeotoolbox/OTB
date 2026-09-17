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
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbObjectList_h
#define otbObjectList_h

#include <vector>
#include "itkDataObject.h"
#include "otbDataObjectListInterface.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \class ObjectList
 *  \brief This class is a generic all-purpose wrapping around an std::vector<itk::SmartPointer<ObjectType> >.
 *
 * ObjectList stores SmartPointer to ObjectType in a std::vector data structure.
 * It provides the same methods and iterators interfaces.
 *
 * \ingroup OTBObjectList
 */
template <class TObject>
class ITK_EXPORT ObjectList : public itk::DataObject, public DataObjectListInterface
{
public:
  /** Standard typedefs */
  using Self         = ObjectList;
  using Superclass   = itk::DataObject;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Type macro */
  itkTypeMacro(ObjectList, DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Template parameter typedefs */
  using ObjectType                = TObject;
  using ObjectPointerType         = itk::SmartPointer<ObjectType>;
  using InternalContainerType     = std::vector<ObjectPointerType>;
  using InternalContainerSizeType = typename InternalContainerType::size_type;

  /**
   * Set the minimum capacity of the vector.
   * \param size Size of the vector to reserve.
   */
  void Reserve(InternalContainerSizeType size);
  /**
   * Get the capacity of the vector.
   * \return The capacity of the vector.
   */
  [[nodiscard]] InternalContainerSizeType Capacity(void) const;
  /**
   * Get the number of elements in the vector.
   * \return The number of elements in the vector.
   */
  [[nodiscard]] InternalContainerSizeType Size(void) const override;
  /**
   * Resize the maximal list capacity.
   * \param size The new maximal size of the list.
   */
  void Resize(InternalContainerSizeType size);
  /**
   * Append an element to the list.
   * \param element Pointer to the element to append.
   */
  void PushBack(ObjectType* element);
  /**
   * Delete the last element to the list.
   */
  void PopBack(void);
  /**
   * Set the nth element of the list.
   * \param index The index where to put the element.
   * \param element Pointer to the element to set.
   */
  void SetNthElement(unsigned int index, ObjectPointerType element);
  void SetNthElement(unsigned int index, const ObjectType* element);
  /**
   * Get the nth element of the list.
   * \param index The index of the object to get.
   * \return The pointer to the nth element of the list.
   */
  [[nodiscard]] ObjectPointerType GetNthElement(unsigned int index) const;
  /**
   * Get the nth element of the list as a DataObject *.
   * \param index The index of the object to get.
   */
  [[nodiscard]] Superclass* GetNthDataObject(unsigned int index) const override;
  /**
   * Return the first element of the list.
   * \return The first element of the list.
   */
  [[nodiscard]] ObjectPointerType Front(void);
  /**
   * Return the last element of the list.
   * \return The last element of the list.
   */
  [[nodiscard]] ObjectPointerType Back(void);
  /**
   * Erase the nth element of the list.
   * \param index The index of the element to erase.
   */
  void Erase(unsigned int index);
  /**
   * Clear the object list.
   */
  void Clear(void);

  class ConstIterator;
  class ReverseIterator;
  class ReverseConstIterator;
  class Iterator;
  friend class Iterator;
  friend class ConstIterator;
  friend class ReverseIterator;
  friend class ReverseConstIterator;

  /**
   * Insert an element at a given position
   * \param position A random access iterator
   * \return An iterator that points to the newly insereted element.
   */
  Iterator Insert(Iterator position, ObjectPointerType element);
  /**
   * Insert an element at a given position
   * \param position A reverse iterator
   * \return A reverse iterator that points to the newly insereted element.
   */
  ReverseIterator Insert(ReverseIterator position, ObjectPointerType element);

  /** \class Iterator
   *  \brief Iterator of the object list.
   *
   * \ingroup OTBObjectList
   * \todo Factorize the iterator classes, see `PixelComponentIterator`
   */
  class ITK_EXPORT Iterator
  {
  public:
    friend class ObjectList;
    friend class ConstIterator;
    /** typedef of the internal iterator */
    using InternalIteratorType = typename InternalContainerType::iterator;

    /** Default Constructor */
    Iterator() = default;

    /** Constructor with iternal iterator parameter */
    Iterator(InternalIteratorType iter)
    : m_Iter(std::move(iter))
    {}

    /**
     * Get the current object.
     * \return The current object pointed by the iterator.
     */
    [[nodiscard]] ObjectPointerType Get(void)
    {
      return *m_Iter;
    }
    /**
     * Set the current object
     */
    void Set(ObjectPointerType element)
    {
      *m_Iter = element;
    }
    /// Increment.
    Iterator& operator++()
    {
      ++m_Iter;
      return *this;
    }
    /// Decrement.
    Iterator& operator--()
    {
      --m_Iter;
      return *this;
    }
    /// Add
    [[nodiscard]] Iterator operator+(int i) const
    {
      Iterator lIter(m_Iter + i);
      return lIter;
    }
    /// Subtract
    [[nodiscard]] Iterator operator-(int i) const
    {
      Iterator lIter(m_Iter - i);
      return lIter;
    }
    /// Increment
    Iterator operator+=(int i)
    {
      return m_Iter + i;
    }
    /// Decrement
    Iterator operator-=(int i)
    {
      return m_Iter - i;
    }

    /// Difference comparison operator.
    [[nodiscard]] friend bool operator==(Iterator const& lhs, Iterator const& rhs) = default;
    /// Equality comparison operator.
    [[nodiscard]] friend bool operator!=(Iterator const& lhs, Iterator const& rhs) = default;

    /// Copy-assignment operator.
    Iterator& operator=(Iterator const& it) = default;
    /// Move-assignment operator.
    Iterator& operator=(Iterator && it) = default;

    /// Copy constructor
    Iterator(Iterator const& it) = default;
    /// Move constructor
    Iterator(Iterator && it) = default;

    /// Get the current internal iterator
    InternalIteratorType& GetIter(void)
    {
      return m_Iter;
    }

  private:
    /// Internal iterator.
    InternalIteratorType m_Iter;
  };

  /** \class ConstIterator
   *  \brief ConstIterator of the object list.
   *
   * \ingroup OTBObjectList
   * \todo Factorize the iterator classes
   */
  class ITK_EXPORT ConstIterator
  {
  public:
    friend class ObjectList;
    friend class Iterator;
    /** typedef of the internal iterator */
    using InternalConstIteratorType = typename InternalContainerType::const_iterator;

    /** Default-Constructor */
    ConstIterator() = default;

    /** Constructor with iternal iterator parameter */
    ConstIterator(InternalConstIteratorType iter)
    : m_Iter(std::move(iter))
    {
    }

    /**
     * Get the current object.
     * \return The current object pointed by the iterator.
     */
    [[nodiscard]] ObjectPointerType Get(void) const
    {
      return *m_Iter;
    }

    /// Increment.
    ConstIterator& operator++()
    {
      ++m_Iter;
      return *this;
    }

    /// Decrement.
    ConstIterator& operator--()
    {
      --m_Iter;
      return *this;
    }

    /// Difference comparison operator.
    [[nodiscard]] friend bool operator==(ConstIterator const& lhs, ConstIterator const& rhs) = default;
    /// Equality comparison operator.
    [[nodiscard]] friend bool operator!=(ConstIterator const& lhs, ConstIterator const& rhs) = default;

    /// Copy-assignment operator.
    ConstIterator& operator=(ConstIterator const& it) = default;
    /// Move-assignment operator.
    ConstIterator& operator=(ConstIterator && it) = default;

    /// Copy-conversion operator.
    ConstIterator& operator=(Iterator const& it)
    {
      m_Iter = it.m_Iter;
      return *this;
    }
    /// Move-conversion operator.
    ConstIterator& operator=(Iterator && it)
    {
      m_Iter = std::move(it.m_Iter);
      return *this;
    }

    /// Copy constructor.
    ConstIterator(ConstIterator const& it) = default;
    /// Move constructor.
    ConstIterator(ConstIterator && it) = default;

    /// Copy-conversion constructor.
    ConstIterator(Iterator const& it)
    : m_Iter(it.m_Iter)
    {}
    /// Move-conversion constructor.
    ConstIterator(Iterator && it)
    : m_Iter(std::move(it.m_Iter))
    {}

  private:
    /// Internal iterator.
    InternalConstIteratorType m_Iter;
  };

  /** \class ReverseIterator
   *  \brief ReverseIterator of the object list.
   *
   * \ingroup OTBObjectList
   * \todo Factorize the iterator classes
   */
  class ITK_EXPORT ReverseIterator
  {
  public:
    friend class ObjectList;
    friend class Iterator;

    friend class ReverseConstIterator;
    /** typedef of the internal iterator */
    typedef typename InternalContainerType::reverse_iterator InternalReverseIteratorType;

    /** Default-Constructor */
    ReverseIterator() = default;
    /** Constructor with iternal iterator parameter */
    ReverseIterator(InternalReverseIteratorType iter)
    : m_Iter(iter)
    {};

    /**
     * Get the current object.
     * \return The current object pointed by the iterator.
     */
    [[nodiscard]] ObjectPointerType Get(void)
    {
      return *m_Iter;
    }
    /// Set the current object
    void Set(ObjectPointerType element)
    {
      *m_Iter = element;
    }
    /// Increment.
    ReverseIterator& operator++()
    {
      ++m_Iter;
      return *this;
    }
    /// Decrement.
    ReverseIterator& operator--()
    {
      --m_Iter;
      return *this;
    }

    /// Difference comparison operator.
    [[nodiscard]] friend bool operator!=(ReverseIterator const& lhs, ReverseIterator const& rhs) = default;
    /// Equality comparison operator.
    [[nodiscard]] friend bool operator==(ReverseIterator const& lhs, ReverseIterator const& rhs) = default;

    /// Copy-assignement operator.
    ReverseIterator& operator=(ReverseIterator const& it) = default;
    /// Move-assignement operator.
    ReverseIterator& operator=(ReverseIterator && it) = default;

    /// Copy-constructor.
    ReverseIterator(ReverseIterator const& it) = default;
    /// Move-constructor.
    ReverseIterator(ReverseIterator && it) = default;

    /// Get the current internal iterator
    InternalReverseIteratorType& GetIter(void)
    {
      return m_Iter;
    }

  private:
    // Internal iterator.
    InternalReverseIteratorType m_Iter;
  };

  /** \class ReverseConstIterator
   *  \brief ReverseConstIterator of the object list.
   *
   * \ingroup OTBObjectList
   * \todo Factorize the iterator classes
   */
  class ITK_EXPORT ReverseConstIterator
  {
  public:
    friend class ObjectList;
    friend class Iterator;
    friend class ConstIterator;
    friend class ReverseIterator;
    /** typedef of the internal iterator */
    using InternalReverseConstIteratorType = typename InternalContainerType::const_reverse_iterator;

    /** Default-Constructor */
    ReverseConstIterator() = default;

    /** Constructor with iternal iterator parameter */
    ReverseConstIterator(InternalReverseConstIteratorType iter)
    : m_Iter(iter)
    {}

    /**
     * Get the current object.
     * \return The current object pointed by the iterator.
     */
    [[nodiscard]] ObjectPointerType Get(void)
    {
      return *m_Iter;
    }
    /// Increment.
    ReverseConstIterator& operator++()
    {
      ++m_Iter;
      return *this;
    }
    /// Decrement.
    ReverseConstIterator& operator--()
    {
      --m_Iter;
      return *this;
    }

    /// Difference comparison operator.
    [[nodiscard]] friend bool operator!=(ReverseConstIterator const& lhs, ReverseConstIterator const& rhs) = default;
    /// Equality comparison operator.
    [[nodiscard]] friend bool operator==(ReverseConstIterator const& lhs, ReverseConstIterator const& rhs) = default;

    /// Copy-Assignment operator.
    ReverseConstIterator& operator=(ReverseConstIterator const& it) = default;
    /// Move.
    ReverseConstIterator& operator=(ReverseConstIterator && it) = default;

    /// Copy-Assignment operator.
    ReverseConstIterator(ReverseConstIterator const& it) = default;
    /// Move-Assignment operator.
    ReverseConstIterator(ReverseConstIterator && it) = default;

    /// Copy-conversion operator.
    ReverseConstIterator& operator=(ReverseIterator const& it)
    {
      m_Iter = it.m_Iter;
      return *this;
    }
    /// Move-conversion operator.
    ReverseConstIterator& operator=(ReverseIterator && it)
    {
      m_Iter = std::move(it.m_Iter);
      return *this;
    }

    /// Copy conversion constructor.
    ReverseConstIterator(ReverseIterator const& it)
    : m_Iter(it.m_Iter)
    {}
    /// Move conversion constructor.
    ReverseConstIterator(ReverseIterator && it)
    : m_Iter(std::move(it.m_Iter))
    {}

  private:
    // Internal iterator.
    InternalReverseConstIteratorType m_Iter;
  };

  /**
   * Get an Iterator that points to the beginning of the container.
   * \return The iterator.
   */
  [[nodiscard]] Iterator Begin(void);
  /**
   * Get a ConstIterator that points to the beginning of the container.
   * \return The iterator.
   */
  [[nodiscard]] ConstIterator Begin(void) const;
  /**
   * Get a ReverseIterator that points to the reverse beginning of the container.
   * \return The iterator.
   */
  [[nodiscard]] ReverseIterator ReverseBegin(void);
  /**
   * Get a ReverseConstIterator that points to the reverse beginning of the container.
   * \return The iterator.
   */
  [[nodiscard]] ReverseConstIterator ReverseBegin(void) const;
  /**
   * Get an Iterator that points past-the-end of the container.
   * \return The iterator.
   */
  [[nodiscard]] Iterator End(void);
  /**
   * Get a ConstIterator that points past-the-end of the container.
   * \return The iterator.
   */
  [[nodiscard]] ConstIterator End(void) const;
  /**
   * Get a ReverseIterator that points to the reverse past-the-end of the container.
   * \return The iterator.
   */
  [[nodiscard]] ReverseIterator ReverseEnd(void);
  /**
   * Get a ReverseConstIterator that points to the reverse past-the-end of the container.
   * \return The iterator.
   */
  [[nodiscard]] ReverseConstIterator ReverseEnd(void) const;
  /**
   * Erase elements from begin to last.
   * \param begin Iterator pointing on first object to erase.
   * \param end Iterator pointing past the last object to erase.
   */
  void Erase(Iterator begin, Iterator end);

  /**
   * Erase loc element.
   * \param loc Iterator pointing on object to erase.
   */
  void Erase(Iterator loc);

protected:
  /** Constructor */
  ObjectList();
  /** Destructor */
  ~ObjectList() override = default;
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

private:
  ObjectList(const Self&) = delete;
  void operator=(const Self&) = delete;
  /** The internal std::vector object container */
  InternalContainerType m_InternalContainer;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbObjectList.hxx"
#endif

#endif
