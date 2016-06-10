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
#ifndef __otbDisparityTranslateFilter_h
#define __otbDisparityTranslateFilter_h

#include "itkImageToImageFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"

namespace otb
{

/** \class DisparityTranslateFilter
 *  \brief Translate epipolar disparities into sensor disparities
 *
 *  This filter resamples a disparity map computed in epipolar geometry
 *  into a disparity map relative to sensor geometry. The output disparity
 *  map is aligned on the left sensor image. The disparity values are computed
 *  relative to the physical coordinates of the left and right sensor images.
 *  The vertical disparity mask input and the mask are optional.
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 *
 * \ingroup OTBDisparityMap
 */
template <class TDisparityImage,  class TGridImage = otb::VectorImage<float,2> ,
  class TSensorImage = otb::Image<unsigned short,2> , class TMaskImage = otb::Image<unsigned char,2> >
class ITK_EXPORT DisparityTranslateFilter :
    public itk::ImageToImageFilter<TDisparityImage,TDisparityImage>
{
public:
  /** Standard class typedef */
  typedef DisparityTranslateFilter                          Self;
  typedef itk::ImageToImageFilter<TDisparityImage,
                                  TDisparityImage>          Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DisparityTranslateFilter, ImageToImageFilter);

  /** Usefull typedefs */
  typedef TDisparityImage  DispMapType;
  typedef TGridImage       GridType;
  typedef TMaskImage       MaskType;
  typedef TSensorImage     SensorType;

  typedef typename DispMapType::PixelType    PixelType;
  typedef typename DispMapType::RegionType   RegionType;
  typedef typename DispMapType::SizeType     SizeType;
  typedef typename DispMapType::IndexType    IndexType;
  typedef typename DispMapType::SpacingType  SpacingType;
  typedef typename DispMapType::PointType    PointType;
  typedef typename DispMapType::IndexValueType IndexValueType;

  typedef typename GridType::RegionType      GridRegionType;

  /** Set/Get macro for NoDataValue */
  itkSetMacro(NoDataValue, PixelType);
  itkGetConstReferenceMacro(NoDataValue, PixelType);

  /** Set the horizontal disparity map */
  void SetHorizontalDisparityMapInput( const TDisparityImage * hmap );

  /** Set the vertical disparity map */
  void SetVerticalDisparityMapInput( const TDisparityImage * vmap );

  /** Set the inverse left epipolar grid  */
  void SetInverseEpipolarLeftGrid( const TGridImage * lgrid );

  /** Set the direct epipolar right grid */
  void SetDirectEpipolarRightGrid( const TGridImage * rgrid );

  /** Set the mask relative to the input disparity map */
  void SetDisparityMaskInput(const TMaskImage * mask);

  /** Set the left input sensor image
   *  (used as a support for the output disparity map)
   */
  void SetLeftSensorImageInput( const TSensorImage * left);

  /** Get the inputs */
  const TDisparityImage * GetHorizontalDisparityMapInput() const;
  const TDisparityImage * GetVerticalDisparityMapInput() const;
  const TGridImage * GetInverseEpipolarLeftGrid() const;
  const TGridImage * GetDirectEpipolarRightGrid() const;
  const TMaskImage * GetDisparityMaskInput() const;
  const TSensorImage * GetLeftSensorImageInput() const;

  /** Get output disparity maps */
  TDisparityImage * GetHorizontalDisparityMapOutput();
  TDisparityImage * GetVerticalDisparityMapOutput();

protected:
  /** Constructor */
  DisparityTranslateFilter();

  /** Destructor */
  ~DisparityTranslateFilter() ITK_OVERRIDE{};

  /** Generate output information */
  void GenerateOutputInformation() ITK_OVERRIDE;

  /** Generate input requrested region */
  void GenerateInputRequestedRegion() ITK_OVERRIDE;

  /** Threaded generate data */
  void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId) ITK_OVERRIDE;

  /** Override VerifyInputInformation() since this filter's inputs do
    * not need to occupy the same physical space.
    *
    * \sa ProcessObject::VerifyInputInformation
    */
 void VerifyInputInformation() ITK_OVERRIDE {}


private:
  DisparityTranslateFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  PixelType              m_NoDataValue;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDisparityTranslateFilter.txx"
#endif

#endif
