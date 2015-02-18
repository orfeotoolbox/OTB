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
#ifndef __otbDisparityMapMedianFilter_h
#define __otbDisparityMapMedianFilter_h

#ifdef ITK_USE_CONSOLIDATED_MORPHOLOGY
#include "itkOptMedianImageFilter.h"
#else

#include "itkImageToImageFilter.h"
#include "otbImage.h"

namespace otb
{
/** \class DisparityMapMedianFilter
 * \brief Applies a median filter to a monoband disparity map with an associated mask
 *
 * Computes an image where a given pixel is the median value of the
 * the pixels in a neighborhood about the corresponding input pixel. Pixels taken into
 * account for the median computation have a value different from zero in the associated mask.
 *
 * A detection of incoherences between the input disparity map and the median is performed (a pixel corresponds
 * to an incoherence if the absolute value of the difference between the pixel value in the disparity map and in the median
 * image is higher than the incoherence threshold (whose default value is 1). The input disparity map and mask are updated: their value on incoherences becomes 0.
 *
 * The median image is then computed again on incoherences using the updated disparity map and mask.
 *
 * Inputs (with corresponding method):
 *  - disparity map  (SetInput())
 *  - associated mask (SetMaskInput())
 *  - radius (SetRadius())
 *  - incoherence threshold (SetIncoherenceThreshold())
 * Outputs (with corresponding method):
 *        - median image (GetOutput())
 *  - mask associated to the median image (GetOutputMask())
 *  - updated disparity map with incoherence removal (GetOutputDisparityMap())
 *  - mask associated to the updated disparity map (GetOutputDisparityMask())
 *
 * A median filter is one of the family of nonlinear filters.  It is
 * used to smooth an image without being biased by outliers or shot noise.
 *
 * This filter requires that the input pixel type provides an operator<()
 * (LessThan Comparable).
 *
 * \sa Image
 * \sa Neighborhood
 * \sa NeighborhoodOperator
 * \sa NeighborhoodIterator
 *
 * \ingroup IntensityImageFilters
 */
template <class TInputImage, class TOutputImage, class TMask>
class ITK_EXPORT DisparityMapMedianFilter :
    public itk::ImageToImageFilter< TInputImage, TOutputImage>
{
public:
  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Convenient typedefs for simplifying declarations. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
  typedef TMask        MaskImageType;


  /** Standard class typedefs. */
  typedef DisparityMapMedianFilter                                  Self;
  typedef itk::ImageToImageFilter< InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                   Pointer;
  typedef itk::SmartPointer<const Self>                             ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DisparityMapMedianFilter, ImageToImageFilter);

  /** Image typedef support. */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename MaskImageType::PixelType   MaskImagePixelType;
  typedef typename MaskImageType::Pointer     MaskImagePointerType;

  typedef typename InputImageType::RegionType       InputImageRegionType;
  typedef typename OutputImageType::RegionType      OutputImageRegionType;
  typedef typename InputImageType::SizeType         SizeType;
  typedef typename OutputImageType::IndexValueType  IndexValueType;

  /** Set input mask **/
  void SetMaskInput( const TMask * inputmask); // mask corresponding to the subpixel disparity map

  /** Get the input mask */
  const TMask * GetMaskInput();

  /** Get the output mask  **/
  TMask * GetOutputMask(); // mask corresponding to the median disparity map

  /** Get the updated disparity map **/
  TOutputImage * GetOutputDisparityMap(); // input disparity map updated (incoherences between median and input disparity map are removed)

  /** Get the updated disparity mask **/
  TMask * GetOutputDisparityMask();

  /** Set/Get the radius of the neighborhood used to compute the median. */
  itkSetMacro(Radius, SizeType);
  itkGetMacro(Radius, SizeType);

  /** Set unsigned int radius */
  void SetRadius(unsigned int radius)
    {
    m_Radius.Fill(radius);
    }

  /** Set/Get the incoherence threshold */
  itkSetMacro(IncoherenceThreshold, double);
  itkGetMacro(IncoherenceThreshold, double);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck,
                  (itk::Concept::SameDimension<InputImageDimension, OutputImageDimension>));
  itkConceptMacro(InputConvertibleToOutputCheck,
                  (itk::Concept::Convertible<InputPixelType, OutputPixelType>));
  itkConceptMacro(InputLessThanComparableCheck,
                  (itk::Concept::LessThanComparable<InputPixelType>));
  /** End concept checking */
#endif

protected:
  DisparityMapMedianFilter();
  virtual ~DisparityMapMedianFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** MedianImageFilter needs a larger input requested region than
   * the output requested region.  As such, MedianImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError);

  /** Generate output information */
  virtual void GenerateOutputInformation(void);

  /** apply median filter */
  virtual void GenerateData();

private:
  DisparityMapMedianFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Radius of median filter */
  SizeType m_Radius;

  /** Threshold of incoherence between original and filtered disparity */
  double m_IncoherenceThreshold;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDisparityMapMedianFilter.txx"
#endif

#endif

#endif
