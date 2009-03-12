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
#include <iostream>

#include "otbGluPolygonDrawingHelper.h"
#include "otbMacro.h"

// This is defined in windows only, and it is needed for FunctionPointerType
// to be properly defined.

// There are function prototype conflits under cygwin between standard w32 API
// and standard C ones
#ifndef CALLBACK
#if defined(__CYGWIN__)
#define CALLBACK __stdcall
#else
#define CALLBACK
#endif
#endif

extern "C"
{
  typedef void (CALLBACK * FunctionPointerType)();

  void CombineCallback(GLdouble coords[3],GLdouble * data[4], GLfloat weights[4],GLdouble **dataOut)
  {
    GLdouble * vertex = new GLdouble[3];
    vertex[0] = coords[0];
    vertex[1] = coords[1];
    vertex[2] = coords[2];
    *dataOut = vertex;
  }

  void ErrorCallback(GLenum errorCode)
  {
    const GLubyte * estring = gluErrorString(errorCode);
    std::cout<<"Glu Tesselation error: "<<estring<<std::endl;
  }
} // end extern C

namespace otb
{

GluPolygonDrawingHelper::GluPolygonDrawingHelper()
{
  // Instantiation of the tesselator object
  m_PointVector.clear();
  m_GluTesselator = gluNewTess();

  // Color
  m_Color[0]=0;
  m_Color[1]=0;
  m_Color[2]=0;
  m_Color[3]=0;

  // Setting up the tesselator callbacks
  gluTessCallback(m_GluTesselator,GLU_TESS_BEGIN,(FunctionPointerType)glBegin);
  gluTessCallback(m_GluTesselator,GLU_TESS_END,(FunctionPointerType)glEnd);
  gluTessCallback(m_GluTesselator,GLU_TESS_ERROR,(FunctionPointerType)ErrorCallback);
  gluTessCallback(m_GluTesselator,GLU_TESS_VERTEX,(FunctionPointerType)glVertex3dv);
  gluTessCallback(m_GluTesselator,GLU_TESS_COMBINE,(FunctionPointerType)CombineCallback);
}

GluPolygonDrawingHelper::~GluPolygonDrawingHelper()
{
  gluDeleteTess(m_GluTesselator);
}

void  GluPolygonDrawingHelper::SetWindingRule(GLdouble windingRule)
{
  gluTessProperty(m_GluTesselator,GLU_TESS_WINDING_RULE,windingRule);
}

void GluPolygonDrawingHelper::SetBoundaryOnly(GLdouble boundaryOnly)
{
  gluTessProperty(m_GluTesselator,GLU_TESS_BOUNDARY_ONLY,boundaryOnly);
}

void GluPolygonDrawingHelper::Color3d(double r, double g, double b)
{
  m_Color[0]=r;
  m_Color[1]=g;
  m_Color[2]=b;
  m_Color[3]=1.;
}

void GluPolygonDrawingHelper::Color4d(double r, double g, double b, double alpha)
{
  m_Color[0]=r;
  m_Color[1]=g;
  m_Color[2]=b;
  m_Color[3]=alpha;
}

void GluPolygonDrawingHelper::Vertex2d(double x, double y)
{
  PointType p;
  p[0]=x;
  p[1]=y;
  p[2]=0;
  m_PointVector.push_back(p);

}

void GluPolygonDrawingHelper::Vertex3d(double x, double y, double z)
{
  PointType p;
  p[0]=x;
  p[1]=y;
  p[2]=z;
  m_PointVector.push_back(p);
}

void GluPolygonDrawingHelper::RenderPolygon()
{
  // Set up a malloced space for the polygon data.
  GLdouble ** data = new GLdouble*[m_PointVector.size()];
  unsigned int i = 0;
  PointVectorType::iterator it;
  for (it = m_PointVector.begin();it!=m_PointVector.end();++i,++it)
  {
    data[i]= new GLdouble[3];
    data[i][0]=(*it)[0];
    data[i][1]=(*it)[1];
    data[i][2]=(*it)[2];
  }
  // Set the color
  glColor4d(m_Color[0],m_Color[1],m_Color[2],m_Color[3]);
  gluTessBeginPolygon(m_GluTesselator, NULL);
  gluTessBeginContour(m_GluTesselator);

  for (i=0;i<m_PointVector.size();++i)
  {
    gluTessVertex(m_GluTesselator,data[i],data[i]);
  }
  gluTessEndContour(m_GluTesselator);
  gluTessEndPolygon(m_GluTesselator);

  for (i=0;i<m_PointVector.size();++i)
  {
    delete []data[i];
  }
  delete []data;

  m_PointVector.clear();
}
} // end namespace otb
