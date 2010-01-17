/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) CS Systemes d'information. All rights reserved.
  See CSCopyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "otbImageToSURFKeyPointSetFilter.h"
#include "otbImage.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"


int otbImageToSURFKeyPointSetFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double PixelType;

  typedef otb::Image<PixelType,Dimension> ImageType;
  typedef itk::VariableLengthVector<PixelType> RealVectorType;
  typedef itk::PointSet<RealVectorType,Dimension> PointSetType;
  typedef otb::ImageToSURFKeyPointSetFilter<ImageType,PointSetType> FilterType;

  // Instantiating object
  FilterType::Pointer object = FilterType::New();


  return EXIT_SUCCESS;
}
