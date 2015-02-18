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
#ifndef __otbVectorImageToIntensityImageFilter_h
#define __otbVectorImageToIntensityImageFilter_h

#include "itkImageToImageFilter.h"

namespace otb
{
/** \class VectorImageToIntensityImageFilter
 *  \brief This filter implements the computation of the mean of the spectral values of each pixel.
 *
 * The spectral mean
 * is defined as:
 *
 * \f[ SM = \frac{1}{n_{b}} \times \sum_{b=1}^{n_{b}}p(b) \f]
 * with \f$b\f$ being the spectral band and \f$p\f$
 * the current pixel.
 *
 * Since the spectral mean deals with multi-bands image, the InputImage pixels are suposed to
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
class ITK_EXPORT VectorImageToIntensityImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef VectorImageToIntensityImageFilter                  Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(VectorImageToIntensityImageFilter, itk::ImageToImageFilter);

  /** Template parameters typedefs */
  typedef TInputImage                           InputImageType;
  typedef typename InputImageType::ConstPointer InputImageConstPointerType;
  typedef typename InputImageType::RegionType   InputImageRegionType;
  typedef typename InputImageType::PixelType    InputPixelType;
  typedef TOutputImage                          OutputImageType;
  typedef typename OutputImageType::Pointer     OutputImagePointerType;
  typedef typename OutputImageType::RegionType  OutputImageRegionType;
  typedef typename OutputImageType::PixelType   OutputPixelType;

protected:
  /** Constructor */
  VectorImageToIntensityImageFilter();
  /** Destructor */
  virtual ~VectorImageToIntensityImageFilter() {}
  /**PrintSelf method */
  virtual void PrintSelf(std::ostream& os, itk::Indent indent) const;
  /** VectorImageToIntensityImageFilter can be implemented as a multithreaded filter.
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

private:
  VectorImageToIntensityImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};
} // End namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImageToIntensityImageFilter.txx"
#endif

#endif
