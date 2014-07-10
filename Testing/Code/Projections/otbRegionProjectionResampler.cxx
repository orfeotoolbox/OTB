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


/*!
 *
 * PURPOSE:
 *
 * Appliaction to project a geographic coordinated image region
 * using an Interpolator, a Regionextractor and an  Iteratorregion.
 *
 */

// iostream is used for general output
#include <iostream>

#include "otbMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbStreamingResampleImageFilter.h"


#include "itkExtractImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkTranslationTransform.h"

#include "otbInverseSensorModel.h"
#include "otbCompositeTransform.h"

int otbRegionProjectionResampler(int argc, char* argv[])
{
  if (argc != 10)
    {
    std::cout << argv[0] <<
    " <input filename> <output filename> <latitude de l'origine> <longitude de l'origine> <taille_x> <taille_y> <NumberOfstreamDivisions>"
              << std::endl;

    return EXIT_FAILURE;
    }

  typedef otb::Image<unsigned char, 2>                               CharImageType;
  typedef otb::Image<double, 2>                                      ImageType;
  typedef otb::ImageFileReader<ImageType>                            ReaderType;
  typedef otb::ImageFileWriter<ImageType>                   WriterType;
  typedef otb::InverseSensorModel<double>                            ModelType;
  typedef itk::LinearInterpolateImageFunction<ImageType, double>     InterpolatorType;
  typedef itk::RescaleIntensityImageFilter<ImageType, CharImageType> RescalerType;
  typedef otb::StreamingResampleImageFilter<ImageType, ImageType>    ResamplerType;

  ImageType::IndexType   start;
  ImageType::SizeType    size;
  ImageType::SpacingType spacing;
  ImageType::PointType   origin;
  ImageType::RegionType  region;

  //Allocate pointer
  ModelType::Pointer        model        = ModelType::New();
  ReaderType::Pointer       reader       = ReaderType::New();
  WriterType::Pointer       writer       = WriterType::New();
  InterpolatorType::Pointer interpolator = InterpolatorType::New();
  RescalerType::Pointer     rescaler     = RescalerType::New();
  ResamplerType::Pointer    resampler    = ResamplerType::New();

  // Set parameters ...
  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);
  rescaler->SetOutputMinimum(0);
  rescaler->SetOutputMaximum(255);

  // Read meta data (ossimKeywordlist)
  reader->GenerateOutputInformation();
  ImageType::ConstPointer inputImage = reader->GetOutput();

 model->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());

 if( model->IsValidSensorModel() == false )
   {
     std::cout<<"Invalid Model pointer m_Model == NULL!\n The ossim keywordlist is invalid!"<<std::endl;
     return EXIT_FAILURE;
   }

  start[0] = 0;
  start[1] = 0;
  size[0] = atoi(argv[5]);  // X size.
  size[1] = atoi(argv[6]);  // Y size.

  region.SetSize(size);
  region.SetIndex(start);

  spacing[0] = atof(argv[8]);
  spacing[1] = atof(argv[9]);

  origin[0] = strtod(argv[3], NULL);         // origin longitude.
  origin[1] = strtod(argv[4], NULL);         // origin latitude.

  otbGenericMsgDebugMacro(<< "Origin " << origin);

  resampler->SetOutputSpacing(spacing);
  resampler->SetOutputOrigin(origin);
  resampler->SetOutputSize(region.GetSize());
  resampler->SetOutputStartIndex(start);

  //Connect pipeline
  resampler->SetInput(reader->GetOutput());
  resampler->SetTransform(model);
  resampler->SetInterpolator(interpolator);

  otbGenericMsgDebugMacro(<< "Sensor Model :" << model);

  writer->SetInput(resampler->GetOutput());
  writer->SetNumberOfDivisionsTiledStreaming(10);
  otbGenericMsgDebugMacro(<< "Update writer ...");
  writer->Update();

  return EXIT_SUCCESS;

} //Fin main()
