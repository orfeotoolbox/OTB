/*
 * otbMosaicFromDirectoryHandler.h
 *
 *  Created on: 24 mars 2016
 *      Author: cresson
 */

#ifndef MODULES_REMOTE_OTB_MosaicFromDirectoryHandler_INCLUDE_OTBMosaicFromDirectoryHandler_H_
#define MODULES_REMOTE_OTB_MosaicFromDirectoryHandler_INCLUDE_OTBMosaicFromDirectoryHandler_H_

#include "itkImageSource.h"
#include "itkExceptionObject.h"
#include "itkImageRegion.h"

#include "otbStreamingSimpleMosaicFilter.h"

#include "otbImageFileReader.h"
#include "itkDirectory.h"
#include "otbImageIOBase.h"
#include "otbImageIOFactory.h"

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbGenericRSResampleImageFilter.h"
#include "itkNearestNeighborInterpolateImageFunction.h"

namespace otb
{
/** \class MosaicFromDirectoryHandler
 * \brief  This ImageSource produces an otb::image from multiple rasters
 * stored in the m_Directory.
 * TODO: Currently only .tif extension is supported. Might be nice to change it.
 *
 *
 * \ingroup OTBMosaic
 *
 */
template <class TOutputImage, class TReferenceImage>
class ITK_EXPORT MosaicFromDirectoryHandler : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MosaicFromDirectoryHandler                    Self;
  typedef itk::ImageSource<TOutputImage>                Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MosaicFromDirectoryHandler, ImageSource);

  /** Typedefs for output image */
  typedef typename TOutputImage::SizeType               SizeType;
  typedef typename TOutputImage::IndexType              IndexType;
  typedef typename TOutputImage::SpacingType            SpacingType;
  typedef typename TOutputImage::PointType              PointType;
  typedef typename TOutputImage::RegionType             ImageRegionType;
  typedef typename TOutputImage::InternalPixelType      OutputImagePixelType;

  /** Typedefs for mosaic filter */
  typedef otb::VectorImage<OutputImagePixelType>        InternalMaskImageType;
  typedef otb::StreamingSimpleMosaicFilter<
      InternalMaskImageType>                            MosaicFilterType;
  typedef typename MosaicFilterType::Pointer            MosaicFilterPointerType;

  /** Typedefs for image reader */
  typedef otb::ImageFileReader<InternalMaskImageType>   ReaderType;
  typedef typename ReaderType::Pointer                  ReaderPointerType;

  /** Typedefs for casting the image */
  typedef otb::MultiToMonoChannelExtractROI<
      OutputImagePixelType, OutputImagePixelType>       CastFilterType;
  typedef typename CastFilterType::Pointer              CastFilterPointerType;

  /** Typedefs for image reprojection */
  typedef otb::GenericRSResampleImageFilter<
      InternalMaskImageType, InternalMaskImageType>     ResamplerType;
  typedef typename ResamplerType::Pointer               ResamplerPointerType;
  typedef itk::NearestNeighborInterpolateImageFunction<
      InternalMaskImageType, double>                    NNInterpolatorType;

  /** Input directory accessors */
  itkGetMacro(Directory, std::string);
  itkSetMacro(Directory, std::string);

  /** Output parameters setters */
  itkSetMacro(OutputSpacing, SpacingType);
  itkSetMacro(OutputSize, SizeType);
  itkSetMacro(OutputOrigin, PointType);
  void SetReferenceImage(TReferenceImage * ptr){m_RefImagePtr = ptr;}
  itkSetMacro(UseReferenceImage, bool);

  /** Prepare image allocation at the first call of the pipeline processing */
  virtual void GenerateOutputInformation(void);

  /** Does the real work. */
   virtual void GenerateData();

protected:
  MosaicFromDirectoryHandler();
  virtual ~MosaicFromDirectoryHandler();

  // Masks directory
  std::string                       m_Directory;

  // Output parameters
  SpacingType                       m_OutputSpacing;
  SizeType                          m_OutputSize;
  PointType                         m_OutputOrigin;

  // Internal filters
  MosaicFilterPointerType           mosaicFilter;
  CastFilterPointerType             castFilter;
  std::vector<ReaderPointerType>    readers;
  std::vector<ResamplerPointerType> resamplers;

  // Reference image pointer
  bool                              m_UseReferenceImage;
  TReferenceImage *                 m_RefImagePtr;

private:

  MosaicFromDirectoryHandler(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} //namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include <otbMosaicFromDirectoryHandler.txx>
#endif

#endif /* MODULES_REMOTE_OTB_MosaicFromDirectoryHandler_INCLUDE_OTBMosaicFromDirectoryHandler_H_ */
