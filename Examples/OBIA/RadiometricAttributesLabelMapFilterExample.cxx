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
./RadiometricAttributesLabelMapFilterExample Input/qb_RoadExtract.tif \
                                             Output/OBIARadiometricAttribute1.png \
                                             Output/qb_ExtractRoad_Radiometry_pretty.jpg \
                                             STATS::Band1::Mean \
                                             0 \
                                             0.5 \
                                             16 \
                                             16 \
                                             50 \
                                             1.0
*/


//  This example shows the basic approach to perform object based analysis on a image.
//  The input image is firstly segmented using the \doxygen{otb}{MeanShiftSegmentationFilter}
//  Then each segmented region is converted to a Map of labeled objects.
//  Afterwards the \doxygen{itk}{UnaryFunctorImageFilter} computes
//  radiometric attributes for each object. In this example the NDVI is computed.
//  The computed feature is passed to the \doxygen{otb}{BandsStatisticsAttributesLabelMapFilter}
//  which computes statistics over the resulting band.
//  Therefore, region's statistics over each band can be access by concatening
//  STATS, the band number and the statistical attribute separated by colons. In this example
//  the mean of the first band (which contains the NDVI) is access over all the regions
//  with the attribute: 'STATS::Band1::Mean'.

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

#include "otbMeanShiftSegmentationFilter.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "otbShapeAttributesLabelMapFilter.h"
#include "otbBandsStatisticsAttributesLabelMapFilter.h"
#include "itkLabelMapToBinaryImageFilter.h"
#include "otbMultiChannelExtractROI.h"
#include "otbAttributesMapOpeningLabelMapFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"
#include "otbVegetationIndicesFunctor.h"
#include "itkUnaryFunctorImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 11)
  {
    std::cerr << "Usage: " << argv[0] << " reffname outfname outprettyfname attribute_name ";
    std::cerr << "lowerThan tresh spatialRadius rangeRadius minregionsize scale" << std::endl;
    return EXIT_FAILURE;
  }

  const char* reffname       = argv[1];
  const char* outfname       = argv[2];
  const char* outprettyfname = argv[3];
  const char* attr           = argv[4];
  bool        lowerThan      = atoi(argv[5]);
  double      thresh         = atof(argv[6]);

  const unsigned int spatialRadius = atoi(argv[7]);
  const double       rangeRadius   = atof(argv[8]);
  const unsigned int minRegionSize = atoi(argv[9]);
  /* const double       scale                  = atoi(argv[10]); */

  const unsigned int Dimension = 2;

  // Labeled image type
  typedef unsigned int                                                                   LabelType;
  typedef unsigned char                                                                  MaskPixelType;
  typedef double                                                                         PixelType;
  typedef otb::Image<LabelType, Dimension>                                               LabeledImageType;
  typedef otb::Image<MaskPixelType, Dimension>                                           MaskImageType;
  typedef otb::Image<PixelType, Dimension>                                               ImageType;
  typedef otb::VectorImage<PixelType, Dimension>                                         VectorImageType;
  typedef otb::VectorImage<unsigned char, Dimension>                                     OutputVectorImageType;
  typedef otb::ImageFileReader<LabeledImageType>                                         LabeledReaderType;
  typedef otb::ImageFileReader<ImageType>                                                ReaderType;
  typedef otb::ImageFileReader<VectorImageType>                                          VectorReaderType;
  typedef otb::ImageFileWriter<MaskImageType>                                            WriterType;
  typedef otb::ImageFileWriter<OutputVectorImageType>                                    VectorWriterType;
  typedef otb::VectorRescaleIntensityImageFilter<VectorImageType, OutputVectorImageType> VectorRescalerType;
  typedef otb::MultiChannelExtractROI<unsigned char, unsigned char>                      ChannelExtractorType;
  // Label map typedef
  typedef otb::AttributesMapLabelObject<LabelType, Dimension, double>                 LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>                                              LabelMapType;
  typedef itk::LabelImageToLabelMapFilter<LabeledImageType, LabelMapType>             LabelMapFilterType;
  typedef otb::ShapeAttributesLabelMapFilter<LabelMapType>                            ShapeLabelMapFilterType;
  typedef otb::BandsStatisticsAttributesLabelMapFilter<LabelMapType, VectorImageType> RadiometricLabelMapFilterType;
  typedef otb::AttributesMapOpeningLabelMapFilter<LabelMapType>                       OpeningLabelMapFilterType;
  typedef itk::LabelMapToBinaryImageFilter<LabelMapType, MaskImageType>               LabelMapToBinaryImageFilterType;
  typedef itk::UnaryFunctorImageFilter<VectorImageType, ImageType, otb::Functor::NDVI<PixelType, PixelType>> NDVIImageFilterType;
  typedef otb::ImageToVectorImageCastFilter<ImageType, VectorImageType>   ImageToVectorImageCastFilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(reffname);

  LabeledReaderType::Pointer lreader = LabeledReaderType::New();
  lreader->SetFileName(reffname);

  VectorReaderType::Pointer vreader = VectorReaderType::New();
  vreader->SetFileName(reffname);
  vreader->Update();
  // Firstly, segment the input image by using the Mean Shift algorithm (see \ref{sec:MeanShift} for deeper
  // explanations).

  typedef otb::MeanShiftSegmentationFilter<VectorImageType, LabeledImageType, VectorImageType> FilterType;
  FilterType::Pointer                                                                          filter = FilterType::New();
  filter->SetSpatialBandwidth(spatialRadius);
  filter->SetRangeBandwidth(rangeRadius);
  filter->SetMinRegionSize(minRegionSize);
  filter->SetThreshold(0.1);
  filter->SetMaxIterationNumber(100);

  // For non regression tests, set the number of threads to 1
  // because MeanShift results depends on the number of threads
  filter->SetNumberOfThreads(1);

  // The \doxygen{otb}{MeanShiftSegmentationFilter} type is instantiated using the image
  // types.

  filter->SetInput(vreader->GetOutput());

  // The \doxygen{itk}{LabelImageToLabelMapFilter} type is instantiated using the output
  // of the \doxygen{otb}{MeanShiftSegmentationFilter}. This filter produces a labeled image
  // where each segmented region has a unique label.
  LabelMapFilterType::Pointer labelMapFilter = LabelMapFilterType::New();
  labelMapFilter->SetInput(filter->GetLabelOutput());
  labelMapFilter->SetBackgroundValue(itk::NumericTraits<LabelType>::min());

  ShapeLabelMapFilterType::Pointer shapeLabelMapFilter = ShapeLabelMapFilterType::New();
  shapeLabelMapFilter->SetInput(labelMapFilter->GetOutput());
  // Instantiate the  \doxygen{otb}{RadiometricLabelMapFilterType} to
  // compute statistics of the feature image on each label object.
  RadiometricLabelMapFilterType::Pointer radiometricLabelMapFilter = RadiometricLabelMapFilterType::New();
  //  Feature image could be one of the following image:
  //  \begin{itemize}
  //  \item GEMI
  //  \item NDVI
  //  \item IR
  //  \item IC
  //  \item IB
  //  \item NDWI2
  //  \item Intensity
  //  \end{itemize}
  //
  //  Input image must be convert to the desired coefficient.
  //  In our case, statistics are computed on the NDVI coefficient on each label object.
  NDVIImageFilterType::Pointer ndviImageFilter = NDVIImageFilterType::New();

  ndviImageFilter->GetFunctor().SetBandIndex(CommonBandNames::RED, 3);
  ndviImageFilter->GetFunctor().SetBandIndex(CommonBandNames::NIR, 4);
  ndviImageFilter->SetInput(vreader->GetOutput());

  ImageToVectorImageCastFilterType::Pointer ndviVectorImageFilter = ImageToVectorImageCastFilterType::New();

  ndviVectorImageFilter->SetInput(ndviImageFilter->GetOutput());

  radiometricLabelMapFilter->SetInput(shapeLabelMapFilter->GetOutput());
  radiometricLabelMapFilter->SetFeatureImage(ndviVectorImageFilter->GetOutput());
  // The \doxygen{otb}{AttributesMapOpeningLabelMapFilter} will perform the selection.
  // There are three parameters. \code{AttributeName} specifies the radiometric attribute, \code{Lambda}
  // controls the thresholding of the input and \code{ReverseOrdering} make this filter to remove the
  // object with an attribute value greater than \code{Lambda} instead.
  OpeningLabelMapFilterType::Pointer opening = OpeningLabelMapFilterType::New();
  opening->SetInput(radiometricLabelMapFilter->GetOutput());
  opening->SetAttributeName(attr);
  opening->SetLambda(thresh);
  opening->SetReverseOrdering(lowerThan);
  opening->Update();
  //  Then, Label objects selected are transform in a Label Image using the
  //  \doxygen{itk}{LabelMapToLabelImageFilter}.
  LabelMapToBinaryImageFilterType::Pointer labelMap2LabeledImage = LabelMapToBinaryImageFilterType::New();
  labelMap2LabeledImage->SetInput(opening->GetOutput());
  // And finally, we declare the writer and call its \code{Update()} method to
  // trigger the full pipeline execution.

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(outfname);
  writer->SetInput(labelMap2LabeledImage->GetOutput());
  writer->Update();

  OutputVectorImageType::PixelType minimum, maximum;
  minimum.SetSize(vreader->GetOutput()->GetNumberOfComponentsPerPixel());
  maximum.SetSize(vreader->GetOutput()->GetNumberOfComponentsPerPixel());
  minimum.Fill(0);
  maximum.Fill(255);

  VectorRescalerType::Pointer vr = VectorRescalerType::New();
  vr->SetInput(filter->GetClusteredOutput());
  vr->SetOutputMinimum(minimum);
  vr->SetOutputMaximum(maximum);
  vr->SetClampThreshold(0.01);

  ChannelExtractorType::Pointer selecter = ChannelExtractorType::New();
  selecter->SetInput(vr->GetOutput());
  selecter->SetExtractionRegion(vreader->GetOutput()->GetLargestPossibleRegion());
  selecter->SetChannel(3);
  selecter->SetChannel(2);
  selecter->SetChannel(1);

  VectorWriterType::Pointer vectWriter = VectorWriterType::New();
  vectWriter->SetFileName(outprettyfname);
  vectWriter->SetInput(selecter->GetOutput());
  vectWriter->Update();

  return EXIT_SUCCESS;
}

// Figure~\ref{fig:RADIOMETRIC_LABEL_MAP_FILTER} shows the result of applying
// the object selection based on radiometric attributes.
// \begin{figure} [htbp]
// \center
// \includegraphics[width=0.44\textwidth]{qb_ExtractRoad_Radiometry_pretty.eps}
// \includegraphics[width=0.44\textwidth]{OBIARadiometricAttribute1.eps}
// \itkcaption[Object based extraction based on ]{Vegetation mask resulting from processing.}
// \label{fig:RADIOMETRIC_LABEL_MAP_FILTER}
// \end{figure}
