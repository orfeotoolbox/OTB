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

/*
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
*/

namespace otb
{

// Function pointer typedef
#ifdef WIN32
  typedef void (CALLBACK *FunctionPointerType)();
# define ICE_CALLBACK CALLBACK
#else
  typedef GLvoid (* FunctionPointerType)();
# define ICE_CALLBACK
#endif

// Static Combine callback for tesselation
static void ICE_CALLBACK TesselationCombineCallback(GLdouble coords[3],
                                                GLdouble * /*data*/[4],
                                                GLfloat /*weights*/[4],
                                                GLdouble **dataOut)
{
  GLdouble * vertex = new GLdouble[3];
  vertex[0] = coords[0];
  vertex[1] = coords[1];
  vertex[2] = 0;
  *dataOut = vertex;
}

// Static error callback fir tesselation
static void ICE_CALLBACK TesselationErrorCallback(GLenum errorCode)
{
  const GLubyte * estring = gluErrorString(errorCode);
  otbMsgDevMacro(<< "Glu Tesselation error: " << estring);
  (void)estring;
}

// Static begin callback for tesselation
static void ICE_CALLBACK BeginCallback(GLenum prim)
{
  glBegin(prim);
}

// Static end callback for tesselation
static void ICE_CALLBACK EndCallback()
{
  glEnd();
}

// static vertex callback for tesselation
static void ICE_CALLBACK VertexCallback(void * data)
{
  glVertex3dv((GLdouble*) data);
}

GlVectorActor::GlVectorActor()
  : m_Color(),
    m_Alpha(1.0),
    m_Fill(true),
    m_SolidBorder(true),
    m_LineWidth(1.0),
    m_ViewportToVectorTransform(),
    m_VectorToViewportTransform(),
    m_OGRDataSource(),
    m_CurrentLayer(""),
    m_InternalFeatures(),
    m_DisplayList(0),
    m_DisplayListNeedsRebuild(true),
    m_ExtentULX(0),
    m_ExtentULY(0),
    m_ExtentLRX(0),
    m_ExtentLRY(0),
    m_OptimizedRendering(true),
    m_OptimizedRenderingActive(false)
    
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

  // Free the display list
  if(m_DisplayList)
    {
    glDeleteLists(m_DisplayList,1);
    }

}

void GlVectorActor::SetFill(bool flag)
{
  if(m_Fill != flag)
    {
    m_DisplayListNeedsRebuild = true;
    m_Fill = flag;
    }
}

void GlVectorActor::SetOptimizedRendering(bool flag)
{
  if(m_OptimizedRendering != flag)
    {
    m_DisplayListNeedsRebuild = true;
    m_InternalFeatures.clear();
    m_OptimizedRendering = flag;
    }
}


void GlVectorActor::SetSolidBorder(bool flag)
{
  if(m_SolidBorder != flag)
    {
    m_DisplayListNeedsRebuild = true;
    m_SolidBorder = flag;
    }
}

void GlVectorActor::Initialize(const std::string & filename, const std::string & layername)
{
  // Open the data source
  m_OGRDataSource = otb::ogr::DataSource::New(filename,otb::ogr::DataSource::Modes::Read);

  if(m_OGRDataSource->GetLayersCount() == 0)
    {
    itkExceptionMacro(<<"No layers found in OGR dataset.");
    }

  // If no layer specified, get the first one
  if(layername == "")
    {
    m_CurrentLayer = m_OGRDataSource->GetLayer(0).GetName();
    }
  else
    {
    m_CurrentLayer = m_OGRDataSource->GetLayerChecked(layername).GetName();
    }
 
  UpdateTransforms();
}

std::vector<std::string> GlVectorActor::GetAvailableLayers() const
{
  std::vector<std::string> resp;

  for(unsigned int i = 0; i<m_OGRDataSource->GetLayersCount();++i)
    {
    resp.push_back(m_OGRDataSource->GetLayer(i).GetName());
    }
  return resp;
}

std::string GlVectorActor::GetCurrentLayer() const
{
  return m_CurrentLayer;
}

void GlVectorActor::SetCurrentLayer(const std::string & layername)
{
  m_CurrentLayer = m_OGRDataSource->GetLayerChecked(layername).GetName();
  
  // Clear transforms
  m_VectorToViewportTransform = NULL;
  m_ViewportToVectorTransform = NULL;

  // Clear features
  m_InternalFeatures.clear();

  // Force display list rebuild
  m_DisplayListNeedsRebuild = true;
}


