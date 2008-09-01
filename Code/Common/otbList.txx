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
#ifndef __otbList_txx
#define __otbList_txx

#include "otbList.h"

namespace otb 
{
  /**
   * Constructor
   */
  template <class TObject>
  List<TObject>
  ::List()
  {}
  /**
   * Set the minimum capacity of the vector.
   * \param size Size of the vector to reserve.
   */
  template <class TObject>
  void 
  List<TObject>
  ::Reserve(unsigned int size)
  {
    m_InternalContainer.reserve(size);
  }
  /**
   * Get the capacity of the vector.
   * \return The capacity of the vector. 
   */
  template <class TObject>
  unsigned int 
  List<TObject>
  ::Capacity(void)
  {
    return m_InternalContainer.capacity();
  }
  /**
   * Get the number of elements in the vector.
   * \return The number of elements in the vector.
   */
  template <class TObject>
  unsigned int 
  List<TObject>
  ::Size(void)
  {
    return m_InternalContainer.size();
  }
  /**
   * Resize the maximal list capacity.
   * \param size The new maximal size of the list.
   */
  template <class TObject>
  void 
  List<TObject>
  ::Resize(unsigned int size)
  {
    m_InternalContainer.resize(size);
  }
  /**
   * Append an element to the list.
   * \param element Pointer to the element to append.
   */
  template <class TObject>
  void 
  List<TObject>
  ::PushBack(const ObjectType* element)
  {
    m_InternalContainer.push_back(const_cast<ObjectType*>(element));
  }
  /**
   * Delete the last element to the list.
   */
  template <class TObject>
  void 
  List<TObject>
  ::PopBack(void)
  {
    if( m_InternalContainer.size() > 0 ) 
    {
        m_InternalContainer.pop_back();
    }
  }
  /**
   * Set the nth element of the list.
   * \param index The index where to put the element.
   * \param element Pointer to the element to set.
   */
  template <class TObject>
  void 
  List<TObject>
  ::SetNthElement(unsigned int index,const ObjectType * element)
  {
    m_InternalContainer[index]=const_cast<ObjectType*>(element);
  }
  /**
   * Get the nth element of the list.
   * \param index The index of the object to get.
   * \return The pointer to the nth element of the list.
   */
  template <class TObject>
  typename List<TObject>::ObjectType *
  List<TObject>
  ::GetNthElement(unsigned int index)
  {
    return m_InternalContainer[index].GetPointer();
  }
  /**
   * Get the nth element of the const list.
   * \param index The index of the object to get.
   * \return The pointer to the nth element of the list.
   */
/*  template <class TObject>
  typename const List<TObject>::ObjectType *
  List<TObject>
  ::GetNthElement(unsigned int index)const
  {
    return m_InternalContainer[index].GetPointer();
  }*/
/**
   * Return the first element of the list.
   * \return The first element of the list.
   */
  template <class TObject>
  typename List<TObject>::ObjectType *
  List<TObject>
  ::Front(void)
  {
    return m_InternalContainer.front().GetPointer();
  }
/**
   * Return the first element of the const list.
   * \return The first element of the list.
   */
/*  template <class TObject>
  typename const List<TObject>::ObjectType *
  List<TObject>
  ::Front(void)const
  {
    return m_InternalContainer.front().GetPointer();
  }*/
  /**
   * Return the last element of the list.
   * \return The last element of the list.
   */
  template <class TObject>
  typename List<TObject>::ObjectType *
  List<TObject>
  ::Back(void)
  {
    return m_InternalContainer.back().GetPointer();
  }
  /**
   * Return the last element of the const list.
   * \return The last element of the list.
   */
/*  template <class TObject>
  typename const List<TObject>::ObjectType *
  List<TObject>
  ::Back(void)const
  {
    return m_InternalContainer.back().GetPointer();
  }*/
  /**
   * Erase the nth element in the list.
   * \param index The index of the element to erase.
   */
  template <class TObject>
  void
  List<TObject>
  ::Erase(unsigned int index)
  {
    m_InternalContainer.erase(m_InternalContainer.begin()+index);
  }
  /**
   * Clear the object list.
   */
  template <class TObject>
  void
  List<TObject>
  ::Clear(void)
  {
    m_InternalContainer.clear();
  }

  /**
   * Get an Iterator that points to the beginning of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename List<TObject>::Iterator 
  List<TObject>
  ::Begin(void)
  {
    Iterator iter(m_InternalContainer.begin());
    return iter;
  }
  /**
   * Get a ConstIterator that points to the beginning of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename List<TObject>::ConstIterator
  List<TObject>
  ::Begin(void) const
  {
    ConstIterator iter(m_InternalContainer.begin());
    return iter;
  }
  /**
   * Get a ReverseIterator that points to the reverse beginning of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename List<TObject>::ReverseIterator
  List<TObject>
  :: ReverseBegin(void)
  {
    ReverseIterator iter(m_InternalContainer.rbegin());
    return iter;
  }
  /**
   * Get a ReverseConstIterator that points to the reverse beginning of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename List<TObject>::ReverseConstIterator 
  List<TObject>
  ::ReverseBegin(void) const
  {
    ReverseConstIterator iter(m_InternalContainer.rbegin());
    return iter;
  }
  /**
   * Get an Iterator that points past-the-end of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename List<TObject>::Iterator
  List<TObject>
  :: End(void)
  { 
    Iterator iter(m_InternalContainer.end());
    return iter;
  }
  /**
   * Get a ConstIterator that points past-the-end of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename List<TObject>::ConstIterator 
  List<TObject>
  ::End(void) const
  {
    ConstIterator iter(m_InternalContainer.end());
    return iter;
  }
  /**
   * Get a ReverseIterator that points to the reverse past-the-end of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename List<TObject>::ReverseIterator 
  List<TObject>
  ::ReverseEnd(void)
  {
    ReverseIterator iter(m_InternalContainer.rend());
    return iter;
  }
  /**
   * Get a ReverseConstIterator that points to the reverse past-the-end of the container.
   * \return The iterator.
   */
  template <class TObject>
  typename List<TObject>::ReverseConstIterator 
  List<TObject>
  ::ReverseEnd(void) const
  {
    ReverseConstIterator iter(m_InternalContainer.rend());
    return iter;
  } 
  /**
   * Erase elements from begin to last.
   * \param begin Iterator pointing on first object to erase.
   * \param end Iterator pointing past the last object to erase.
   */
  template <class TObject>
  void
  List<TObject>
  ::Erase(Iterator begin, Iterator end)
  {
    m_InternalContainer.erase(begin.GetIter(),end.GetIter());
  }
  /**PrintSelf method */
  template <class TObject>
  void 
  List<TObject>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
        Superclass::PrintSelf(os,indent);
        os << indent << "Size: " << m_InternalContainer.size() << std::endl;
        os << indent << "List contains : " << std::endl;
        ConstIterator iter = this->Begin();
        while(iter!=this->End())
	{
                os << indent.GetNextIndent() << iter.Get().GetPointer() << std::endl;
                os << indent.GetNextIndent() << iter.Get() << std::endl;
//                iter.Get()->PrintSelf(os,indent.GetNextIndent());
//                iter.Get()->Print(os,indent.GetNextIndent());
	        ++iter;
	}
  }
} // end namespace otb

#endif
