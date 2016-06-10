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

#ifndef __otbVectorImageToASImageAdaptor_h
#define __otbVectorImageToASImageAdaptor_h

#include "itkImageAdaptor.h"
#include "otbVectorImageToASPixelAccessor.h"

namespace otb
{

/** \class VectorImageToASImageAdaptor
 * \brief Presents an image of pixel type Vector as being and image of
 * FixelArray<unsigned short, 3> type.
 *
 * \ingroup ImageAdaptors
 *
 *
 * \ingroup OTBCommon
 */
template <class TImage>
class ITK_EXPORT VectorImageToASImageAdaptor : public
  itk::ImageAdaptor<TImage, otb::Accessor::VectorImageToASPixelAccessor<
          typename TImage::PixelType::ValueType> >
{
public:
  /** Standard class typedefs. */
  typedef VectorImageToASImageAdaptor Self;
  typedef itk::ImageAdaptor<TImage,
      otb::Accessor::VectorImageToASPixelAccessor<
          typename TImage::PixelType::ValueType
          > > Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorImageToASImageAdaptor, ImageAdaptor);

  /** PixelContainer typedef support. Used to construct a container for
   * the pixel data. */
  typedef typename Superclass::PixelContainer             PixelContainer;
  typedef typename Superclass::PixelContainerPointer      PixelContainerPointer;
  typedef typename Superclass::PixelContainerConstPointer PixelContainerConstPointer;

protected:
  VectorImageToASImageAdaptor() {}
  ~VectorImageToASImageAdaptor() ITK_OVERRIDE {}

private:
  VectorImageToASImageAdaptor(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
