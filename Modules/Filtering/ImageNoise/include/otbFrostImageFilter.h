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
#ifndef __otbFrostImageFilter_h
#define __otbFrostImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class FrostImageFilter
 * \brief Anti-speckle image filter
 *
 * Uses a negative exponential convolution kernel.
 * The output of the filter for pixel p is:
 *      \f$ \hat I_{s}=\sum_{p\in\eta_{p}} m_{p}I_{p} \f$
 *
 * where :   \f$ m_{p}=\frac{KC_{s}^{2}\exp(-KC_{s}^{2}d_{s, p})}{\sum_{p\in\eta_{p}} KC_{s}^{2}\exp(-KC_{s}^{2}d_{s, p})} \f$
 *    and  \f$ d_{s, p}=\sqrt{(i-i_{p})^2+(j-j_{p})^2} \f$
 *
 * \f$ K \f$     : the decrease coefficient
 * \f$ (i, j)\f$ : the coordinates of the pixel inside the region
 * defined by \f$ \eta_{s} \f$
 * \f$ (i_{p}, j_{p})\f$ : the coordinates of the pixels belonging to \f$ \eta_{p} \subset \eta_{s} \f$
 * \f$ C_{s}\f$ : the variation coefficient computed over \f$ \eta_{p}\f$
 *
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT FrostImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Extract input and output images sizes. */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  /** typedef to simplify variables definition and declaration. */
  typedef TInputImage InputImageType;
  /** typedef to simplify variables definition and declaration. */
  typedef TOutputImage OutputImageType;

  /** typedef for standard classes. */
  typedef FrostImageFilter                                         Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** "object factory" management method. */
  itkNewMacro(Self);

  /** Return the class name. */
  itkTypeMacro(FrostImageFilter, ImageToImageFilter);

  /** Supported images definition. */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  /** "typedef" to define a real. */
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  /** "typedef" to define an image size. */
  typedef typename InputImageType::SizeType SizeType;

  /** Set the radius used to define the neighborhood for the filter calculation. */
  itkSetMacro(Radius, SizeType);

  /** Get the radius used to define the neighborhood for the filter calculation. */
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Set The numbers of view used for the filter calculation. */
  itkSetMacro(Deramp, double);
  /** Get The numbers of view used for the filter calculation. */
  itkGetConstReferenceMacro(Deramp, double);

  /** To be allowed to use the pipeline method FrostImageFilter needs
    * an input processing area larger than the output one.
    * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion()
    throw(itk::InvalidRequestedRegionError);

protected:
  FrostImageFilter();
  virtual ~FrostImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** FrostImageFilter can be implemented for a multithreaded filter treatment.
   * Thus, this implementation give the ThreadedGenerateData() method.
   * that is called for each process thread. Image datas are automatically allocated
   * throught the parent class calling the ThreadedGenerateData() method.
   * ThreadedGenerateData() can only write the area of the image specified by the parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

private:
  FrostImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Radius declaration */
  SizeType m_Radius;
  /** Decrease factor declaration */
  double m_Deramp;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFrostImageFilter.txx"
#endif

#endif
