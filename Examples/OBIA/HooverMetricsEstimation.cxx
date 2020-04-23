/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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
./HooverMetricsEstimation Input/maur_GT.tif Input/maur_labelled.tif Output/maur_colored_GT.tif
*/


// The following example shows how to compare two segmentations, using Hoover
// metrics. For instance, it can be used to compare a segmentation produced
// by your algorithm against a partial ground truth segmentation. In this
// example, the ground truth segmentation will be referred by the letters GT
// whereas the machine segmentation will be referred by MS.
//
// The estimation of Hoover metrics is done with two filters :
// \doxygen{otb}{HooverMatrixFilter} and \doxygen{otb}{HooverInstanceFilter}.
// The first one produces a matrix containing the number of overlapping pixels
// between MS regions and GT regions. The second one classifies each region among
// four types (called Hoover instances):
// \begin{itemize}
// \item Correct detection : a region is matched with an other one in the
// opposite segmentation, because they cover nearly the same area.
// \item Over-segmentation : a GT region is matched with a group of MS
// regions because they cover nearly the same area.
// \item Under-segmentation : a MS region is matched with a group of GT
// regions because they cover nearly the same area.
// \item Missed detection (for GT regions) or Noise (for MS region) :
// un-matched regions.
// \end{itemize}
// Note that a region can be tagged with two types. When the Hoover instance
// have been found, the instance filter computes overall scores for each
// category : they are the Hoover metrics \footnote{see http://www.trop.mips.uha.fr/pdf/ORASIS-2009.pdf}.

#include "otbHooverMatrixFilter.h"
#include "otbHooverInstanceFilter.h"
#include "otbLabelMapToAttributeImageFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkLabelImageToLabelMapFilter.h"

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " segmentationGT segmentationMS outputAttributeImage" << std::endl;
    return EXIT_FAILURE;
  }

  // The filters \doxygen{otb}{HooverMatrixFilter} and \doxygen{otb}{HooverInstanceFilter}
  // are designed to handle \doxygen{itk}{LabelMap} images, made with \doxygen{otb}{AttributesMapLabelObject}.
  // This type of label object allows storing generic attributes. Each region can store
  // a set of attributes: in this case, Hoover instances and metrics will be stored.

  using LabelObjectType        = otb::AttributesMapLabelObject<unsigned int, 2, float>;
  using LabelMapType           = itk::LabelMap<LabelObjectType>;
  using HooverMatrixFilterType = otb::HooverMatrixFilter<LabelMapType>;
  using InstanceFilterType     = otb::HooverInstanceFilter<LabelMapType>;

  using ImageType                 = otb::Image<unsigned int, 2>;
  using ImageToLabelMapFilterType = itk::LabelImageToLabelMapFilter<ImageType, LabelMapType>;

  using VectorImageType          = otb::VectorImage<float, 2>;
  using AttributeImageFilterType = otb::LabelMapToAttributeImageFilter<LabelMapType, VectorImageType>;
  using ImageReaderType          = otb::ImageFileReader<ImageType>;
  using WriterType               = otb::ImageFileWriter<VectorImageType>;

  ImageReaderType::Pointer gt_reader = ImageReaderType::New();
  gt_reader->SetFileName(argv[1]);

  ImageReaderType::Pointer ms_reader = ImageReaderType::New();
  ms_reader->SetFileName(argv[2]);

  // The first step is to convert the images to label maps : we use
  // \doxygen{itk}{LabelImageToLabelMapFilter}. The background value sets
  // the label value of regions considered as background: there is no label object for the
  // background region.

  ImageToLabelMapFilterType::Pointer gt_filter = ImageToLabelMapFilterType::New();
  gt_filter->SetInput(gt_reader->GetOutput());
  gt_filter->SetBackgroundValue(0);

  ImageToLabelMapFilterType::Pointer ms_filter = ImageToLabelMapFilterType::New();
  ms_filter->SetInput(ms_reader->GetOutput());
  ms_filter->SetBackgroundValue(0);

  // The Hoover matrix filter has to be updated here. This matrix must be computed
  // before being given to the instance filter.

  HooverMatrixFilterType::Pointer hooverFilter = HooverMatrixFilterType::New();
  hooverFilter->SetGroundTruthLabelMap(gt_filter->GetOutput());
  hooverFilter->SetMachineSegmentationLabelMap(ms_filter->GetOutput());
  hooverFilter->Update();

  // The instance filter computes the Hoover metrics for each region. These metrics
  // are stored as attributes in each label object. The threshold parameter
  // corresponds to the overlapping ratio above which two regions can be matched.
  // The extended attributes can be used if the user wants to keep a trace of the
  // associations between MS and GT regions : i.e. if a GT region has been matched
  // as a correct detection, it will carry an attribute containing the label value
  // of the associated MS region (the same principle goes for other types of instance).

  InstanceFilterType::Pointer instances = InstanceFilterType::New();
  instances->SetGroundTruthLabelMap(gt_filter->GetOutput());
  instances->SetMachineSegmentationLabelMap(ms_filter->GetOutput());
  instances->SetThreshold(0.75);
  instances->SetHooverMatrix(hooverFilter->GetHooverConfusionMatrix());
  instances->SetUseExtendedAttributes(false);

  // The \doxygen{otb}{LabelMapToAttributeImageFilter} is designed to extract attributes
  // values from a label map and output them in the channels of a vector image. We set
  // the attribute to plot in each channel.

  AttributeImageFilterType::Pointer attributeImageGT = AttributeImageFilterType::New();
  attributeImageGT->SetInput(instances->GetOutputGroundTruthLabelMap());
  attributeImageGT->SetAttributeForNthChannel(0, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RC));
  attributeImageGT->SetAttributeForNthChannel(1, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RF));
  attributeImageGT->SetAttributeForNthChannel(2, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RA));
  attributeImageGT->SetAttributeForNthChannel(3, InstanceFilterType::GetNameFromAttribute(InstanceFilterType::ATTRIBUTE_RM));

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(attributeImageGT->GetOutput());
  writer->SetFileName(argv[3]);
  writer->Update();

  // The output image contains for each GT region its correct detection score ("RC", band 1),
  // its over-segmentation score ("RF", band 2), its under-segmentation score ("RA", band 3)
  // and its missed detection score ("RM", band 4).

  std::cout << "Mean RC =" << instances->GetMeanRC() << std::endl;
  std::cout << "Mean RF =" << instances->GetMeanRF() << std::endl;
  std::cout << "Mean RA =" << instances->GetMeanRA() << std::endl;
  std::cout << "Mean RM =" << instances->GetMeanRM() << std::endl;
  std::cout << "Mean RN =" << instances->GetMeanRN() << std::endl;

  // The Hoover scores are also computed for the whole segmentations. Here is some explanation about the score names :
  // C = correct, F = fragmentation, A = aggregation, M = missed, N = noise.

  return EXIT_SUCCESS;
}
