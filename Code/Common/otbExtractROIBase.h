/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   18 janvier 2005
  Version   :   
  Role      :   Classe d'extraction d'une ROI d'une image 
  $Id$

=========================================================================*/
#ifndef __otbExtractROIBase_h
#define __otbExtractROIBase_h

#include "itkExtractImageFilter.h"
#include "itkMacro.h"

namespace otb
{

/** \class ExtractROIBase
 * \brief Extrait une partie d'une image. Il est possible d'extraire tous les canaux de l'image ou 
 * seulement ceux précisés par l'utilisateur.
 * Cette classe s'appuie sur la classe d'ITK "ExtractImageFilter"
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ExtractROIBase:
    public itk::ExtractImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ExtractROIBase         Self;
  typedef itk::ExtractImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractROIBase, itk::ExtractImageFilter);

  /** Image type information. */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Typedef to describe the output and input image region types. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;
  typedef typename TInputImage::RegionType InputImageRegionType;

  /** Typedef to describe the type of pixel. */
  typedef typename TOutputImage::PixelType OutputImagePixelType;
  typedef typename TInputImage::PixelType InputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename TOutputImage::IndexType OutputImageIndexType;
  typedef typename TInputImage::IndexType InputImageIndexType;
  typedef typename TOutputImage::SizeType OutputImageSizeType;
  typedef typename TInputImage::SizeType InputImageSizeType;


  
  /** Set/Get Start methode */
  itkSetMacro(StartX,unsigned long);
  itkGetConstMacro(StartX,unsigned long);
  itkSetMacro(StartY,unsigned long);
  itkGetConstMacro(StartY,unsigned long);
  itkSetMacro(SizeX,unsigned long);
  itkGetConstMacro(SizeX,unsigned long);
  itkSetMacro(SizeY,unsigned long);
  itkGetConstMacro(SizeY,unsigned long);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

protected:
  ExtractROIBase();
  ~ExtractROIBase() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ExtractROIBase 
   *
   * \sa itk::ExtractImageFilter::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

private:
  ExtractROIBase(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  
  /** Coordonnees X/Y du premier point de la région à extraire  */
  unsigned long m_StartX;
  unsigned long m_StartY;
  /** Nombre de pixels en X/Y de la région à extraire  */
  unsigned long m_SizeX;
  unsigned long m_SizeY;
  
};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractROIBase.txx"
#endif
  
#endif
