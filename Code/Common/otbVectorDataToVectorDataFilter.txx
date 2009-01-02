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
#ifndef __otbVectorDataToVectorDataFilter_txx
#define __otbVectorDataToVectorDataFilter_txx

#include "otbVectorDataToVectorDataFilter.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
   * Constructor
 */
  template <class TInputVectorData, class TOutputVectorData>
      VectorDataToVectorDataFilter<TInputVectorData,TOutputVectorData>
  ::VectorDataToVectorDataFilter()
  {
    this->SetNumberOfRequiredInputs( 1 );
  }



  template <class TInputVectorData, class TOutputVectorData>
      void
          VectorDataToVectorDataFilter<TInputVectorData,TOutputVectorData>
  ::SetInput(const InputVectorDataType *input)
  {
 // Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(0,
                                          const_cast< InputVectorDataType * >( input ) );
  }

  template <class TInputVectorData, class TOutputVectorData>
      const typename VectorDataToVectorDataFilter<TInputVectorData,TOutputVectorData>::InputVectorDataType *
          VectorDataToVectorDataFilter<TInputVectorData,TOutputVectorData>
  ::GetInput(void)
  {
    if (this->GetNumberOfInputs() < 1)
    {
      return 0;
    }

    return static_cast<const TInputVectorData * >
        (this->itk::ProcessObject::GetInput(0) );
  }

/**
   * PrintSelf Method
 */
  template <class TInputVectorData, class TOutputVectorData>
      void
          VectorDataToVectorDataFilter<TInputVectorData,TOutputVectorData>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

} // end namespace otb

#endif
