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

#include "itkPolyLineParametricPath.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbRoadExtractionFilter.h"
#include "otbDrawPathListFilter.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbPolyLineParametricPathWithValue.h"
#include "otbMath.h"

int otbRoadExtractionFilter(int itkNotUsed(argc), char * argv[])
{
  const unsigned int Dimension = 2;
  typedef otb::VectorImage<double, Dimension>                     InputImageType;
  typedef otb::Image<double, Dimension>                           OutputImageType;
  typedef otb::PolyLineParametricPathWithValue<double, Dimension> PathType;

  typedef otb::ImageFileReader<InputImageType>                                ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>                               WriterType;
  typedef otb::RoadExtractionFilter<InputImageType, PathType>                 RoadExtractionFilterType;
  typedef RoadExtractionFilterType::InputPixelType                            InputPixelType;
  typedef otb::DrawPathListFilter<OutputImageType, PathType, OutputImageType> DrawPathFilterType;

  //Parameters
  const char *   inputFileName(argv[1]);
  const char *   outputFileName(argv[2]);
  InputPixelType ReferencePixel;
  ReferencePixel.Reserve(4);
  ReferencePixel.SetElement(0, ::atof(argv[3]));
  ReferencePixel.SetElement(1, ::atof(argv[4]));
  ReferencePixel.SetElement(2, ::atof(argv[5]));
  ReferencePixel.SetElement(3, ::atof(argv[6]));
  const double Alpha = ::atof(argv[7]);
  const double AmplitudeThreshold = ::atof(argv[8]);
  const double Tolerance = ::atof(argv[9]);
  const double MaxAngle = ((otb::CONST_PI * ::atof(argv[10])) / 180.);
  const double FirstMeanDistanceThreshold  = ::atof(argv[11]);
  const double SecondMeanDistanceThreshold  = ::atof(argv[12]);
  const double LinkAngularThreshold = ((otb::CONST_PI * ::atof(argv[13])) / 180.);
  const double LinkDistanceThreshold  = ::atof(argv[14]);

  // Instantiating object
  ReaderType::Pointer               reader = ReaderType::New();
  RoadExtractionFilterType::Pointer roadExtraction = RoadExtractionFilterType::New();
  DrawPathFilterType::Pointer       draw = DrawPathFilterType::New();
  WriterType::Pointer               writer = WriterType::New();

  //Initialization parameters
  reader->SetFileName(inputFileName);

  roadExtraction->SetInput(reader->GetOutput());
  roadExtraction->SetReferencePixel(ReferencePixel);
  roadExtraction->SetAlpha(Alpha);
  roadExtraction->SetAmplitudeThreshold(AmplitudeThreshold);
  roadExtraction->SetTolerance(Tolerance);
  roadExtraction->SetMaxAngle(MaxAngle);
  roadExtraction->SetFirstMeanDistanceThreshold(FirstMeanDistanceThreshold);
  roadExtraction->SetSecondMeanDistanceThreshold(SecondMeanDistanceThreshold);
  roadExtraction->SetAngularThreshold(LinkAngularThreshold);
  roadExtraction->SetDistanceThreshold(LinkDistanceThreshold);

  reader->GenerateOutputInformation();
  OutputImageType::Pointer image = OutputImageType::New();
  image->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
  image->Allocate();
  image->FillBuffer(0);

  draw->SetInput(image);
  //Use internal value of path
  draw->UseInternalPathValueOn();
  draw->SetInputPath(roadExtraction->GetOutput());

  writer->SetFileName(outputFileName);
  writer->SetInput(draw->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
