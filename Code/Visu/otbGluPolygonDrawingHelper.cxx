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
  std::cout<<"GluPolygonDrawingHelper: Constructor, entering"<<std::endl; 
  m_GluTesselator = gluNewTess();
  std::cout<<"GluPolygonDrawingHelper: Constructor, after tesselator allocation. pointer: "<<m_GluTesselator<<std::endl; 
  //FreeVertices();


  // Pointer to a function
  typedef void (CALLBACK * FunctionPointerType)();
  // Setting up the tesselator callbacks
  gluTessCallback(m_GluTesselator,GLU_TESS_BEGIN,(FunctionPointerType)glBegin);
  gluTessCallback(m_GluTesselator,GLU_TESS_END,(FunctionPointerType)glEnd);
  gluTessCallback(m_GluTesselator,GLU_TESS_ERROR,(FunctionPointerType)ErrorCallback);
  gluTessCallback(m_GluTesselator,GLU_TESS_VERTEX,(FunctionPointerType)VertexCallback);
  gluTessCallback(m_GluTesselator,GLU_TESS_COMBINE,(FunctionPointerType)CombineCallback);

  // Color
  m_Color[0]=0;
  m_Color[1]=0;
  m_Color[2]=0;
  m_Color[3]=0;
std::cout<<"GluPolygonDrawingHelper: Constructor, leaving."<<std::endl; 
}

GluPolygonDrawingHelper::~GluPolygonDrawingHelper()
{
  std::cout<<"GluPolygonDrawingHelper: Destructor, entering"<<std::endl; 
  //FreeVertices();
  std::cout<<"GluPolygonDrawingHelper: Destructor, before tesselator desallocation."<<std::endl; 
  gluDeleteTess(m_GluTesselator);
  std::cout<<"GluPolygonDrawingHelper: Destructor, leaving"<<std::endl; 
}

void GluPolygonDrawingHelper::FreeVertices()
{
  std::cout<<"GluPolygonDrawingHelper: FreeVertices(), entering"<<std::endl; 
  VertexVectorIteratorType it = m_Vertices.begin();
  while(it!=m_Vertices.end())
  {
	if((*it)!=NULL)
	{
          std::cout<<"GluPolygonDrawingHelper: FreeVertices(), freeing "<<(*it)<<std::endl; 
	  delete [](*it);
	}
	++it;
  }
        std::cout<<"GluPolygonDrawingHelper: FreeVertices(), clearing vertex list. "<<std::endl; 
	m_Vertices.clear();
       std::cout<<"GluPolygonDrawingHelper: FreeVertices(), leaving. "<<std::endl; 
}

void GluPolygonDrawingHelper::BeginPolygon()
{
  std::cout<<"GluPolygonDrawingHelper: BeginPolygon(), entering"<<std::endl; 
  gluTessBeginPolygon(m_GluTesselator, NULL);
  std::cout<<"GluPolygonDrawingHelper: BeginPolygon(), leaving. "<<std::endl; 
}

void GluPolygonDrawingHelper::BeginContour()
{
std::cout<<"GluPolygonDrawingHelper: BeginContour(), entering"<<std::endl; 
  gluTessBeginContour(m_GluTesselator);
  std::cout<<"GluPolygonDrawingHelper: BeginContour(), leaving. "<<std::endl;
}

void GluPolygonDrawingHelper::EndPolygon()
{
  std::cout<<"GluPolygonDrawingHelper: EndPolygon(), entering. "<<std::endl;
  gluTessEndPolygon(m_GluTesselator);
  std::cout<<"GluPolygonDrawingHelper: EndPolygon(), after gluTessPolygon(m_GluTesselator) call. "<<std::endl;
  //FreeVertices();
  std::cout<<"GluPolygonDrawingHelper: EndPolygon(), leaving. "<<std::endl;
}

