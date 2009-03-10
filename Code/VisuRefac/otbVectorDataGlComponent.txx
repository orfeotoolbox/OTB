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
#ifndef __otbVectorDataGlComponent_txx
#define __otbVectorDataGlComponent_txx

#include "otbVectorDataGlComponent.h"

namespace otb
{
template <class TVectorData> 
VectorDataGlComponent<TVectorData>
::VectorDataGlComponent() : m_VectorData(),m_Spacing(), m_Origin()
{
  m_Origin.Fill(0.);
  m_Spacing.Fill(1.);
}

template <class TVectorData>   
void 
VectorDataGlComponent<TVectorData>
::Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform)
{
  if(m_VectorData.IsNull())
    {
    // nothing to render, return
    return;
    }

  // Iterate on the data tree
  TreeIteratorType it(m_VectorData->GetDataTree());
  
  it.GoToBegin();

  while(!it.IsAtEnd())
    {
    DataNodePointerType node = it.Get();

    switch(node->GetNodeType())
      {
      case FEATURE_POINT:
      {
      this->RenderPoint(node->GetPoint(),extent,space2ScreenTransform);
	break;

      }
      case FEATURE_LINE:
      {
      this->RenderLine(node->GetLine(),extent,space2ScreenTransform);
      break;
      }
      case FEATURE_POLYGON:
      {
      this->RenderPolygon(node->GetPolygonExteriorRing(),node->GetPolygonInteriorRings(),
			  extent,space2ScreenTransform);
      break;
      }
      default:
      {
      // discard
      break;
      }
      }
    ++it;
    }
}
   template <class TVectorData>   
void 
VectorDataGlComponent<TVectorData>
::RenderPoint(const PointType & p, const RegionType & extent, const AffineTransformType * transform)
{


}

template <class TVectorData>   
void 
VectorDataGlComponent<TVectorData>
::RenderLine(const LineType * l, const RegionType & extent, const AffineTransformType * transform)
{


}

template <class TVectorData>   
void 
VectorDataGlComponent<TVectorData>
::RenderPolygon(const PolygonType * extRing, const PolygonListType * intRings, const RegionType & extent, const AffineTransformType * transform)
{


}

}
#endif


