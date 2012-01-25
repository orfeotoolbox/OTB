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
#include "itkMacro.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbMeanShiftVectorImageFilter.h"
#include "otbStreamingShrinkImageFilter.h"


int main(int argc, char * argv[])
{
  const char *       infname                = argv[1];
  const unsigned int spatialRadius          = atoi(argv[2]);
  const double       rangeRadius            = atof(argv[3]);
  const unsigned int minRegionSize          = atoi(argv[4]);
  unsigned int       factor                 = atoi(argv[5]);

  const unsigned int Dimension = 2;
  typedef float                                           PixelType;
  typedef otb::VectorImage<PixelType, Dimension>          ImageType;
  typedef otb::ImageFileReader<ImageType>                 ReaderType;
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType> FilterType;
  typedef otb::StreamingShrinkImageFilter<ImageType, ImageType> ShrinkType;

  // Instantiating object
  FilterType::Pointer filter   = FilterType::New();
  ReaderType::Pointer reader   = ReaderType::New();
  ShrinkType::Pointer shrinker = ShrinkType::New();

  reader->SetFileName(infname);
  
    
  filter->SetSpatialRadius(spatialRadius);
  filter->SetRangeRadius(rangeRadius);
  filter->SetMinimumRegionSize(minRegionSize);
  filter->SetInput(reader->GetOutput());
  filter->UpdateOutputInformation();

  shrinker->SetInput(filter->GetOutput());
  shrinker->SetShrinkFactor(factor);
  shrinker->Update();

  return EXIT_SUCCESS;
}
