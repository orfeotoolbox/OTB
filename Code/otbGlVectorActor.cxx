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
#include "otbGlVectorActor.h"
#include "otbViewSettings.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>
#include <GL/glext.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

namespace otb
{

// Function pointer typedef
typedef GLvoid (* FunctionPointerType)();


// Static Combine callback for tesselation
static void TesselationCombineCallback(GLdouble coords[2],
                                                GLdouble * /*data*/[4],
                                                GLfloat /*weights*/[4],
                                                GLdouble **dataOut)
{
  GLdouble * vertex = new GLdouble[2];
  vertex[0] = coords[0];
  vertex[1] = coords[1];
  *dataOut = vertex;
}

// Static error callback fir tesselation
static void TesselationErrorCallback(GLenum errorCode)
{
  const GLubyte * estring = gluErrorString(errorCode);
  otbMsgDevMacro(<< "Glu Tesselation error: " << estring);
  //estring = 0; // avoid unused variable warning
}

// Static begin callback for tesselation
static void BeginCallback(GLenum prim)
{
  glBegin(prim);
}

// Static end callback for tesselation
static void EndCallback()
{
  glEnd();
}

// static vertex callback for tesselation
static void VertexCallback(void * data)
{
  glVertex3dv((GLdouble*) data);
}

GlVectorActor::GlVectorActor()
  : m_Color(),
    m_Alpha(1.0),
    m_Fill(true),
    m_ViewportToVectorTransform(),
    m_VectorToViewportTransform(),
    m_OGRDataSource(),
    m_InternalFeatures()
    
{
  m_Color.Fill(0);
  m_Color[0]=1.0;

  // Create tesselator
  m_GluTesselator = gluNewTess();

  // Setting up the tesselator callbacks
  gluTessCallback(m_GluTesselator, GLU_TESS_BEGIN,   (FunctionPointerType) BeginCallback);
  gluTessCallback(m_GluTesselator, GLU_TESS_END,     (FunctionPointerType) EndCallback);
  gluTessCallback(m_GluTesselator, GLU_TESS_ERROR,   (FunctionPointerType) TesselationErrorCallback);
  gluTessCallback(m_GluTesselator, GLU_TESS_VERTEX,  (FunctionPointerType) VertexCallback);
  gluTessCallback(m_GluTesselator, GLU_TESS_COMBINE, (FunctionPointerType) TesselationCombineCallback);

}

GlVectorActor::~GlVectorActor()
{
  // Delete tesselator
  gluDeleteTess(m_GluTesselator);
}

void GlVectorActor::Initialize(const std::string & filename)
{
  // Open the data source
  m_OGRDataSource = otb::ogr::DataSource::New(filename,otb::ogr::DataSource::Modes::Read);

}

void GlVectorActor::GetExtent(double & ulx, double & uly, double & lrx, double & lry) const
{
  PointType ul,lr,ur,ll, vpul,vplr, vpll, vpur;
  m_OGRDataSource->GetGlobalExtent(ul[0],ul[1],lr[0],lr[1]);
  ur=ul;
  ur[0]=lr[0];
  ll=lr;
  ll[0]=ul[0];
  
  
  vpul = m_VectorToViewportTransform->TransformPoint(ul);
  vpur = m_VectorToViewportTransform->TransformPoint(ur);
  vplr = m_VectorToViewportTransform->TransformPoint(lr);
  vpll = m_VectorToViewportTransform->TransformPoint(ll);

  ulx = std::min(std::min(vpul[0],vplr[0]),std::min(vpur[0],vpll[0]));
  uly = std::min(std::min(vpul[1],vplr[1]),std::min(vpur[1],vpll[1]));
  lrx = std::max(std::max(vpul[0],vplr[0]),std::max(vpur[0],vpll[0]));
  lry = std::max(std::max(vpul[1],vplr[1]),std::max(vpur[1],vpll[1]));
}

void GlVectorActor::ProcessViewSettings()
{
  // Is there anything to do ?
  ViewSettings::ConstPointer settings = this->GetSettings();

  
  if((m_ViewportToVectorTransform.IsNull() || m_VectorToViewportTransform.IsNull()) || (settings->GetUseProjection() && settings->GetGeometryChanged()))
    {
    UpdateTransforms();
    InternalFeaturesTransform();
    }
}

void GlVectorActor::UpdateData()
{
  ViewSettings::ConstPointer settings = this->GetSettings();

  unsigned int nbLayers = m_OGRDataSource->GetLayersCount();

  if(nbLayers == 0)
    {
    return;
    }

  otb::ogr::Layer layer = m_OGRDataSource->GetLayer(0);

  m_InternalFeatures.clear();

  // Retrieve the viewport extent
  PointType vpul,vplr,vpur,vpll, ul, lr, ll, ur;
  double ulx,uly,lrx,lry;
  settings->GetViewportExtent(vpul[0],vpul[1],vplr[0],vplr[1]);
  vpur = vpul;
  vpur[0] = vplr[0];
  vpll = vplr;
  vpll[0] = vpul[0];

  // Convert to layer extent  
  ul = m_ViewportToVectorTransform->TransformPoint(vpul);
  ur = m_ViewportToVectorTransform->TransformPoint(vpur);
  ll = m_ViewportToVectorTransform->TransformPoint(vpll);
  lr = m_ViewportToVectorTransform->TransformPoint(vplr);

  ulx = std::min(std::min(ul[0],lr[0]),std::min(ur[0],ll[0]));
  uly = std::min(std::min(ul[1],lr[1]),std::min(ur[1],ll[1]));
  lrx = std::max(std::max(ul[0],lr[0]),std::max(ur[0],ll[0]));
  lry = std::max(std::max(ul[1],lr[1]),std::max(ur[1],ll[1]));

  layer.SetSpatialFilterRect(ulx,uly,lrx,lry);

  otb::ogr::Layer::const_iterator featIt = layer.begin();
  for(; featIt!=layer.end(); ++featIt)
    {
    otb::ogr::Feature srcFeature(layer.GetLayerDefn());
    srcFeature.SetFrom( *featIt, TRUE );

    InternalFeature newInternalFeature(layer.GetLayerDefn());
    newInternalFeature.m_SourceFeature = srcFeature.Clone();
    newInternalFeature.m_RenderedFeature = srcFeature.Clone();

    m_InternalFeatures.push_back(newInternalFeature);
    }

  InternalFeaturesTransform();
}

void GlVectorActor::Render()
{
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
   glEnable(GL_LINE_SMOOTH);
   glColor4d(m_Color[0],m_Color[1],m_Color[2],m_Alpha);
   
   gluTessProperty(m_GluTesselator, GLU_TESS_BOUNDARY_ONLY, !m_Fill);

   for(std::vector<InternalFeature>::iterator it = m_InternalFeatures.begin();
      it!=m_InternalFeatures.end();++it)
    {
    const OGRGeometry * geom = it->m_RenderedFeature.GetGeometry();
    const OGRPoint * inPoint = dynamic_cast<const OGRPoint *>(geom);
    const OGRLineString * inLineString = dynamic_cast<const OGRLineString *>(geom);
    const OGRPolygon * inPolygon = dynamic_cast<const OGRPolygon *>(geom);
    
    if(inPoint)
      {
      // TODO
      }
    else if(inLineString)
      {
      glBegin(GL_LINE_STRIP);

      for(unsigned int i = 0; i < inLineString->getNumPoints();++i)
        {
        glVertex2d(inLineString->getX(i),inLineString->getY(i));
        }

      glEnd();
      }
    else if(inPolygon)
      {
      std::vector<GLdouble *> vertices;

      // Begin a new polygon
      gluTessBeginPolygon(m_GluTesselator, NULL);
      
      // Render the outer boundary
      gluTessBeginContour(m_GluTesselator);
      
      for(unsigned int i = 0; i < inPolygon->getExteriorRing()->getNumPoints();++i)
        {
        GLdouble * glp = new GLdouble[2];
        glp[0] = inPolygon->getExteriorRing()->getX(i);
        glp[1] = inPolygon->getExteriorRing()->getY(i);
        vertices.push_back(glp);

        gluTessVertex(m_GluTesselator,glp,glp);
        }

      // End the outer boundary contour
      gluTessEndContour(m_GluTesselator);
      
      for(unsigned int j = 0; j < inPolygon->getNumInteriorRings();++j)
        {        
        gluTessBeginContour(m_GluTesselator);

        for(unsigned int i = 0; i < inPolygon->getInteriorRing(j)->getNumPoints();++i)
          {
          GLdouble * glp = new GLdouble[2];
          glp[0] = inPolygon->getInteriorRing(j)->getX(i);
          glp[1] = inPolygon->getInteriorRing(j)->getY(i);
          vertices.push_back(glp);
          
          gluTessVertex(m_GluTesselator,glp,glp);
          }
        
        gluTessEndContour(m_GluTesselator);
        }

       // End the polygon
      gluTessEndPolygon(m_GluTesselator);


      // free vertices
      for(std::vector<GLdouble *>::iterator it = vertices.begin();it!=vertices.end();++it)
        {
        delete[] (*it);
        }
      }
    }

   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_BLEND);
   
}


