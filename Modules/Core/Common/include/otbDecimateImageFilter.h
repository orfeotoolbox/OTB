/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbDecimateImageFilter_h
#define __otbDecimateImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"

namespace otb {

/** \class DecimateImageFilter
 * \brief Performs a down sampling of an image
 *
 * This class performs a basic down sampling of an image. On the contrary to
 * itk::ResampleImageFilter, it does not perform any interpolation.
 *
 * \sa ResampleImageFilter
 * \sa SubsampleImageRegionConstIterator
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT DecimateImageFilter :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef DecimateImageFilter                                Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(DecimateImageFilter, ImageToImageFilter);

  /** Extract dimension from input and output image. */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Image typedef support. */
  typedef TInputImage                         InputImageType;
  typedef typename InputImageType::RegionType InputImageRegionType;

  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename OutputImageType::PixelType  OutputPixelType;

  /** Set/Get the DecimateFactor */
  itkGetMacro(DecimationFactor, unsigned int);
  itkSetMacro(DecimationFactor, unsigned int);

protected:
  DecimateImageFilter ()
    {
    m_DecimationFactor = 1;
    }
  virtual ~DecimateImageFilter() {}

  /** Since input and output image are very likely to be of different size.
   * Region estimation functions has to be reimplemented
   */
  virtual void CallCopyOutputRegionToInputRegion
    (InputImageRegionType& destRegion, const OutputImageRegionType& srcRegion);
  virtual void CallCopyInputRegionToOutputRegion
    (OutputImageRegionType& destRegion, const InputImageRegionType& srcRegion);

  /** Output image region size is not of the same dimension as the input.
   * That is why GenerateOutputInformation has to be redefined.
   */
  virtual void GenerateOutputInformation();

  /** Allows multithreading */
  virtual void ThreadedGenerateData
    (const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId);

  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  DecimateImageFilter (const Self &);   // purposely not implemented
  void operator =(const Self&);    // purposely not implemented

  unsigned int m_DecimationFactor;
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbDecimateImageFilter.txx"
#endif

#endif
