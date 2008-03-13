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
#ifndef CALLBACK
#define CALLBACK
#endif

namespace otb
{

GluPolygonDrawingHelper::GluPolygonDrawingHelper()
{
  // Instantiation of the tesselator object
  //std::cout<<"GluPolygonDrawingHelper: Constructor, entering"<<std::endl; 
  m_PointVector.clear();
  m_GluTesselator = gluNewTess();
  // Color
  m_Color[0]=0;
  m_Color[1]=0;
  m_Color[2]=0;
  m_Color[3]=0;

  // Pointer to a function
    typedef void (CALLBACK * FunctionPointerType)();
    // Setting up the tesselator callbacks
    gluTessCallback(m_GluTesselator,GLU_TESS_BEGIN,(FunctionPointerType)glBegin);
    gluTessCallback(m_GluTesselator,GLU_TESS_END,(FunctionPointerType)glEnd);
    gluTessCallback(m_GluTesselator,GLU_TESS_ERROR,(FunctionPointerType)ErrorCallback);
    gluTessCallback(m_GluTesselator,GLU_TESS_VERTEX,(FunctionPointerType)VertexCallback);
    gluTessCallback(m_GluTesselator,GLU_TESS_COMBINE,(FunctionPointerType)CombineCallback);

    //std::cout<<"GluPolygonDrawingHelper: Constructor, leaving."<<std::endl; 
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
  GLdouble ** data = new GLdouble*[m_PointVector.size()];
  unsigned int i = 0;
  PointVectorType::iterator it;
  for(it = m_PointVector.begin();it!=m_PointVector.end();++i,++it)
    {
      data[i]= new GLdouble[7];
      data[i][0]=(*it)[0];
      data[i][1]=(*it)[1];
      data[i][2]=(*it)[2];
      data[i][3]=m_Color[0];
      data[i][4]=m_Color[1];
      data[i][5]=m_Color[2];
      data[i][6]=m_Color[3];
    }
    
    gluTessBeginPolygon(m_GluTesselator, NULL);
    gluTessBeginContour(m_GluTesselator);
    
    for(i=0;i<m_PointVector.size();++i)
      {
	gluTessVertex(m_GluTesselator,data[i],data[i]); 
      }
    gluTessEndContour(m_GluTesselator);
    gluTessEndPolygon(m_GluTesselator);

    m_PointVector.clear();

    for(i=0;i<m_PointVector.size();++i)
      {
	delete []data[i];
      }
    delete []data;
}



void GluPolygonDrawingHelper::VertexCallback(GLvoid * vertex)
{
  //std::cout<<"GluPolygonDrawingHelper: VertexCallBack("<<vertex<<"), entering. "<<std::endl;
  const GLdouble * pointer = (GLdouble *) vertex;
  glColor4dv(pointer+3);
  glVertex3dv(pointer);

  //std::cout<<"Glu Tesselator --Vertex callback: "<<pointer[0]<<" "<<pointer[1]
  //		 <<" "<<pointer[2]<<" "<<pointer[3]
  //		 <<" "<<pointer[4]<<" "<<pointer[5]
  //	   <<" "<<pointer[6]<<std::endl;
    //std::cout<<"GluPolygonDrawingHelper: VertexCallBack("<<vertex<<"), leaving. "<<std::endl;
}

void GluPolygonDrawingHelper::CombineCallback(GLdouble coords[3],GLdouble * data[4], GLfloat weights[4],GLdouble **dataOut)
{
   //std::cout<<"GluPolygonDrawingHelper: CombineCallBack(), entering. "<<std::endl;
  GLdouble * vertex = new GLdouble[7];
  vertex[0] = coords[0];
  vertex[1] = coords[1];
  vertex[2] = coords[2];
  //std::cout<<"GluPolygonDrawingHelper: CombineCallBack(), after memory allocation. pointer: "<<vertex<<std::endl;

  for(unsigned int i = 3; i<7;++i)
    {
      vertex[i]= data[0][i]
	/**+weights[1]*data[1][i]*/
	/*+weights[2]*data[2][i]
	  +weights[3]*data[3][i]*/;
    }
  //std::cout<<"GluPolygonDrawingHelper: CombineCallBack(), after data computation."<<std::endl;
  //otbMsgDevMacro(<<"Glu Tesselator -- Combine callback: "<<vertex[0]<<" "<<vertex[1]
  //		 <<" "<<vertex[2]<<" "<<vertex[4]
  //		 <<" "<<vertex[4]<<" "<<vertex[5]
  //		 <<" "<<vertex[6]);
  *dataOut = vertex;
//std::cout<<"GluPolygonDrawingHelper: CombineCallBack(), leaving. "<<std::endl;
}

void GluPolygonDrawingHelper::ErrorCallback(GLenum errorCode)
{
  const GLubyte * estring = gluErrorString(errorCode);
  itkGenericExceptionMacro(<<"Tesselation error: "<<estring);
}
} // end namespace otb
