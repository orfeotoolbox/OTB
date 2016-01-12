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
#ifndef __otbDisparityMapTo3DFilter_h
#define __otbDisparityMapTo3DFilter_h

#include "itkImageToImageFilter.h"
#include "otbGenericRSTransform.h"
#include "otbLineOfSightOptimizer.h"
#include "otbVectorImage.h"
#include "otbImage.h"

namespace otb
{

/** \class DisparityMapTo3DFilter
 *  \brief Project an input disparity map into a 3D points
 *
 *  This filter uses an input disparity map (horizontal and vertical) to project 3D points.
 *  The output image contains the 3D points coordinates for each location of input disparity.
 *  The 3D coordinates (sorted by band) are : longitude , latitude (in degree, wrt WGS84) and altitude (in meters)
 *
 *  \sa FineRegistrationImageFilter
 *  \sa StereorectificationDisplacementFieldSource
 *  \sa SubPixelDisparityImageFilter
 *  \sa PixelWiseBlockMatchingImageFilter
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 *
 * \ingroup OTBDisparityMap
 */
template <class TDisparityImage, class TOutputImage =  otb::VectorImage<float,2>,
          class TEpipolarGridImage = otb::VectorImage<float,2> , class TMaskImage = otb::Image<unsigned char> >
class ITK_EXPORT DisparityMapTo3DFilter :
    public itk::ImageToImageFilter<TDisparityImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef DisparityMapTo3DFilter                            Self;
  typedef itk::ImageToImageFilter<TDisparityImage,
                                  TOutputImage>             Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DisparityMapTo3DFilter, ImageToImageFilter);

  /** Usefull typedefs */
  typedef TDisparityImage         DisparityMapType;
  typedef TOutputImage            OutputImageType;
  typedef TEpipolarGridImage      GridImageType;
  typedef TMaskImage              MaskImageType;

  typedef typename OutputImageType::RegionType         RegionType;
  typedef typename OutputImageType::PixelType          DEMPixelType;

  // 3D RS transform
  // TODO: Allow tuning precision (i.e. double or float)
  typedef double                                      PrecisionType;
  typedef otb::GenericRSTransform<PrecisionType,3,3>  RSTransformType;

  // 3D points
  typedef typename RSTransformType::InputPointType  TDPointType;

  typedef otb::LineOfSightOptimizer<PrecisionType>  OptimizerType;
  typedef typename OptimizerType::PointSetType      PointSetType;
  typedef typename PointSetType::PointsContainer    PointsContainer;
  typedef typename PointSetType::PointDataContainer LabelContainer;

  typedef otb::ImageKeywordlist                     ImageKeywordListType;

  /** Set horizontal disparity map input */
  void SetHorizontalDisparityMapInput( const TDisparityImage * hmap);

  /** Set vertical disparity map input */
  void SetVerticalDisparityMapInput( const TDisparityImage * vmap);

  /** Set left epipolar grid (deformation grid from sensor image to epipolar space, regular in epipolar space)*/
  void SetLeftEpipolarGridInput( const TEpipolarGridImage * grid);

  /** Set right epipolar grid (deformation grid from sensor image to epipolar space, regular in epipolar space)*/
  void SetRightEpipolarGridInput( const TEpipolarGridImage * grid);

  /** Set mask associated to disparity maps (optional, pixels with a null mask value are ignored) */
  void SetDisparityMaskInput( const TMaskImage * mask);

  /** Get the inputs */
  const TDisparityImage * GetHorizontalDisparityMapInput() const;
  const TDisparityImage * GetVerticalDisparityMapInput() const;
  const TEpipolarGridImage * GetLeftEpipolarGridInput() const;
  const TEpipolarGridImage * GetRightEpipolarGridInput() const;
  const TMaskImage  * GetDisparityMaskInput() const;

  /** Set left keywordlist */
  void SetLeftKeywordList(const ImageKeywordListType kwl)
    {
    this->m_LeftKeywordList = kwl;
    this->Modified();
    }

  /** Get left keywordlist */
  const ImageKeywordListType & GetLeftKeywordList() const
    {
    return this->m_LeftKeywordList;
    }

   /** Set right keywordlist */
  void SetRightKeywordList(const ImageKeywordListType kwl)
    {
    this->m_RightKeywordList = kwl;
    this->Modified();
    }

  /** Get right keywordlist */
  const ImageKeywordListType & GetRightKeywordList() const
    {
    return this->m_RightKeywordList;
    }

protected:
  /** Constructor */
  DisparityMapTo3DFilter();

  /** Destructor */
  virtual ~DisparityMapTo3DFilter();

  /** Generate output information */
  virtual void GenerateOutputInformation();

  /** Generate input requrested region */
  virtual void GenerateInputRequestedRegion();

  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId);

  /** Override VerifyInputInformation() since this filter's inputs do
    * not need to occupy the same physical space.
    *
    * \sa ProcessObject::VerifyInputInformation
    */
  virtual void VerifyInputInformation() {}


private:
  DisparityMapTo3DFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Keywordlist of left sensor image */
  ImageKeywordListType m_LeftKeywordList;

  /** Keywordlist of right sensor image */
  ImageKeywordListType m_RightKeywordList;

  /** Left sensor image transform */
  RSTransformType::Pointer m_LeftToGroundTransform;

  /** Right sensor image transform */
  RSTransformType::Pointer m_RightToGroundTransform;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDisparityMapTo3DFilter.txx"
#endif

#endif
