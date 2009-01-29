/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.

Copyright (c) CS systèmes d'information. All rights reserved.
See CSCopyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbPointSetToDensityImageFilter.h"


namespace otb
{
  /**---------------------------------------------------------
   * Constructor
   ----------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  PointSetToDensityImageFilter<  TInputPointSet ,  TOutputImage  >
  ::PointSetToDensityImageFilter()
  {
  }


 /*---------------------------------------------------------
  * Destructor.c
  ----------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  PointSetToDensityImageFilter< TInputPointSet ,  TOutputImage >
  ::~PointSetToDensityImageFilter()
  {}

 /*-------------------------------------------------------
  * Generate Data
  --------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  void
  PointSetToDensityImageFilter<  TInputPointSet ,  TOutputImage>
  ::GenerateData(void)
  {


  }/** End of GenerateData()*/

  /*----------------------------------------------------------------
    PrintSelf
    -----------------------------------------------------------------*/
  template <class TInputPointSet , class TOutputImage  >
  void
  PointSetToDensityImageFilter< TInputPointSet ,  TOutputImage >
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
}
