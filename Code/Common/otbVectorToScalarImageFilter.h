
#ifndef __otbVectorToScalarImageFilter_h
#define __otbVectorToScalarImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkVectorImage.h"

namespace otb
{
  
/** \class VectorToScalarImageFilter
 *
 * \brief Creer une image mono-canal a partir d'une image multi-canal (type VectorImage)
 * Il est possible de sélectionner le canal de l'image source que l'on souhaite extraire dans l'image destination.
 *
 * \ingroup Multithreaded
 */


template <class TInputPixel, class TOutputPixel, unsigned int VImageDimension=2>
class ITK_EXPORT VectorToScalarImageFilter :
    public itk::ImageToImageFilter< itk::VectorImage < TInputPixel, VImageDimension> , itk::Image<TOutputPixel , VImageDimension> >
{
public:
  /** Standard class typedefs. */
  typedef VectorToScalarImageFilter  Self;
  typedef itk::ImageToImageFilter< itk::VectorImage < TInputPixel, VImageDimension> , itk::Image<TOutputPixel , VImageDimension> >  Superclass;
  typedef itk::SmartPointer<Self>   Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** Standard class macros */
  itkNewMacro(Self);
  itkTypeMacro(VectorToScalarImageFilter, itk::ImageToImageFilter) ;

  typedef typename Superclass::OutputImageRegionType OutputImageRegionType ;

  typedef itk::VectorImage<TInputPixel,VImageDimension>                 InputImageType;
  typedef itk::Image<TOutputPixel,VImageDimension>                      OutputImageType;

  typedef typename OutputImageType::PixelType OutputImagePixelType ;

  /** Set/Get le numero de canal à traiter */
  itkSetMacro(Channel,unsigned int);
  itkGetConstMacro(Channel,unsigned int);

protected:
  VectorToScalarImageFilter() ;
  virtual ~VectorToScalarImageFilter() {}
  
  void ThreadedGenerateData(const OutputImageRegionType &outputRegionForThread,
                            int threadId) ;
 
private:
  VectorToScalarImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
    /** Contient le numero de canal a extraire */
    unsigned int  m_Channel; // [1...]

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorToScalarImageFilter.txx"
#endif

#endif
