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
#ifndef __otbVectorDataGlComponent_h
#define __otbVectorDataGlComponent_h

#include "otbGlComponent.h"
#include "itkPreOrderTreeIterator.h"

#  ifdef __APPLE__
#    include <OpenGL/glu.h>
#  else
#    include <GL/glu.h>
#  endif

// There are function prototype conflits under cygwin between standard w32 API
// and standard C ones
#ifndef CALLBACK
#if defined(__CYGWIN__)
#define CALLBACK __stdcall
#else
#define CALLBACK
#endif
#endif


namespace otb
{
/** \class VectorDataGlComponent
*   \brief This Gl Component to render a VectorData.
*   No checking is done upon the adequation between the VectorData
*   projection and the underlying image projection.
*  
*   Origin and Spacing allows to fit to the image axis.
*/

template <class TVectorData> 
class VectorDataGlComponent
  : public GlComponent
{
public:
  /** Standard class typedefs */
  typedef VectorDataGlComponent                 Self;
  typedef GlComponent                           Superclass;
  typedef itk::SmartPointer<Self>               Pointer;
  typedef itk::SmartPointer<const Self>         ConstPointer;
  typedef typename Superclass::RegionType       RegionType;
 
  // affine transform
  typedef Superclass::AffineTransformType       AffineTransformType;
  typedef AffineTransformType::InputPointType   PointType;
  typedef AffineTransformType::InputVectorType  VectorType;
  typedef Superclass::ColorType                 ColorType;

  /** VectorData typedef */
  typedef TVectorData                             VectorDataType;
  typedef typename VectorDataType::Pointer        VectorDataPointerType;
  typedef typename VectorDataType::DataNodeType   DataNodeType;
  typedef typename DataNodeType::Pointer          DataNodePointerType;
  typedef typename VectorDataType::DataTreeType   DataTreeType;
  typedef itk::PreOrderTreeIterator<DataTreeType> TreeIteratorType;
  typedef typename DataNodeType::LineType         LineType;
  typedef typename DataNodeType::PolygonType      PolygonType;
  typedef typename DataNodeType::PolygonListType  PolygonListType;

  /** Runtime information */
  itkTypeMacro(VectorDataGlComponent,GlComponent);

  /** New macro */
  itkNewMacro(Self);

  /// Render the vector data
  virtual void  Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform);
   
  /** Set/Get the grid spacing */
  itkSetMacro(Spacing,VectorType);
  itkGetConstReferenceMacro(Spacing,VectorType);
  
  /** Set/Get the grid origin */
  itkSetMacro(Origin,PointType);
  itkGetConstReferenceMacro(Origin,PointType);

  /** Set/Get the VectorData to render */
  itkSetObjectMacro(VectorData,VectorDataType);
  itkGetObjectMacro(VectorData,VectorDataType);

  /** Set/Get the color */
  itkSetMacro(Color,ColorType);
  itkGetConstReferenceMacro(Color,ColorType);

protected:
  /** Constructor */
  VectorDataGlComponent();
  /** Destructor */
  virtual ~VectorDataGlComponent();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
  
  /// Render a point
  void RenderPoint(const PointType & p, const RegionType & extent, const AffineTransformType * transform);
  /// Render a polyline
  void RenderLine(const LineType * l, const RegionType & extent, const AffineTransformType * transform);
  // Render a complex polygon (with holes)
  void RenderPolygon(const PolygonType * extRing, const PolygonListType * intRings, const RegionType & extent, const AffineTransformType * transform);

  /// Frame a given point using the frame width and color (point
  /// should be in gl screen coordinates)
  void FramePoint(const PointType & p);

private:
  VectorDataGlComponent(const Self&); // purposely not implemented
  void operator=(const Self&);        // purposely not implemented

  // Function pointer typedef
  typedef void (CALLBACK * FunctionPointerType)();
  
  // Static Combine callback for tesselation
  static void TesselationCombineCallback(GLdouble coords[3],GLdouble * data[4], GLfloat weights[4],GLdouble **dataOut)
  {
    GLdouble * vertex = new GLdouble[3];
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    *dataOut = vertex;
  }

  // Static error callback fir tesselation
  static void TesselationErrorCallback(GLenum errorCode)
  {
    const GLubyte * estring = gluErrorString(errorCode);
    itkGenericExceptionMacro(<<"Glu Tesselation error: "<<estring);
  }


  /// Pointer to the vector data to render
  VectorDataPointerType m_VectorData;

  /// Spacing of the image grid
  VectorType m_Spacing;
  
  /// Origin of the image
  PointType  m_Origin;

  /// The GluTesselator object to render complex polygons
  GLUtesselator * m_GluTesselator;

  /// Color of the vector layer
  ColorType m_Color;

}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataGlComponent.txx" 
#endif

#endif