void GlVectorActor::UpdateTransforms()
{
  // Retrieve settings
  ViewSettings::ConstPointer settings = this->GetSettings();

  m_ViewportToVectorTransform = RSTransformType::New();
  m_VectorToViewportTransform = RSTransformType::New(); 

  if(settings->GetUseProjection())
    {
    m_ViewportToVectorTransform->SetInputProjectionRef(settings->GetWkt());
    m_ViewportToVectorTransform->SetInputKeywordList(settings->GetKeywordList());
    m_ViewportToVectorTransform->SetOutputProjectionRef(m_OGRDataSource->GetLayer(0).GetProjectionRef());

    m_VectorToViewportTransform->SetOutputProjectionRef(settings->GetWkt());
    m_VectorToViewportTransform->SetOutputKeywordList(settings->GetKeywordList());
    m_VectorToViewportTransform->SetInputProjectionRef(m_OGRDataSource->GetLayer(0).GetProjectionRef());
    }
  m_ViewportToVectorTransform->InstanciateTransform();
  m_VectorToViewportTransform->InstanciateTransform();
}


// Inner helper function
OGRPoint TransformPoint(OGRPoint * inPoint,otb::GenericRSTransform<> * transform)
{
  OGRPoint outPoint;
  otb::GenericRSTransform<>::InputPointType in,out;
  in[0] = inPoint->getX();
  in[1] = inPoint->getY();
  
  out = transform->TransformPoint(in);
  
  
  outPoint.setX(out[0]);
  outPoint.setY(out[1]);
  
  return outPoint;
}

