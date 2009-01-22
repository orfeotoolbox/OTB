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


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbExtractROI.h"
#include "itkIdentityTransform.h"
#include "otbStreamingResampleImageFilter.h"


// Pipeline reader -> Resampler (with Identity transform) -> ExtractROI
// -> Writer
// the resample is optionaly removed with the command line parameter.
//

int otbExtractROIResample(int argc, char* argv[])
{
  if( argc < 4 )
  {
    std::cout << argv[0] <<" <input filename> <output filename> <use resample>"  << std::endl;
    return EXIT_FAILURE;
  }

  bool useResample = true;
  if (atoi(argv[3]) == 0)
  {
    useResample = false;
  }

  unsigned int startX = 10;
  unsigned int startY= 10;
  unsigned int sizeX = 100;
  unsigned int sizeY = 100;

  typedef double PixelType;
  typedef otb::Image<PixelType,2> ImageType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer filter = ReaderType::New();

  filter->SetFileName(argv[1]);

  filter->UpdateOutputInformation();


  typedef itk::IdentityTransform< double, 2 > IdentityTransformType;
  IdentityTransformType::Pointer transform = IdentityTransformType::New();

  typedef otb::StreamingResampleImageFilter<ImageType, ImageType, double> ResampleType;
  ResampleType::Pointer filterResample = ResampleType::New();
  filterResample->SetTransform(transform);
  filterResample->SetInput(filter->GetOutput());

  typedef otb::ExtractROI<PixelType, PixelType> ExtractROIType;


  ExtractROIType::Pointer filterResampleRoi = ExtractROIType::New();
  filterResampleRoi->SetStartX(startX);
  filterResampleRoi->SetStartY(startY);
  filterResampleRoi->SetSizeX(sizeX);
  filterResampleRoi->SetSizeY(sizeY);

  if (useResample)
  {
    filterResampleRoi->SetInput(filterResample->GetOutput());
  }
  else
  {
    filterResampleRoi->SetInput(filter->GetOutput());
  }

  typedef otb::StreamingImageFileWriter<ImageType> WriterType;
  WriterType::Pointer filterResampleWriter = WriterType:: New();
  filterResampleWriter->SetFileName(argv[2]);
  filterResampleWriter->SetInput(filterResampleRoi->GetOutput());
  filterResampleWriter->Update();

  return EXIT_SUCCESS;
}
