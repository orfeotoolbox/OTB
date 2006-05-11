/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   17 janvier 2005
  Version   :   
  Role      :   Ressource d'ecriture d'une image dans OTB
  $Id$

=========================================================================*/
#ifndef __otbStreamingImageFileWriter_h
#define __otbStreamingImageFileWriter_h

#include "itkImageFileWriter.h"
#include "itkImageIOBase.h"
#include "itkExceptionObject.h"
#include "itkSize.h"
#include "itkImageIORegion.h"

namespace otb
{

/** \class StreamingImageFileWriter
 * \brief Ressource en ecriture pour ecrire une image depuis un fichier.
 *
 * \sa ImageSeriesWriter
 * \sa ImageIOBase
 * 
 * \ingroup IOFilters
 *
 */
template <class TInputImage>
class ITK_EXPORT StreamingImageFileWriter : public itk::ImageFileWriter<TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StreamingImageFileWriter                      Self;
  typedef itk::ImageFileWriter<TInputImage>             Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StreamingImageFileWriter,itk::ImageFileWriter);

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer InputImagePointer;
  typedef typename InputImageType::RegionType InputImageRegionType; 
  typedef typename InputImageType::PixelType InputImagePixelType; 
  
  /** A special version of the Update() method for writers.  It
   * invokes start and end events and handles releasing data. It
   * eventually calls GenerateData() which does the actual writing.
   * Note: the write method will write data specified by the
   * IORegion. If not set, then then the whole image is written.  Note
   * that the region will be cropped to fit the input image's
   * LargestPossibleRegion. */
  virtual void Write(void);
  itkSetMacro(UseStreaming,bool);
  itkGetMacro(UseStreaming,bool);
  
  
protected:
  StreamingImageFileWriter();
  ~StreamingImageFileWriter();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  StreamingImageFileWriter(const Self&); //purposely not implemented
  void operator=(const Self&);  //purposely not implemented

  bool m_FactorySpecifiedImageIO; //track whether the factory mechanism set the ImageIO
  bool m_UserSpecifiedIORegion;   //track whether the region is user specified

  bool m_UseStreaming;

};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingImageFileWriter.txx"
#endif

#endif // __otbStreamingImageFileWriter_h
  
