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

#ifndef __otbStreamingResampleImageFilter_h
#define __otbStreamingResampleImageFilter_h

#include "itkResampleImageFilter.h"
#include "otbStreamingTraits.h"

namespace otb
{

/** \class StreamingResampleImageFilter
 * \brief Resample image filter
 *
 * This class add streaming aspect on it::ResampleImageFilter
 */

template <class TInputImage, class TOutputImage, class TInterpolatorPrecisionType=double>
class ITK_EXPORT StreamingResampleImageFilter :  public itk::ResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
{
public:

	/** Standard class typedefs. */
  typedef StreamingResampleImageFilter Self;
  typedef itk::ResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Image related typedefs. */
  typedef typename TInputImage::Pointer InputImagePointer;
  typedef typename TOutputImage::Pointer OutputImagePointer;

	typedef typename TInputImage::IndexType IndexType;
	typedef typename TInputImage::SizeType SizeType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

   /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingResampleImageFilter, itk::ResampleImageFilter);

	/** Type definitions */
  typedef typename TInputImage::PixelType InputPixelType;
  typedef typename TOutputImage::PixelType OutputPixelType;

	typedef typename TInputImage::RegionType InputImageRegionType;
  typedef typename TOutputImage::RegionType OutputImageRegionType;

	typedef typename Superclass::InterpolatorType InterpolatorType;
	typedef typename InterpolatorType::PointType  PointType;

  itkSetMacro(InterpolatorNeighborhoodRadius,unsigned int);
	itkGetMacro(InterpolatorNeighborhoodRadius,unsigned int);

  itkSetMacro(AddedRadius,unsigned int);
	itkGetMacro(AddedRadius,unsigned int);

  /** ResampleImageFilter needs a different input requested region than
   * the output requested region.  As such, ResampleImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   * \sa ProcessObject::GenerateInputRequestedRegion()
	 */
  virtual void GenerateInputRequestedRegion() ;

protected:
  StreamingResampleImageFilter();
  virtual ~StreamingResampleImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;


private:
  StreamingResampleImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

	// Determine size of pad needed for interpolators neighborhood
	unsigned int m_InterpolatorNeighborhoodRadius;

	// Used to be sure that each final region will be contiguous
	unsigned int m_AddedRadius;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingResampleImageFilter.txx"
#endif


#endif
