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
#ifndef __otbMultiDisparityMapTo3DFilter_h
#define __otbMultiDisparityMapTo3DFilter_h

#include "itkImageToImageFilter.h"
#include "otbGenericRSTransform.h"
#include "otbLineOfSightOptimizer.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionConstIterator.h"
#include "otbVectorImage.h"
#include "otbImage.h"

namespace otb
{

/** \class MultiDisparityMapTo3DFilter
 *  \brief Project a group of disparity map into 3D points
 *
 *  This filter uses a group of N horizontal disparity map (and vertical if any) to project 3D points.
 *  The output image contains the 3D points coordinates for each location of input disparity. The
 *  3D positions are compute by a least square optimisation between the N lines of sight.
 *  The 3D coordinates (sorted by band) are : longitude , latitude (in degree, wrt WGS84) and altitude (in meters)
 *  BEWARE : this filter is not fully compatible with the filters using the epipolar geometry. The N disparity
 *  maps shall come from the matching of a single 'reference' sensor image versus N 'moving' sensor images. In
 *  addition, the disparities shall be computed in physical space (not in index space)
 *  N disparity masks can be provided for each disparity map.
 *
 *  \sa FineRegistrationImageFilter
 *  \sa LineOfSightOptimizer
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
          class TMaskImage = otb::Image<unsigned char>, class TResidueImage = otb::Image<float> >
class ITK_EXPORT MultiDisparityMapTo3DFilter :
    public itk::ImageToImageFilter<TDisparityImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef MultiDisparityMapTo3DFilter                       Self;
  typedef itk::ImageToImageFilter<TDisparityImage,
                                  TOutputImage>             Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiDisparityMapTo3DFilter, ImageToImageFilter);

  /** Usefull typedefs */
  typedef TDisparityImage         DisparityMapType;
  typedef TOutputImage            OutputImageType;
  typedef TMaskImage              MaskImageType;
  typedef TResidueImage           ResidueImageType;

  typedef typename OutputImageType::RegionType         RegionType;
  typedef typename OutputImageType::PixelType          DEMPixelType;

  // 3D RS transform
  // TODO: Allow tuning precision (i.e. double or float)
  typedef double                  PrecisionType;
  typedef otb::GenericRSTransform
    <PrecisionType,3,3>           RSTransformType;

  // 3D points
  typedef typename RSTransformType::InputPointType  TDPointType;

  typedef otb::LineOfSightOptimizer<PrecisionType>  OptimizerType;
  typedef typename OptimizerType::PointSetType      PointSetType;
  typedef typename PointSetType::PointsContainer    PointsContainer;
  typedef typename PointSetType::PointDataContainer LabelContainer;

  typedef otb::ImageKeywordlist                     ImageKeywordListType;

  typedef std::map
    <unsigned int,
     itk::ImageRegionConstIterator<DisparityMapType> >   DispMapIteratorList;

  typedef std::map
    <unsigned int,
     itk::ImageRegionConstIterator<MaskImageType> >      MaskIteratorList;

  /** Set the number of moving images (referred earlier as N) */
  void SetNumberOfMovingImages(unsigned int nb);

  /** Get the number of moving images (referred earlier as N) */
  unsigned int GetNumberOfMovingImages();

  /** Set horizontal disparity map input corresponding to the moving image 'index' */
  void SetHorizontalDisparityMapInput(unsigned int index, const TDisparityImage * hmap);

  /** Set vertical disparity map input corresponding to the moving image 'index'*/
  void SetVerticalDisparityMapInput(unsigned int index, const TDisparityImage * vmap);

  /** Set mask associated to disparity maps corresponding to the moving image 'index'
   * (optional, pixels with a null mask value are ignored)
   */
  void SetDisparityMaskInput(unsigned int index, const TMaskImage * mask);

  /** Get the inputs */
  const TDisparityImage * GetHorizontalDisparityMapInput(unsigned int index) const;
  const TDisparityImage * GetVerticalDisparityMapInput(unsigned int index) const;
  const TMaskImage  * GetDisparityMaskInput(unsigned int index) const;

  /** Get residue output*/
  const TResidueImage * GetResidueOutput() const;
  TResidueImage * GetResidueOutput();

  /** Set keywordlist of the reference image */
  void SetReferenceKeywordList(const ImageKeywordListType kwl)
    {
    this->m_ReferenceKeywordList = kwl;
    this->Modified();
    }

  /** Get keywordlist of the reference image */
  const ImageKeywordListType & GetReferenceKeywordList() const
    {
    return this->m_ReferenceKeywordList;
    }

   /** Set keywordlist of the moving image 'index' */
  void SetMovingKeywordList(unsigned int index, const ImageKeywordListType kwl);

  /** Get keywordlist of the moving image 'index' */
  const ImageKeywordListType & GetMovingKeywordList(unsigned int index) const;

protected:
  /** Constructor */
  MultiDisparityMapTo3DFilter();

  /** Destructor */
  virtual ~MultiDisparityMapTo3DFilter();

  /** Generate output information */
  virtual void GenerateOutputInformation();

  /** Generate input requrested region */
  virtual void GenerateInputRequestedRegion();

  /** Before threaded generate data */
  virtual void BeforeThreadedGenerateData();

  /** Threaded generate data */
  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId);

private:
  MultiDisparityMapTo3DFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Keywordlist of reference sensor image */
  ImageKeywordListType m_ReferenceKeywordList;

  /** Keywordlists of moving sensor images */
  std::vector<ImageKeywordListType> m_MovingKeywordLists;

  /** Reference sensor image transform */
  RSTransformType::Pointer m_ReferenceToGroundTransform;

  /** Moving sensor image transforms */
  std::vector<RSTransformType::Pointer> m_MovingToGroundTransform;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiDisparityMapTo3DFilter.txx"
#endif

#endif
