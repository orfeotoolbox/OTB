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
#ifndef __otbImageWidgetPolygonForm_h
#define __otbImageWidgetPolygonForm_h

#include "itkSize.h"
#include "itkIndex.h"
#include "otbImageWidgetFormBase.h"
#include "otbPolygon.h"
#  ifdef __APPLE__
#    include <OpenGL/glu.h>
#  else
#    include <GL/glu.h>
#  endif

namespace otb
{
/** \class ImageWidgetPolygonForm
   * \brief Widget to draw polygons on the viewer
 *
 */
template<class TValue = double>
class ITK_EXPORT ImageWidgetPolygonForm
  : public ImageWidgetFormBase
{
  public:
  /** Standard class typedefs */
  typedef ImageWidgetPolygonForm        Self;
  typedef ImageWidgetFormBase           Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef TValue                        ValueType;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageWidgetPolygonForm, ImageWidgetFormBase);

  /** Usefull typedef. */
  typedef itk::Size<2>                               SizeType;
  typedef itk::Index<2>                              IndexType;
  typedef Polygon<ValueType>                         PolygonType;
  typedef typename PolygonType::Pointer              PolygonPointerType;
  typedef typename PolygonType::VertexType           VertexType;
  typedef typename PolygonType::VertexListType       VertexListType;
  typedef typename VertexListType::ConstIterator     VertexListConstIteratorType;
  typedef typename PolygonType::ContinuousIndexType  ContinuousIndexType;

  /** Accessors */
  itkSetMacro(Polygon, PolygonPointerType);
  itkGetMacro(Polygon, PolygonPointerType);
  itkSetMacro(Solid,bool);
  itkGetMacro(Solid,bool);
  itkSetMacro(InternalValueToAlphaChannel,bool);
  itkGetMacro(InternalValueToAlphaChannel,bool);

  /** Actually draw the polygon */
  void Draw(double openGlZoom, unsigned int originx, unsigned int originy, unsigned int windowh, unsigned int ss_rate);


  protected:
  /** Constructor. */
  ImageWidgetPolygonForm();
  /** Destructor. */
  ~ImageWidgetPolygonForm();

  private:
  ImageWidgetPolygonForm(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented

  /** Internal pointer to the otb::Polygon data structure */
  PolygonPointerType m_Polygon;
  /** true if needed to draw a solid polygon */
  bool m_Solid;
  /** True if polygon internal value affects the alpha color value
    * (can be used to denote membership values )
    */
  bool m_InternalValueToAlphaChannel;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageWidgetPolygonForm.txx"
#endif
#endif
