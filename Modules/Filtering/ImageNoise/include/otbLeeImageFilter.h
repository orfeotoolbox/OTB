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
#ifndef __otbLeeImageFilter_h
#define __otbLeeImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class LeeImageFilter
 * \brief Anti-speckle image filter
 *
 * This class implements Lee's filter for despeckleing of SAR
 * images. The estimated reflectivity \f$R\f$ is computed as follows:

\f[R = E[I] + b(I-E[I]) \f] with
\f$ b = C^2_r / ( C^2_r + C^2_v )\f$ and \f$C_v =
\frac{1}{\sqrt{L}}\f$,  where
\f$L\f$ the image number of looks and
\f$C_r = \frac{\sqrt{Var(I)}}{E[I]} \f$ and  \f$Var(I) = E[I^2] - E[I]^2\f$.
 *
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT LeeImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract input and output image dimension */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** standard class typedefs */
  typedef LeeImageFilter                                           Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Object factory management */
  itkNewMacro(Self);

  /** typemacro */
  itkTypeMacro(LeeImageFilter, ImageToImageFilter);

  typedef typename InputImageType::PixelType                    InputPixelType;
  typedef typename OutputImageType::PixelType                   OutputPixelType;
  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;
  typedef typename InputImageType::RegionType                   InputImageRegionType;
  typedef typename OutputImageType::RegionType                  OutputImageRegionType;
  typedef typename InputImageType::SizeType                     SizeType;

  /** Set the radius of the neighborhood used in this filter */
  itkSetMacro(Radius, SizeType);

  /** Get the radius of the neighborhood used in this filter  */
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Set the number of look used for computation */
  itkSetMacro(NbLooks, double);
  /** Getthe number of look used for computation */
  itkGetConstReferenceMacro(NbLooks, double);

  /** LeeImageFilter needs a larger input requested region than
   * the output requested region.  As such, LeeImageFilter needs
   * to provide an implementation for GenerateInputRequestedRegion()
   * in order to inform the pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion() */
  virtual void GenerateInputRequestedRegion()
    throw(itk::InvalidRequestedRegionError);

protected:
  LeeImageFilter();
  virtual ~LeeImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** LeeImageFilter can be multithreaded.
   * As such, it provides a definition of ThreadedGenerateData()
   *
   *  LEE filter:
   *\f$   R = E[I] + b(I-E[I])\f$ with \f$  b  = C^2r / ( C^2r + C^2v )\f$
   *          \f$ Cv = 1 / \sqrt(L) \f$ with L the number of look.
   *          \f$ Cr = \sqrt(Var(I)) / E[I] avec Var(I) = E[I^2] - E[I]^2 \f$
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData() */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

private:
  LeeImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Radius of the filter */
  SizeType m_Radius;
  /** Number of look of the filter */
  double m_NbLooks;
};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLeeImageFilter.txx"
#endif

#endif
