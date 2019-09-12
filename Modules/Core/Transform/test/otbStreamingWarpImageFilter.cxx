/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbVectorImage.h"
#include "itkVector.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStreamingWarpImageFilter.h"

// Images definition
const unsigned int Dimension = 2;
typedef double                                      PixelType;
typedef otb::Image<PixelType, Dimension>            ImageType;
typedef itk::Vector<PixelType, 2>                   DisplacementValueType;
typedef otb::Image<DisplacementValueType, Dimension> DisplacementFieldType;

  // Warper
  typedef otb::StreamingWarpImageFilter<ImageType, ImageType, DisplacementFieldType> ImageWarperType;


int otbStreamingWarpImageFilter(int argc, char* argv[])
{
  if (argc != 5)
    {
    std::cout << "usage: " << argv[0] << "infname deffname outfname radius" << std::endl;
    return EXIT_SUCCESS;
    }

  // Input parameters
  const char * infname = argv[1];
  const char * deffname = argv[2];
  const char * outfname = argv[3];
  const double maxdef = atoi(argv[4]);


  // Change default output origin
  ImageType::PointType origin;
  origin.Fill(0.5);

  // Reader/Writer
  typedef otb::ImageFileReader<ImageType>            ReaderType;
  typedef otb::ImageFileReader<DisplacementFieldType> DisplacementReaderType;
  typedef otb::ImageFileWriter<ImageType>   WriterType;

  // Objects creation
  DisplacementReaderType::Pointer displacementReader = DisplacementReaderType::New();
  ReaderType::Pointer            reader = ReaderType::New();
  WriterType::Pointer            writer = WriterType::New();
  ImageWarperType::Pointer       warper = ImageWarperType::New();

  // Reading
  reader->SetFileName(infname);
  displacementReader->SetFileName(deffname);

  // Warping
  DisplacementValueType maxDisplacement;
  maxDisplacement.Fill(maxdef);
  warper->SetMaximumDisplacement(maxDisplacement);
  warper->SetInput(reader->GetOutput());
  warper->SetDisplacementField(displacementReader->GetOutput());
  warper->SetOutputOrigin(origin);

  // Writing
  writer->SetInput(warper->GetOutput());
  writer->SetFileName(outfname);
  writer->Update();

  return EXIT_SUCCESS;
}

int otbStreamingWarpImageFilterEmptyRegion(int itkNotUsed(argc), char* itkNotUsed(argv)[])
{
  ImageType:: Pointer inputPtr = ImageType::New();

  ImageType::RegionType largestRegion;
  ImageType::SizeType largestSize = {{10,10}};
  ImageType::IndexType largestIndex = {{1,1}};
  
  largestRegion.SetIndex(largestIndex);
  largestRegion.SetSize(largestSize);

  inputPtr->SetRegions(largestRegion);

  ImageType::RegionType emptyRegion;
  ImageType::SizeType emptySize = {{0,0}};
  ImageType::IndexType emptyIndex = {{0,0}};
  emptyRegion.SetSize(emptySize);
  emptyRegion.SetIndex(emptyIndex);

  inputPtr->SetRequestedRegion(emptyRegion);
  inputPtr->SetBufferedRegion(emptyRegion);

  DisplacementFieldType::Pointer dispPtr = DisplacementFieldType::New();
  dispPtr->SetRegions(largestRegion);
  dispPtr->Allocate();
  
  DisplacementValueType v;
  v[0]=-100;
  v[1]=-100;
  dispPtr->FillBuffer(v);

  ImageWarperType::Pointer warper = ImageWarperType::New();

  warper->SetDisplacementField(dispPtr);
  warper->SetInput(inputPtr);

  ImageType::PointType outputOrigin;
  outputOrigin.Fill(0);
  warper->SetOutputOrigin(outputOrigin);

  // requested region for full output is completely outside largest
  // possible region of input
  warper->GetOutput()->UpdateOutputInformation();

  // Before bugfix this would lead to famous ITK exception outside of
  // largest possible region
  warper->GetOutput()->PropagateRequestedRegion();

  // After requested region has been propagated, we need to be sure
  // that requested region can be cropped by largest region
  auto requestedRegion = inputPtr->GetRequestedRegion();

  if (! requestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    std::cerr<<"Requested region can not be cropped by largest region"<<std::endl;
    return EXIT_FAILURE;
    }
  
  // And we also need to check that requested region is not largest
  // region
  if( inputPtr->GetRequestedRegion().GetNumberOfPixels() != 0)
    {
    std::cerr<<"Requested region should have {{0, 0}} size"<<std::endl;
    return EXIT_FAILURE;
    }
    
  return EXIT_SUCCESS;
}
