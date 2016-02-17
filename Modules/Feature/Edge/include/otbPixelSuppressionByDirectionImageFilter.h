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
#ifndef __otbPixelSuppressionByDirectionImageFilter_h
#define __otbPixelSuppressionByDirectionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkNumericTraits.h"

namespace otb
{

/** \class PixelSuppressionByDirectionImageFilter
 * \brief Application of a filter of suppression of isolated pixels,
 * not belonging to a line, strating from the directions of pixels.
 *
 * This class implements an image filter which detects isolated pixels
 * that have little chance of belonging to a raod and performs a pixel
 * suppression. For each pixel kept with direction \f$ \theta_{i} \f$, we look
 * for other pixels with a direction close to \f$ \theta_i \f$ in an angular
 * beam around it. If none is found, the pixel is suppressed.
 *
 * This filter is the first step to generate an image of segments
 * primitives. It starts from the two output images of the line detector
 * image filters which are the image of intensity of detection and
 * the image of direction.
 *
 *
 * \ingroup OTBEdge
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT PixelSuppressionByDirectionImageFilter :  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /**   Extract input and output image dimensions */
  itkStaticConstMacro(InputImageDimension,
                      unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension,
                      unsigned int,
                      TOutputImage::ImageDimension);

  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** typedef for the classes standards. */
  typedef PixelSuppressionByDirectionImageFilter                   Self;
  typedef itk::ImageToImageFilter<InputImageType, OutputImageType> Superclass;
  typedef itk::SmartPointer<Self>                                  Pointer;
  typedef itk::SmartPointer<const Self>                            ConstPointer;

  /** Method for management of the "object factory". */
  itkNewMacro(Self);

  /** Return the nale of the class. */
  itkTypeMacro(PixelSuppressionByDirectionImageFilter, ImageToImageFilter);

  /** Definition of the input and output images */
  typedef typename InputImageType::PixelType  InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  typedef typename itk::NumericTraits<InputPixelType>::RealType InputRealType;

  typedef typename InputImageType::RegionType  InputImageRegionType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;

  typedef typename InputImageType::SizeType SizeType;

  /** Set the radius of one zone. */
  itkSetMacro(Radius, SizeType);
  /** Get the radius of one zone. */
  itkGetConstReferenceMacro(Radius, SizeType);

  /** Set the angular beam. */
  itkSetMacro(AngularBeam, InputRealType);
  /** Get the angular beam. */
  itkGetConstReferenceMacro(AngularBeam, InputRealType);

  /** Set/Get the image input of this process object.  */
  void SetInputImage(const InputImageType *image);
  const InputImageType * GetInputImage(void);

  void SetInputImageDirection(const InputImageType *image);
  const InputImageType * GetInputImageDirection(void);

  virtual void GenerateInputRequestedRegion()
    throw(itk::InvalidRequestedRegionError);

protected:
  PixelSuppressionByDirectionImageFilter();
  virtual ~PixelSuppressionByDirectionImageFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

private:
  PixelSuppressionByDirectionImageFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  // Radius of the region
  SizeType m_Radius;
  // Angular Accuracy on the direction of the central pixel
  InputRealType m_AngularBeam;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPixelSuppressionByDirectionImageFilter.txx"
#endif

#endif
