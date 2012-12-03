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

#include "itkImageFileReader.h"
#include "otbCurlHelperInterface.h"
#include "otbImageKeywordlist.h"

namespace otb
{

/** \class ImageFileReader
 * \brief Resource to read an image from a file.
 *
 * Available options for extended file name are:
 * - &geom : to specify an external geom file
 * - &sdataidx : sub-dataset index for composite files
 * - &resol : resolution factor for jpeg200 files
 * - &skippr : switch to skip the projection referentiel in ortho-ready files
 *
 * \sa ImageSeriesReader
 * \sa ImageIOBase
 *
 * \ingroup IOFilters
 *
 */
template <class TOutputImage>
class ITK_EXPORT ImageFileReader : public itk::ImageFileReader<TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ImageFileReader                    Self;
  typedef itk::ImageFileReader<TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>            Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileReader, itk::ImageFileReader);

  /** The pixel type of the output image. */
  typedef typename TOutputImage::InternalPixelType OutputImagePixelType;

  /** The size of the output image. */
  typedef typename TOutputImage::SizeType SizeType;

  /** The region of the output image. */
  typedef typename TOutputImage::RegionType ImageRegionType;

  /** The pixel type of the output image. */
  //typedef typename TOutputImage::InternalPixelType OutputImagePixelType;

  /** The reading option structure. */
  struct readerOptions
  {
      std::string  fileName;
      std::string  extGEOMFileName;
      unsigned int subDatasetIndex;
      unsigned int resolutionFactor;
      bool         skipProjRef;
  };

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

  itkSetObjectMacro(Curl, CurlHelperInterface);

  virtual void SetFileName(const char* extendedFileName);
  virtual void SetFileName(std::string extendedFileName);
  virtual const char* GetFileName () const;

protected:
  ImageFileReader();
  virtual ~ImageFileReader();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  /** Test whether the given filename exist and it is readable,
      this is intended to be called before attempting to use
      ImageIO classes for actually reading the file. If the file
      doesn't exist or it is not readable, and exception with an
      approriate message will be thrown. */
  void TestFileExistanceAndReadability();

  /** Generate the filename (for GDALImageI for example). If filename is a directory, look if is a
    * CEOS product (file "DAT...") In this case, the GdalFileName contain the open image file.
    */
  bool GetGdalReadImageFileName(const std::string& filename, std::string& GdalFileName);

  ImageFileReader(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string   m_ExceptionMessage;

  std::string   m_ExtendedFilename;
  readerOptions m_Options;

  CurlHelperInterface::Pointer m_Curl;
};

} //namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileReader.txx"
#endif

#endif // __otbImageFileReader_h
