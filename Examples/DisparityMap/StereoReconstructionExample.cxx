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


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {sensor_stereo_left.tif}, {sensor_stereo_right.tif}
//    OUTPUTS: {elevationOutput.tif}
//    140
//  Software Guide : EndCommandLineArgs

// Software Guide : BeginLatex
//
// This example demonstrates the use of the stereo reconstruction chain, using the filters :
// \begin{itemize}
// \item \doxygen{otb}{StereorectificationDeformationFieldSource}
// \item \doxygen{otb}{StreamingWarpImageFilter}
// \item \doxygen{otb}{PixelWiseBlockMatchingImageFilter}
// \item \doxygen{otb}{otbSubPixelDisparityImageFilter}
// \item \doxygen{otb}{otbDisparityMapMedianFilter}
// \item \doxygen{otb}{DisparityMapToDEMFilter}
//
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "otbStereorectificationDeformationFieldSource.h"
#include "otbStreamingWarpImageFilter.h"
#include "otbPixelWiseBlockMatchingImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "otbSubPixelDisparityImageFilter.h"
#include "otbDisparityMapMedianFilter.h"
#include "otbDisparityMapToDEMFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkVectorCastImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
// Software Guide : EndCodeSnippet


int main(int argc, char* argv[])
{
  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " sensorImage1 sensorImage2 outputDEM ";
    std::cerr << "averageElevation  " << std::endl;
    return EXIT_FAILURE;
    }
  
  typedef otb::Image<float,2>                 FloatImageType;
  typedef otb::VectorImage<float,2>           FloatVectorImageType;
  
  typedef otb::ImageList<FloatImageType>      ImageListType;
  
  typedef otb::ImageListToVectorImageFilter
    <ImageListType,
     FloatVectorImageType>                    ImageListToVectorImageFilterType;
  
  typedef otb::ImageFileReader
    <FloatImageType>                          ImageReaderType;
  
  typedef otb::StereorectificationDeformationFieldSource
    <FloatImageType,FloatVectorImageType>     DeformationFieldSourceType;
  
  typedef itk::Vector<double,2>               DeformationType;
  typedef otb::Image<DeformationType>         DeformationFieldType;
  
  typedef itk::VectorCastImageFilter
    <FloatVectorImageType,
     DeformationFieldType>                    DeformationFieldCastFilterType;
  
  typedef otb::StreamingWarpImageFilter
    <FloatImageType,
     FloatImageType,
     DeformationFieldType>                    WarpFilterType;
  
  typedef otb::BCOInterpolateImageFunction<FloatImageType> BCOInterpolationType;
  
  typedef otb::Functor::NCCBlockMatching<FloatImageType,FloatImageType> NCCBlockMatchingFunctorType;
  
  typedef otb::PixelWiseBlockMatchingImageFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     FloatImageType,
     NCCBlockMatchingFunctorType>               NCCBlockMatchingFilterType;
  
  typedef otb::BandMathImageFilter<FloatImageType>         BandMathFilterType;
  
  typedef otb::SubPixelDisparityImageFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     FloatImageType,
     NCCBlockMatchingFunctorType>               NCCSubPixelDisparityFilterType;
  
  typedef otb::DisparityMapMedianFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType>                            MedianFilterType;
  
  double avgElevation = atof(argv[4]);
  
  ImageReaderType::Pointer leftReader =  ImageReaderType::New();
  ImageReaderType::Pointer rightReader =  ImageReaderType::New();
  
  leftReader->SetFileName(argv[1]);
  rightReader->SetFileName(argv[2]);
  
  DeformationFieldSourceType::Pointer m_DeformationFieldSource = DeformationFieldSourceType::New();
  m_DeformationFieldSource->SetLeftImage(leftReader->GetOutput());
  m_DeformationFieldSource->SetRightImage(rightReader->GetOutput());
  m_DeformationFieldSource->SetGridStep(4);
  m_DeformationFieldSource->SetScale(1.0);
  m_DeformationFieldSource->SetAverageElevation(avgElevation);
  
  m_DeformationFieldSource->Update();
  
  // compute epipolar parameters
  FloatImageType::SpacingType epipolarSpacing;
  epipolarSpacing[0] = 1.0;
  epipolarSpacing[1] = 1.0;
  
  FloatImageType::SizeType epipolarSize;
  epipolarSize = m_DeformationFieldSource->GetRectifiedImageSize();
  
  FloatImageType::PointType epipolarOrigin;
  epipolarOrigin[0] = 0.0;
  epipolarOrigin[1] = 0.0;
  
  FloatImageType::PixelType defaultValue = 0;
  
  // resample left image
  DeformationFieldCastFilterType::Pointer m_LeftDeformationFieldCaster = DeformationFieldCastFilterType::New();
  m_LeftDeformationFieldCaster->SetInput(m_DeformationFieldSource->GetLeftDeformationFieldOutput());
  m_LeftDeformationFieldCaster->GetOutput()->UpdateOutputInformation();
  
  BCOInterpolationType::Pointer leftInterpolator = BCOInterpolationType::New();
  leftInterpolator->SetRadius(2);
  
  WarpFilterType::Pointer m_LeftWarpImageFilter = WarpFilterType::New();
  m_LeftWarpImageFilter->SetInput(leftReader->GetOutput());
  m_LeftWarpImageFilter->SetDeformationField(m_LeftDeformationFieldCaster->GetOutput());
  m_LeftWarpImageFilter->SetInterpolator(leftInterpolator);
  m_LeftWarpImageFilter->SetOutputSize(epipolarSize);
  m_LeftWarpImageFilter->SetOutputSpacing(epipolarSpacing);
  m_LeftWarpImageFilter->SetOutputOrigin(epipolarOrigin);
  m_LeftWarpImageFilter->SetEdgePaddingValue(defaultValue);
  
  // resample right image
  DeformationFieldCastFilterType::Pointer m_RightDeformationFieldCaster = DeformationFieldCastFilterType::New();
  m_RightDeformationFieldCaster->SetInput(m_DeformationFieldSource->GetRightDeformationFieldOutput());
  m_RightDeformationFieldCaster->GetOutput()->UpdateOutputInformation();
  
  BCOInterpolationType::Pointer rightInterpolator = BCOInterpolationType::New();
  rightInterpolator->SetRadius(2);
  
  WarpFilterType::Pointer m_RightWarpImageFilter = WarpFilterType::New();
  m_RightWarpImageFilter->SetInput(rightReader->GetOutput());
  m_RightWarpImageFilter->SetDeformationField(m_RightDeformationFieldCaster->GetOutput());
  m_RightWarpImageFilter->SetInterpolator(rightInterpolator);
  m_RightWarpImageFilter->SetOutputSize(epipolarSize);
  m_RightWarpImageFilter->SetOutputSpacing(epipolarSpacing);
  m_RightWarpImageFilter->SetOutputOrigin(epipolarOrigin);
  m_RightWarpImageFilter->SetEdgePaddingValue(defaultValue);
  
  // mask no-data pixels on left and right epipolar images
  BandMathFilterType::Pointer m_LBandMathFilter = BandMathFilterType::New();
  m_LBandMathFilter->SetNthInput(0,m_LeftWarpImageFilter->GetOutput(),"inleft");
  std::string leftExpr = "if(inleft != 0,255,0)";
  m_LBandMathFilter->SetExpression(leftExpr);
  
  BandMathFilterType::Pointer m_RBandMathFilter = BandMathFilterType::New();
  m_RBandMathFilter->SetNthInput(0,m_RightWarpImageFilter->GetOutput(),"inright");
  std::string rightExpr = "if(inright != 0,255,0)";
  m_RBandMathFilter->SetExpression(rightExpr);
  
  NCCBlockMatchingFilterType::Pointer m_NCCBlockMatcher = NCCBlockMatchingFilterType::New();
  m_NCCBlockMatcher->SetLeftInput(m_LeftWarpImageFilter->GetOutput());
  m_NCCBlockMatcher->SetRightInput(m_RightWarpImageFilter->GetOutput());
  m_NCCBlockMatcher->SetRadius(3);
  m_NCCBlockMatcher->SetMinimumHorizontalDisparity(-24);
  m_NCCBlockMatcher->SetMaximumHorizontalDisparity(0);
  m_NCCBlockMatcher->SetMinimumVerticalDisparity(0);
  m_NCCBlockMatcher->SetMaximumVerticalDisparity(0);
  m_NCCBlockMatcher->MinimizeOff();
  m_NCCBlockMatcher->SetLeftMaskInput(m_LBandMathFilter->GetOutput());
  m_NCCBlockMatcher->SetRightMaskInput(m_RBandMathFilter->GetOutput());

  NCCSubPixelDisparityFilterType::Pointer m_NCCSubPixFilter = NCCSubPixelDisparityFilterType::New();
  m_NCCSubPixFilter->SetInputsFromBlockMatchingFilter(m_NCCBlockMatcher);
  m_NCCSubPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::DICHOTOMY);
  
  
  ImageListType::Pointer m_OutputImageList = ImageListType::New();
  m_OutputImageList->Clear();
  m_OutputImageList->PushBack(m_NCCSubPixFilter->GetHorizontalDisparityOutput());
  m_OutputImageList->PushBack(m_NCCSubPixFilter->GetVerticalDisparityOutput());
  m_OutputImageList->PushBack(m_NCCSubPixFilter->GetMetricOutput());
  
  ImageListToVectorImageFilterType::Pointer m_ImageListFilter = ImageListToVectorImageFilterType::New();
  m_ImageListFilter->SetInput(m_OutputImageList);
  
  
  typedef otb::StreamingImageFileWriter<FloatVectorImageType> WriterType;
  WriterType::Pointer dispOutput = WriterType::New();
  dispOutput->SetInput(m_ImageListFilter->GetOutput());
  dispOutput->SetFileName("/home2/gpasero/ORFEO-TOOLBOX/stereo/disp_example.tif");
  dispOutput->Update();

  std::cout << "epi.rectsizex" << m_DeformationFieldSource->GetRectifiedImageSize()[0] << std::endl;
  std::cout << "epi.rectsizey" << m_DeformationFieldSource->GetRectifiedImageSize()[1] << std::endl;
  std::cout << "epi.baseline" << m_DeformationFieldSource->GetMeanBaselineRatio() << std::endl;
  
  return EXIT_SUCCESS;
}
