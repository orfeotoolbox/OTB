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


/* Example usage:
./MeanShiftSegmentationFilterExample Input/ROI_QB_MUL_1.png \
                                     Output/MSLabeledOutput.tif \
                                     Output/MSClusteredOutput.tif \
                                     Output/MSLabeledOutput-pretty.png \
                                     Output/MSClusteredOutput-pretty.png \
                                     16 \
                                     16 \
                                     100 \
                                     100 \
                                     0.1
*/

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImageFileWriter.h"
#include "otbPrintableImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbMeanShiftSegmentationFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 11)
  {
    std::cerr << "Usage: " << argv[0] << " infname labeledfname clusteredfname labeledpretty clusteredpretty "
              << "spatialRadius rangeRadius minRegionSize maxiter thres" << std::endl;
    return EXIT_FAILURE;
  }

  const char*        infname         = argv[1];
  const char*        labeledfname    = argv[2];
  const char*        clusteredfname  = argv[3];
  const char*        labeledpretty   = argv[4];
  const char*        clusteredpretty = argv[5];
  const unsigned int spatialRadius   = atoi(argv[6]);
  const double       rangeRadius     = atof(argv[7]);
  const unsigned int minRegionSize   = atoi(argv[8]);
  const unsigned int maxiter         = atoi(argv[9]);
  const double       thres           = atof(argv[10]);

  const unsigned int Dimension = 2;

  using PixelType      = float;
  using LabelPixelType = unsigned int;
  using ColorPixelType = itk::RGBPixel<unsigned char>;

  using ImageType      = otb::VectorImage<PixelType, Dimension>;
  using LabelImageType = otb::Image<LabelPixelType, Dimension>;
  using RGBImageType   = otb::Image<ColorPixelType, Dimension>;

  using ReaderType      = otb::ImageFileReader<ImageType>;
  using WriterType      = otb::ImageFileWriter<ImageType>;
  using LabelWriterType = otb::ImageFileWriter<LabelImageType>;

  using FilterType = otb::MeanShiftSegmentationFilter<ImageType, LabelImageType, ImageType>;

  // We instantiate the filter, the reader, and 2 writers (for the
  // labeled and clustered images).

  FilterType::Pointer      filter  = FilterType::New();
  ReaderType::Pointer      reader  = ReaderType::New();
  WriterType::Pointer      writer1 = WriterType::New();
  LabelWriterType::Pointer writer2 = LabelWriterType::New();

  // We set the file names for the reader and the writers:
  reader->SetFileName(infname);
  writer1->SetFileName(clusteredfname);
  writer2->SetFileName(labeledfname);

  // We can now set the parameters for the filter. There are 3 main
  // parameters: the spatial radius used for defining the neighborhood,
  // the range radius used for defining the interval in the color space
  // and the minimum size for the regions to be kept after clustering.

  filter->SetSpatialBandwidth(spatialRadius);
  filter->SetRangeBandwidth(rangeRadius);
  filter->SetMinRegionSize(minRegionSize);

  // Two another parameters can be set: the maximum iteration number, which
  // defines maximum number of iteration until convergence.  Algorithm
  // iterative scheme will stop if convergence hasn't been reached after the
  // maximum number of iterations.  Threshold parameter defines mean-shift
  // vector convergence value. Algorithm iterative scheme will stop if
  // mean-shift vector is below this threshold or if iteration number reached
  // maximum number of iterations.

  filter->SetMaxIterationNumber(maxiter);
  filter->SetThreshold(thres);
  // We can now plug the pipeline and run it.

  filter->SetInput(reader->GetOutput());
  writer1->SetInput(filter->GetClusteredOutput());
  writer2->SetInput(filter->GetLabelOutput());

  writer1->Update();
  writer2->Update();

  using PrintableFilterType                         = otb::PrintableImageFilter<ImageType>;
  PrintableFilterType::Pointer printableImageFilter = PrintableFilterType::New();

  printableImageFilter->SetChannel(1);
  printableImageFilter->SetChannel(2);
  printableImageFilter->SetChannel(3);

  using OutputImageType  = PrintableFilterType::OutputImageType;
  using PrettyWriterType = otb::ImageFileWriter<OutputImageType>;

  PrettyWriterType::Pointer prettyWriter = PrettyWriterType::New();

  printableImageFilter->SetInput(filter->GetClusteredOutput());
  prettyWriter->SetFileName(clusteredpretty);
  prettyWriter->SetInput(printableImageFilter->GetOutput());
  prettyWriter->Update();

  using LabelRGBWriterType = otb::ImageFileWriter<RGBImageType>;

  LabelRGBWriterType::Pointer labelRGBWriter = LabelRGBWriterType::New();

  // Label to RGB image
  using FunctorType                        = itk::Functor::ScalarToRGBPixelFunctor<LabelPixelType>;
  using ColorLabelFilterType               = itk::UnaryFunctorImageFilter<LabelImageType, RGBImageType, FunctorType>;
  ColorLabelFilterType::Pointer labelToRGB = ColorLabelFilterType::New();

  labelToRGB->SetInput(filter->GetLabelOutput());

  labelRGBWriter->SetFileName(labeledpretty);
  labelRGBWriter->SetInput(labelToRGB->GetOutput());
  labelRGBWriter->Update();
}
