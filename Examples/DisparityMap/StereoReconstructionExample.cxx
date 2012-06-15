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
// This example demonstrates the use of the stereo reconstruction chain
// from an image pair. The images are assumed to come from the same sensor
// but with different positions. The approach presented here has the
// following steps:
// \begin{itemize}
// \item Epipolar resampling of the image pair
// \item Dense disparity map estimation
// \item Projection of the disparities on an existing Digital Elevation Model (DEM)
// \end{itemize}
// It is important to note that this method requires the sensor models with
// a pose estimate for each image.


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

  typedef otb::StreamingImageFileWriter
    <FloatImageType>                          WriterType;

// Software Guide : BeginLatex
// This example demonstrates the use of the following filters :
// \begin{itemize}
// \item \doxygen{otb}{StereorectificationDeformationFieldSource}
// \item \doxygen{otb}{StreamingWarpImageFilter}
// \item \doxygen{otb}{PixelWiseBlockMatchingImageFilter}
// \item \doxygen{otb}{otbSubPixelDisparityImageFilter}
// \item \doxygen{otb}{otbDisparityMapMedianFilter}
// \item \doxygen{otb}{DisparityMapToDEMFilter}
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
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

  typedef otb::BCOInterpolateImageFunction
    <FloatImageType>                          BCOInterpolationType;

  typedef otb::Functor::NCCBlockMatching
    <FloatImageType,FloatImageType>           NCCBlockMatchingFunctorType;

  typedef otb::PixelWiseBlockMatchingImageFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     FloatImageType,
     NCCBlockMatchingFunctorType>             NCCBlockMatchingFilterType;

  typedef otb::BandMathImageFilter
    <FloatImageType>                          BandMathFilterType;

  typedef otb::SubPixelDisparityImageFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     FloatImageType,
     NCCBlockMatchingFunctorType>             NCCSubPixelDisparityFilterType;

  typedef otb::DisparityMapMedianFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType>                          MedianFilterType;

  typedef otb::DisparityMapToDEMFilter
    <FloatImageType,
     FloatImageType,
     FloatImageType,
     FloatVectorImageType,
     FloatImageType>                          DisparityToElevationFilterType;
// Software Guide : EndCodeSnippet

  double avgElevation = atof(argv[4]);

  ImageReaderType::Pointer leftReader =  ImageReaderType::New();
  ImageReaderType::Pointer rightReader =  ImageReaderType::New();

  leftReader->SetFileName(argv[1]);
  rightReader->SetFileName(argv[2]);

// Software Guide : BeginLatex
// The image pair is supposed to be in sensor geometry. From two images covering
// nearly the same area, one can estimate a common epipolar geometry. In this geometry,
// an altitude variation corresponds to an horizontal shift between the two images.
// The filter \doxygen{otb}{StereorectificationDeformationFieldSource} computes the
// deformation grids for each image.
//
// These grids are sampled in epipolar geometry. They have two bands, containing
// the position offset (in physical space units) between the current epipolar
// point and the corresponding sensor point in horizontal and vertical
// direction. They can be computed at a lower resolution than sensor
// resolution. The application \code{StereoRectificationGridGenerator} also
// provides a simple tool to generate the epipolar grids for your image pair.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  DeformationFieldSourceType::Pointer m_DeformationFieldSource = DeformationFieldSourceType::New();
  m_DeformationFieldSource->SetLeftImage(leftReader->GetOutput());
  m_DeformationFieldSource->SetRightImage(rightReader->GetOutput());
  m_DeformationFieldSource->SetGridStep(4);
  m_DeformationFieldSource->SetScale(1.0);
  m_DeformationFieldSource->SetAverageElevation(avgElevation);

  m_DeformationFieldSource->Update();
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// Then, the sensor images can be resampled in epipolar geometry, using the
// \doxygen{otb}{StreamingWarpImageFilter}. The application
// \code{GridBasedImageResampling} also gives an easy access to this filter. The user
// can choose the epipolar region to resample, as well as the resampling step
// and the interpolator.
//
// Note that the epipolar image size can be retrieved from the stereo rectification grid
// filter.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  FloatImageType::SpacingType epipolarSpacing;
  epipolarSpacing[0] = 1.0;
  epipolarSpacing[1] = 1.0;

  FloatImageType::SizeType epipolarSize;
  epipolarSize = m_DeformationFieldSource->GetRectifiedImageSize();

  FloatImageType::PointType epipolarOrigin;
  epipolarOrigin[0] = 0.0;
  epipolarOrigin[1] = 0.0;

  FloatImageType::PixelType defaultValue = 0;
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// The deformation grids are cast into deformation fields, then the left
// and right sensor images are resampled.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
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
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// Since the resampling produces black regions around the image, it is useless
// to estimate disparities on these \textit{no-data} regions. We use a \doxygen{otb}{BandMathImageFilter}
// to produce a mask on left and right epipolar images.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  BandMathFilterType::Pointer m_LBandMathFilter = BandMathFilterType::New();
  m_LBandMathFilter->SetNthInput(0,m_LeftWarpImageFilter->GetOutput(),"inleft");
  std::string leftExpr = "if(inleft != 0,255,0)";
  m_LBandMathFilter->SetExpression(leftExpr);

  BandMathFilterType::Pointer m_RBandMathFilter = BandMathFilterType::New();
  m_RBandMathFilter->SetNthInput(0,m_RightWarpImageFilter->GetOutput(),"inright");
  std::string rightExpr = "if(inright != 0,255,0)";
  m_RBandMathFilter->SetExpression(rightExpr);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// Once the two sensor images have been resampled in epipolar geometry, the
