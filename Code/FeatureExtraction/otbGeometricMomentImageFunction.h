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
#ifndef __otbGeometricMomentImageFunction_h
#define __otbGeometricMomentImageFunction_h

#include "itkImageFunction.h"


namespace otb
{

/**
 * \class GeometricMomentImageFunction
 * \brief Virtual class for the Geometric moments for an image function
 *
 * \ingroup ImageFunctions
 */

template < class TInput, 
           class TOutput = float,
	   class TPrecision = double,
           class TCoordRep = float >
class ITK_EXPORT GeometricMomentImageFunction :
  public itk::ImageFunction<TInput, TOutput,TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef GeometricMomentImageFunction                            Self;
  typedef itk::ImageFunction< TInput, TOutput,TCoordRep >         Superclass;
  typedef itk::SmartPointer<Self>                                 Pointer;
  typedef itk::SmartPointer<const Self>                           ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(GeometricMomentImageFunction, itk::ImageFunction);


  /** InputImageType typedef support. */
  typedef typename Superclass::InputType            InputType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::ContinuousIndexType  ContinuousIndexType;
  typedef typename Superclass::PointType            PointType;
 
  typedef TOutput                                   OutputType;
  /** Type for calculation precision */
  typedef TPrecision                                PrecisionType;

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkSetMacro( NeighborhoodRadius, int );
  itkGetConstReferenceMacro( NeighborhoodRadius, int );

protected:
  GeometricMomentImageFunction() 
  	{
	m_NeighborhoodRadius = -1;
	};
	
  ~GeometricMomentImageFunction(){};
  void PrintSelf(std::ostream& os, itk::Indent indent) const 
     {
      Superclass::PrintSelf( os, indent );
      os << indent << " m_NeighborhoodRadius: "  << m_NeighborhoodRadius << std::endl;     
     }
     
private:
  GeometricMomentImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& );               //purposely not implemented

  int m_NeighborhoodRadius;


};

} // namespace otb

#endif
