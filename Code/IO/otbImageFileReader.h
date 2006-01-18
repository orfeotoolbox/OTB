/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   16 janvier 2005
  Version   :   
  Role      :   Ressource de lecture d'une image dans OTB
  $Id$

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
template <class TOutputImage,
          class ConvertPixelTraits = 
          itk::DefaultConvertPixelTraits< ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT ImageFileReader : public itk::ImageFileReader<TOutputImage, ConvertPixelTraits >
{
public:
  /** Standard class typedefs. */
  typedef ImageFileReader         Self;
  typedef itk::ImageFileReader<TOutputImage,ConvertPixelTraits>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageFileReader, itk::ImageFileReader);

  /** The size of the output image. */
  typedef typename TOutputImage::SizeType  SizeType;

  /** The region of the output image. */
  typedef typename TOutputImage::RegionType  ImageRegionType;

  /** The pixel type of the output image. */
  typedef typename TOutputImage::InternalPixelType OutputImagePixelType;
  
  /** Prepare l'allocation de l'image output lors du premier appel de traitement 
   * pipeline. */
  virtual void GenerateOutputInformation(void);


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
