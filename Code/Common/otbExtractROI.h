/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   18 janvier 2005
  Version   :   
  Role      :   Classe d'extraction d'une ROI d'une image en précisant le numéro de canal
  $Id$

=========================================================================*/
#ifndef __otbExtractImageFilter_h
#define __otbExtractImageFilter_h

#include "itkExtractImageFilter.h"
#include "itkMacro.h"
#include <vector>

namespace otb
{

/** \class ExtractImageFilter
 * \brief Extrait une partie d'une image. Il est possible d'extraire tous les canaux de l'image ou 
 * seulement ceux précisés par l'utilisateur.
 * Cette classe s'appuie sur la classe d'ITK "ExtractImageFilter"
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT ExtractImageFilter:
    public itk::ExtractImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ExtractImageFilter         Self;
  typedef itk::ExtractImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(ExtractImageFilter, itk::ExtractImageFilter);

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

  itkSetMacro(FirstChannel,unsigned int);
  itkGetConstMacro(FirstChannel,unsigned int);
  itkSetMacro(LastChannel,unsigned int);
  itkGetConstMacro(LastChannel,unsigned int);
  
  /** Typedef Liste des canaux */
  typedef std::vector<unsigned int> ChannelsType;
  /** Selectionne un canal a traiter */
  void SetChannel(unsigned int channel);
  /** Annule la selection des canaux */
  void ClearChannels(void);

  /** ImageDimension enumeration */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

protected:
  ExtractImageFilter();
  ~ExtractImageFilter() {};
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** ExtractImageFilter can produce an image which is a different
   * resolution than its input image.  As such, ExtractImageFilter
   * needs to provide an implementation for
   * GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below.
   *
   * \sa ProcessObject::GenerateOutputInformaton()  */
  virtual void GenerateOutputInformation();

  /** ExtractImageFilter peut etre implementee comme un filtre multithreaded.
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()  */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );

private:
  ExtractImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  
  /** Coordonnees X/Y du premier point de la région à extraire  */
  unsigned long m_StartX;
  unsigned long m_StartY;
  /** Nombre de pixels en X/Y de la région à extraire  */
  unsigned long m_SizeX;
  unsigned long m_SizeY;
  
  /** Premier/Dernier canal à traiter [1...] */
  unsigned int  m_FirstChannel;
  unsigned int  m_LastChannel;
  /** Liste des canaux à traiter  [1...] */
  ChannelsType  m_Channels;
  /** Liste des canaux qui seront réellement traités [1...] */
  ChannelsType  m_ChannelsWorks;
  /** */
  bool m_ChannelsWorksBool;
};

  
} // end namespace otb
  
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbExtractImageFilter.txx"
#endif
  
#endif
