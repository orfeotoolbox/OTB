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
#include "otbAdhesionCorrectionFilter.h"
#include "otbImageFileWriter.h"
#include "otbStandardWriterWatcher.h"
#include "otbImageFileReader.h"

const unsigned int Dimension = 2;
typedef double                                                        PixelType;
typedef otb::Image<PixelType, Dimension>                              ImageType;
typedef unsigned char                                                 MaskPixelType;
typedef otb::Image<MaskPixelType, Dimension>                          MaskImageType;


typedef otb::ImageFileReader<ImageType>                      ReaderType;
typedef otb::ImageFileReader<MaskImageType>                  MaskReaderType;

typedef otb::ImageFileWriter<ImageType>                      WriterType;
typedef otb::ImageFileWriter<MaskImageType>                  MaskWriterType;

typedef otb::AdhesionCorrectionFilter<ImageType, MaskImageType> AdhesionCorrectionFilterType;

int otbAdhesionCorrectionFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{

  // Instantiation
  AdhesionCorrectionFilterType::Pointer filter = AdhesionCorrectionFilterType::New();

  return EXIT_SUCCESS;
}

int otbAdhesionCorrectionFilter(int argc, char* argv[])
{

  if (argc != 12)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << "MedianDisparityFileName MedianMaskFileName SubpixelDisparityFileName SubpixelMaskFileName CannyRefFileName CannyMedianFileName Radius Tolerance";
    std::cerr << " CorrectedDisparityFileName CorrectedMaskFileName RiskEdgesFileName";
    return EXIT_FAILURE;
    }

  const char* medianDisparityFileName = argv[1];
  const char* medianMaskFileName = argv[2];
  const char* subpixelDisparityFileName = argv[3];
  const char* subpixelMaskFileName = argv[4];
  const char* cannyRefFileName = argv[5];
  const char* cannyMedianFileName = argv[6];
  const unsigned int radius = atoi(argv[7]);
  const float tolerance = atof(argv[8]);
  const char* correctedDisparityFileName = argv[9];
  const char* correctedMaskFileName = argv[10];
  const char* riskEdgesFileName = argv[11];

  typedef otb::ImageFileReader<ImageType> DisparityMapReaderType;
  DisparityMapReaderType::Pointer mediandisparityreader = DisparityMapReaderType::New();
  mediandisparityreader->SetFileName(medianDisparityFileName);

  MaskReaderType::Pointer medianmaskreader = MaskReaderType::New();
  medianmaskreader->SetFileName(medianMaskFileName);

  DisparityMapReaderType::Pointer subpixeldisparityreader = DisparityMapReaderType::New();
  subpixeldisparityreader->SetFileName(subpixelDisparityFileName);

  MaskReaderType::Pointer subpixelmaskreader = MaskReaderType::New();
  subpixelmaskreader->SetFileName(subpixelMaskFileName);

  /// Edge images are read from Canny filtering performed with MARC2
  ReaderType::Pointer cannyrefReader = ReaderType::New();
  cannyrefReader->SetFileName(cannyRefFileName);

  ReaderType::Pointer cannymedianReader = ReaderType::New();
  cannymedianReader->SetFileName(cannyMedianFileName);

  AdhesionCorrectionFilterType::Pointer adhesioncorrectionfilter = AdhesionCorrectionFilterType::New();

  adhesioncorrectionfilter->SetInput(cannyrefReader->GetOutput());
  adhesioncorrectionfilter->SetMedianDisparityInput(mediandisparityreader->GetOutput());
  adhesioncorrectionfilter->SetMedianMaskInput(medianmaskreader->GetOutput());
  adhesioncorrectionfilter->SetEdgesDisparityInput(cannymedianReader->GetOutput());
  adhesioncorrectionfilter->SetSubPixelMaskInput(subpixelmaskreader->GetOutput());
  adhesioncorrectionfilter->SetRadius(radius);
  adhesioncorrectionfilter->SetTolerance(tolerance);
  adhesioncorrectionfilter->SetDiscontinuityThreshold(10);
  adhesioncorrectionfilter->SetDiscontinuityHighThreshold(30);
  adhesioncorrectionfilter->SetMaxEdgeGap(10);
  adhesioncorrectionfilter->SetEdgeThreshold(30);

  WriterType::Pointer dfWriter = WriterType::New();
  dfWriter->SetFileName(correctedDisparityFileName);
  dfWriter->SetInput(adhesioncorrectionfilter->GetOutput());
  dfWriter->Update();

  MaskWriterType::Pointer mask_writer = MaskWriterType::New();
  mask_writer->SetInput(adhesioncorrectionfilter->GetOutputMask());
  mask_writer->SetFileName(correctedMaskFileName);
  mask_writer->Update();

  WriterType::Pointer riskedgesWriter = WriterType::New();
  riskedgesWriter->SetFileName(riskEdgesFileName);
  riskedgesWriter->SetInput(adhesioncorrectionfilter->GetOutputRiskEdges());
  riskedgesWriter->Update();

  return EXIT_SUCCESS;
}
