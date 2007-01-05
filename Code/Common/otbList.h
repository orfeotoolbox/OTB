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
#ifndef _otbList_h
#define _otbList_h

#include <vector>
#include "itkDataObject.h"
#include "itkObjectFactory.h"

namespace otb 
{
/** \class List
 *  \brief This class is a generic all-purpose wrapping around an std::vector<ObjectType*>.
 *
 * List stores Pointer (*) to ObjectType in a std::vector data structure. It provides the same
 * methods and iterators interfaces.
 */
template <class TObject>
class ITK_EXPORT List
  : public itk::DataObject
{
 public:
  /** Standard typedefs */
  typedef List<TObject> Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(List,DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Template parameter typedefs */
  typedef TObject ObjectType;
//  typedef ObjectType* ObjectPointerType;
  typedef typename ObjectType::Pointer ObjectPointerType;
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
  unsigned int Capacity(void);
  /**
   * Get the number of elements in the vector.
   * \return The number of elements in the vector.
   */
  unsigned int Size(void);
  /**
   * Resize the maximal list capacity.
   * \param size The new maximal size of the list.
   */
  void Resize(unsigned int size);
  /**
   * Append an element to the list.
   * \param element Pointer to the element to append.
   */
  void PushBack(const ObjectType* element);
  /**
   * Delete the last element to the list.
   */
  void PopBack(void);
  /**
   * Set the nth element of the list.
   * \param index The index where to put the element.
   * \param element Pointer to the element to set.
   */
  void SetNthElement(unsigned int index, const ObjectType* element);
  /**
   * Get the nth element of the list.
   * \param index The index of the object to get.
   * \return The pointer to the nth element of the list.
   */
  ObjectType* GetNthElement(unsigned int index);
  /**
   * Get the nth element of the const list.
   * \param index The index of the object to get.
   * \return The pointer to the nth element of the list.
   */
  const ObjectType* GetNthElement(unsigned int index)const
  {
    return m_InternalContainer[index].GetPointer();
  }
  const ObjectPointerType GetNthPointerElement(unsigned int index)const
  {
    return m_InternalContainer[index];
  }
 
 
   /**
   * Return the first element of the list.
   * \return The first element of the list.
   */
  ObjectType* Front(void);
  /**
   * Return the first element of the const list.
   * \return The first element of the list.
   */
//  const ObjectType* Front(void) const;
  /**
   * Return the last element of the list.
   * \return The last element of the list.
   */
  ObjectType* Back(void);
  /**
   * Return the last element of the const list.
   * \return The last element of the list.
   */
//  const ObjectType* Back(void)const;
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
  class ReverseConstIterator;

  /** \class Iterator
   *  \brief Iterator of the object list.
   */
  class Iterator
    {
    public:
      friend class List;
      friend class ConstIterator;
      /** typedef of the internal iterator */
      typedef typename InternalContainerType::iterator InternalIteratorType;
      /** Constructor */
      Iterator();
      /** Constructor with iternal iterator parameter */
      Iterator(InternalIteratorType iter);
      /**
       * Get the current object.
       * \return The current object pointed by the iterator.
       */
      ObjectPointerType Get(void);
      /**
       * Increment.
       */
      Iterator& operator++();
      /**
       * Decrement.
       */
      Iterator& operator--();
      /**
       * Add
       */
      Iterator& operator+(int i);

      /**
       * Remove
       */
      Iterator& operator-(int i);
      /**
       * Difference comparison operator.
       */
      bool operator!=(const Iterator &it);
      /**
       * Equality comparison operator.
       */
      bool operator==(const Iterator &it);
      /**
       * Instantiation operator.
       */
      Iterator& operator=(const Iterator &it);
      /**
       * Copy operator.
       */
      Iterator(const Iterator &it);
    private:
      // Internal iterator.
      InternalIteratorType m_Iter;
    };
  /** \class ConstIterator
   *  \brief ConstIterator of the object list.
   */
  class ConstIterator
    {
    public:
      /** typedef of the internal iterator */
      typedef typename InternalContainerType::const_iterator InternalConstIteratorType;
      /** Constructor */
      ConstIterator();
      /** Constructor with iternal iterator parameter */
      ConstIterator(InternalConstIteratorType iter);
      /**
       * Get the current object.
       * \return The current object pointed by the iterator.
       */
      ObjectPointerType Get(void);
      /**
       * Increment.
       */
      ConstIterator& operator++();
      /**
       * Decrement.
       */
      ConstIterator& operator--();
      /**
       * Difference comparison operator.
       */
      bool operator!=(const ConstIterator &it);
      /**
       * Equality comparison operator.
       */
      bool operator==(const ConstIterator &it);
      /**
       * Instantiation operator.
       */
      ConstIterator& operator=(const ConstIterator &it);
      /**
       * Instantiation operator.
       */
      ConstIterator& operator=(const Iterator &it);
      /**
       * Copy operator.
       */
      ConstIterator(const ConstIterator &it);
      /**
       * Copy operator.
       */
      ConstIterator(const Iterator &it);

    private:
      // Internal iterator.
      InternalConstIteratorType m_Iter;
    };
  /** \class ReverseIterator
   *  \brief ReverseIterator of the object list.
   */
  class ReverseIterator
    {
    public:
      friend class ReverseConstIterator;
      /** typedef of the internal iterator */
      typedef typename InternalContainerType::reverse_iterator InternalReverseIteratorType;
      /** Constructor */
      ReverseIterator();
      /** Constructor with iternal iterator parameter */
      ReverseIterator(InternalReverseIteratorType iter);
      /**
       * Get the current object.
       * \return The current object pointed by the iterator.
       */
      ObjectPointerType Get(void);
      /**
       * Increment.
       */
      ReverseIterator& operator++();
      /**
       * Decrement.
       */
      ReverseIterator& operator--();
      /**
       * Difference comparison operator.
       */
      bool operator!=(const ReverseIterator &it);
      /**
       * Equality comparison operator.
       */
      bool operator==(const ReverseIterator &it);
      /**
       * Instantiation operator.
       */
      ReverseIterator& operator=(const ReverseIterator &it);
      /**
       * Copy operator.
       */
      ReverseIterator(const ReverseIterator &it);

    private:
      // Internal iterator.
      InternalReverseIteratorType m_Iter;
    };
  /** \class ReverseConstIterator
   *  \brief ReverseConstIterator of the object list.
   */
  class ReverseConstIterator
    {
    public:
      /** typedef of the internal iterator */
      typedef typename InternalContainerType::reverse_iterator InternalReverseConstIteratorType;
      /** Constructor */
      ReverseConstIterator();
      /** Constructor with iternal iterator parameter */
      ReverseConstIterator(InternalReverseConstIteratorType iter);
      /**
       * Get the current object.
       * \return The current object pointed by the iterator.
       */
      ObjectPointerType Get(void);
      /**
       * Increment.
       */
      ReverseConstIterator& operator++();
      /**
       * Decrement.
       */
      ReverseConstIterator& operator--();
      /**
       * Difference comparison operator.
       */
      bool operator!=(const ReverseConstIterator &it);
      /**
       * Equality comparison operator.
       */
      bool operator==(const ReverseConstIterator &it);
      /**
       * Instantiation operator.
       */
      ReverseConstIterator& operator=(const ReverseConstIterator &it);
      /**
       * Instantiation operator.
       */
      ReverseConstIterator& operator=(const ReverseIterator &it);
      /**
       * Copy operator.
       */
      ReverseConstIterator(const ReverseConstIterator &it);
      /**
       * Copy operator.
       */
      ReverseConstIterator(const ReverseIterator &it);

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

 protected:
  /** Constructor */
  List();
  /** Destructor */
  ~List(){};
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

 private:
  List(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented
  /** The internal std::vector object container */
  InternalContainerType m_InternalContainer;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbList.txx"
#endif

#endif


