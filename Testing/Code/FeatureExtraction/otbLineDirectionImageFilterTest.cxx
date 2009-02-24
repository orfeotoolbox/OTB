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
//#include "otbStreamingImageFileWriter.h"
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


  typedef otb::VectorImage<PixelType,Dimension>                           ImageType;
  typedef ImageType::PixelType                                InputPixelType;
  typedef otb::VectorImage<PixelType,Dimension>                     VectorImageType;
  typedef otb::ImageFileReader<ImageType>                           ReaderType;
  //typedef otb::StreamingImageFileWriter<VectorImageType>            WriterType;
  typedef otb::ImageFileWriter<VectorImageType>            WriterType;
  typedef otb::LineDirectionImageFilter<ImageType, VectorImageType> FilterType;

  FilterType::Pointer filter = FilterType::New(); 
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();




  reader->SetFileName(inName);
  reader->GenerateOutputInformation();
  writer->SetFileName(outName);

  InputPixelType spect;
  // TO MODIFY
  //spect.SetSize(reader->GetOutput()->GetNumberOfComponentsPerPixel());
  //spect.Fill(spectThresh);
  filter->SetSpectralThreshold(spectThresh);
  filter->SetSpatialThreshold(spatialThresh);
  filter->SetNumberOfDirections(dirNb);
  filter->SetRatioMaxConsiderationNumber(maxConsideration);
  filter->SetAlpha(alpha);
  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetOutput() );

  writer->Update();
 

  return EXIT_SUCCESS;
}
