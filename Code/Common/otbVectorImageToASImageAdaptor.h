/*=========================================================================

  Program:   Orfeo Tool Box
  Module:    $RCSfile: otbVectorImageToASImageAdaptor.h,v $
  Language:  C++
  Date:      $Date: 2006/16/05 17:55:25 $
  Version:   $Revision: 1.0 $

  Copyright (c) R. Garrigues

=========================================================================*/

#ifndef __otbVectorImageToASImageAdaptor_h
#define __otbVectorImageToASImageAdaptor_h

#include "itkImageAdaptor.h"
#include "otbVectorImageToASPixelAccessor.h"

namespace otb
{
 

/** \class VectorImageToASImageAdaptor
 * \brief Presents an image of pixel type Vector as being and image of
 * FixelArray<unsigned short,3> type.
 *
 * \ingroup ImageAdaptors
 *
 */
template <class TImage>
class VectorImageToASImageAdaptor : public
      itk::ImageAdaptor<TImage,otb::Accessor::VectorImageToASPixelAccessor<
           typename TImage::PixelType::ValueType> >
{
public:
  /** Standard class typedefs. */
  typedef VectorImageToASImageAdaptor  Self;
  typedef itk::ImageAdaptor<TImage,
              otb::Accessor::VectorImageToASPixelAccessor<
                 typename TImage::PixelType::ValueType
                                       >  > Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro( VectorImageToASImageAdaptor, ImageAdaptor );

  /** PixelContainer typedef support. Used to construct a container for
   * the pixel data. */
  typedef typename Superclass::PixelContainer        PixelContainer;
  typedef typename Superclass::PixelContainerPointer PixelContainerPointer;
  typedef typename Superclass::PixelContainerConstPointer PixelContainerConstPointer;
  
 protected:
  VectorImageToASImageAdaptor() {}
  virtual ~VectorImageToASImageAdaptor() {}
  
 private:
  VectorImageToASImageAdaptor(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
