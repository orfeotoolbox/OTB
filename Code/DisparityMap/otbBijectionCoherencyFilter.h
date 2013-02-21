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
#ifndef __otBijectionCoherencyFilter_h
#define __otBijectionCoherencyFilter_h

#include "itkImageToImageFilter.h"
#include "otbLineOfSightOptimizer.h"
#include "otbImage.h"

namespace otb
{

/** \class BijectionCoherencyFilter
 *  \brief Left-right coherency filter for disparity maps
 *
 *  This filter checks that left-to-right and right-to-left disparities (computed
 *  for a given stereo pair) are compatible, within a given tolerance radius.
 *  Disparities are expected to be given in index coordinates between a pair of
 *  epipolar images of equal size. 1D or 2D disparities may be supplied.
 *  Left-to-right disparities will be referred as direct, right-to-left as
 *  reverse.
 *  The output is a mask for coherent direct disparities.
 *
 *  \ingroup Streamed
 *  \ingroup Threaded
 *
 */
template <class TDisparityImage, class TOutputImage = otb::Image<unsigned char> >
class ITK_EXPORT BijectionCoherencyFilter :
    public itk::ImageToImageFilter<TDisparityImage,TOutputImage>
{
public:
  /** Standard class typedef */
  typedef BijectionCoherencyFilter                            Self;
  typedef itk::ImageToImageFilter<TDisparityImage,
                                  TOutputImage>             Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BijectionCoherencyFilter, ImageToImageFilter);

  /** Usefull typedefs */
  typedef TDisparityImage  DispMapType;
  typedef TOutputImage     MaskType;
  
  typedef TOutputImage::RegionType      OutputRegionType;
  typedef TDisparityImage::RegionType   InputRegionType;
  typedef TDisparityImage::SizeType     SizeType;
  
  /** Set the direct horizontal disparity map */
  void SetDirectHorizontalDisparityMapInput( const TDisparityImage * hmap );
  
  /** Set the direct vertical disparity map */
  void SetDirectVerticalDisparityMapInput( const TDisparityImage * vmap );
  
  /** Set the reverse horizontal disparity map */
  void SetReverseHorizontalDisparityMapInput( const TDisparityImage * hmap );
  
  /** Set the reverse vertical disparity map */
  void SetReverseVerticalDisparityMapInput( const TDisparityImage * vmap );
  
  /** Get the inputs */
  const TDisparityImage * GetDirectHorizontalDisparityMapInput() const;
  const TDisparityImage * GetDirectVerticalDisparityMapInput() const;
  const TDisparityImage * GetReverseHorizontalDisparityMapInput() const;
  const TDisparityImage * GetReverseVerticalDisparityMapInput() const;
  
  /** Set the tolerance radius */
  itkSetMacro(Tolerance,double);
  
  /** Get the tolerance radius */
  itkGetMacro(Tolerance,double);

protected:
  /** Constructor */
  BijectionCoherencyFilter();

  /** Destructor */
  virtual ~BijectionCoherencyFilter(){};

  /** Generate output information */
  virtual void GenerateOutputInformation();

  /** Generate input requrested region */
  virtual void GenerateInputRequestedRegion();
  
  /** Threaded generate data */
  virtual void ThreadedGenerateData(const RegionType & outputRegionForThread, int threadId);
  
private:
  BijectionCoherencyFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  /** Tolerance radius (in pixels) */
  double m_Tolerance;
  
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbBijectionCoherencyFilter.txx"
#endif

#endif
