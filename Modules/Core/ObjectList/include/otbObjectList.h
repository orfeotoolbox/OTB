/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbObjectList_h
#define __otbObjectList_h

#include <vector>
#include "itkDataObject.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \class ObjectList
 *  \brief This class is a generic all-purpose wrapping around an std::vector<itk::SmartPointer<ObjectType> >.
 *
 * ObjectList stores SmartPointer to ObjectType in a std::vector data structure. It provides the same
 * methods and iterators interfaces.
 *
 * \ingroup OTBObjectList
 */
template <class TObject>
class ITK_EXPORT ObjectList : public itk::DataObject
{
public:
  /** Standard typedefs */
  typedef ObjectList                    Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(ObjectList, DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Template parameter typedefs */
  typedef TObject                        ObjectType;
  typedef itk::SmartPointer<ObjectType>  ObjectPointerType;
  typedef std::vector<ObjectPointerType> InternalContainerType;

  /**
   * Set the minimum capacity of the vector.
   * \param size Size of the vector to reserve.
   */
  void Reserve(unsigned int size);
  /**
   * Get the capacity of the vector.
   * \return The capacity of the vector.
   */
  unsigned int Capacity(void) const;
  /**
   * Get the number of elements in the vector.
   * \return The number of elements in the vector.
   */
  unsigned int Size(void) const;
  /**
   * Resize the maximal list capacity.
   * \param size The new maximal size of the list.
   */
  void Resize(unsigned int size);
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
  void SetNthElement(unsigned int index, const ObjectType * element);
  /**
   * Get the nth element of the list.
   * \param index The index of the object to get.
   * \return The pointer to the nth element of the list.
   */
  ObjectPointerType GetNthElement(unsigned int index) const;
  /**
   * Return the first element of the list.
   * \return The first element of the list.
   */
  ObjectPointerType Front(void);
  /**
   * Return the last element of the list.
   * \return The last element of the list.
   */
  ObjectPointerType Back(void);
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
   */
  class ITK_EXPORT Iterator
  {
public:
    friend class ObjectList;
    friend class ConstIterator;
    /** typedef of the internal iterator */
    typedef typename InternalContainerType::iterator InternalIteratorType;
    /** Constructor */
    Iterator() {};
    /** Constructor with iternal iterator parameter */
    Iterator(InternalIteratorType iter)
      {
      m_Iter = iter;
      };
    /**
       * Get the current object.
       * \return The current object pointed by the iterator.
       */
    ObjectPointerType Get(void)
    {
      return (*m_Iter);
    }
    /**
     * Set the current object
     */
    void Set(ObjectPointerType element)
    {
      (*m_Iter) = element;
      this->Modified();
    }
    /**
       * Increment.
       */
    Iterator & operator ++()
    {
      ++m_Iter;
      return *this;
    }
    /**
       * Decrement.
       */
    Iterator & operator --()
    {
      --m_Iter;
      return *this;
    }
    /**
       * Add
       */
    Iterator operator +(int i)
    {
      Iterator lIter(m_Iter + i);
      return lIter;
    }

    /**
       * Remove
       */
    Iterator operator -(int i)
    {
      Iterator lIter(m_Iter - i);
      return lIter;
    }
    /**
       * Difference comparison operator.
       */
    bool operator !=(const Iterator& it)
    {
      return (m_Iter != it.m_Iter);
    }
    /**
       * Equality comparison operator.
       */
    bool operator ==(const Iterator& it)
    {
      return (m_Iter == it.m_Iter);
    }
    /**
       * Instantiation operator.
       */
    Iterator & operator =(const Iterator& it)
    {
      m_Iter = it.m_Iter;
      return *this;
    }
    /**
       * Copy operator.
       */
    Iterator(const Iterator &it)
      {
      m_Iter = it.m_Iter;
      };

    /**
      * Get the current internal iterator
    */
    InternalIteratorType& GetIter(void)
    {
      return (m_Iter);
    }
private:
    // Internal iterator.
    InternalIteratorType m_Iter;
  };
  /** \class ConstIterator
   *  \brief ConstIterator of the object list.
 *
 * \ingroup OTBObjectList
   */
  class ITK_EXPORT ConstIterator
  {
public:
    friend class ObjectList;
    friend class Iterator;
    /** typedef of the internal iterator */
    typedef typename InternalContainerType::const_iterator InternalConstIteratorType;
    /** Constructor */
    ConstIterator() {};
    /** Constructor with iternal iterator parameter */
    ConstIterator(InternalConstIteratorType iter)
      {
      m_Iter = iter;
      };
    /**
       * Get the current object.
       * \return The current object pointed by the iterator.
       */
    ObjectPointerType Get(void)
    {
      return (*m_Iter);
    }
    /**
       * Increment.
       */
    ConstIterator & operator ++()
    {
      ++m_Iter;
      return *this;
    }
    /**
       * Decrement.
       */
    ConstIterator & operator --()
    {
      --m_Iter;
      return *this;
    }
    /**
       * Difference comparison operator.
       */
    bool operator !=(const ConstIterator& it)
    {
      return (m_Iter != it.m_Iter);
    }
    /**
       * Equality comparison operator.
       */
    bool operator ==(const ConstIterator& it)
    {
      return (m_Iter == it.m_Iter);
    }
    /**
       * Instantiation operator.
       */
    ConstIterator & operator =(const ConstIterator& it)
    {
      m_Iter = it.m_Iter;
      return *this;
    }
    /**
       * Instantiation operator.
       */
    ConstIterator & operator =(const Iterator& it)
    {
      m_Iter = it.m_Iter;
      return *this;
    }
    /**
       * Copy operator.
       */
    ConstIterator(const ConstIterator &it)
      {
      m_Iter = it.m_Iter;
      };
    /**
       * Copy operator.
       */
    ConstIterator(const Iterator &it)
      {
      m_Iter = it.m_Iter;
      };

private:
    // Internal iterator.
    InternalConstIteratorType m_Iter;
  };
  /** \class ReverseIterator
   *  \brief ReverseIterator of the object list.
 *
 * \ingroup OTBObjectList
   */
  class ITK_EXPORT ReverseIterator
  {
public:
    friend class ObjectList;
    friend class Iterator;

    friend class ReverseConstIterator;
    /** typedef of the internal iterator */
    typedef typename InternalContainerType::reverse_iterator InternalReverseIteratorType;
    /** Constructor */
    ReverseIterator() {};
    /** Constructor with iternal iterator parameter */
    ReverseIterator(InternalReverseIteratorType iter)
      {
      m_Iter = iter;
      };
    /**
       * Get the current object.
       * \return The current object pointed by the iterator.
       */
    ObjectPointerType Get(void)
    {
      return (*m_Iter);
    }
    /**
     * Set the current object
     */
    void Set(ObjectPointerType element)
    {
      (*m_Iter) = element;
    }
    /**
       * Increment.
       */
    ReverseIterator & operator ++()
    {
      ++m_Iter;
      return *this;
    }
    /**
       * Decrement.
       */
    ReverseIterator & operator --()
    {
      --m_Iter;
      return *this;
    }
    /**
       * Difference comparison operator.
       */
    bool operator !=(const ReverseIterator& it)
    {
      return (m_Iter != it.m_Iter);
    }
    /**
       * Equality comparison operator.
       */
    bool operator ==(const ReverseIterator& it)
    {
      return (m_Iter == it.m_Iter);
    }
    /**
       * Instantiation operator.
       */
    ReverseIterator & operator =(const ReverseIterator& it)
    {
      m_Iter = it.m_Iter;
      return *this;
    }
    /**
       * Copy operator.
       */
    ReverseIterator(const ReverseIterator &it)
      {
      m_Iter = it.m_Iter;
      };

    /**
      * Get the current internal iterator
    */
    InternalReverseIteratorType& GetIter(void)
    {
      return (m_Iter);
    }

private:
    // Internal iterator.
    InternalReverseIteratorType m_Iter;
  };
  /** \class ReverseConstIterator
   *  \brief ReverseConstIterator of the object list.
 *
 * \ingroup OTBObjectList
   */
  class ITK_EXPORT ReverseConstIterator
  {
public:
    friend class ObjectList;
    friend class Iterator;
    friend class ConstIterator;
    friend class ReverseIterator;
    /** typedef of the internal iterator */
    typedef typename InternalContainerType::reverse_iterator InternalReverseConstIteratorType;
    /** Constructor */
    ReverseConstIterator() {};
    /** Constructor with iternal iterator parameter */
    ReverseConstIterator(InternalReverseConstIteratorType iter)
      {
      m_Iter = iter;
      };
    /**
       * Get the current object.
       * \return The current object pointed by the iterator.
       */
    ObjectPointerType Get(void)
    {
      return (*m_Iter);
    }
    /**
       * Increment.
       */
    ReverseConstIterator & operator ++()
    {
      ++m_Iter;
      return *this;
    }
    /**
       * Decrement.
       */
    ReverseConstIterator & operator --()
    {
      --m_Iter;
      return *this;
    }
    /**
       * Difference comparison operator.
       */
    bool operator !=(const ReverseConstIterator& it)
    {
      return (m_Iter != it.m_Iter);
    }
    /**
       * Equality comparison operator.
       */
    bool operator ==(const ReverseConstIterator& it)
    {
      return (m_Iter == it.m_Iter);
    }
    /**
       * Instantiation operator.
       */
    ReverseConstIterator & operator =(const ReverseConstIterator& it)
    {
      m_Iter = it.m_Iter;
      return *this;
    }
    /**
       * Instantiation operator.
       */
    ReverseConstIterator & operator =(const ReverseIterator& it)
    {
      m_Iter = it.m_Iter;
      return *this;
    }
    /**
       * Copy operator.
       */
    ReverseConstIterator(const ReverseConstIterator &it)
      {
      m_Iter = it.m_Iter;
      };
    /**
       * Copy operator.
       */
    ReverseConstIterator(const ReverseIterator &it)
      {
      m_Iter = it.m_Iter;
      };

private:
    // Internal iterator.
    InternalReverseConstIteratorType m_Iter;
  };
  /**
   * Get an Iterator that points to the beginning of the container.
   * \return The iterator.
   */
  Iterator Begin(void);
  /**
   * Get a ConstIterator that points to the beginning of the container.
   * \return The iterator.
   */
  ConstIterator Begin(void) const;
  /**
   * Get a ReverseIterator that points to the reverse beginning of the container.
   * \return The iterator.
   */
  ReverseIterator ReverseBegin(void);
  /**
   * Get a ReverseConstIterator that points to the reverse beginning of the container.
   * \return The iterator.
   */
  ReverseConstIterator ReverseBegin(void) const;
  /**
   * Get an Iterator that points past-the-end of the container.
   * \return The iterator.
   */
  Iterator End(void);
  /**
   * Get a ConstIterator that points past-the-end of the container.
   * \return The iterator.
   */
  ConstIterator End(void) const;
  /**
   * Get a ReverseIterator that points to the reverse past-the-end of the container.
   * \return The iterator.
   */
  ReverseIterator ReverseEnd(void);
  /**
   * Get a ReverseConstIterator that points to the reverse past-the-end of the container.
   * \return The iterator.
   */
  ReverseConstIterator ReverseEnd(void) const;
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
  ~ObjectList() {}
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ObjectList(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
  /** The internal std::vector object container */
  InternalContainerType m_InternalContainer;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbObjectList.txx"
#endif

#endif
