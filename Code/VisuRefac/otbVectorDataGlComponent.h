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

protected:
  /** Constructor */
  VectorDataGlComponent();
  /** Destructor */
  virtual ~VectorDataGlComponent(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
  
  // Render a point
  void RenderPoint(const PointType & p, const RegionType & extent, const AffineTransformType * transform);
  // Render a polyline
  void RenderLine(const LineType * l, const RegionType & extent, const AffineTransformType * transform);
  // Render a complex polygon (with holes)
  void RenderPolygon(const PolygonType * extRing, const PolygonListType * intRings, const RegionType & extent, const AffineTransformType * transform);

private:
  VectorDataGlComponent(const Self&); // purposely not implemented
  void operator=(const Self&);        // purposely not implemented

  /// Pointer to the vector data to render
  VectorDataPointerType m_VectorData;

  /// Spacing of the image grid
  VectorType m_Spacing;
  
  /// Origin of the image
  PointType  m_Origin;

}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataGlComponent.txx" 
#endif

#endif



