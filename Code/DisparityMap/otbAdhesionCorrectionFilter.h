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
#ifndef __otbAdhesionCorrectionFilter_h
#define __otbAdhesionCorrectionFilter_h

#include "itkImageToImageFilter.h"
#include "otbImage.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include <vcl_algorithm.h>

namespace otb
{

/** \class AdhesionCorrectionFilter
 * \brief This filter eliminates pixels risking adhesion from the input disparity map.
 * To do so it applies the following steps :
 * - computation of incoherence areas (look for jumps in the disparity map)
 * - computation of no detected jumps thanks to an image representing edges (Canny filtering)
 * - computation of intersections between jumps in the disparity map
 * - creation of a risk map (pixels potentially affected by adhesion)
 * - computation of extrema in the risk map
 * - extension of risk map borders: pixels touching borders of the risk map are removed
 *
 * Inputs are provided as follows:
 *  - SetInput() provides an image of edges detected in the reference image (for instance using Canny algorithm)
 *  - SetMedianDisparityInput() provides the median disparity map
 *  - SetMedianMaskInput()  provides the mask associated to the median disparity map
 *  - SetEdgesDisparityInput() provides an image of edges detected in the median disparity map
 *  - SetSubPixelMaskInput() provides the mask associated to the subpixel disparity map
 *
 * The filter proposes as outputs:
 *  - GetOutput() provides the disparity map after adhesion correction
 *  - GetOutputMask() provides the mask associated to the disparity map after adhesion correction
 *  - GetOutputRiskEdges() provides the map of risk edges
 *
 *
 * \sa      FineRegistrationImageFilter, FastCorrelationImageFilter, DisparityMapEstimationMethod, DisparityMapAndMaskEstimationFilter
 * \ingroup IntensityImageFilters, Streamed
 *
 * See Publications : N. Sabater, J.M. Morel, A. Almansa and G. Blanchet, "Discarding moving objects in quasi-simultaneous stereovision", in IEEE International Conference on Image Processing, ICIP'10, 2010.
 *                   N. Sabater, J.M. Morel and A. Almansa, "Sub-pixel stereo matching", in IEEE International Geoscience and Remote Sensing Symposium, IGARSS'10, 2010.
 */


template <class TImage, class TMask>
class ITK_EXPORT AdhesionCorrectionFilter : public itk::ImageToImageFilter<TImage, TImage>
{
public:
  /** Standard class typedefs. */
  typedef AdhesionCorrectionFilter                                Self;
  typedef itk::ImageToImageFilter<TImage,TImage>  Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AdhesionCorrectionFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef typename TImage::RegionType            ImageRegionType;
  typedef typename TImage::PixelType             PixelType;
  typedef typename TImage::Pointer               ImagePointerType;
  typedef typename TImage::SizeType              SizeType;
  typedef typename TImage::IndexType             IndexType;
  typedef typename TImage::SpacingType           SpacingType;
  typedef typename TImage::PointType             PointType;
  typedef typename TImage::OffsetType            OffsetType;
  typedef typename TMask::Pointer                MaskImagePointerType;
  typedef typename TMask::PixelType              MaskImagePixelType;

  typedef itk::ConstNeighborhoodIterator<TImage> NeighborhoodIteratorType;

  typedef otb::Image<double, 2> AuxImageType;
  typedef typename AuxImageType::Pointer AuxImagePointerType;

  typedef std::vector<int>  IntVectorType;

  /** Set input images **/
  void SetMedianDisparityInput( const TImage * medianmap);
  void SetMedianMaskInput( const TMask * medianmask);
  void SetEdgesDisparityInput( const TImage * cannymedianmap);
  void SetSubPixelMaskInput( const TMask * subpixelmask);

  /** Get the inputs */
  const TImage * GetMedianDisparityInput();
  const TMask * GetMedianMaskInput();
  const TImage * GetEdgesDisparityInput();
  const TMask * GetSubPixelMaskInput();

  /** Get the output mask  **/
  TMask * GetOutputMask(); // mask corresponding to the corrected disparity map

  /** Get the output risk edges **/
  TImage * GetOutputRiskEdges();

  /** Set/Get the radius */
  itkSetMacro(Radius, SizeType);
  itkGetMacro(Radius, SizeType);

  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
    {
    m_Radius.Fill(radius);
    }

  /** Set/Get the threshold for the tolerated difference between disparities */
  itkSetMacro(Tolerance, double);
  itkGetMacro(Tolerance, double);

  /** Set/Get the discontinuity threshold  */
  itkSetMacro(DiscontinuityThreshold, double);
  itkGetMacro(DiscontinuityThreshold, double);

  /** Set/Get the high discontinuity threshold   */
  itkSetMacro(DiscontinuityHighThreshold, double);
  itkGetMacro(DiscontinuityHighThreshold, double);

  /** Set/Get the maximum edge gap  */
  itkSetMacro(MaxEdgeGap, double);
  itkGetMacro(MaxEdgeGap, double);

  /** Set/Get the edge threshold  */
  itkSetMacro(EdgeThreshold, double);
  itkGetMacro(EdgeThreshold, double);

protected:
  /** Constructor */
  AdhesionCorrectionFilter();
  /** Destructor */
  virtual ~AdhesionCorrectionFilter() {};

  /** Threaded generate data */
  virtual void GenerateData();

  /** Generate the input requested regions  */
  virtual void GenerateInputRequestedRegion(void);

  /** Generate output information */
  virtual void GenerateOutputInformation(void);

private:
  AdhesionCorrectionFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  SizeType                      m_ImageSize;
  /** The radius for correlation */
  SizeType                      m_Radius;

  /** Tolerance  */
  double                        m_Tolerance;

  /** Thresholds  */
  double                        m_DiscontinuityThreshold;
  double                        m_DiscontinuityHighThreshold;
  double                        m_MaxEdgeGap;
  double                        m_EdgeThreshold;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbAdhesionCorrectionFilter.txx"
#endif

#endif