// disparity map can be computed. The approach presented here is a 2D matching
// based on a pixel-wise metric optimization. This approach doesn't give the best
// results compared to global optimization methods, but it is suitable for
// streaming and threading on large images.
//
// The major filter used for this step is \doxygen{otb}{PixelWiseBlockMatchingImageFilter}.
// The metric is computed on a window centered around the tested epipolar position.
// It performs a pixel-to-pixel matching between the two epipolar images. The output disparities
// are given as index offset from left to right position. The following features are available
// in this filter:
// \begin{itemize}
// \item Available metrics : SSD, NCC and $L^{p}$ pseudo norm (computed on a square window)
// \item Rectangular disparity exploration area.
// \item Input masks for left and right images (optional).
// \item Output metric values (optional).
// \item Possibility to use input disparity estimate (as a uniform value or a full map) and an
// exploration radius around these values to reduce the size of the exploration area (optional).
// \end{itemize}
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
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
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// Some other filters have been added to enhance these \textit{pixel-to-pixel} disparities. The filter
// \doxygen{otb}{SubPixelDisparityImageFilter} can estimate the disparities with sub-pixel
// precision. Several interpolation methods can be used : parabolic fit, triangular fit, and
// dichotomy search.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  NCCSubPixelDisparityFilterType::Pointer m_NCCSubPixFilter = NCCSubPixelDisparityFilterType::New();
  m_NCCSubPixFilter->SetInputsFromBlockMatchingFilter(m_NCCBlockMatcher);
  m_NCCSubPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::DICHOTOMY);
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// The filter \doxygen{otb}{DisparityMapMedianFilter} can be used to remove outliers. It has two
// parameters:
// \begin{itemize}
// \item The radius of the local neighborhood to compute the median
// \item An incoherence threshold to reject disparities whose distance from the local median
// is superior to the threshold.
// \end{itemize}
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  MedianFilterType::Pointer m_HMedianFilter = MedianFilterType::New();
  m_HMedianFilter->SetInput(m_NCCSubPixFilter->GetHorizontalDisparityOutput());
  m_HMedianFilter->SetRadius(2);
  m_HMedianFilter->SetIncoherenceThreshold(2.0);
  m_HMedianFilter->SetMaskInput(m_LBandMathFilter->GetOutput());

  MedianFilterType::Pointer m_VMedianFilter = MedianFilterType::New();
  m_VMedianFilter->SetInput(m_NCCSubPixFilter->GetVerticalDisparityOutput());
  m_VMedianFilter->SetRadius(2);
  m_VMedianFilter->SetIncoherenceThreshold(2.0);
  m_VMedianFilter->SetMaskInput(m_LBandMathFilter->GetOutput());
// Software Guide : EndCodeSnippet

// Software Guide : BeginLatex
// The application \code{PixelWiseBlockMatching} contains all these filters and
// provides a single interface to compute your disparity maps.
//
// The disparity map obtained with the previous step usually gives a good idea of
// the altitude profile. However, it is more useful to study altitude with a DEM (Digital
// Elevation Model) representation.
//
// The filter \doxygen{otb}{DisparityMapToDEMFilter} performs this last step. The behavior
// of this filter is to :
// \begin{itemize}
// \item Compute the DEM extent from the left sensor image envelope (spacing is set by the user)
// \item Compute the left and right rays corresponding to each valid disparity
// \item Compute the intersection with the \textit{mid-point} method
// \item If the 3D point falls inside a DEM cell and has a greater elevation than the
// current height, the cell height is updated
// \end{itemize}
// The rule of keeping the highest elevation makes sense for buildings seen from the side
// because the roof edges elevation has to be kept. However this rule is not suited for
// noisy disparities.
//
// The application \code{DisparityMapToElevationMap} also gives an example of use.
// Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  DisparityToElevationFilterType::Pointer m_DispToElev = DisparityToElevationFilterType::New();
  m_DispToElev->SetHorizontalDisparityMapInput(m_HMedianFilter->GetOutput());
  m_DispToElev->SetVerticalDisparityMapInput(m_VMedianFilter->GetOutput());
  m_DispToElev->SetLeftInput(leftReader->GetOutput());
  m_DispToElev->SetRightInput(rightReader->GetOutput());
  m_DispToElev->SetLeftEpipolarGridInput(m_DeformationFieldSource->GetLeftDeformationFieldOutput());
  m_DispToElev->SetRightEpipolarGridInput(m_DeformationFieldSource->GetRightDeformationFieldOutput());
  m_DispToElev->SetElevationMin(130.0);
  m_DispToElev->SetElevationMax(220.0);
  m_DispToElev->SetDEMGridStep(2.5);
  m_DispToElev->SetDisparityMaskInput(m_LBandMathFilter->GetOutput());
  m_DispToElev->SetAverageElevation(avgElevation);

  WriterType::Pointer m_DEMWriter = WriterType::New();
  m_DEMWriter->SetInput(m_DispToElev->GetOutput());
  m_DEMWriter->SetFileName(argv[3]);
  m_DEMWriter->Update();
// Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
