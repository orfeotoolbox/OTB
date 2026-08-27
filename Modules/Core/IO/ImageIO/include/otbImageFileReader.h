/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
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
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  include "itkImageSource.h"
#  pragma GCC diagnostic pop
#else
#  include "itkImageSource.h"
#endif

#include "OTBImageIOExport.h"

#include "otbDefaultConvertPixelTraits.h"
#include "otbExtendedFilenameToReaderOptions.h"
#include "otbImageIOBase.h"
#include "otbNewMacro.h"

#include "itkMacro.h"
#include "itkImageRegion.h"

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
 * https://www.orfeo-toolbox.org/CookBook/ExtendedFilenames.html for more
 * information.
 *
 * \sa ExtendedFilenameToReaderOptions
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 *
 * \ingroup IOFilters
 * \ingroup OTBImageIO
 */
template <
    class TOutputImage,
    class ConvertPixelTraits = DefaultConvertPixelTraits<typename TOutputImage::IOPixelType>
>
class OTBImageIO_EXPORT_TEMPLATE ImageFileReader
: public itk::ImageSource<TOutputImage>
{
public:
  // Standard class typedefs.
  using Self       = ImageFileReader;
  using Superclass = itk::ImageSource<TOutputImage>;
  using Pointer    = itk::SmartPointer<Self>;

  /** Method for creation through the object factory. */
  otbNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileReader, ImageSource);

  /** The size of the output image. */
  using SizeType             = typename TOutputImage::SizeType;

  /** The size of the output image. */
  using IndexType            = typename TOutputImage::IndexType;

  /** The region of the output image. */
  using ImageRegionType      = typename TOutputImage::RegionType;

  /** The pixel type of the output image. */
  using OutputImagePixelType = typename TOutputImage::InternalPixelType;

  /** The Filename Helper. */
  using FNameHelperType      = ExtendedFilenameToReaderOptions;

  /** Prepare image allocation at the first call of the pipeline processing */
  void GenerateOutputInformation(void) override;

  /** Does the real work. */
  void GenerateData() override;

  /** Give the reader a chance to indicate that it will produce more output than
   * it was requested to produce.
   * ImageFileReader cannot currently read a portion of an image (since the
   * ImageIO objects cannot read a portion of an image), so the ImageFileReader
   * must enlarge the RequestedRegion to the size of the image on disk.
   */
  void EnlargeOutputRequestedRegion(itk::DataObject* output) override;

  /** Set/Get the ImageIO helper class.
   * Often this is created via the object factory mechanism that determines
   * whether a particular ImageIO can read a certain file. This method provides
   * a way to get the ImageIO instance that is created. Or you can directly
   * specify the ImageIO to use to read a particular file in case the factory
   * mechanism will not work properly (e.g., unknown or unusual extension).
   */
  void SetImageIO(otb::ImageIOBase* imageIO);
  itkGetObjectMacro(ImageIO, otb::ImageIOBase);

  virtual void SetFileName(std::string extendedFileName);

  virtual const char* GetFileName() const;

  /** Get the number of overviews available into the file specified
   * Returns: overview count, zero if none. */
  unsigned int GetOverviewsCount();

  /** Get description about overviews available into the file specified
   * Returns: overview info, empty if none.*/
  std::vector<std::string> GetOverviewsInfo();

  // Retrieve the real source file name if derived dataset */
  static std::string GetDerivedDatasetSourceFileName(const std::string& filename);

protected:
  ImageFileReader(unsigned long streamHeight = 0);
  ~ImageFileReader() override = default;
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Convert a block of pixels from one type to another. */
  void DoConvertBuffer(void* buffer, size_t numberOfPixels, OutputImagePixelType* outputData);

private:
  /**
   * Internal: Read and convert input image into destination, using a cached
   * buffer.
   *
   * Reads requested region `ioRegion` from current GDAL image into destination
   * buffer `destBuffer`, while using `loadBuffer` as an intermediary buffer
   * to dump as a direct dump on input image.
   * 
   * \param[in]     ioRegion defines the region to read from input image
   * \param[in,out] loadBuffer cached intermediary buffer where image data is
   *                dumped before being de-interleaved.
   *                It will be resized on-the-fly.
   * \param[in]     destBuffer pointer to where the decoded images will be
   *                stored.
   * \return `destBuffer + number_of_bytes_written`
   * \pre `destBuffer != nullptr`
   */
  OutputImagePixelType* ReadInto(
      itk::ImageIORegion const& ioRegion,
      std::vector<char> & loadBuffer,
      OutputImagePixelType* destBuffer
  );

  /** Test whether m_ImageIO is valid (not NULL).
   * This is intended to be called after trying to create it via an
   * ImageIOFactory. Throws an exception with an appropriate message otherwise.
   * \throw otb::ImageFileReaderException
   */
  void TestValidImageIO();

  /** Generate the filename (for GDALImageI for example).
   * If filename is a directory, look if is a CEOS product (file "DAT...") In
   * this case, the GdalFileName contain the open image file.
   */
  bool GetGdalReadImageFileName(const std::string& filename, std::string& GdalFileName);

  ImageFileReader(const Self&) = delete;
  void operator=(const Self&) = delete;

  otb::ImageIOBase::Pointer m_ImageIO;

  /// keeps track whether the ImageIO is user specified
  bool                      m_UserSpecifiedImageIO = false;

  std::string m_FileName; // The file to be read

  /** The region that the ImageIO class will return when we ask to produce the
   * requested region.
   */
  itk::ImageIORegion m_ActualIORegion;

  FNameHelperType::Pointer m_FilenameHelper;

  unsigned int m_AdditionalNumber = 0;

  /** Store the number of components to be exported to the output image.
   * This variable can be the number of components in m_ImageIO or the number of
   * components in the m_BandList (if used)
   */
  unsigned int m_IOComponents = 0;

  /** Option to limit the number of lines loaded at a time.
   * The option is considered _set_ when it's strictly positive (> 0).
   * Sometimes loading an image region requires twice as much RAM than what the
   * buffered region would use, and we may not have that much memory. This
   * paremeter will tell to stream the loading.
   */
  unsigned long m_StreamHeight;

  /** Mapping between origin components and output components (before any
   * conversion).
   */
  std::vector<unsigned int> m_BandList;
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#  include "otbImageFileReader.hxx"
#endif

#include "otbImage.h"
#include "otbVectorImage.h"
#include <complex>

namespace otb
{

// Prevent implicit instantiation of common types to improve build performance
// Explicit instantiations are provided in the .cxx
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
