/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   18 janvier 2005
  Version   :   
  Role      :   Classe d'extraction d'une ROI d'une image mono-canal
  $Id$

=========================================================================*/
#ifndef __otbExtractROI_h
#define __otbExtractROI_h

#include "otbExtractROIBase.h"
#include "itkImage.h"
#include "itkMacro.h"

namespace otb
{

/** \class ExtractROI
 * \brief Extrait une partie d'une image (mono-canal ou multi-canal).
 * Le Pixel peut etre de type simple ou alors un itk::RGBPixel, etc.
 * Cette classe s'appuie sur la classe "otb::ExtractROIBase"
 *
 */
template <class TInputPixel, class TOutputPixel,unsigned int VImageDimension=2>
class ITK_EXPORT ExtractROI:
    public ExtractROIBase<itk::Image<TInputPixel,VImageDimension> , itk::Image<TOutputPixel,VImageDimension> >
{
public:
  /** Standard class typedefs. */
  typedef ExtractROI         Self;
  typedef ExtractROIBase<itk::Image<TInputPixel,VImageDimension> , itk::Image<TOutputPixel,VImageDimension> >  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractROI,ExtractROIBase);

  /** Image type information. */
  typedef itk::Image<TInputPixel,VImageDimension>  InputImageType;
  typedef itk::Image<TOutputPixel,VImageDimension> OutputImageType;

  /** Typedef to describe the output and input image region types. */
  typedef typename OutputImageType::RegionType OutputImageRegionType;
  typedef typename InputImageType::RegionType InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename OutputImageType::PixelType OutputImagePixelType;
  typedef typename InputImageType::PixelType InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename OutputImageType::IndexType OutputImageIndexType;
  typedef typename InputImageType::IndexType InputImageIndexType;
  typedef typename OutputImageType::SizeType OutputImageSizeType;
  typedef typename InputImageType::SizeType InputImageSizeType;

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      InputImageType::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      OutputImageType::ImageDimension);

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );


protected:
  ExtractROI();
  ~ExtractROI() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ExtractROI 
   *
   * \sa ExtractROIBase::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

private:
  ExtractROI(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractROI.txx"
#endif
  
#endif
