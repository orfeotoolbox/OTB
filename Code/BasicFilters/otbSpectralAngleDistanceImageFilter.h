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
#ifndef __otbSpectralAngleDistanceImageFilter_h
#define __otbSpectralAngleDistanceImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class SpectralAngleDistanceImageFilter
 *  \brief This filter implements the computation of the spectral angle
 * distance with respect to a reference pixel.
 *
 * Spectral angle distance is an efficient way to convert spectral information to
 * scalar data with respect to a reference pixel. The spectral angle distance
 * is defined as:
 *
 * \f[SA = cos^{-1}\Big(\frac{{\sum_{b=1}^{n_{b}}}r(b)\cdot p(b)}{\sqrt{{\sum_{b=1}^{n_{b}}}r(b)^{2}{\sum_{b=1}^{n_{b}}p(b)^{2}}}}\Big) \f]
 * with \f$b\f$ being the spectral band, \f$r\f$ the reference pixel and \f$p\f$
 * the current pixel.
 *
 * Since the spectral angle deals with multi-bands image, the InputImage pixels are suposed to
 * support the [] operator, and the input image to support the GetNumberOfComponentsPerPixel() method.
 *
 * \sa VectorImage
 *
 * This filter is implemented as a multithreaded filter.
 *
 * \ingroup IntensityImageFilters
 * \ingroup Threading
 * \ingroup Streamed
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT SpectralAngleDistanceImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef SpectralAngleDistanceImageFilter                   Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(SpectralAngleDistanceImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointerType;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputPixelType;
  /** Get/Set the reference pixel */
  itkGetConstReferenceMacro(ReferencePixel, InputPixelType);
  itkSetMacro(ReferencePixel, InputPixelType);

protected:
  /** Constructor */
  SpectralAngleDistanceImageFilter();
  /** Destructor */
  virtual ~SpectralAngleDistanceImageFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** SpectralAngleDistanceImageFilter can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData() routine
   * which is called for each processing thread. The output image data is
   * allocated automatically by the superclass prior to calling
   * ThreadedGenerateData().  ThreadedGenerateData can only write to the
   * portion of the output image specified by the parameter
   * "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

  void BeforeThreadedGenerateData();

private:
  SpectralAngleDistanceImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** The reference pixel */
  InputPixelType m_ReferencePixel;
};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpectralAngleDistanceImageFilter.txx"
#endif

#endif
