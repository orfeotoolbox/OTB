/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkSphereFunctionControl.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:25:08 $
  Version:   $Revision: 1.3 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __fltkSphereFunctionControl_h
#define __fltkSphereFunctionControl_h

#include "fltkSphereFunctionControlGUI.h"
#include "itkSphereSpatialFunction.h"
#include "fltkSphere3D.h"
#include "itkCommand.h"


namespace fltk {

template < class TSphereSpatialFunction >
class SphereFunctionControl : 
                    public itk::Object, 
                    public fltkSphereFunctionControlGUI
{

public:

   /**
   * Standard "Self" typedef.
   */
  typedef SphereFunctionControl         Self;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(SphereFunctionControl, Object);

  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);  

 
  /**
   * Spatial Function Type
   */
  typedef TSphereSpatialFunction                  SpatialFunctionType;
  typedef typename SpatialFunctionType::Pointer   SpatialFunctionPointer;
  

  /**
   * Set the Spatial Function Type
   */
  itkSetObjectMacro( SpatialFunction, SpatialFunctionType );


  /**
   * Shape type
   */
  typedef Sphere3D                      ShapeType;
  typedef ShapeType::Pointer            ShapePointer;
  
  
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
   * Set Radius
   */
  void SetRadius( double radius );

  /**
   * Set Center
   */
  void SetCenter(  double x, double y, double z );
  void SetCenterX( double x );
  void SetCenterY( double y );
  void SetCenterZ( double z );

  /**
   * Set Parameters from GUI
   */
  void SetParametersFromGUI( void );

  /**
   * Draw the shape in OpenGL
   */
  void glDraw( void ) const;


protected:
  
  SphereFunctionControl();
  virtual ~SphereFunctionControl();


private:
  SpatialFunctionPointer              m_SpatialFunction;
  ShapePointer                        m_Shape;
  DrawCommandPointer                  m_DrawCommand;

};


} // end namespace fltk

#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkSphereFunctionControl.txx>
#endif




#endif
