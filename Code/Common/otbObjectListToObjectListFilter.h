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
#ifndef __otbObjectListToObjectListFilter_h
#define __otbObjectListToObjectListFilter_h

#include "otbObjectListSource.h"

namespace otb
{
  
/** \class ObjectListToObjectListFilter
   * \brief Base class for filters that take an ObjectList as input and produce an ObjectList as output. 
   *
   * ObjectListToObjectListFilter is the base class for all process objects that output 
   * ObjectList data and require ObjectList data as input. Specifically, this class 
   * defines the SetInput() method for defining the input to a filter.
   * 
   *
   * \ingroup ObjectListFilter  
 */
  template <class TInputList, class TOutputList >
      class ITK_EXPORT ObjectListToObjectListFilter : public otb::ObjectListSource<TOutputList>
      {
        public:
          /** Standard class typedefs. */
          typedef ObjectListToObjectListFilter  Self;
          typedef otb::ObjectListSource<TOutputList>  Superclass;
          typedef itk::SmartPointer<Self>   Pointer;
          typedef itk::SmartPointer<const Self>  ConstPointer;

          /** Method for creation through the object factory. */
          itkNewMacro(Self);
  
          /** Run-time type information (and related methods). */
          itkTypeMacro(ObjectListToObjectListFilter, ObjectListSource);

          /** Some typedefs. */
          typedef TInputList InputListType;
          typedef TOutputList OutputListType;
          typedef typename TInputList::ConstPointer InputListPointer;
          typedef typename TOutputList::Pointer OutputListPointer;
          typedef typename TInputList::ConstIterator InputListIterator;
  
          typedef itk::DataObject::Pointer DataObjectPointer;

  
          virtual void SetInput( const InputListType *input);
          const InputListType * GetInput(void);

  
        protected:
          ObjectListToObjectListFilter();
          virtual ~ObjectListToObjectListFilter() {};


        private:
          ObjectListToObjectListFilter(const Self&); //purposely not implemented
          void operator=(const Self&); //purposely not implemented

      };

} // end namespace otb

#ifndef ITK_MANUAL_INSTANTIATION
#include "otbObjectListToObjectListFilter.txx"
#endif

#endif
