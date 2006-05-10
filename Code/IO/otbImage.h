/*=========================================================================

  Programme :   OTB (ORFEO ToolBox)
  Auteurs   :   CS - C.Ruffel
  Language  :   C++
  Date      :   21 avril 2006
  Version   :   
  Role      :   Création d'une otb::Image
  $Id$

=========================================================================*/
#ifndef __otbImage_h
#define __otbImage_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkImage.h"

#include "otbImageBase.h"

#include <iostream>
#include <string.h>

namespace otb
{
/** \class Image
 * \brief Creation of an "otb" image which contains metadata.
 *
 */
template <class TPixel, unsigned int VImageDimension=2> 
class ITK_EXPORT Image : public itk::Image<TPixel, VImageDimension>,
			 public otb::ImageBase
{
public: 

  typedef TPixel InputPixelType; 
  
  
  /** Standard class typedefs. */
  typedef Image   Self;
  typedef itk::Image<InputPixelType, VImageDimension>  Superclass;
  typedef itk::SmartPointer<Self>  Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  
  typedef ImageBase::VectorType	VectorType;	

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Image, itk::Image);

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
  Image();
  virtual ~Image(){};

private:
  Image(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};
  
  
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImage.txx"
#endif

#endif
