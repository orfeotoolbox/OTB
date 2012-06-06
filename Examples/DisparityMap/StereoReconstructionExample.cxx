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
#include "otbSubPixelDisparityImageFilter.h"
#include "otbDisparityMapMedianFilter.h"
#include "otbDisparityMapToDEMFilter.h"

#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbBCOInterpolateImageFunction.h"
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
  
  typedef otb::ImageFileReader
    <FloatImageType>                          ImageReaderType;
  
  typedef otb::StereorectificationDeformationFieldSource
    <FloatImageType,FloatVectorImageType>     DeformationFieldSourceType;
  
  typedef itk::Vector<double,2>               DeformationType;
  typedef otb::Image<DeformationType>         DeformationFieldType;
  
//   typedef itk::VectorCastImageFilter
//     <FloatVectorImageType,
//      DeformationFieldType>                    DeformationFieldCastFilterType;

//   typedef itk::InverseDeformationFieldImageFilter
//     <DeformationFieldType,DeformationFieldType> InverseDeformationFieldFilterType;

//   typedef itk::VectorIndexSelectionCastImageFilter
//     <DeformationFieldType,
//      FloatImageType>                          IndexSelectionCastFilterType;
// 
//   typedef otb::ImageList<FloatImageType>      ImageListType;
//   typedef otb::ImageListToVectorImageFilter
//     <ImageListType,FloatVectorImageType>      ImageListFilterType;
// 
//   typedef otb::DEMToImageGenerator
//     <FloatImageType>                          DEMToImageGeneratorType;
//   typedef otb::StreamingStatisticsImageFilter
//     <FloatImageType>                          StatisticsFilterType;
  
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
  
  //m_DeformationFieldSource->Update();
  
  
  
  typedef otb::StreamingImageFileWriter<FloatVectorImageType> WriterType;
  WriterType::Pointer leftOutput = WriterType::New();
  WriterType::Pointer rightOutput = WriterType::New();
  
  leftOutput->SetInput(m_DeformationFieldSource->GetLeftDeformationFieldOutput());
  rightOutput->SetInput(m_DeformationFieldSource->GetRightDeformationFieldOutput());
  
  leftOutput->SetFileName("/home2/gpasero/ORFEO-TOOLBOX/stereo/grid_example_left.tif");
  rightOutput->SetFileName("/home2/gpasero/ORFEO-TOOLBOX/stereo/grid_example_right.tif");
  
  leftOutput->Update();
  rightOutput->Update();

  std::cout << "epi.rectsizex" << m_DeformationFieldSource->GetRectifiedImageSize()[0] << std::endl;
  std::cout << "epi.rectsizey" << m_DeformationFieldSource->GetRectifiedImageSize()[1] << std::endl;
  std::cout << "epi.baseline" << m_DeformationFieldSource->GetMeanBaselineRatio() << std::endl;  
  
  return EXIT_SUCCESS;
}
