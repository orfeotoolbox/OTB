/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: fltkVectorImage3D.h,v $
  Language:  C++
  Date:      $Date: 2002/01/22 04:27:48 $
  Version:   $Revision: 1.1 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef fltkVectorImage3D_h
#define fltkVectorImage3D_h

#include "fltkShape3D.h"
#include "itkImage.h"


namespace fltk {


/**
 *  \class VectorImage3D derives from Shape3D and 
 *         contains a pointer to an itk::VectorImage.
 */
template <class TVectorImage>
class ITK_EXPORT VectorImage3D : public Shape3D 
{
public:
  /**
   * Standard "Self" typedef.
   */
  typedef VectorImage3D<TVectorImage>   Self;

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
  itkTypeMacro( VectorImage3D, Shape3D );
  

  /** 
   * VectorImage Type
   */
   typedef           TVectorImage                    VectorImageType;
   typedef  typename VectorImageType::Pointer        VectorImagePointer;
   typedef  typename VectorImageType::ConstPointer   VectorImageConstPointer;
   typedef  typename VectorImageType::IndexType      IndexType;
   typedef  typename VectorImageType::RegionType     RegionType;
   typedef  typename VectorImageType::SizeType       SizeType;
   
   typedef  typename VectorImageType::PixelType      VectorType;


  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);
    

  /**
   * Draw the geometry of the shape
   */
  void DrawGeometry(void) const;



  /**
   * Accepts a Vector Image
   */
  void SetVectorImage( VectorImageType * );



  /**
   * Return a mesh
   */
  VectorImageConstPointer GetVectorImage( void ) const;


  typedef   double          VectorLengthScaleType;

  itkSetMacro( VectorLengthScale, VectorLengthScaleType );

  itkGetConstMacro( VectorLengthScale, VectorLengthScaleType );

protected:
  /**
   * Constructor
   */
  VectorImage3D();

  
private:

   /**
   * VectorImage that contains all the data
   */
   VectorImagePointer                  m_VectorImage;


   VectorLengthScaleType               m_VectorLengthScale;


};


} // end namespace fltk



#ifndef ITK_MANUAL_INSTANTIATION
#include <fltkVectorImage3D.txx>
#endif




#endif
