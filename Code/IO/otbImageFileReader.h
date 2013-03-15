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
#ifndef __otbImageFileReader_h
#define __otbImageFileReader_h

#include "itkImageSource.h"
#include "itkImageIOBase.h"
#include "itkExceptionObject.h"
#include "itkSize.h"
#include "itkImageRegion.h"

#include "otbDefaultConvertPixelTraits.h"
#include "otbCurlHelperInterface.h"
#include "otbImageKeywordlist.h"
#include "otbExtendedFilenameToReaderOptions.h"

namespace otb
{

/** \class ImageFileReaderException
 *
 * \brief Base exception class for IO conflicts.
 */
class ImageFileReaderException : public itk::ExceptionObject
{
public:
  /** Run-time information. */
  itkTypeMacro( ImageFileReaderException, ExceptionObject );

  /** Constructor. */
  ImageFileReaderException(const char *file, unsigned int line,
                           const char* message = "Error in IO",
                           const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc)
  {
  }

  /** Constructor. */
  ImageFileReaderException(const std::string &file, unsigned int line,
                           const char* message = "Error in IO",
                           const char* loc = "Unknown") :
    ExceptionObject(file, line, message, loc)
  {
  }
};

/** \class ImageFileReader
 * \brief  Reads image data.
 *
 * ImageFileReader reads its output data from a single output file.
 * It interfaces with an ImageIO class to read in the data and
 * supports streaming (partial reading) if the source dataset does so.
 *
 * ImageFileReader supports extended filenames, which allow to control
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
 */
template <class TOutputImage,
          class ConvertPixelTraits=DefaultConvertPixelTraits<
                   ITK_TYPENAME TOutputImage::IOPixelType > >
class ITK_EXPORT ImageFileReader : public itk::ImageSource<TOutputImage>
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
  virtual void GenerateOutputInformation(void);

  /** Does the real work. */
  virtual void GenerateData();

  /** Give the reader a chance to indicate that it will produce more
   * output than it was requested to produce. ImageFileReader cannot
   * currently read a portion of an image (since the ImageIO objects
   * cannot read a portion of an image), so the ImageFileReader must
   * enlarge the RequestedRegion to the size of the image on disk. */
  virtual void EnlargeOutputRequestedRegion(itk::DataObject *output);

  /** Set/Get the ImageIO helper class. Often this is created via the object
   * factory mechanism that determines whether a particular ImageIO can
   * read a certain file. This method provides a way to get the ImageIO
   * instance that is created. Or you can directly specify the ImageIO
   * to use to read a particular file in case the factory mechanism will
   * not work properly (e.g., unknown or unusual extension). */
  void  SetImageIO( itk::ImageIOBase * imageIO );
  itkGetObjectMacro(ImageIO,itk::ImageIOBase);

  itkSetObjectMacro(Curl, CurlHelperInterface);

  virtual void SetFileName(const char* extendedFileName);
  virtual void SetFileName(std::string extendedFileName);
  virtual const char* GetFileName () const;

  itkLegacyMacro( itkSetMacro(AdditionalNumber, unsigned int) );
  itkLegacyMacro( itkGetMacro(AdditionalNumber, unsigned int) );

  /** Get the resolution information from the file */
  bool GetResolutionsInfo( std::vector<unsigned int>& res,
                          std::vector<std::string>& desc);

  /** Get the list of available resolutions from the file */
  std::vector<unsigned int> GetAvailableResolutions();

  /** Get the number of available overviews into the file */
  unsigned int GetNbOfAvailableOverviews();

  /** Indicate if the ImageIO support overviews*/
  bool HasOverviewsSupport();

protected:
  ImageFileReader();
  virtual ~ImageFileReader();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Convert a block of pixels from one type to another. */
  void DoConvertBuffer(void* buffer, size_t numberOfPixels);

private:
  /** Test whether the given filename exist and it is readable,
      this is intended to be called before attempting to use
      ImageIO classes for actually reading the file. If the file
      doesn't exist or it is not readable, and exception with an
      appropriate message will be thrown. */
  void TestFileExistanceAndReadability();

  /** Generate the filename (for GDALImageI for example). If filename is a directory, look if is a
    * CEOS product (file "DAT...") In this case, the GdalFileName contain the open image file.
    */
  bool GetGdalReadImageFileName(const std::string& filename, std::string& GdalFileName);

  ImageFileReader(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  itk::ImageIOBase::Pointer m_ImageIO;
  bool                 m_UserSpecifiedImageIO; // keep track whether the
                                               // ImageIO is user specified

  std::string m_FileName; // The file to be read

  bool m_UseStreaming;

  std::string   m_ExceptionMessage;

  // The region that the ImageIO class will return when we ask to
  // produce the requested region.
  itk::ImageIORegion m_ActualIORegion;

  FNameHelperType::Pointer m_FilenameHelper;

  CurlHelperInterface::Pointer m_Curl;

  unsigned int m_AdditionalNumber;
};

} //namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileReader.txx"
#endif

#endif // __otbImageFileReader_h
