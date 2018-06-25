/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbImageSeriesFileReader_h
#define otbImageSeriesFileReader_h

#include "otbImageSeriesFileReaderBase.h"

#include "otbObjectList.h"

#include "otbExtractROI.h"
#include "otbMultiChannelExtractROI.h"
#include "otbMultiToMonoChannelExtractROI.h"

namespace otb {

/**
  * \class ImageSeriesFileReader
  * \brief Reader class dedicated to image series reading
  *
  * Actually, this class is mostly dedicated to read ENVI META FILE. Specializations
  * of the template are given relatively to the king of image (or vector image) used
  * in input and/or output.
  *
  *
  * \see ImageFileReader
 *
 * \ingroup OTBImageIO
 */
template <class TImage, class TInternalImage = TImage>
class ITK_EXPORT ImageSeriesFileReader
  : public ImageSeriesFileReaderBase<TImage, TInternalImage>
{
public:
  /** Standard typedefs */
  typedef ImageSeriesFileReader                             Self;
  typedef ImageSeriesFileReaderBase<TImage, TInternalImage> Superclass;
  typedef itk::SmartPointer<Self>                           Pointer;
  typedef itk::SmartPointer<const Self>                     ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(ImageSeriesFileReader, ImageSeriesFileReaderBase);

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::ValueType              ValueType;
  typedef typename Superclass::IndexType              IndexType;
  typedef typename Superclass::SizeType               SizeType;
  typedef typename Superclass::RegionType             RegionType;

  typedef typename Superclass::OutputImageListType             OutputImageListType;
  typedef typename Superclass::OutputImageListPointerType      OutputImageListPointerType;
  typedef typename Superclass::OutputImageListConstPointerType OutputImageListConstPointerType;

  typedef typename Superclass::InternalImageType        InternalImageType;
  typedef typename Superclass::InternalImagePointerType InternalImagePointerType;
  typedef typename Superclass::InternalPixelType        InternalPixelType;
  typedef typename Superclass::InternalValueType        InternalValueType;
  typedef typename Superclass::InternalIndexType        InternalIndexType;
  typedef typename Superclass::InternalSizeType         InternalSizeType;
  typedef typename Superclass::InternalRegionType       InternalRegionType;

  typedef typename Superclass::ReaderType ReaderType;
  typedef typename Superclass::ReaderType ReaderPointerType;

  typedef typename Superclass::ReaderListType        ReaderListType;
  typedef typename Superclass::ReaderListPointerType ReaderListPointerType;

  /** This is a generic template definition of the ROI extraction procedure.
   * In fact, it will be specialised with:
   * - MultiChannelExtractROI if TImage is a VectorImage
   * - MultiToMonoChannelExtractROI if TImage is an Image and TInteranalImage is a VectorImage
   * - ExtractROI if TImage and TInternalImage are of Image type.
   */
  // typedef MultiChannelExtractROI< InternalPixelType, PixelType > ExtractSelectionType;
  // typedef MultiToMonoChannelExtractROI< InternalPixelType, PixelType > ExtractSelectionType;
  // typedef ExtractROI< InternalPixelType, PixelType > ExtractSelectionType;
  typedef itk::ImageToImageFilter<InternalImageType, OutputImageType> ExtractSelectionType;
  typedef typename ExtractSelectionType::Pointer                      ExtractSelectionPointerType;

  typedef ObjectList<ExtractSelectionType>           ExtractSelectionListType;
  typedef typename ExtractSelectionListType::Pointer ExtractSelectionListPointerType;

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

protected:
  ImageSeriesFileReader();
  virtual ~ImageSeriesFileReader () {}

  /** TestBandSelection
   * Tests the coherency of the Meta File (especifically band selection) with the image types
   */
  virtual void TestBandSelection(std::vector<unsigned int>& itkNotUsed(bands)) {}

  /** GenerateData
   * This method will be specialised if template definitions follow:
   * - TImage is a VectorImage
   * - TImage is an Image and TInteranalImage is a VectorImage
   * - TImage and TInternalImage are of Image type.
   */
  virtual void GenerateData(DataObjectPointerArraySizeType idx);
  using Superclass::GenerateData;

  /** AllocateListOfComponents
   * Once MetaFile is read, allocation of lists are performed in SetFileName.
   * This allows specific (or global) initialization in the GenerateData methods,
   * that the user may invoke through GenerateOutput() or GenerateOutput( idx ).
   */
  virtual void AllocateListOfComponents(void);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    return Superclass::PrintSelf(os, indent);
  }