void GlVectorActor::GetExtent(double & ulx, double & uly, double & lrx, double & lry) const
{
  PointType ul,lr,ur,ll, vpul,vplr, vpll, vpur;
  m_OGRDataSource->GetLayerChecked(m_CurrentLayer).GetExtent(ul[0],ul[1],lr[0],lr[1],true);
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

GlVectorActor::PointType GlVectorActor::ViewportToVectorTransform(const PointType & vpPoint)
{
  if(m_ViewportToVectorTransform.IsNull())
    {
    UpdateTransforms();
    }
  
  return m_ViewportToVectorTransform->TransformPoint(vpPoint);

}

std::string GlVectorActor::GetWkt() const
{
  if(m_OGRDataSource.IsNotNull())
    {
    return m_OGRDataSource->GetLayerChecked(m_CurrentLayer).GetProjectionRef();
    }

  return "";
}

void GlVectorActor::ProcessViewSettings()
{
  // Is there anything to do ?
  ViewSettings::ConstPointer settings = this->GetSettings();

  
  if((m_ViewportToVectorTransform.IsNull() || m_VectorToViewportTransform.IsNull()) || (settings->GetUseProjection() && settings->GetGeometryChanged()))
    {
    UpdateTransforms();
    InternalFeaturesTransform();
    UpdateDisplayList();
    }
}

void GlVectorActor::UpdateData()
{
  ViewSettings::ConstPointer settings = this->GetSettings();


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

  if(m_ExtentULX != ulx
     || m_ExtentULY != uly
     || m_ExtentLRX != lrx
     || m_ExtentLRY != lry
     || m_InternalFeatures.empty())
    {
  
    m_ExtentULX = ulx;
    m_ExtentULY = uly;
    m_ExtentLRX = lrx;
    m_ExtentLRY = lry;

    double areaOfScreenPixel = vcl_abs(lrx-ulx)*vcl_abs(lry-uly)
      /(settings->GetViewportSize()[0]*settings->GetViewportSize()[1]);

    OGRPolygon spatialFilter;
    OGRLinearRing spatialFilterRing;
    OGRPoint ul,ur,lr,ll;
    ul.setX(ulx);
    ul.setY(uly);
    ur.setX(lrx);
    ur.setY(uly);
    lr.setX(lrx);
    lr.setY(lry);
    ll.setX(ulx);
    ll.setY(lry);

    spatialFilterRing.addPoint(&ul);
    spatialFilterRing.addPoint(&ur);
    spatialFilterRing.addPoint(&lr);
    spatialFilterRing.addPoint(&ll);
    spatialFilterRing.closeRings();

    spatialFilter.addRing(&spatialFilterRing);
    
    otb::ogr::Layer filtered = m_OGRDataSource->GetLayerChecked(m_CurrentLayer);
    filtered.SetSpatialFilterRect(ulx,uly,lrx,lry);

   m_OptimizedRenderingActive = m_OptimizedRendering && filtered.GetFeatureCount(true)>(settings->GetViewportSize()[0]*settings->GetViewportSize()[1]/100);

    if(m_OptimizedRenderingActive)
      {
      std::ostringstream oss;
      oss<<"SELECT * FROM "<<m_CurrentLayer<<" WHERE OGR_GEOM_AREA>"<<100*areaOfScreenPixel;
      filtered = m_OGRDataSource->ExecuteSQL(oss.str(), &spatialFilter,NULL);
      }
   
    m_InternalFeatures.clear();
    
    otb::ogr::Layer::const_iterator featIt = filtered.begin();
    for(; featIt!=filtered.end(); ++featIt)
      {
      otb::ogr::Feature srcFeature(m_OGRDataSource->GetLayerChecked(m_CurrentLayer).GetLayerDefn());
      srcFeature.SetFrom( *featIt, TRUE );
      
      InternalFeature newInternalFeature(m_OGRDataSource->GetLayerChecked(m_CurrentLayer).GetLayerDefn());
      newInternalFeature.m_SourceFeature = srcFeature.Clone();
      if(m_OptimizedRenderingActive)
        {
        newInternalFeature.m_SourceFeature.SetGeometry(srcFeature.GetGeometry()->SimplifyPreserveTopology(vcl_sqrt(areaOfScreenPixel)));
        }
      m_InternalFeatures.push_back(newInternalFeature);
      }
    
    InternalFeaturesTransform();
    UpdateDisplayList();
    }
}


// Inner helper functions
namespace ice_internal
{ 
void GeometryRender(const OGRPoint * inPoint)
{
  
}

void GeometryRender(const OGRLineString * in)
{
  glBegin(GL_LINE_STRIP);
  
  for(unsigned int i = 0; i < in->getNumPoints();++i)
    {
    glVertex2d(in->getX(i),in->getY(i));
    }
  glEnd();
}

void GeometryRender(const OGRPolygon * in, GLUtesselator * tesselator, bool fill, bool solid)
{
   std::vector<GLdouble *> vertices;

      gluTessProperty(tesselator, GLU_TESS_BOUNDARY_ONLY, !fill);
      // Begin a new polygon
      gluTessBeginPolygon(tesselator, NULL);
      
      // Render the outer boundary
      gluTessBeginContour(tesselator);
      
      for(unsigned int i = 0; i < in->getExteriorRing()->getNumPoints();++i)
        {
        GLdouble * glp = new GLdouble[3];
        glp[0] = in->getExteriorRing()->getX(i);
        glp[1] = in->getExteriorRing()->getY(i);
        glp[2] = 0;
        vertices.push_back(glp);

        gluTessVertex(tesselator,glp,glp);
        }

      // End the outer boundary contour
      gluTessEndContour(tesselator);
      
      for(unsigned int j = 0; j < in->getNumInteriorRings();++j)
        {        
        gluTessBeginContour(tesselator);

        for(unsigned int i = 0; i < in->getInteriorRing(j)->getNumPoints();++i)
          {
          GLdouble * glp = new GLdouble[3];
          glp[0] = in->getInteriorRing(j)->getX(i);
          glp[1] = in->getInteriorRing(j)->getY(i);
          glp[2] = 0;
          vertices.push_back(glp);
          
          gluTessVertex(tesselator,glp,glp);
          }
        
        gluTessEndContour(tesselator);
        }

       // End the polygon
      gluTessEndPolygon(tesselator);

      if(solid)
        {
        glDisable(GL_BLEND);

        gluTessProperty(tesselator, GLU_TESS_BOUNDARY_ONLY, true);
        // Begin a new polygon
        gluTessBeginPolygon(tesselator, NULL);
        
        // Render the outer boundary
        gluTessBeginContour(tesselator);
        
        for(unsigned int i = 0; i < in->getExteriorRing()->getNumPoints();++i)
          {
          GLdouble * glp = new GLdouble[3];
          glp[0] = in->getExteriorRing()->getX(i);
          glp[1] = in->getExteriorRing()->getY(i);
          glp[2] = 0;
          vertices.push_back(glp);
          
          gluTessVertex(tesselator,glp,glp);
          }
        
        // End the outer boundary contour
        gluTessEndContour(tesselator);
      
        for(unsigned int j = 0; j < in->getNumInteriorRings();++j)
          {        
          gluTessBeginContour(tesselator);
          
          for(unsigned int i = 0; i < in->getInteriorRing(j)->getNumPoints();++i)
          {
          GLdouble * glp = new GLdouble[3];
          glp[0] = in->getInteriorRing(j)->getX(i);
          glp[1] = in->getInteriorRing(j)->getY(i);
          glp[2] = 0;
          vertices.push_back(glp);
          
          gluTessVertex(tesselator,glp,glp);
          }
          
          gluTessEndContour(tesselator);
          }
      // End the polygon
        gluTessEndPolygon(tesselator);
        
        glEnable(GL_BLEND);
        
        }
      
      // free vertices
      for(std::vector<GLdouble *>::iterator it = vertices.begin();it!=vertices.end();++it)
        {
        delete[] (*it);
        }
}

void GeometryRender(const OGRMultiPoint * in)
{
  for(unsigned int i = 0; i < in->getNumGeometries();++i)
    {
    GeometryRender(dynamic_cast<const OGRPoint *>(in->getGeometryRef(i)));   
    }
}

void GeometryRender(const OGRMultiLineString * in)
{
  for(unsigned int i = 0; i < in->getNumGeometries();++i)
    {
    GeometryRender(dynamic_cast<const OGRLineString *>(in->getGeometryRef(i)));
    }
}

void GeometryRender(const OGRMultiPolygon * in, GLUtesselator * tesselator, bool fill, bool solid)
{
  for(unsigned int i = 0; i < in->getNumGeometries();++i)
    {
    GeometryRender(dynamic_cast<const OGRPolygon *>(in->getGeometryRef(i)),tesselator,fill,solid);
    }
}
} // end namespace ice_internal

void GlVectorActor::UpdateDisplayList()
{
  // Build display list

  if(m_DisplayList == 0)
    {
    m_DisplayList = glGenLists (1);
    }
  
  glNewList(m_DisplayList, GL_COMPILE);

  for(std::vector<InternalFeature>::iterator it = m_InternalFeatures.begin();
      it!=m_InternalFeatures.end();++it)
    {
    const OGRGeometry * geom = it->m_SourceFeature.GetGeometry();
    const OGRPoint * inPoint = dynamic_cast<const OGRPoint *>(geom);
    const OGRLineString * inLineString = dynamic_cast<const OGRLineString *>(geom);
    const OGRPolygon * inPolygon = dynamic_cast<const OGRPolygon *>(geom);
    const OGRMultiPoint * inMPoints = dynamic_cast<const OGRMultiPoint *>(geom);
    const OGRMultiLineString * inMLineStrings = dynamic_cast<const OGRMultiLineString *>(geom);
    const OGRMultiPolygon * inMPolygons = dynamic_cast<const OGRMultiPolygon *>(geom);

    if(inPoint)
      {
      ice_internal::GeometryRender(const_cast<OGRPoint *>(inPoint));
      }
    else if(inLineString)
      {
      ice_internal::GeometryRender(inLineString);
      }
    else if(inPolygon)
      {
      ice_internal::GeometryRender(inPolygon,m_GluTesselator,m_Fill, m_SolidBorder);
      }
    else if(inMPoints)
      {
      ice_internal::GeometryRender(inMPoints);
      }
    else if(inMLineStrings)
      {
      ice_internal::GeometryRender(inMLineStrings);
      }
    else if(inMPolygons)
      {
      ice_internal::GeometryRender(inMPolygons,m_GluTesselator,m_Fill, m_SolidBorder);
      }
    }


  glEndList();
  
  m_DisplayListNeedsRebuild = false;
}

void GlVectorActor::Render()
{
  if(m_DisplayListNeedsRebuild)
    {
    UpdateDisplayList();
    }

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
   glEnable(GL_LINE_SMOOTH);
   glLineWidth(m_LineWidth);
   glColor4d(m_Color[0],m_Color[1],m_Color[2],m_Alpha);
   
   glCallList(m_DisplayList);

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
    m_ViewportToVectorTransform->SetOutputProjectionRef((m_OGRDataSource->GetLayerChecked(m_CurrentLayer).GetProjectionRef()));
    
    m_VectorToViewportTransform->SetOutputProjectionRef(settings->GetWkt());
    m_VectorToViewportTransform->SetOutputKeywordList(settings->GetKeywordList());
    m_VectorToViewportTransform->SetInputProjectionRef((m_OGRDataSource->GetLayerChecked(m_CurrentLayer).GetProjectionRef()));
    }
  m_ViewportToVectorTransform->InstanciateTransform();
  m_VectorToViewportTransform->InstanciateTransform();
}


