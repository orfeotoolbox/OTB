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

// We need this include to get NodeType enum right.
#include "otbDataNode.h"

namespace otb
{
template <class TVectorData> 
VectorDataGlComponent<TVectorData>
::VectorDataGlComponent() : m_VectorData(),m_Spacing(), m_Origin(), m_GluTesselator(), m_Color(),
			    m_LineWidth(2),m_CrossWidth(10),m_RenderPolygonBoundariesOnly(false)
{
  // Default color is red
  m_Color.Fill(0);
  m_Color[0]=1.;
  m_Color[3]=0.75;

  // Intialize origin and spacing
  m_Origin.Fill(0.);
  m_Spacing.Fill(1.);
  
  // Create the tesselator
  m_GluTesselator = gluNewTess();
  
  // Set the winding rule
//   gluTessProperty(m_GluTesselator,GLU_TESS_WINDING_RULE,GLU_TESS_WINDING_ODD);

  // Setting up the tesselator callbacks
  gluTessCallback(m_GluTesselator,GLU_TESS_BEGIN,  (FunctionPointerType) glBegin);
  gluTessCallback(m_GluTesselator,GLU_TESS_END,    (FunctionPointerType) glEnd);
  gluTessCallback(m_GluTesselator,GLU_TESS_ERROR,  (FunctionPointerType) TesselationErrorCallback);
  gluTessCallback(m_GluTesselator,GLU_TESS_VERTEX, (FunctionPointerType) glVertex3dv);
  gluTessCallback(m_GluTesselator,GLU_TESS_COMBINE,(FunctionPointerType) TesselationCombineCallback);
}

template <class TVectorData> 
VectorDataGlComponent<TVectorData>
::~VectorDataGlComponent() 
{
  // Delete the tesselator
  m_GluTesselator = gluNewTess();
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


  // Set up blending and color
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4d(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
  
  // Set up line width
  double previousWidth = 0.;
  glGetDoublev(GL_LINE_WIDTH,&previousWidth);
  glLineWidth(m_LineWidth);

  // Do we need to render boundaries only (for polygons)
  gluTessProperty(m_GluTesselator,GLU_TESS_BOUNDARY_ONLY,m_RenderPolygonBoundariesOnly);


  // Enabling line antialiasing
  glEnable(GL_LINE_SMOOTH);

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
  glDisable(GL_LINE_SMOOTH);
  glDisable(GL_BLEND);
  glLineWidth(previousWidth);
}
   template <class TVectorData>   
void 
VectorDataGlComponent<TVectorData>
::RenderPoint(const PointType & p, const RegionType & extent, const AffineTransformType * transform)
{
  // Take into account pixel spacing and origin
  PointType spacePoint = p;
  spacePoint[0]*= m_Spacing[0];
  spacePoint[1]*= m_Spacing[1];
  spacePoint[0]-= m_Origin[0];
  spacePoint[1]-=m_Origin[1];

  // Transform to a screen point
  PointType screenPoint = transform->TransformPoint(spacePoint);
    
  glBegin(GL_LINES);
  // Draw a cross
  glVertex2d(screenPoint[0]-m_CrossWidth,screenPoint[1]);
  glVertex2d(screenPoint[0]+m_CrossWidth,screenPoint[1]);
  glVertex2d(screenPoint[0],screenPoint[1]-m_CrossWidth);
  glVertex2d(screenPoint[0],screenPoint[1]+m_CrossWidth);
  glEnd();
}

template <class TVectorData>   
void 
VectorDataGlComponent<TVectorData>
::RenderLine(const LineType * l, const RegionType & extent, const AffineTransformType * transform)
{
  // Iterate on the line
  typename LineType::VertexListType::ConstIterator vIt = l->GetVertexList()->Begin();

  glBegin(GL_LINE_STRIP);
  
  while(vIt != l->GetVertexList()->End())
    {
    // Take into account pixel spacing and origin
    PointType spacePoint = vIt.Value();
    spacePoint[0]*= m_Spacing[0];
    spacePoint[1]*= m_Spacing[1];
    spacePoint[0]-= m_Origin[0];
    spacePoint[1]-= m_Origin[1];
    
    // Transform to a screen point
    PointType screenPoint = transform->TransformPoint(spacePoint);
    
    // Add a point to the rendered line
    glVertex2d(screenPoint[0],screenPoint[1]);
    ++vIt;
    }
  glEnd();
}

template <class TVectorData>   
void 
VectorDataGlComponent<TVectorData>
::RenderPolygon(const PolygonType * extRing, const PolygonListType * intRings, const RegionType & extent, const AffineTransformType * transform)
{
  typedef std::vector<GLdouble * > VertexVectorType;

  // A buffer to hold vertex until they are rendered
  VertexVectorType vertexBuffer;

  // Begin a new polygon
  gluTessBeginPolygon(m_GluTesselator,NULL);

  // Render the outer boundary
  gluTessBeginContour(m_GluTesselator);

 // Iterate on the polygon
  typename PolygonType::VertexListType::ConstIterator vIt = extRing->GetVertexList()->Begin();

  while(vIt!= extRing->GetVertexList()->End())
    {    
    // Take into account pixel spacing and origin
    PointType spacePoint = vIt.Value();
    spacePoint[0]*= m_Spacing[0];
    spacePoint[1]*= m_Spacing[1];
    spacePoint[0]-= m_Origin[0];
    spacePoint[1]-= m_Origin[1];
    
    // Transform to a screen point
    PointType screenPoint = transform->TransformPoint(spacePoint);
    
    // Convert to double array
    GLdouble * glp = new GLdouble[3];
    glp[0]=screenPoint[0];
    glp[1]=screenPoint[1];
    glp[2]=0.;
    vertexBuffer.push_back(glp);

    // Add a point to the outer boundary
    gluTessVertex(m_GluTesselator,glp,glp);
    ++vIt;
    }

  // End the outer boundary contour
  gluTessEndContour(m_GluTesselator);

  // Render remaining inner boundaries
  typename PolygonListType::ConstIterator pIt = intRings->Begin();

  // For each inner boundary
  while(pIt != intRings->End())
    {
    vIt = pIt.Get()->GetVertexList()->Begin();
    
    // Begin a new contour
     gluTessBeginContour(m_GluTesselator);

    // Render each of its vertex
     while(vIt!= pIt.Get()->GetVertexList()->End())
       {    
       // Take into account pixel spacing and origin
       PointType spacePoint = vIt.Value();
       spacePoint[0]*= m_Spacing[0];
       spacePoint[1]*= m_Spacing[1];
       spacePoint[0]-= m_Origin[0];
       spacePoint[1]-= m_Origin[1];
       
       // Transform to a screen point
       PointType screenPoint = transform->TransformPoint(spacePoint);
       
       // Convert to double array
       GLdouble * glp = new GLdouble[3];
       glp[0]=screenPoint[0];
       glp[1]=screenPoint[1];
       glp[2]=0.;
       vertexBuffer.push_back(glp);

       // Add a point to the outer boundary
       gluTessVertex(m_GluTesselator,glp,glp);
       ++vIt;
       }
     
     // End the contour
     gluTessEndContour(m_GluTesselator);
    ++pIt;
    }

  // End the polygon
  gluTessEndPolygon(m_GluTesselator);

  // // Do not forget to free all the vertex
  for(typename VertexVectorType::iterator it = vertexBuffer.begin();
      it!=vertexBuffer.end();++it)
    {
    delete [] (*it);
    }
}

// template <class TVectorData>   
// void 
// VectorDataGlComponent<TVectorData>
// ::FramePoint(const PointType & point)
// {
//   // Set the frame color
//   glColor4d(0,0,1,1);

//   glBegin(GL_LINE_LOOP);
//   glVertex2d(point[0]-5,point[1]-5);
//   glVertex2d(point[0]-5,point[1]+5);
//   glVertex2d(point[0]+5,point[1]+5);
//   glVertex2d(point[0]+5,point[1]-5);
//   glEnd();
  
//   // Set back the drawing color
//   glColor4d(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);

// }
}
#endif


