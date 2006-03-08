/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkFrustumFunctionControl.h,v $
  Language:  C++
  Date:      $Date: 2005/06/13 13:44:06 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkFrustumFunctionControl_h
#define __fltkFrustumFunctionControl_h

#include "fltkFrustumFunctionControlGUI.h"
#include "itkFrustumSpatialFunction.h"
#include "fltkFrustum3D.h"
#include "itkCommand.h"


namespace fltk {

template < class TFrustumSpatialFunction >
class FrustumFunctionControl : 
                    public itk::Object, 
                    public fltkFrustumFunctionControlGUI
{

public:

   /**
   * Standard "Self" typedef.
   */
  typedef FrustumFunctionControl         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(FrustumFunctionControl, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

 
  /**
   * Spatial Function Type
   */
  typedef TFrustumSpatialFunction                 SpatialFunctionType;
  typedef typename SpatialFunctionType::Pointer   SpatialFunctionPointer;
  

  /**
   * Set the Spatial Function Type
   */
  itkSetObjectMacro( SpatialFunction, SpatialFunctionType );


  /**
   * Shape type
   */
  typedef Frustum3D                      ShapeType;
  typedef ShapeType::Pointer             ShapePointer;
  
  
  /**
   *  Simple Command for drawing in OpenGL 
   */
  typedef itk::SimpleConstMemberCommand<Self>  DrawCommandType;
  typedef typename DrawCommandType::Pointer    DrawCommandPointer;
  itkGetObjectMacro( DrawCommand, DrawCommandType );
    

  /**
   * Get the Shape type
   */
  itkGetObjectMacro( Shape, ShapeType );


  /**
   * Set Angle Z
   */
  void SetAngleZ( double angle );

  /**
   * Set Aperture Angle X
   */
  void SetApertureAngleX( double angle );

  /**
   * Set Aperture Angle Y
   */
  void SetApertureAngleY( double angle );

  /**
   * Set Top Plane
   */
  void SetTopPlane( double distance );

  /**
   * Set Bottom Plane
   */
  void SetBottomPlane( double distance );

  /**
   * Set Apex
   */
  void SetApex(  double x, double y, double z );
  void SetApexX( double x );
  void SetApexY( double y );
  void SetApexZ( double z );

  /**
   * Set Parameters from GUI
   */
  void SetParametersFromGUI( void );

  /**
   * Draw the shape in OpenGL
   */
  void glDraw( void ) const;

  /** Rotate the frustum in the XZ or the YZ plane */
  typedef enum{ 
    RotateInXZPlane=1,
    RotateInYZPlane
  } FrustumRotationPlaneType;
  
  /** Set the plane in which the frustum should rotate. Default: XZ plane. */
  void SetRotationPlane( FrustumRotationPlaneType RotationPlane )
    {
    m_RotationPlane = RotateInYZPlane;
    if( RotationPlane == RotateInYZPlane )
      {
      m_SpatialFunction->SetRotationPlane( SpatialFunctionType::RotateInYZPlane );
      }
    }
  
protected:
  
  FrustumFunctionControl();
  virtual ~FrustumFunctionControl();


private:
  SpatialFunctionPointer                m_SpatialFunction;
  ShapePointer                          m_Shape;
  DrawCommandPointer                    m_DrawCommand;
  double                                m_AngleZ;
  FrustumRotationPlaneType              m_RotationPlane;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkFrustumFunctionControl.txx>
#endif




#endif