// Inner helper functions
namespace ice_internal
{ 
OGRPoint GeometryTransform(const OGRPoint * inPoint, otb::GenericRSTransform<> * transform)
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

OGRPoint GeometryTransform(OGRPoint * inPoint,otb::GenericRSTransform<> * transform)
{
  return GeometryTransform(const_cast<const OGRPoint *>(inPoint),transform);
}

OGRLineString GeometryTransform(const OGRLineString * in,otb::GenericRSTransform<> * transform)
{
  OGRLineString outLineString;
  
  for(unsigned int i = 0; i<in->getNumPoints();++i)
    {
    OGRPoint p,op;
    in->getPoint(i,&p);
    op = GeometryTransform(&p,transform);
    outLineString.addPoint(&op);
    }
  return outLineString;
}

OGRPolygon GeometryTransform(const OGRPolygon * in,otb::GenericRSTransform<> * transform)
{
  OGRPolygon outPolygon;
  
  // First process external ring
  const OGRLinearRing * inExtRing = in->getExteriorRing();
  OGRLinearRing outExtRing;
  
  for(unsigned int i = 0; i<inExtRing->getNumPoints();++i)
    {
    OGRPoint p,op;
    inExtRing->getPoint(i,&p);
    op = GeometryTransform(&p,transform);
    outExtRing.addPoint(&op);
    }
  outPolygon.addRing(&outExtRing);
  
  // Then process any interior ring
  for(unsigned int j = 0; j<in->getNumInteriorRings();++j)
    {
    const OGRLinearRing * inIntRing = in->getInteriorRing(j);
    OGRLinearRing outIntRing;
    
    for(unsigned int i = 0; i<inIntRing->getNumPoints();++i)
      {
      OGRPoint p,op;
      inIntRing->getPoint(i,&p);
      op = GeometryTransform(&p,transform);
      outIntRing.addPoint(&op);
      }
    outPolygon.addRing(&outIntRing);
    }
  return outPolygon;
}

OGRMultiPoint GeometryTransform(const OGRMultiPoint * in, otb::GenericRSTransform<> * transform)
{
  OGRMultiPoint outMultiPoint;

  for(unsigned int i = 0; i < in->getNumGeometries();++i)
    {
    OGRPoint p = GeometryTransform(dynamic_cast<const OGRPoint *>(in->getGeometryRef(i)),transform);
    outMultiPoint.addGeometry(&p);
    }
  return outMultiPoint;
}


OGRMultiLineString GeometryTransform(const OGRMultiLineString * in, otb::GenericRSTransform<> * transform)
{
 OGRMultiLineString outMultiLineString;

  for(unsigned int i = 0; i < in->getNumGeometries();++i)
    {
    OGRLineString l = GeometryTransform(dynamic_cast<const OGRLineString *>(in->getGeometryRef(i)),transform);
    outMultiLineString.addGeometry(&l);
    }
  return outMultiLineString;
}

OGRMultiPolygon GeometryTransform(const OGRMultiPolygon * in, otb::GenericRSTransform<> * transform)
{
OGRMultiPolygon outMultiPolygon;

  for(unsigned int i = 0; i < in->getNumGeometries();++i)
    {
    OGRPolygon p = GeometryTransform(dynamic_cast<const OGRPolygon *>(in->getGeometryRef(i)),transform);
    outMultiPolygon.addGeometry(&p);
    }
  return outMultiPolygon;

}
} // end namespace ice_internal

