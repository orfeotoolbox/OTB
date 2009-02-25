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
#include "itkExceptionObject.h"

#include "otbLineDirectionImageFilter.h"
#include "otbUnaryFunctorNeighborhoodImageFilter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImageFileWriter.h"


int otbLineDirectionImageFilterTest(int argc, char * argv[])
{
  typedef double PixelType;
  const unsigned int Dimension =2;

  std::string inName            = argv[1];
  std::string outName           = argv[2];
  PixelType spectThresh         = atof(argv[3]);
  unsigned int spatialThresh    = atoi(argv[4]);
  unsigned int dirNb            = atoi(argv[5]);
  unsigned int maxConsideration = atoi(argv[6]);
  double alpha                  = atof(argv[7]);  


  typedef otb::Image<PixelType,Dimension>                           ImageType;
  typedef ImageType::PixelType                                InputPixelType;
  typedef otb::VectorImage<PixelType,Dimension>                     VectorImageType;
  typedef otb::ImageFileReader<ImageType>                           ReaderType;
  //typedef otb::StreamingImageFileWriter<VectorImageType>            WriterType;
  typedef otb::ImageFileWriter<ImageType>            WriterType;
  typedef otb::LineDirectionImageFilter<ImageType, ImageType> FilterType;

  FilterType::Pointer filter = FilterType::New(); 
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();




  reader->SetFileName(inName);
  reader->GenerateOutputInformation();
  writer->SetFileName(outName);

  
  filter->SetSpectralThreshold(spectThresh);
  filter->SetSpatialThreshold(spatialThresh);
  filter->SetNumberOfDirections(dirNb);
  filter->SetRatioMaxConsiderationNumber(maxConsideration);
  filter->SetAlpha(alpha);
  for(unsigned int i = 1; i<7; i++)
    {
      filter->SetTextureStatus(i, false);
    }
  filter->SetTextureStatus(1, true);
  
  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetLengthOutput() );

  writer->Update();
 

  return EXIT_SUCCESS;
}
