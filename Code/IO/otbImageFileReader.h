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
#include "itkExceptionObject.h"
#include "itkSize.h"
#include "itkImageRegion.h"
#include "itkDefaultConvertPixelTraits.h"

namespace otb
{

/** \class ImageFileReader
 * \brief Ressource en lecture pour lire une image depuis un fichier.
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
  typedef ImageFileReader         Self;
  typedef itk::ImageFileReader<TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileReader, itk::ImageFileReader);

  /** The pixel type of the output image. */
  typedef typename TOutputImage::InternalPixelType OutputImagePixelType;

  /** The size of the output image. */
  typedef typename TOutputImage::SizeType  SizeType;

  /** The region of the output image. */
  typedef typename TOutputImage::RegionType  ImageRegionType;

  /** The pixel type of the output image. */
  //typedef typename TOutputImage::InternalPixelType OutputImagePixelType;
  
  /** Prepare l'allocation de l'image output lors du premier appel de traitement 
   * pipeline. */
  virtual void GenerateOutputInformation(void);

  /** Does the real work. */
  virtual void GenerateData();

  /** Give the reader a chance to indicate that it will produce more
   * output than it was requested to produce. ImageFileReader cannot
   * currently read a portion of an image (since the ImageIO objects
   * cannot read a portion of an image), so the ImageFileReader must
   * enlarge the RequestedRegion to the size of the image on disk. */
  virtual void EnlargeOutputRequestedRegion(itk::DataObject *output);


protected:
  ImageFileReader();
  ~ImageFileReader();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  
private:
  ImageFileReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};


} //namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageFileReader.txx"
#endif

#endif // __otbImageFileReader_h
