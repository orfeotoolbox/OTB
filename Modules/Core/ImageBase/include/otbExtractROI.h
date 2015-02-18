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
#ifndef __otbExtractROI_h
#define __otbExtractROI_h

#include "otbExtractROIBase.h"
#include "otbImage.h"
#include "itkMacro.h"

namespace otb
{

/** \class ExtractROI
 * \brief Extract a subset of a mono-channel image.
 *
 * Pixel can be of a simple type or an itk::RGBPixel, etc.
 * \note Parameter to this class are the input pixel type and the output
 * pixel type.
 *
 */
template <class TInputPixel, class TOutputPixel>
class ITK_EXPORT ExtractROI :
  public ExtractROIBase<Image<TInputPixel, 2>, Image<TOutputPixel, 2> >
{
public:
  /** Standard class typedefs. */
  typedef ExtractROI                                                     Self;
  typedef ExtractROIBase<Image<TInputPixel, 2>, Image<TOutputPixel, 2> > Superclass;
  typedef itk::SmartPointer<Self>                                        Pointer;
  typedef itk::SmartPointer<const Self>                                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractROI, ExtractROIBase);

  /** Image type information. */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** Typedef to describe the output and input image region types. */
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::RegionType  InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename InputImageType::PixelType  InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename InputImageType::IndexType  InputImageIndexType;
  typedef typename OutputImageType::SizeType  OutputImageSizeType;
  typedef typename InputImageType::SizeType   InputImageSizeType;

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

protected:
  ExtractROI();
  virtual ~ExtractROI() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ExtractROI
   *
   * \sa ExtractROIBase::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

private:
  ExtractROI(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractROI.txx"
#endif

#endif
