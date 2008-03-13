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
#ifndef _otbGluPolygonDrawingHelper_h
#define _otbGluPolygonDrawingHelper_h

#include <FL/gl.h>
#include <GL/glu.h>

#include "itkImageToImageFilter.h"
#include "itkObject.h"
#include "itkSmartPointer.h"
#include "itkMacro.h"

namespace otb
{
  /** \class GluPolygonDrawingHelper
   *  \brief This class is a short helper class to render complex polygons in openGl.
   *
   *  It uses the glu tesselator functionality.
   */
class ITK_EXPORT GluPolygonDrawingHelper
  : public itk::Object
  {
  public:

    typedef GluPolygonDrawingHelper Self;
    typedef itk::Object             Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;
    typedef itk::Point<double,3> PointType;
    typedef std::vector<PointType> PointVectorType;

     itkTypeMacro(GluPolygonDrawingHelper,Object);
    
    itkNewMacro(Self);
   

    /**
     * Set the current color.
     * \param r red component
     * \param g green component
     * \param b blue component
     * \param alpha component
     */
    void Color4d(double r, double g, double b, double alpha);

     /**
     * Set the current color.
     * \param r red component
     * \param g green component
     * \param b blue component
     */
    void Color3d(double r, double g, double b);

    /**
     * Add a new 3d vertex.
     * \param x position
     * \param y position
     * \param z position
     */
    void Vertex3d(double x, double y, double z);

    /**
     * Add a new 2d vertex.
     * \param x position
     * \param y position
     */
    void Vertex2d(double x,double y);

    /**
     * Set the the winding rule for the tesselator.
     * \param windingRule the rule.
     */
    void SetWindingRule(GLdouble windingRule);

    /**
     * Set the the winding rule for the tesselator.
     * \param boundaryOnly the flag.
     */
    void SetBoundaryOnly(GLdouble boundaryOnly);

    void RenderPolygon();

  protected:    
    
    /**
     * Constructor.
     */
    GluPolygonDrawingHelper();
    /**
     * Destructor.
     */
    ~GluPolygonDrawingHelper();

    /* Callback for the GLU_TESS_COMBINE
     * \param coords The new point coordinates
     * \param data data field
     * \param weights weight field
     * \param dataOut output data.
     */
    static void CombineCallback(GLdouble coords[3],GLdouble * data[4], GLfloat weights[4],GLdouble **dataOut);
    
    /** 
     * Callback for GLU_TESS_VERTEX
     * \param vertex Contains the new vertex data (i.e. 3 coordinates and 4 color values)
     */
    static void VertexCallback(GLvoid * vertex);
   
    /**
     * Callback for the GLU_TESS_ERROR.
     * Raises a generic ITK exception.
     * \param errorCode The gl error code.
     */
    static void ErrorCallback(GLenum errorCode);


    GluPolygonDrawingHelper(const Self&);// purposely not implemented
    void operator=(const Self&);// purposely not implemented
    

  private:
    /** The glu tesselator object */
    PointVectorType m_PointVector;
    GLUtesselator * m_GluTesselator;
    GLdouble m_Color[4];
  };
} // end namespace otb
#endif
