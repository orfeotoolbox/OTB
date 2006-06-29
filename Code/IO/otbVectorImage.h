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
#ifndef __otbVectorImage_h
#define __otbVectorImage_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkVectorImage.h"

#include "otbImageBase.h"
#include "itkDefaultVectorPixelAccessorFunctor.h"
#include "itkVectorImageNeighborhoodAccessorFunctor.h"

#include <iostream>
#include <string.h>

namespace otb
{
/** \class VectorImage
 * \brief Creation of an "otb" vector image which contains metadata.
 *
 */
template <class TPixel, unsigned int VImageDimension=2> 
class ITK_EXPORT VectorImage : public itk::VectorImage<TPixel, VImageDimension>,
			 public ImageBase
{
public: 

  typedef TPixel InputPixelType; 
  
  
  /** Standard class typedefs. */
  typedef VectorImage   Self;
  typedef itk::VectorImage<InputPixelType, VImageDimension>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  typedef ImageBase::VectorType	VectorType;	

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorImage, itk::VectorImage);

  /** Functor to provide a common API between DefaultPixelAccessor and
   * DefaultVectorPixelAccessor */
  typedef itk::DefaultVectorPixelAccessorFunctor< Self >       AccessorFunctorType;

  /** Tyepdef for the functor used to access a neighborhood of pixel pointers.*/
  typedef itk::VectorImageNeighborhoodAccessorFunctor< 
                          Self >              NeighborhoodAccessorFunctorType;

  /** Get the projection coordinate system of the image. */
  virtual std::string GetProjectionRef( void );
  
  /** Get the GCP projection coordinates of the image. */
  virtual std::string GetGCPProjection( void );
  
  virtual unsigned int GetGCPCount( void );
  
  virtual OTB_GCP & GetGCPs ( unsigned int GCPnum );
    
  virtual std::string GetGCPId( unsigned int GCPnum );   
  virtual std::string GetGCPInfo( unsigned int GCPnum );
  virtual double GetGCPRow( unsigned int GCPnum ); 
  virtual double GetGCPCol( unsigned int GCPnum ); 	
  virtual double GetGCPX( unsigned int GCPnum ); 
  virtual double GetGCPY( unsigned int GCPnum ); 
  virtual double GetGCPZ( unsigned int GCPnum );
  
  /** Get the six coefficients of affine geoTtransform. */
  virtual VectorType GetGeoTransform( void ); 
  
  /** Get image corners. */
  virtual VectorType GetUpperLeftCorner( void );
  virtual VectorType GetUpperRightCorner( void );
  virtual VectorType GetLowerLeftCorner( void );
  virtual VectorType GetLowerRightCorner( void );
  
  void PrintSelf(std::ostream& os, Indent indent) const;

protected:
  VectorImage();
  virtual ~VectorImage(){};

private:
  VectorImage(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};
  
  
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorImage.txx"
#endif

#endif
