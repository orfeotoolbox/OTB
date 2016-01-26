/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


//  Software Guide : BeginCommandLineArgs
//  INPUTS: {maur_GT.tif}, {maur_labelled.tif}
//  OUTPUTS: {maur_colored_GT.tif}
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// The following example shows how to compare two segmentations, using Hoover
// metrics. For instance, it can be used to compare a segmentation produced
// by your algorithm against a partial ground truth segmentation. In this
// example, the ground truth segmentation will be refered by the letters GT
// whereas the machine segmentation will be refered by MS.
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
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbHooverMatrixFilter.h"
#include "otbHooverInstanceFilter.h"
#include "otbLabelMapToAttributeImageFilter.h"
// Software Guide : EndCodeSnippet

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "itkLabelImageToLabelMapFilter.h"

int main(int argc, char* argv[])
{
  if(argc != 4)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " segmentationGT segmentationMS outputAttributeImage" << std::endl;
    return EXIT_FAILURE;
    }

  // Software Guide : BeginLatex
  // The filters \doxygen{otb}{HooverMatrixFilter} and \doxygen{otb}{HooverInstanceFilter}
  // are designed to handle \doxygen{itk}{LabelMap} images, made with \doxygen{otb}{AttributesMapLabelObject}.
  // This type of label object allows storing generic attributes. Each region can store
  // a set of attributes: in this case, Hoover instances and metrics will be stored.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::AttributesMapLabelObject<unsigned int, 2, float> LabelObjectType;
  typedef itk::LabelMap<LabelObjectType>            LabelMapType;
  typedef otb::HooverMatrixFilter<LabelMapType>     HooverMatrixFilterType;
  typedef otb::HooverInstanceFilter<LabelMapType>   InstanceFilterType;

  typedef otb::Image<unsigned int, 2>               ImageType;
  typedef itk::LabelImageToLabelMapFilter
    <ImageType, LabelMapType>                       ImageToLabelMapFilterType;

  typedef otb::VectorImage<float, 2>                VectorImageType;
  typedef otb::LabelMapToAttributeImageFilter
      <LabelMapType, VectorImageType>               AttributeImageFilterType;
  // Software Guide : EndCodeSnippet
  typedef otb::ImageFileReader<ImageType>           ImageReaderType;
  typedef otb::ImageFileWriter<VectorImageType>     WriterType;

  ImageReaderType::Pointer gt_reader = ImageReaderType::New();
  gt_reader->SetFileName(argv[1]);

  ImageReaderType::Pointer ms_reader = ImageReaderType::New();
  ms_reader->SetFileName(argv[2]);

  // Software Guide : BeginLatex
  // The first step is to convert the images to label maps : we use
  // \doxygen{itk}{LabelImageToLabelMapFilter}. The background value sets
  // the label value of regions considered as background: there is no label object for the
  // background region.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  ImageToLabelMapFilterType::Pointer gt_filter = ImageToLabelMapFilterType::New();
  gt_filter->SetInput(gt_reader->GetOutput());
  gt_filter->SetBackgroundValue(0);

  ImageToLabelMapFilterType::Pointer ms_filter = ImageToLabelMapFilterType::New();
  ms_filter->SetInput(ms_reader->GetOutput());
  ms_filter->SetBackgroundValue(0);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The Hoover matrix filter has to be updated here. This matrix must be computed
  // before being given to the instance filter.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  HooverMatrixFilterType::Pointer hooverFilter = HooverMatrixFilterType::New();
  hooverFilter->SetGroundTruthLabelMap(gt_filter->GetOutput());
  hooverFilter->SetMachineSegmentationLabelMap(ms_filter->GetOutput());
  hooverFilter->Update();
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The instance filter computes the Hoover metrics for each region. These metrics
  // are stored as attributes in each label object. The threshold parameter
  // corresponds to the overlapping ratio above which two regions can be matched.
  // The extended attributes can be used if the user wants to keep a trace of the
  // associations between MS and GT regions : i.e. if a GT region has been matched
  // as a correct detection, it will carry an attribute containing the label value
  // of the associated MS region (the same principle goes for other types of instance).
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  InstanceFilterType::Pointer instances = InstanceFilterType::New();
  instances->SetGroundTruthLabelMap(gt_filter->GetOutput());
  instances->SetMachineSegmentationLabelMap(ms_filter->GetOutput());
  instances->SetThreshold(0.75);
  instances->SetHooverMatrix(hooverFilter->GetHooverConfusionMatrix());
  instances->SetUseExtendedAttributes(false);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The \doxygen{otb}{LabelMapToAttributeImageFilter} is designed to extract attributes
  // values from a label map and output them in the channels of a vector image. We set
  // the attribute to plot in each channel.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The output image contains for each GT region its correct detection score ("RC", band 1),
  // its over-segmentation score ("RF", band 2), its under-segmentation score ("RA", band 3)
  // and its missed detection score ("RM", band 4).
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  std::cout << "Mean RC ="<< instances->GetMeanRC() << std::endl;
  std::cout << "Mean RF ="<< instances->GetMeanRF() << std::endl;
  std::cout << "Mean RA ="<< instances->GetMeanRA() << std::endl;
  std::cout << "Mean RM ="<< instances->GetMeanRM() << std::endl;
  std::cout << "Mean RN ="<< instances->GetMeanRN() << std::endl;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  // The Hoover scores are also computed for the whole segmentations. Here is some explanation about the score names :
  // C = correct, F = fragmentation, A = aggregation, M = missed, N = noise.
  // Software Guide : EndLatex

  return EXIT_SUCCESS;
}