  /**
   * Type of extractor to use
   */
  ExtractSelectionListPointerType m_ExtractorList;

private:
  ImageSeriesFileReader (const Self &);
  void operator =(const Self&);
}; // end of class

/**
 * \class ImageSeriesFileReader
 * \brief Specific definition for template Images
 *
 * In the case where TImage and TInternalImage stand for otb::Image, the
 * TExtractSelection must be of type ExtractROI.
 *
 * GenerateData and TestBandSelection methods are specialised.
 *
 * \see ImageSeriesFileReader
 *
 * \ingroup OTBImageIO
 */
template <class TPixel, class TInternalPixel>
class ITK_EXPORT ImageSeriesFileReader<Image<TPixel, 2>, Image<TInternalPixel, 2> >
  : public ImageSeriesFileReaderBase<Image<TPixel, 2>, Image<TInternalPixel, 2> >
{
public:
  /** Standard typedefs */
  typedef ImageSeriesFileReader                                                  Self;
  typedef ImageSeriesFileReaderBase<Image<TPixel, 2>, Image<TInternalPixel, 2> > Superclass;
  typedef itk::SmartPointer<Self>                                                Pointer;
  typedef itk::SmartPointer<const Self>                                          ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(ImageSeriesFileReader, ImageSeriesFileReaderBase);

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::ValueType              ValueType;
  typedef typename Superclass::IndexType              IndexType;
  typedef typename Superclass::SizeType               SizeType;
  typedef typename Superclass::RegionType             RegionType;

  typedef typename Superclass::OutputImageListType             OutputImageListType;
  typedef typename Superclass::OutputImageListPointerType      OutputImageListPointerType;
  typedef typename Superclass::OutputImageListConstPointerType OutputImageListConstPointerType;

  typedef typename Superclass::InternalImageType        InternalImageType;
  typedef typename Superclass::InternalImagePointerType InternalImagePointerType;
  typedef typename Superclass::InternalPixelType        InternalPixelType;
  typedef typename Superclass::InternalValueType        InternalValueType;
  typedef typename Superclass::InternalIndexType        InternalIndexType;
  typedef typename Superclass::InternalSizeType         InternalSizeType;
  typedef typename Superclass::InternalRegionType       InternalRegionType;

  typedef typename Superclass::ReaderType ReaderType;
  typedef typename Superclass::ReaderType ReaderPointerType;

  typedef typename Superclass::ReaderListType        ReaderListType;
  typedef typename Superclass::ReaderListPointerType ReaderListPointerType;

  /** This is a specialised template definition of the ROI extraction procedure.
   * Here TExtractSelection is a ExtractROI class since TImage and TInternalImage are of Image type.
   */
  typedef ExtractROI<InternalPixelType, PixelType> ExtractSelectionType;
  typedef typename ExtractSelectionType::Pointer   ExtractSelectionPointerType;

  typedef ObjectList<ExtractSelectionType>           ExtractSelectionListType;
  typedef typename ExtractSelectionListType::Pointer ExtractSelectionListPointerType;

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

protected:
  ImageSeriesFileReader();
  virtual ~ImageSeriesFileReader () {}

  /** TestBandSelection
   * Tests the coherency of the Meta File (especifically band selection) with the image types
   */
  virtual void TestBandSelection(std::vector<unsigned int>& bands);

  /** GenerateData
   * This method will be specialised if template definitions follow:
   * - TImage is a VectorImage
   * - TImage is an Image and TInteranalImage is a VectorImage
   * - TImage and TInternalImage are of Image type.
   */
  virtual void GenerateData(DataObjectPointerArraySizeType idx);
  using Superclass::GenerateData;

  /**
   * Once MetaFile is read, allocation of lists are performed in SetFileName.
   * This allows specific (or global) initialization in the GenerateData methods,
   * that the user may invoke through GenerateOutput() or GenerateOutput( idx ).
   */
  virtual void AllocateListOfComponents(void);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    return Superclass::PrintSelf(os, indent);
  }

  /**
   * Type of extractor to use
   */
  ExtractSelectionListPointerType m_ExtractorList;

