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
#ifndef _otbObjectList_h
#define _otbObjectList_h

#include <vector>
#include "itkDataObject.h"
#include "itkObjectFactory.h"

namespace otb 
{
/** \class ObjectList
 *  \brief This class is a generic all-purpose wrapping around an std::vector<SmartPointer<ObjectType>>.
 *
 * ObjectList stores SmartPointer to ObjectType in a std::vector data structure. It provides the same
 * methods and iterators interfaces.
 */
template <class TObject>
class ITK_EXPORT ObjectList : public itk::DataObject
{
 public:
  /** Standard typedefs */
  typedef ObjectList Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkTypeMacro(ObjectList,DataObject);

  /** Creation through object factory macro */
  itkNewMacro(Self);

  /** Template parameter typedefs */
  typedef TObject ObjectType;
  typedef itk::SmartPointer<ObjectType> ObjectPointerType;
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
  void PushBack(ObjectPointerType element);
  /**
   * Set the nth element of the list.
   * \param index The index where to put the element.
   * \param element Pointer to the element to set.
   */
  void SetNthElement(unsigned int index, ObjectPointerType element);
  /**
   * Get the nth element of the list.
   * \param index The index of the object to get.
   * \return The pointer to the nth element of the list.
   */
  ObjectPointerType GetNthElement(unsigned int index);
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

  /** \class Iterator
   *  \brief Iterator of the object list.
   */
  class ITK_EXPORT Iterator
    {
    public:
      friend class ObjectList;
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

	  /** 
	    * Get the current internal iterator 
		*/
	  InternalIteratorType & GetIter(void)
	  {
		return(m_Iter);
	  }
    private:
      // Internal iterator.
      InternalIteratorType m_Iter;
    };
  /** \class ConstIterator
   *  \brief ConstIterator of the object list.
   */
  class ITK_EXPORT ConstIterator
    {
    public:
	  friend class ObjectList;
      friend class Iterator;
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
  class ITK_EXPORT ReverseIterator
    {
    public:
	  friend class ObjectList;
      friend class Iterator;

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
  ObjectList();
  /** Destructor */
  ~ObjectList(){};
  /**PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

 private:
  ObjectList(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented
  /** The internal std::vector object container */
  InternalContainerType m_InternalContainer;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbObjectList.txx"
#endif

#endif


