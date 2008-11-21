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
#ifndef __otbObjectListToObjectListFilter_txx
#define __otbObjectListToObjectListFilter_txx

#include "otbObjectListToObjectListFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
   * Constructor
 */
  template <class TInputList, class TOutputList>
      ObjectListToObjectListFilter<TInputList,TOutputList>
  ::ObjectListToObjectListFilter()
  {
    this->SetNumberOfRequiredInputs( 1 );
  }



  template <class TInputList, class TOutputList>
      void
          ObjectListToObjectListFilter<TInputList,TOutputList>
  ::SetInput(const InputListType *input)
  {
 // Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(0, 
                                          const_cast< InputListType * >( input ) );
  }

  template <class TInputList, class TOutputList>
      const typename ObjectListToObjectListFilter<TInputList,TOutputList>::InputListType *
          ObjectListToObjectListFilter<TInputList,TOutputList>
  ::GetInput(void) 
  {
    if (this->GetNumberOfInputs() < 1)
    {
      return 0;
    }
  
    return static_cast<const TInputList * >
        (this->itk::ProcessObject::GetInput(0) );
  }

/**
   * PrintSelf Method
 */
  template <class TInputList, class TOutputList>
      void
          ObjectListToObjectListFilter<TInputList,TOutputList>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

} // end namespace otb

#endif