private:
  ImageSeriesFileReader (const Self &);
  void operator =(const Self&);
}; // end of class specialized for image

/** \class ImageSeriesFileReader
 * \brief Specific definition for VectorImage in reading and Image type for output
 *
 * In the case where TImage stands for an otb::Image while TInternalImage stands for otb::VectorImage, the
 * TExtractSelection must be of type MultiToMonoChannelExtractROI.
 *
 * GenerateData and TestBandSelection methods are specialised.
 *
 * \see ImageSeriesFileReader
 *
 * \ingroup OTBImageIO
 */
template <class TPixel, class TInternalPixel>
class ITK_EXPORT ImageSeriesFileReader<Image<TPixel, 2>, VectorImage<TInternalPixel, 2> >
  : public ImageSeriesFileReaderBase<Image<TPixel, 2>, VectorImage<TInternalPixel, 2> >
{
public:
  /** Standard typedefs */
  typedef ImageSeriesFileReader                                                        Self;
  typedef ImageSeriesFileReaderBase<Image<TPixel, 2>, VectorImage<TInternalPixel, 2> > Superclass;
  typedef itk::SmartPointer<Self>                                                      Pointer;
  typedef itk::SmartPointer<const Self>                                                ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(ImageSeriesFileReader, ImageSeriesFileReaderBase);

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::ValueType              ValueType;
  typedef typename Superclass::IndexType              IndexType;
  typedef typename Superclass::SizeType               SizeType;
  typedef typename Superclass::RegionType             RegionType;

  typedef typename Superclass::OutputImageListType             OutputImageListType;
  typedef typename Superclass::OutputImageListPointerType      OutputImageListPointerType;
  typedef typename Superclass::OutputImageListConstPointerType OutputImageListConstPointerType;

  typedef typename Superclass::InternalImageType        InternalImageType;
  typedef typename Superclass::InternalImagePointerType InternalImagePointerType;
  typedef typename Superclass::InternalPixelType        InternalPixelType;
  typedef typename Superclass::InternalValueType        InternalValueType;
  typedef typename Superclass::InternalIndexType        InternalIndexType;
  typedef typename Superclass::InternalSizeType         InternalSizeType;
  typedef typename Superclass::InternalRegionType       InternalRegionType;

  typedef typename Superclass::ReaderType ReaderType;
  typedef typename Superclass::ReaderType ReaderPointerType;

  typedef typename Superclass::ReaderListType        ReaderListType;
  typedef typename Superclass::ReaderListPointerType ReaderListPointerType;

  /** This is a specific template definition of the ROI extraction procedure.
   * MultiToMonoChannelExtractROI since TImage is an Image and TInteranalImage is a VectorImage
   */
  typedef MultiToMonoChannelExtractROI<InternalPixelType, PixelType> ExtractSelectionType;
  typedef typename ExtractSelectionType::Pointer                     ExtractSelectionPointerType;

  typedef ObjectList<ExtractSelectionType>           ExtractSelectionListType;
  typedef typename ExtractSelectionListType::Pointer ExtractSelectionListPointerType;

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

protected:
  ImageSeriesFileReader();
  virtual ~ImageSeriesFileReader () {}

  /**
   * Tests the coherency of the Meta File (especifically band selection) with the image types
   */
  virtual void TestBandSelection(std::vector<unsigned int>& bands);

  /** GenerateData
   * This method will be specialised if template definitions follow:
   * - TImage is a VectorImage
   * - TImage is an Image and TInteranalImage is a VectorImage
   * - TImage and TInternalImage are of Image type.
   */
  virtual void GenerateData(DataObjectPointerArraySizeType idx);
  using Superclass::GenerateData;

  /**
   * Once MetaFile is read, allocation of lists are performed in SetFileName.
   * This allows specific (or global) initialization in the GenerateData methods,
   * that the user may invoke through GenerateOutput() or GenerateOutput( idx ).
   */
  virtual void AllocateListOfComponents(void);

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    return Superclass::PrintSelf(os, indent);
  }

  /**
   * Type of extractor to use
   */
  ExtractSelectionListPointerType m_ExtractorList;

private:
  ImageSeriesFileReader (const Self &);
  void operator =(const Self&);

}; // end of class specialized for Image and VectorImage

