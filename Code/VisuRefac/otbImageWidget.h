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
#ifndef __otbImageWidget_h
#define __otbImageWidget_h

#include "otbGlWidget.h"

// This is included for the default template
#include "otbImage.h"
#include "itkRGBPixel.h"
#include "itkFixedArray.h"
#include "otbGlComponent.h"
#include "otbObjectList.h"

namespace otb
{
/** \class ImageWidget
*   \brief This class renders an RGB bytes image buffer to the screen.
*   Rendered data can be loaded using the ReadBuffer() method.
*   The SetIsotropicZoom() method allows to tune the zooming (zooming
*   is centered).
*
*   It is also able to display a rectangle on the displayed image.
*/

template <class TInputImage=otb::Image<itk::RGBPixel<unsigned char>,2 > >
class ImageWidget
  : public GlWidget
{
public:
  /** Standard class typedefs */
  typedef ImageWidget                       Self;
  typedef GlWidget                          Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageWidget,GlWidget);
  /** Input image typedef */
  typedef TInputImage                         InputImageType;
  /** Image region typedef */
  typedef typename InputImageType::RegionType RegionType;
  /** Region size & index typedef */
  typedef typename RegionType::SizeType       SizeType;
  typedef typename RegionType::IndexType      IndexType;
  
  /** GlComponent typedef */
  typedef GlComponent                                   GlComponentType;
  typedef typename GlComponentType::Pointer             GlComponentPointerType;
  typedef typename GlComponentType::ColorType           ColorType;     
  typedef typename GlComponentType::AffineTransformType AffineTransformType;
  typedef typename AffineTransformType::Pointer         AffineTransformPointerType;
  typedef typename GlComponentType::VectorType          VectorType;
  typedef typename GlComponentType::PointType           PointType;
  typedef ObjectList<GlComponentType>                   GlComponentListType;
  typedef typename GlComponentListType::Pointer        GlComponentListPointerType;
  typedef typename GlComponentListType::Iterator       GlComponentIteratorType;

  /** Reads the OpenGl buffer from an image pointer
   *  \param image The image pointer,
   *  \param region The region to read.
   *  Potential exception thrown if region is outside of the buffered
   *  region.
   * This method fills the m_OpenGl buffer according to the region
   *  size. Buffer in flipped over X axis if OTB_USE_GL_ACCEL is OFF.
   */
  virtual void ReadBuffer(const InputImageType * image, const RegionType & region);

  /** Set/Get the Isotropic zoom */
  itkSetMacro(IsotropicZoom,double);
  itkGetMacro(IsotropicZoom,double);

  /** Set/Get the subsampling rate */
  itkSetMacro(SubsamplingRate,unsigned int);
  itkGetMacro(SubsamplingRate,unsigned int);

  /** Get the image to screen transform */
  itkGetObjectMacro(ImageToScreenTransform,AffineTransformType);
  itkGetObjectMacro(ScreenToImageTransform,AffineTransformType);


  /** Add a GlComponent */
  unsigned int AddGlComponent(GlComponent * glComponent)
  {
    m_GlComponents->PushBack(glComponent);
    return m_GlComponents->Size()-1;
  }

  /** Remove a GlComponent */
  void RemoveGlComponent(unsigned int index)
  {
    m_GlComponents->Erase(index);
  }

  /** Clear the GlComponent list */
  void ClearGlComponents()
  {
    m_GlComponents->Clear();
  }

  /** Get the number of GlComponent */
  unsigned int GetNumberOfGlComponents()
  {
    return m_GlComponents->Size();
  }

protected:
  /** Constructor */
  ImageWidget();
  /** Destructor */
  ~ImageWidget();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Actually render the buffer to the screen. This method is
    * used by FLTK routines and should not be called on its own.
    */

  /** Update the image to screen transform */
  void UpdateTransforms();

  virtual void draw(void);

   /** Compute the linear buffer index according to the 2D region and
   * its 2D index.This method is used when OTB_GL_USE_ACCEL is ON.
   * \param index 2D index
   * \param region 2D region
   */
  static inline unsigned int ComputeBufferIndex(const IndexType& index, const RegionType & region)
  {
    return (index[1]-region.GetIndex()[1])*3*region.GetSize()[0]+3*(index[0]-region.GetIndex()[0]);
  }

  /** Compute the linear buffer index according to the 2D region and
   * its 2D index.This method is used when OTB_GL_USE_ACCEL is OFF.
   * The resulting buffer will be flipped over the X axis.
   * \param index 2D index
   * \param region 2D region
   */
  static inline unsigned int ComputeXAxisFlippedBufferIndex(const IndexType& index,const RegionType & region)
  {
    return  (region.GetSize()[1]-1+region.GetIndex()[1]-index[1])*3*region.GetSize()[0]+3*(index[0]-region.GetIndex()[0]);
  }

private:
  ImageWidget(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** OpenGl zoom factor */
  double m_IsotropicZoom;

  /** OpenGl buffer      */
  unsigned char * m_OpenGlBuffer;

  /** OpenGl buffered region */
  RegionType m_OpenGlBufferedRegion;
  
  /** The display extent */
  RegionType m_Extent;

  /** If the image is subsampled with respect to the original image,
   * this indicates the subsampling rate */
  unsigned int m_SubsamplingRate;

  /** Space to screen transform */
  AffineTransformPointerType m_ImageToScreenTransform;
  AffineTransformPointerType m_ScreenToImageTransform;

  /** Addtionnal Gl components */
  GlComponentListPointerType m_GlComponents;

}; // end class
} // end namespace otb

#ifndef  OTB_MANUAL_INSTANTIATION
#include "otbImageWidget.txx"
#endif

#endif


