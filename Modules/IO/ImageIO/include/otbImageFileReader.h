/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbImageFileReader_h
#define otbImageFileReader_h

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "itkImageSource.h"
#pragma GCC diagnostic pop
#else
#include "itkImageSource.h"
#endif
#include "otbImageIOBase.h"
#include "itkExceptionObject.h"
#include "itkImageRegion.h"
#include "OTBImageIOExport.h"

#include "otbDefaultConvertPixelTraits.h"
#include "otbImageKeywordlist.h"
#include "otbExtendedFilenameToReaderOptions.h"
#include "otbImageFileReaderException.h"
#include <string>

namespace otb
{

/** \class ImageFileReader
 * \brief  Reads image data.
 *
 * ImageFileReader reads its output data from a single output file.
 * It interfaces with an ImageIO class to read in the data and
 * supports streaming (partial reading) if the source dataset does so.
 *
 * ImageFileReader supports extended filenames, which allow controlling
 * how the source dataset is read. See
 * http://wiki.orfeo-toolbox.org/index.php/ExtendedFileName for more
 * information.
 *
 * \sa ExtendedFilenameToReaderOptions
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 *
 * \ingroup IOFilters
 *
 *
 * \ingroup OTBImageIO
 */
template <class TOutputImage,
          class ConvertPixelTraits=DefaultConvertPixelTraits<
                   typename TOutputImage::IOPixelType > >
class OTBImageIO_EXPORT_TEMPLATE ImageFileReader : public itk::ImageSource<TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageFileReader                    Self;
  typedef itk::ImageSource<TOutputImage>     Superclass;
  typedef itk::SmartPointer<Self>            Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileReader, ImageSource);

  /** The size of the output image. */
  typedef typename TOutputImage::SizeType  SizeType;

  /** The size of the output image. */
  typedef typename TOutputImage::IndexType  IndexType;

  /** The region of the output image. */
  typedef typename TOutputImage::RegionType  ImageRegionType;

  /** The pixel type of the output image. */
  typedef typename TOutputImage::InternalPixelType OutputImagePixelType;

  /** The Filename Helper. */
  typedef ExtendedFilenameToReaderOptions            FNameHelperType;

  /** Prepare image allocation at the first call of the pipeline processing */
  void GenerateOutputInformation(void) override;

  /** Does the real work. */
  void GenerateData() override;

  /** Give the reader a chance to indicate that it will produce more
   * output than it was requested to produce. ImageFileReader cannot
   * currently read a portion of an image (since the ImageIO objects
   * cannot read a portion of an image), so the ImageFileReader must
   * enlarge the RequestedRegion to the size of the image on disk. */
  void EnlargeOutputRequestedRegion(itk::DataObject *output) override;

  /** Set/Get the ImageIO helper class. Often this is created via the object
   * factory mechanism that determines whether a particular ImageIO can
   * read a certain file. This method provides a way to get the ImageIO
   * instance that is created. Or you can directly specify the ImageIO
   * to use to read a particular file in case the factory mechanism will
   * not work properly (e.g., unknown or unusual extension). */
  void  SetImageIO( otb::ImageIOBase * imageIO );
  itkGetObjectMacro(ImageIO,otb::ImageIOBase);

  virtual void SetFileName(const std::string& extendedFileName);

  virtual const char* GetFileName () const;

  /** Get the number of overviews available into the file specified
   * Returns: overview count, zero if none. */
  unsigned int GetOverviewsCount();


  /** Get description about overviews available into the file specified
   * Returns: overview info, empty if none.*/
  std::vector<std::string> GetOverviewsInfo();

protected:
  ImageFileReader();
  ~ImageFileReader() override;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Convert a block of pixels from one type to another. */
  void DoConvertBuffer(void* buffer, size_t numberOfPixels);

private:
  /** Test whether m_ImageIO is valid (not NULL). This is intended to be called
   * after trying to create it via an ImageIOFactory. Throws an exception with
   * an appropriate message otherwise. */
  void TestValidImageIO();

  /** Generate the filename (for GDALImageI for example). If filename is a directory, look if is a
    * CEOS product (file "DAT...") In this case, the GdalFileName contain the open image file.
    */
  bool GetGdalReadImageFileName(const std::string& filename, std::string& GdalFileName);

  // Retrieve the real source file name if derived dataset */
  std::string GetDerivedDatasetSourceFileName(const std::string& filename) const;
  
  ImageFileReader(const Self &) = delete;
  void operator =(const Self&) = delete;

  otb::ImageIOBase::Pointer m_ImageIO;
  bool                 m_UserSpecifiedImageIO; // keep track whether the
                                               // ImageIO is user specified

  std::string m_FileName; // The file to be read

  bool m_UseStreaming;

  // The region that the ImageIO class will return when we ask to
  // produce the requested region.
  itk::ImageIORegion m_ActualIORegion;

  FNameHelperType::Pointer m_FilenameHelper;

  unsigned int m_AdditionalNumber;

  bool m_KeywordListUpToDate;

  /** Mapping between origin components and output components (before any
   * conversion) */
  std::vector<unsigned int> m_BandList;

  /** Store the number of components to be exported to the output image
   *  This variable can be the number of components in m_ImageIO or the
   *  number of components in the m_BandList (if used) */
  unsigned int m_IOComponents;
};

} //namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileReader.hxx"
#endif

#include "otbImage.h"
#include "otbVectorImage.h"
#include <complex>

namespace otb {

// Prevent implicit instanciation of common types to improve build performance
// Explicit instanciations are provided in the .cxx
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<unsigned int, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<int, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<unsigned char, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<char, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<unsigned short, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<short, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<float, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<double, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<std::complex<int>, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<std::complex<short>, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<std::complex<float>, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<Image<std::complex<double>, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<unsigned int, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<int, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<unsigned char, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<char, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<unsigned short, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<short, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<float, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<double, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<std::complex<int>, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<std::complex<short>, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<std::complex<float>, 2>>;
extern template class OTBImageIO_EXPORT_TEMPLATE ImageFileReader<VectorImage<std::complex<double>, 2>>;

}

#endif // otbImageFileReader_h
