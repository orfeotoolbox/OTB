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
#ifndef __otbExtractROIBase_h
#define __otbExtractROIBase_h

#include "itkImageToImageFilter.h"
#include "itkSmartPointer.h"
#include "itkExtractImageFilterRegionCopier.h"

namespace otb
{

/** \class ExtractROIBase
 * \brief Base class to extract area of images.
 *
 * Extracted region can be defined by the methods SetStartX/Y (region origin) and
 * SetSizeX/Y (region size). If the starting point is not defined, the extracted
 * region start from the upper left corner of the input image. If the size is not
 * defined, the extracted region extend to the lower right corner of the input
 * image. If no parameter is defined all image is extracted.
 *
 * Alternatively, a region can be specified using the SetROI() method.
 *
 * \ingroup Common
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ExtractROIBase :
  public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ExtractROIBase                                     Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractROIBase, itk::ImageToImageFilter);

  /** Image type information. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Typedef to describe the output and input image region types. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;
  typedef typename TInputImage::RegionType  InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename TOutputImage::PixelType OutputImagePixelType;
  typedef typename TInputImage::PixelType  InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename TOutputImage::IndexType OutputImageIndexType;
  typedef typename TInputImage::IndexType  InputImageIndexType;
  typedef typename TOutputImage::SizeType  OutputImageSizeType;
  typedef typename TInputImage::SizeType   InputImageSizeType;

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  typedef
  itk::ImageToImageFilterDetail::ExtractImageFilterRegionCopier<itkGetStaticConstMacro(InputImageDimension),
      itkGetStaticConstMacro(OutputImageDimension)>
  ExtractROIBaseRegionCopierType;

  itkGetMacro(ExtractionRegion, InputImageRegionType);

  /** Give the region to extract, same effect as given m_StartX/Y and m_SizeX/Y*/
  void SetExtractionRegion(InputImageRegionType roi);

  /** Set/Get Start methods */
  itkSetMacro(StartX, unsigned long);
  itkGetConstMacro(StartX, unsigned long);
  itkSetMacro(StartY, unsigned long);
  itkGetConstMacro(StartY, unsigned long);
  /** Set/Get Size methods */
  itkSetMacro(SizeX, unsigned long);
  itkGetConstMacro(SizeX, unsigned long);
  itkSetMacro(SizeY, unsigned long);
  itkGetConstMacro(SizeY, unsigned long);

protected:
  ExtractROIBase();
  virtual ~ExtractROIBase() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  virtual void GenerateInputRequestedRegion();

  /** ExtractROIBase can produce an image which is a different
   * resolution than its input image.  As such, ExtractROIBase
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** This function calls the actual region copier to do the mapping from
   * output image space to input image space.  It uses a
   * Function object used for dispatching to various routines to
   * copy an output region (start index and size) to an input region.
   * For most filters, this is a trivial copy because most filters
   * require the input dimension to match the output dimension.
   * However, some filters like itk::ExtractROIBase can
   * support output images of a lower dimension that the input.
   *
   * \sa ImageToImageFilter::CallCopyRegion() */
  virtual void CallCopyOutputRegionToInputRegion(InputImageRegionType& destRegion,
                                                 const OutputImageRegionType& srcRegion);

  /** ExtractROIBase can be implemented as a multithreaded filter.
   * Therefore, this implementation provides a ThreadedGenerateData()
   * routine which is called for each processing thread. The output
   * image data is allocated automatically by the superclass prior to
   * calling ThreadedGenerateData().  ThreadedGenerateData can only
   * write to the portion of the output image specified by the
   * parameter "outputRegionForThread"
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */

//  ATTENTION bizarre

  void ThreadedGenerateData(const OutputImageRegionType& /*outputRegionForThread*/,
                            itk::ThreadIdType /*threadId*/)

  {

  }

  /** Set/Get the output image region.
   *  If any of the ExtractionRegion.Size = 0 for any particular dimension dim,
   *  we have to collapse dimension dim.  This means the output image will have
   *  'c' dimensions less than the input image, where c = # of
   *  ExtractionRegion.Size = 0. */
  void SetInternalExtractionRegion(InputImageRegionType extractRegion);

  InputImageRegionType  m_ExtractionRegion;
  OutputImageRegionType m_OutputImageRegion;

private:
  ExtractROIBase(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** X/Y coordinates of the first point of the region to extract. */
  unsigned long m_StartX;
  unsigned long m_StartY;
  /** Number of X/Y pixels of the region to extract. */
  unsigned long m_SizeX;
  unsigned long m_SizeY;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractROIBase.txx"
#endif

#endif
