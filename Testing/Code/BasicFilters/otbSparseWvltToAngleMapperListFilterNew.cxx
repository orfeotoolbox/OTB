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

#include "otbMacro.h"
#include "otbObjectList.h"
#include "otbImage.h"
#include "otbWaveletOperator.h"
#include "otbWaveletFilterBank.h"
#include "otbWaveletTransform.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbCommandLineArgumentParser.h"

#include "itkListSample.h"
#include "itkFixedArray.h"

#include "otbSparseWvltToAngleMapperListFilter.h"

int otbSparseWvltToAngleMapperListFilterNewTest ( int argc, char * argv[] )
{
  const unsigned int Dimension = 2;
  const unsigned int SpaceDimension = 3;
  
  typedef float  PixelType;
  typedef double PrecisionType;
  
  typedef itk::FixedArray< PrecisionType, SpaceDimension > AngleType;
  typedef itk::Statistics::ListSample< AngleType >         AngleListType;

  typedef otb::Image< PixelType, Dimension >  ImageType;
  typedef otb::ImageList< ImageType >         ImageListType;
  typedef otb::SparseWvltToAngleMapperListFilter< 
    ImageListType, AngleListType, SpaceDimension+1 > AngleListFilterType;

  AngleListFilterType::Pointer filter = AngleListFilterType::New();

  return EXIT_SUCCESS;
}

