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
./StereoReconstructionExample Input/sensor_stereo_left.tif Input/sensor_stereo_right.tif Output/elevationOutput.tif Output/elevationOutputPrintable.png 140
*/


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


#include "otbStereorectificationDisplacementFieldSource.h"
#include "otbStreamingWarpImageFilter.h"
#include "otbBandMathImageFilter.h"
#include "otbSubPixelDisparityImageFilter.h"
#include "otbDisparityMapMedianFilter.h"
#include "otbDisparityMapToDEMFilter.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkUnaryFunctorImageFilter.h"
#include "itkVectorCastImageFilter.h"
#include "otbImageList.h"
#include "otbImageListToVectorImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "otbDEMHandler.h"


int main(int argc, char* argv[])
{
  if (argc != 6)
  {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " sensorImage1 sensorImage2 outputDEM outputDEMPNG";
    std::cerr << "averageElevation  " << std::endl;
    return EXIT_FAILURE;
  }

  using FloatImageType       = otb::Image<float, 2>;
  using FloatVectorImageType = otb::VectorImage<float, 2>;

  using ImageReaderType = otb::ImageFileReader<FloatImageType>;

  using WriterType = otb::ImageFileWriter<FloatImageType>;

  using OutputPixelType = unsigned char;
  using OutputImageType = otb::Image<OutputPixelType, 2>;

  using RescalerType = itk::RescaleIntensityImageFilter<FloatImageType, OutputImageType>;

  using OutputWriterType = otb::ImageFileWriter<OutputImageType>;
  // This example demonstrates the use of the following filters :
  // \begin{itemize}
  // \item \doxygen{otb}{StereorectificationDisplacementFieldSource}
  // \item \doxygen{otb}{StreamingWarpImageFilter}
  // \item \doxygen{otb}{PixelWiseBlockMatchingImageFilter}
  // \item \doxygen{otb}{otbSubPixelDisparityImageFilter}
  // \item \doxygen{otb}{otbDisparityMapMedianFilter}
  // \item \doxygen{otb}{DisparityMapToDEMFilter}
  // \end{itemize}

  using DisplacementFieldSourceType = otb::StereorectificationDisplacementFieldSource<FloatImageType, FloatVectorImageType>;

  using DisplacementType      = itk::Vector<double, 2>;
  using DisplacementFieldType = otb::Image<DisplacementType>;

  using DisplacementFieldCastFilterType = itk::VectorCastImageFilter<FloatVectorImageType, DisplacementFieldType>;

  using WarpFilterType = otb::StreamingWarpImageFilter<FloatImageType, FloatImageType, DisplacementFieldType>;

  using BCOInterpolationType = otb::BCOInterpolateImageFunction<FloatImageType>;

  using NCCBlockMatchingFunctorType = otb::Functor::NCCBlockMatching<FloatImageType, FloatImageType>;

  using NCCBlockMatchingFilterType =
      otb::PixelWiseBlockMatchingImageFilter<FloatImageType, FloatImageType, FloatImageType, FloatImageType, NCCBlockMatchingFunctorType>;

  using BandMathFilterType = otb::BandMathImageFilter<FloatImageType>;

  using NCCSubPixelDisparityFilterType =
      otb::SubPixelDisparityImageFilter<FloatImageType, FloatImageType, FloatImageType, FloatImageType, NCCBlockMatchingFunctorType>;

  using MedianFilterType = otb::DisparityMapMedianFilter<FloatImageType, FloatImageType, FloatImageType>;

  using DisparityToElevationFilterType = otb::DisparityMapToDEMFilter<FloatImageType, FloatImageType, FloatImageType, FloatVectorImageType, FloatImageType>;

  double avgElevation = atof(argv[5]);
  otb::DEMHandler::Instance()->SetDefaultHeightAboveEllipsoid(avgElevation);

  ImageReaderType::Pointer leftReader  = ImageReaderType::New();
  ImageReaderType::Pointer rightReader = ImageReaderType::New();

  leftReader->SetFileName(argv[1]);
  rightReader->SetFileName(argv[2]);

  // The image pair is supposed to be in sensor geometry. From two images covering
  // nearly the same area, one can estimate a common epipolar geometry. In this geometry,
  // an altitude variation corresponds to an horizontal shift between the two images.
  // The filter \doxygen{otb}{StereorectificationDisplacementFieldSource} computes the
  // deformation grids for each image.
  //
  // These grids are sampled in epipolar geometry. They have two bands, containing
  // the position offset (in physical space units) between the current epipolar
  // point and the corresponding sensor point in horizontal and vertical
  // direction. They can be computed at a lower resolution than sensor
  // resolution. The application \code{StereoRectificationGridGenerator} also
  // provides a simple tool to generate the epipolar grids for your image pair.

  DisplacementFieldSourceType::Pointer m_DisplacementFieldSource = DisplacementFieldSourceType::New();
  m_DisplacementFieldSource->SetLeftImage(leftReader->GetOutput());
  m_DisplacementFieldSource->SetRightImage(rightReader->GetOutput());
  m_DisplacementFieldSource->SetGridStep(4);
  m_DisplacementFieldSource->SetScale(1.0);
  // m_DisplacementFieldSource->SetAverageElevation(avgElevation);

  m_DisplacementFieldSource->Update();

  // Then, the sensor images can be resampled in epipolar geometry, using the
  // \doxygen{otb}{StreamingWarpImageFilter}. The application
  // \code{GridBasedImageResampling} also gives an easy access to this filter. The user
  // can choose the epipolar region to resample, as well as the resampling step
  // and the interpolator.
  //
  // Note that the epipolar image size can be retrieved from the stereo rectification grid
  // filter.

  FloatImageType::SpacingType epipolarSpacing;
  epipolarSpacing[0] = 1.0;
  epipolarSpacing[1] = 1.0;

  FloatImageType::SizeType epipolarSize;
  epipolarSize = m_DisplacementFieldSource->GetRectifiedImageSize();

  FloatImageType::PointType epipolarOrigin;
  epipolarOrigin[0] = 0.0;
  epipolarOrigin[1] = 0.0;

  FloatImageType::PixelType defaultValue = 0;

  // The deformation grids are casted into deformation fields, then the left
  // and right sensor images are resampled.

  DisplacementFieldCastFilterType::Pointer m_LeftDisplacementFieldCaster = DisplacementFieldCastFilterType::New();
  m_LeftDisplacementFieldCaster->SetInput(m_DisplacementFieldSource->GetLeftDisplacementFieldOutput());
  m_LeftDisplacementFieldCaster->GetOutput()->UpdateOutputInformation();

  BCOInterpolationType::Pointer leftInterpolator = BCOInterpolationType::New();
  leftInterpolator->SetRadius(2);

  WarpFilterType::Pointer m_LeftWarpImageFilter = WarpFilterType::New();
  m_LeftWarpImageFilter->SetInput(leftReader->GetOutput());
  m_LeftWarpImageFilter->SetDisplacementField(m_LeftDisplacementFieldCaster->GetOutput());
  m_LeftWarpImageFilter->SetInterpolator(leftInterpolator);
  m_LeftWarpImageFilter->SetOutputSize(epipolarSize);
  m_LeftWarpImageFilter->SetOutputSpacing(epipolarSpacing);
  m_LeftWarpImageFilter->SetOutputOrigin(epipolarOrigin);
  m_LeftWarpImageFilter->SetEdgePaddingValue(defaultValue);

  DisplacementFieldCastFilterType::Pointer m_RightDisplacementFieldCaster = DisplacementFieldCastFilterType::New();
  m_RightDisplacementFieldCaster->SetInput(m_DisplacementFieldSource->GetRightDisplacementFieldOutput());
  m_RightDisplacementFieldCaster->GetOutput()->UpdateOutputInformation();

  BCOInterpolationType::Pointer rightInterpolator = BCOInterpolationType::New();
  rightInterpolator->SetRadius(2);

  WarpFilterType::Pointer m_RightWarpImageFilter = WarpFilterType::New();
  m_RightWarpImageFilter->SetInput(rightReader->GetOutput());
  m_RightWarpImageFilter->SetDisplacementField(m_RightDisplacementFieldCaster->GetOutput());
  m_RightWarpImageFilter->SetInterpolator(rightInterpolator);
  m_RightWarpImageFilter->SetOutputSize(epipolarSize);
  m_RightWarpImageFilter->SetOutputSpacing(epipolarSpacing);
  m_RightWarpImageFilter->SetOutputOrigin(epipolarOrigin);
  m_RightWarpImageFilter->SetEdgePaddingValue(defaultValue);

  // Since the resampling produces black regions around the image, it is useless
  // to estimate disparities on these \textit{no-data} regions. We use a \doxygen{otb}{BandMathImageFilter}
  // to produce a mask on left and right epipolar images.

  BandMathFilterType::Pointer m_LBandMathFilter = BandMathFilterType::New();
  m_LBandMathFilter->SetNthInput(0, m_LeftWarpImageFilter->GetOutput(), "inleft");
#ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
  std::string leftExpr = "inleft != 0 ? 255 : 0";
#else
  std::string leftExpr = "if(inleft != 0,255,0)";
#endif

  m_LBandMathFilter->SetExpression(leftExpr);

  BandMathFilterType::Pointer m_RBandMathFilter = BandMathFilterType::New();
  m_RBandMathFilter->SetNthInput(0, m_RightWarpImageFilter->GetOutput(), "inright");

#ifdef OTB_MUPARSER_HAS_CXX_LOGICAL_OPERATORS
  std::string rightExpr = "inright != 0 ? 255 : 0";
#else
  std::string rightExpr = "if(inright != 0,255,0)";
#endif

  m_RBandMathFilter->SetExpression(rightExpr);

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

  // Some other filters have been added to enhance these \textit{pixel-to-pixel} disparities. The filter
  // \doxygen{otb}{SubPixelDisparityImageFilter} can estimate the disparities with sub-pixel
  // precision. Several interpolation methods can be used : parabolic fit, triangular fit, and
  // dichotomy search.

  NCCSubPixelDisparityFilterType::Pointer m_NCCSubPixFilter = NCCSubPixelDisparityFilterType::New();
  m_NCCSubPixFilter->SetInputsFromBlockMatchingFilter(m_NCCBlockMatcher);
  m_NCCSubPixFilter->SetRefineMethod(NCCSubPixelDisparityFilterType::DICHOTOMY);

  // The filter \doxygen{otb}{DisparityMapMedianFilter} can be used to remove outliers. It has two
  // parameters:
  // \begin{itemize}
  // \item The radius of the local neighborhood to compute the median
  // \item An incoherence threshold to reject disparities whose distance from the local median
  // is superior to the threshold.
  // \end{itemize}

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

  DisparityToElevationFilterType::Pointer m_DispToElev = DisparityToElevationFilterType::New();
  m_DispToElev->SetHorizontalDisparityMapInput(m_HMedianFilter->GetOutput());
  m_DispToElev->SetVerticalDisparityMapInput(m_VMedianFilter->GetOutput());
  m_DispToElev->SetLeftInput(leftReader->GetOutput());
  m_DispToElev->SetRightInput(rightReader->GetOutput());
  m_DispToElev->SetLeftEpipolarGridInput(m_DisplacementFieldSource->GetLeftDisplacementFieldOutput());
  m_DispToElev->SetRightEpipolarGridInput(m_DisplacementFieldSource->GetRightDisplacementFieldOutput());
  m_DispToElev->SetElevationMin(avgElevation - 10.0);
  m_DispToElev->SetElevationMax(avgElevation + 80.0);
  m_DispToElev->SetDEMGridStep(2.5);
  m_DispToElev->SetDisparityMaskInput(m_LBandMathFilter->GetOutput());
  // m_DispToElev->SetAverageElevation(avgElevation);

  WriterType::Pointer m_DEMWriter = WriterType::New();
  m_DEMWriter->SetInput(m_DispToElev->GetOutput());
  m_DEMWriter->SetFileName(argv[3]);
  m_DEMWriter->Update();

  RescalerType::Pointer fieldRescaler = RescalerType::New();
  fieldRescaler->SetInput(m_DispToElev->GetOutput());
  fieldRescaler->SetOutputMaximum(255);
  fieldRescaler->SetOutputMinimum(0);

  OutputWriterType::Pointer fieldWriter = OutputWriterType::New();
  fieldWriter->SetInput(fieldRescaler->GetOutput());
  fieldWriter->SetFileName(argv[4]);
  fieldWriter->Update();

  // Figure~\ref{fig:STEREORECONSTRUCTIONOUTPUT} shows the result of applying
  // terrain reconstruction based using pixel-wise block matching, sub-pixel
  // interpolation and DEM estimation using a pair of Pleiades images over the
  // \textit{Stadium} in Toulouse, France.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.4\textwidth]{elevationOutputPrintable.eps}
  // \itkcaption[From stereo pair to elevation]{DEM image estimated from the disparity.}
  // \label{fig:STEREORECONSTRUCTIONOUTPUT}
  // \end{figure}

  return EXIT_SUCCESS;
}
