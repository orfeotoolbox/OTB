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
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbStandardWriterWatcher.h"
#include "otbImageFileReader.h"

const unsigned int Dimension = 2;
typedef float                                                         PixelType;
typedef otb::Image<PixelType, Dimension>                              ImageType;
typedef float                                                         MaskPixelType;
typedef otb::Image<MaskPixelType, Dimension>                          MaskType;


typedef otb::ImageFileReader<ImageType>               ReaderType;
typedef otb::ImageFileReader<MaskType>                MaskReaderType;

typedef otb::ImageFileWriter<ImageType>                      WriterType;
typedef otb::ImageFileWriter<MaskType>                       MaskWriterType;

typedef otb::AdhesionCorrectionFilter<ImageType, MaskType> AdhesionCorrectionFilterType;

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

  const unsigned int ImageDimension = 2;
  typedef double PixelType;
  typedef otb::Image<unsigned char, ImageDimension>  MaskImageType;
  typedef otb::Image<PixelType, ImageDimension> ImageType;
  typedef otb::ImageFileReader<ImageType> InputReaderType;

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

  typedef otb::ImageFileReader<MaskImageType> MaskReaderType;
  MaskReaderType::Pointer medianmaskreader = MaskReaderType::New();
  medianmaskreader->SetFileName(medianMaskFileName);

  DisparityMapReaderType::Pointer subpixeldisparityreader = DisparityMapReaderType::New();
  subpixeldisparityreader->SetFileName(subpixelDisparityFileName);

  typedef otb::ImageFileReader<MaskImageType> MaskReaderType;
  MaskReaderType::Pointer subpixelmaskreader = MaskReaderType::New();
  subpixelmaskreader->SetFileName(subpixelMaskFileName);

  /// Edge images are read from Canny filtering performed with MARC2
  InputReaderType::Pointer cannyrefReader = InputReaderType::New();
  cannyrefReader->SetFileName(cannyRefFileName);

  InputReaderType::Pointer cannymedianReader = InputReaderType::New();
  cannymedianReader->SetFileName(cannyMedianFileName);

  typedef otb::AdhesionCorrectionFilter<ImageType, MaskImageType> AdhesionCorrectionFilterType;
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


  typedef otb::ImageFileWriter<ImageType> DFWriterType;
  DFWriterType::Pointer dfWriter = DFWriterType::New();
  dfWriter->SetFileName(correctedDisparityFileName);
  dfWriter->SetInput(adhesioncorrectionfilter->GetOutput());
  dfWriter->Update();

  typedef otb::ImageFileWriter<MaskImageType> MaskWriterType;
  MaskWriterType::Pointer mask_writer = MaskWriterType::New();
  mask_writer->SetInput(adhesioncorrectionfilter->GetOutputMask());
  mask_writer->SetFileName(correctedMaskFileName);
  mask_writer->Update();

  DFWriterType::Pointer riskedgesWriter = DFWriterType::New();
  riskedgesWriter->SetFileName(riskEdgesFileName);
  riskedgesWriter->SetInput(adhesioncorrectionfilter->GetOutputRiskEdges());
  riskedgesWriter->Update();

  return EXIT_SUCCESS;
}
