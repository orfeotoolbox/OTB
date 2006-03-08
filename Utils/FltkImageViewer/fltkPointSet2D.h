/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkPointSet2D.h,v $
  Language:  C++
  Date:      $Date: 2002/01/15 20:25:03 $
  Version:   $Revision: 1.2 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef fltkPointSet2D_h
#define fltkPointSet2D_h

#include "fltkShape3D.h"
#include "itkPointSet.h"


namespace fltk {


/**
 *  \class PointSet2D derives from Shape3D and 
 *         contains a pointer to an itk::PointSet.
 */
template <class TPointSet>
class ITK_EXPORT PointSet2D : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef PointSet2D<TPointSet>   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Shape3D  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<Self>  Pointer;

  /** 
   * Smart pointer typedef support.
   */
  typedef itk::SmartPointer<const Self>  ConstPointer;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro( PointSet2D, Shape3D );
  

  /** 
   * PointSet Type
   */
   typedef           TPointSet                    PointSetType;
   typedef  typename PointSetType::Pointer        PointSetPointer;
   typedef  typename PointSetType::ConstPointer   PointSetConstPointer;
   typedef  typename PointSetType::PointType      PointType;


  /** 
   * Points Container Type
   */
  typedef typename PointSetType::PointsContainer     PointsContainerType;
  typedef typename PointsContainerType::Pointer      PointsContainerPointer;

  /** 
   * Point Data Container Type
   */
  typedef typename PointSetType::PointDataContainer     PointDataContainerType;
  typedef typename PointDataContainerType::Pointer      PointDataContainerPointer;


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
    

  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;



  /**
   * Accepts a mesh
   */
  void SetPointSet( PointSetType * );



  /**
   * Return a mesh
   */
  PointSetConstPointer GetPointSet( void ) const;


protected:
  /**
   * Constructor
   */
  PointSet2D();

  
private:

   /**
   * PointSet that contains all the data
   */
   PointSetPointer                  m_PointSet;

};


} // end namespace fltk



#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkPointSet2D.txx>
#endif




#endif