void GluPolygonDrawingHelper::EndContour()
{
   std::cout<<"GluPolygonDrawingHelper: EndContour(), entering. "<<std::endl;
  gluTessEndContour(m_GluTesselator);
  std::cout<<"GluPolygonDrawingHelper: EndContour(), leaving. "<<std::endl;
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
  std::cout<<"GluPolygonDrawingHelper: Vertex2d("<<x<<","<<y<<"), entering. "<<std::endl;
  GLdouble * vertex = new GLdouble[7];
  std::cout<<"GluPolygonDrawingHelper: Vertex2d("<<x<<","<<y<<"), after vertex allocation. pointer: "<<vertex<<std::endl;
  vertex[0]=x;
  vertex[1]=y;
  vertex[2]=0;
  vertex[3]=m_Color[0];
  vertex[4]=m_Color[1];
  vertex[5]=m_Color[2];
  vertex[6]=m_Color[3];

  m_Vertices.push_back(vertex);
  std::cout<<"GluPolygonDrawingHelper: Vertex2d("<<x<<","<<y<<"), Before gluTessVertex() call."<<std::endl;
  gluTessVertex(m_GluTesselator,vertex,vertex); 
  std::cout<<"GluPolygonDrawingHelper: Vertex2d("<<x<<","<<y<<"), entering. "<<std::endl;
}

void GluPolygonDrawingHelper::Vertex3d(double x, double y, double z)
{
  GLdouble * vertex = new GLdouble[7];
  vertex[0]=x;
  vertex[1]=y;
  vertex[2]=z;
  vertex[3]=m_Color[0];
  vertex[4]=m_Color[1];
  vertex[5]=m_Color[2];
  vertex[6]=m_Color[3];
 
  m_Vertices.push_back(vertex);
  gluTessVertex(m_GluTesselator,vertex,vertex);
}

void GluPolygonDrawingHelper::VertexCallback(GLvoid * vertex)
{
  std::cout<<"GluPolygonDrawingHelper: VertexCallBack("<<vertex<<"), entering. "<<std::endl;
  const GLdouble * pointer = (GLdouble *) vertex;
  glColor4dv(pointer+3);
  glVertex3dv(pointer);
  std::cout<<"GluPolygonDrawingHelper: VertexCallBack("<<vertex<<"), leaving. "<<std::endl;
  otbMsgDevMacro(<<"Glu Tesselator --Vertex callback: "<<pointer[0]<<" "<<pointer[1]
		 <<" "<<pointer[2]<<" "<<pointer[3]
		 <<" "<<pointer[4]<<" "<<pointer[5]
		 <<" "<<pointer[6]);
}

void GluPolygonDrawingHelper::CombineCallback(GLdouble coords[3],GLdouble * data[4], GLfloat weights[4],GLdouble **dataOut)
{
   std::cout<<"GluPolygonDrawingHelper: CombineCallBack(), entering. "<<std::endl;
  GLdouble * vertex = new GLdouble[7];
  vertex[0] = coords[0];
  vertex[1] = coords[1];
  vertex[2] = coords[2];
  std::cout<<"GluPolygonDrawingHelper: CombineCallBack(), after memory allocation. pointer: "<<vertex<<std::endl;

  for(unsigned int i = 3; i<7;++i)
    {
      vertex[i]= weights[0]*data[0][i]
	+weights[1]*data[1][i]
	+weights[2]*data[2][i]
	+weights[3]*data[3][i];
    }
  std::cout<<"GluPolygonDrawingHelper: CombineCallBack(), after data computation."<<std::endl;
  otbMsgDevMacro(<<"Glu Tesselator -- Combine callback: "<<vertex[0]<<" "<<vertex[1]
		 <<" "<<vertex[2]<<" "<<vertex[4]
		 <<" "<<vertex[4]<<" "<<vertex[5]
		 <<" "<<vertex[6]);
  *dataOut = vertex;
std::cout<<"GluPolygonDrawingHelper: CombineCallBack(), leaving. "<<std::endl;
}

void GluPolygonDrawingHelper::ErrorCallback(GLenum errorCode)
{
  const GLubyte * estring = gluErrorString(errorCode);
  itkGenericExceptionMacro(<<"Tesselation error: "<<estring);
}
} // end namespace otb