void GlVectorActor::InternalFeaturesTransform()
{
  for(std::vector<InternalFeature>::iterator it = m_InternalFeatures.begin();
      it!=m_InternalFeatures.end();++it)
    {
    const OGRGeometry * geom = it->m_SourceFeature.GetGeometry();
    const OGRPoint * inPoint = dynamic_cast<const OGRPoint *>(geom);
    const OGRLineString * inLineString = dynamic_cast<const OGRLineString *>(geom);
    const OGRPolygon * inPolygon = dynamic_cast<const OGRPolygon *>(geom);
    const OGRMultiPoint * inMPoints = dynamic_cast<const OGRMultiPoint *>(geom);
    const OGRMultiLineString * inMLineStrings = dynamic_cast<const OGRMultiLineString *>(geom);
    const OGRMultiPolygon * inMPolygons = dynamic_cast<const OGRMultiPolygon *>(geom);

    if(inPoint)
      {
      OGRPoint outPoint = ice_internal::GeometryTransform(const_cast<OGRPoint *>(inPoint),m_VectorToViewportTransform);
      it->m_RenderedFeature.SetGeometry(&outPoint);
      }
    else if(inLineString)
      {
      OGRLineString outLineString = ice_internal::GeometryTransform(inLineString,m_VectorToViewportTransform);
      it->m_RenderedFeature.SetGeometry(&outLineString);
      }
    else if(inPolygon)
      {
      OGRPolygon outPolygon = ice_internal::GeometryTransform(inPolygon,m_VectorToViewportTransform);
      it->m_RenderedFeature.SetGeometry(&outPolygon);
      }
    else if(inMPoints)
      {
      OGRMultiPoint outMPoints = ice_internal::GeometryTransform(inMPoints,m_VectorToViewportTransform);
      it->m_RenderedFeature.SetGeometry(&outMPoints);
      }
    else if(inMLineStrings)
      {
      OGRMultiLineString outMLineStrings = ice_internal::GeometryTransform(inMLineStrings,m_VectorToViewportTransform);
      it->m_RenderedFeature.SetGeometry(&outMLineStrings);
      }
    else if(inMPolygons)
      {
      OGRMultiPolygon outMPolygons = ice_internal::GeometryTransform(inMPolygons,m_VectorToViewportTransform);
      it->m_RenderedFeature.SetGeometry(&outMPolygons);
      }
    }
}

}