void GlVectorActor::InternalFeaturesTransform()
{
  for(std::vector<InternalFeature>::iterator it = m_InternalFeatures.begin();
      it!=m_InternalFeatures.end();++it)
    {
    const OGRGeometry * geom = it->m_SourceFeature.GetGeometry();
    const OGRPoint * inPoint = dynamic_cast<const OGRPoint *>(geom);
    const OGRLineString * inLineString = dynamic_cast<const OGRLineString *>(geom);
    const OGRPolygon * inPolygon = dynamic_cast<const OGRPolygon *>(geom);

    if(inPoint)
      {
      OGRPoint outPoint = TransformPoint(const_cast<OGRPoint *>(inPoint),m_VectorToViewportTransform);
      it->m_RenderedFeature.SetGeometry(&outPoint);
      }
  else if(inLineString)
      {
      OGRLineString outLineString;
      
      for(unsigned int i = 0; i<inLineString->getNumPoints();++i)
        {
        OGRPoint p,op;
        inLineString->getPoint(i,&p);
        op = TransformPoint(&p,m_VectorToViewportTransform);
         outLineString.addPoint(&op);
        }
      it->m_RenderedFeature.SetGeometry(&outLineString);
      }
  else if(inPolygon)
    {
    OGRPolygon outPolygon;
    
    // First process external ring
    const OGRLinearRing * inExtRing = inPolygon->getExteriorRing();
    OGRLinearRing outExtRing;

     for(unsigned int i = 0; i<inExtRing->getNumPoints();++i)
        {
        OGRPoint p,op;
        inExtRing->getPoint(i,&p);
        op = TransformPoint(&p,m_VectorToViewportTransform);
        outExtRing.addPoint(&op);
        }
     outPolygon.addRing(&outExtRing);

     // Then process any interior ring
     for(unsigned int j = 0; j<inPolygon->getNumInteriorRings();++j)
       {
       const OGRLinearRing * inIntRing = inPolygon->getInteriorRing(j);
       OGRLinearRing outIntRing;

       for(unsigned int i = 0; i<inIntRing->getNumPoints();++i)
         {
         OGRPoint p,op;
         inIntRing->getPoint(i,&p);
         op = TransformPoint(&p,m_VectorToViewportTransform);
         outIntRing.addPoint(&op);
         }
       outPolygon.addRing(&outIntRing);
       }
     it->m_RenderedFeature.SetGeometry(&outPolygon);
      }
    }
}

}
