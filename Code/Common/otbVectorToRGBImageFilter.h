
#ifndef __otbVectorToRGBImageFilter_h
#define __otbVectorToRGBImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkVectorImage.h"
#include "itkRGBPixel.h"

namespace otb
{
  
/** \class VectorToRGBImageFilter
 *
 * \brief Creer une image RGB a partir d'une image multi-canal de type VectorImage
 * Il est possible de sélectionner les trois canaux RGB de l'image source que l'on souhaite insérer dans l'image destination.
 *
 * \ingroup Multithreaded
 */


template <class TInputPixel, class TOutputPixel, unsigned int VImageDimension=2>
class ITK_EXPORT VectorToRGBImageFilter :
    public itk::ImageToImageFilter< itk::VectorImage < TInputPixel, VImageDimension> , itk::Image<itk::RGBPixel<TOutputPixel> , VImageDimension> >
{
public:
  /** Standard class typedefs. */
  typedef VectorToRGBImageFilter  Self;
  typedef itk::ImageToImageFilter< itk::VectorImage < TInputPixel, VImageDimension> , itk::Image<itk::RGBPixel<TOutputPixel> , VImageDimension> >  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Standard class macros */
  itkNewMacro(Self);
  itkTypeMacro(VectorToRGBImageFilter, itk::ImageToImageFilter) ;

  typedef typename Superclass::OutputImageRegionType OutputImageRegionType ;

  typedef itk::VectorImage<TInputPixel,VImageDimension>                 InputImageType;
  typedef itk::Image<itk::RGBPixel<TOutputPixel>,VImageDimension>       OutputImageType;

  typedef typename OutputImageType::PixelType OutputImagePixelType ;

  /** Set/Get le numero de canal à traiter pour le canal rouge */
  itkSetMacro(RedChannel,unsigned int);
  itkGetConstMacro(RedChannel,unsigned int);
  /** Set/Get le numero de canal à traiter pour le canal vert */
  itkSetMacro(GreenChannel,unsigned int);
  itkGetConstMacro(GreenChannel,unsigned int);
  /** Set/Get le numero de canal à traiter pour le canal bleu */
  itkSetMacro(BlueChannel,unsigned int);
  itkGetConstMacro(BlueChannel,unsigned int);


protected:
  VectorToRGBImageFilter() ;
  virtual ~VectorToRGBImageFilter() {}
  
  void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread,
                            int threadId) ;
 
private:
  VectorToRGBImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  
  
    /** Contient le numero de canal rouge a extraire */
    unsigned int  m_RedChannel;
    /** Contient le numero de canal vert a extraire */
    unsigned int  m_GreenChannel;
    /** Contient le numero de canal bleu a extraire */
    unsigned int  m_BlueChannel;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorToRGBImageFilter.txx"
#endif

#endif