/** \class ImageSeriesFileReader
 * \brief Specific definition for VectorImage in reading and output
 *
 * In the case where TImage stands for an otb::VectorImage as well as TInternalImage, the
 * TExtractSelection must be of type MultiChannelExtractROI.
 *
 * GenerateData method is specialised.
 *
 * \see ImageSeriesFileReader
 *
 * \ingroup OTBImageIO
 */
template <class TPixel, class TInternalPixel>
class ITK_EXPORT ImageSeriesFileReader<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2> >
  : public ImageSeriesFileReaderBase<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2> >
{
public:
  /** Standard typedefs */
  typedef ImageSeriesFileReader                                                              Self;
  typedef ImageSeriesFileReaderBase<VectorImage<TPixel, 2>, VectorImage<TInternalPixel, 2> > Superclass;
  typedef itk::SmartPointer<Self>                                                            Pointer;
  typedef itk::SmartPointer<const Self>                                                      ConstPointer;

  /** Creation through object factory macro */
  itkNewMacro(Self);
  /** Runtime information macro */
  itkTypeMacro(ImageSeriesFileReader, ImageSeriesFileReaderBase);

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename Superclass::OutputImagePointerType OutputImagePointerType;
  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::ValueType              ValueType;
  typedef typename Superclass::IndexType              IndexType;
  typedef typename Superclass::SizeType               SizeType;
  typedef typename Superclass::RegionType             RegionType;

  typedef typename Superclass::OutputImageListType             OutputImageListType;
  typedef typename Superclass::OutputImageListPointerType      OutputImageListPointerType;
  typedef typename Superclass::OutputImageListConstPointerType OutputImageListConstPointerType;

  typedef typename Superclass::InternalImageType        InternalImageType;
  typedef typename Superclass::InternalImagePointerType InternalImagePointerType;
  typedef typename Superclass::InternalPixelType        InternalPixelType;
  typedef typename Superclass::InternalValueType        InternalValueType;
  typedef typename Superclass::InternalIndexType        InternalIndexType;
  typedef typename Superclass::InternalSizeType         InternalSizeType;
  typedef typename Superclass::InternalRegionType       InternalRegionType;

  typedef typename Superclass::ReaderType ReaderType;
  typedef typename Superclass::ReaderType ReaderPointerType;

  typedef typename Superclass::ReaderListType        ReaderListType;
  typedef typename Superclass::ReaderListPointerType ReaderListPointerType;

  /** This is a specific template definition of the ROI extraction procedure.
   * Here, it will be specialised with:
   * MultiChannelExtractROI if TImage is a VectorImage
   */
  typedef MultiChannelExtractROI<InternalPixelType, PixelType> ExtractSelectionType;
  typedef typename ExtractSelectionType::Pointer               ExtractSelectionPointerType;

  typedef ObjectList<ExtractSelectionType>           ExtractSelectionListType;
  typedef typename ExtractSelectionListType::Pointer ExtractSelectionListPointerType;

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

protected:
  ImageSeriesFileReader();
  ~ImageSeriesFileReader () override {}

  /**
   * Tests the coherency of the Meta File (especifically band selection) with the image types
   */
  void TestBandSelection(std::vector<unsigned int>& itkNotUsed(bands)) override{}

  /** GenerateData
   * This method will be specialised if template definitions follow:
   * - TImage is a VectorImage
   * - TImage is an Image and TInteranalImage is a VectorImage
   * - TImage and TInternalImage are of Image type.
   */
  void GenerateData(DataObjectPointerArraySizeType idx) override;
  using Superclass::GenerateData;

  /**
   * Once MetaFile is read, allocation of lists are performed in SetFileName.
   * This allows specific (or global) initialization in the GenerateData methods,
   * that the user may invoke through GenerateOutput() or GenerateOutput( idx ).
   */
  void AllocateListOfComponents(void) override;

  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override
  {
    return Superclass::PrintSelf(os, indent);
  }

  /**
   * Type of extractor to use
   */
  ExtractSelectionListPointerType m_ExtractorList;

private:
  ImageSeriesFileReader (const Self &);
  void operator =(const Self&);
}; // end of class specialized for VectorImage

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageSeriesFileReader.hxx"
#endif

#endif
