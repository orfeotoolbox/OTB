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

#include "otbScalarImageToHigherOrderTexturesFilter.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbStandardFilterWatcher.h"

int otbScalarImageToHigherOrderTexturesFilter(int argc, char * argv[])
{
  if (argc != 7)
    {
    std::cerr << "Usage: " << argv[0] << " infname outprefix nbBins radius offsetx offsety" << std::endl;
    return EXIT_FAILURE;
    }
  const char *       infname      = argv[1];
  const char *       outprefix    = argv[2];
  const unsigned int nbBins       = atoi(argv[3]);
  const unsigned int radius       = atoi(argv[4]);
  const int          offsetx      = atoi(argv[5]);
  const int          offsety      = atoi(argv[6]);

  const unsigned int Dimension = 2;
  typedef float                            PixelType;
  typedef otb::Image<PixelType, Dimension> ImageType;
  typedef otb::ScalarImageToHigherOrderTexturesFilter
  <ImageType, ImageType>                        TexturesFilterType;
  typedef otb::ImageFileReader<ImageType> ReaderType;
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer         reader = ReaderType::New();
  TexturesFilterType::Pointer filter = TexturesFilterType::New();
  WriterType::Pointer         writer = WriterType::New();

  // Read image
  reader->SetFileName(infname);

  // Build radius
  TexturesFilterType::SizeType sradius;
  sradius.Fill(radius);

  // Build offset
  TexturesFilterType::OffsetType offset;
  offset[0] = offsetx;
  offset[1] = offsety;

  filter->SetInput(reader->GetOutput());
  filter->SetNumberOfBinsPerAxis(nbBins);
  filter->SetRadius(sradius);
  filter->SetOffset(offset);

  otb::StandardFilterWatcher watcher(filter, "Textures filter");

  filter->SetInputImageMinimum(0);
  filter->SetInputImageMaximum(255);

  // Write outputs
  itk::OStringStream oss;

  //writer->SetNumberOfStreamDivisions(2);

  oss.str("");
  oss << outprefix << "ShortRunEmphasis.tif";
  writer->SetInput(filter->GetShortRunEmphasisOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "LongRunEmphasis.tif";
  writer->SetInput(filter->GetLongRunEmphasisOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "GreyLevelNonuniformity.tif";
  writer->SetInput(filter->GetGreyLevelNonuniformityOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "RunLengthNonuniformity.tif";
  writer->SetInput(filter->GetRunLengthNonuniformityOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "RunPercentage.tif";
  writer->SetInput(filter->GetRunPercentageOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "LowGreyLevelRunEmphasis.tif";
  writer->SetInput(filter->GetLowGreyLevelRunEmphasisOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "HighGreyLevelRunEmphasis.tif";
  writer->SetInput(filter->GetHighGreyLevelRunEmphasisOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "ShortRunLowGreyLevelEmphasis.tif";
  writer->SetInput(filter->GetShortRunLowGreyLevelEmphasisOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "ShortRunHighGreyLevelEmphasis.tif";
  writer->SetInput(filter->GetShortRunHighGreyLevelEmphasisOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "LongRunLowGreyLevelEmphasis.tif";
  writer->SetInput(filter->GetLongRunLowGreyLevelEmphasisOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  oss.str("");
  oss << outprefix << "LongRunHighGreyLevelEmphasis.tif";
  writer->SetInput(filter->GetLongRunHighGreyLevelEmphasisOutput());
  writer->SetFileName(oss.str());
  writer->Update();

  return EXIT_SUCCESS;
}
