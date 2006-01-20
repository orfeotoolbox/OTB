/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - T.Feuvrier
  Language  :   C++
  Date      :   19 janvier 2005
  Version   :   
  Role      :   Classe image mono-canal de l'OTB 
  $Id$

=========================================================================*/
#ifndef __otbImage_h
#define __otbImage_h

#include "itkImage.h"

namespace otb
{

/** \class Image
 *  \brief Classe pour une image mono-canal de l'OTB.
 *
 */
template <class TPixel, unsigned int VImageDimension=2>
class ITK_EXPORT Image : public itk::Image<TPixel,VImageDimension>
{
public:
  /** Standard class typedefs */
  typedef Image                                 Self;
  typedef itk::Image<TPixel,VImageDimension>    Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;
  typedef itk::WeakPointer<const Self>          ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Image, itk::Image);

  /** Pixel typedef support. Used to declare pixel type in filters
   * or other operations. */
  typedef TPixel PixelType;

  /** Typedef alias for PixelType */
  typedef TPixel ValueType ;

  /** Internal Pixel representation. Used to maintain a uniform API
   * with Image Adaptors and allow to keep a particular internal
   * representation of data while showing a different external
   * representation. */
  typedef TPixel InternalPixelType;

  /** Accessor type that convert data between internal and external
   *  representations.  */
  typedef itk::DefaultPixelAccessor< PixelType > AccessorType;
  typedef itk::DefaultPixelAccessorFunctor< Self > AccessorFunctorType;

  /** Tyepdef for the functor used to access a neighborhood of pixel pointers.*/
  typedef itk::NeighborhoodAccessorFunctor< Self > 
                                            NeighborhoodAccessorFunctorType;

  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel type
   * and dimension) when they need compile time access to the dimension of
   * the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  /** Container used to store pixels in the image. */
  typedef itk::ImportImageContainer<unsigned long, PixelType> PixelContainer;

  /** Index typedef support. An index is used to access pixel values. */
  typedef typename Superclass::IndexType  IndexType;

  /** Offset typedef support. An offset is used to access pixel values. */
  typedef typename Superclass::OffsetType OffsetType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef typename Superclass::SizeType  SizeType;

  /** Direction typedef support. A matrix of direction cosines. */
  typedef typename Superclass::DirectionType  DirectionType;

  /** Region typedef support. A region is used to specify a subset of an image. */
  typedef typename Superclass::RegionType  RegionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. */
  typedef typename Superclass::SpacingType SpacingType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0,0). */
  typedef typename Superclass::PointType PointType;

  /** A pointer to the pixel container. */
  typedef typename PixelContainer::Pointer PixelContainerPointer;
  typedef typename PixelContainer::ConstPointer PixelContainerConstPointer;

  /** Offset typedef (relative position between indices) */
  typedef typename Superclass::OffsetValueType OffsetValueType;


protected:
  Image();
  void PrintSelf(std::ostream& os, itk::Indent indent) const;
  virtual ~Image() {};
private:
  Image(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};
#ifdef OTB_EXPLICIT_INSTANTIATION
   extern template class Image<float         ,2>;
   extern template class Image<double        ,2>;
   extern template class Image<unsigned char ,2>;
   extern template class Image<unsigned short,2>;
   extern template class Image<unsigned int  ,2>;
   extern template class Image<signed char   ,2>;
   extern template class Image<signed short  ,2>;
   extern template class Image<signed int    ,2>;
#endif
} // end namespace otb
#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImage.txx"
#endif

#endif

