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
#ifndef __otbMultiToMonoChannelExtractROI_h
#define __otbMultiToMonoChannelExtractROI_h

#include "otbExtractROIBase.h"
#include "otbImage.h"
#include "otbVectorImage.h"

#include "itkMacro.h"

namespace otb
{

/** \class MultiToMonoChannelExtractROI
 * \brief Extract a mono channel part of a multi-channel image.
 *
 * This filter extracts either all channels or only those specified by the user.
 * The SetChannel method allows selecting the channels to process.
 * \note If nothing is specified, only the first channel is processed.
 * \note This class is templated over the pixel types of the input and output images.
 * The input image has to be an otb::VectorImage, whereas the output image has to be an otb::Image.
 *
 * \ingroup OTBImageBase
 */
template <class TInputPixelType, class TOutputPixelType>
class ITK_EXPORT MultiToMonoChannelExtractROI :
//    public ExtractROIBase< itk::VectorImage<TInputPixelType, 2> , itk::Image<TOutputPixelType, 2> >
  public ExtractROIBase<VectorImage<TInputPixelType, 2>, Image<TOutputPixelType, 2> >
{
public:
  /** Standard class typedefs. */
  typedef MultiToMonoChannelExtractROI Self;
//  typedef ExtractROIBase< itk::VectorImage<TInputPixelType, 2> , itk::Image<TOutputPixelType, 2> > Superclass;
  typedef ExtractROIBase<VectorImage<TInputPixelType, 2>, Image<TOutputPixelType, 2> > Superclass;
  typedef itk::SmartPointer<Self>                                                      Pointer;
  typedef itk::SmartPointer<const Self>                                                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MultiToMonoChannelExtractROI, ExtractROIBase);

  /** Image type information. */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** Pixel type information */
  typedef typename OutputImageType::ValueType OutputValueType;

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

  /** Selectionne le canal a traiter */
  itkSetMacro(Channel, unsigned int);
  itkGetConstMacro(Channel, unsigned int);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

protected:
  MultiToMonoChannelExtractROI();
  virtual ~MultiToMonoChannelExtractROI() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ExtractImageFilter can produce an image which is a different
   * resolution than its input image.  As such, ExtractImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** ExtractImageFilter can be implemented as a multithreaded filter.
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            itk::ThreadIdType threadId);

private:
  MultiToMonoChannelExtractROI(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Channel to process [1...] */
  unsigned int m_Channel;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMultiToMonoChannelExtractROI.txx"
#endif

#endif
