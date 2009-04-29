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

#ifndef __otbVectorDataToImageFilter_txx
#define __otbVectorDataToImageFilter_txx

#include "otbVectorDataToImageFilter.h"

namespace otb
{

/**
   * Constructor
 */
  template <class TVectorData, class TImage>
      VectorDataToImageFilter<TVectorData, TImage>
  ::VectorDataToImageFilter()
  {
    this->SetNumberOfRequiredInputs( 1 );
  }


  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::SetInput(const VectorDataType *input)
  {
// Process object is not const-correct so the const_cast is required here
    this->itk::ProcessObject::SetNthInput(0,
                                          const_cast< VectorDataType * >( input ) );
  }

  template <class TVectorData, class TImage>
      const typename VectorDataToImageFilter<TVectorData, TImage>::VectorDataType *
          VectorDataToImageFilter<TVectorData, TImage>
  ::GetInput(void)
  {
    if (this->GetNumberOfInputs() < 1)
    {
      return 0;
    }

    return static_cast<const TVectorData * >
        (this->itk::ProcessObject::GetInput(0) );
  }



  /**
   * Generate Data
   */
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::GenerateData(void)
  {

  }




  /**
   * PrintSelf Method
   */
  template <class TVectorData, class TImage>
      void
          VectorDataToImageFilter<TVectorData, TImage>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
}

#endif