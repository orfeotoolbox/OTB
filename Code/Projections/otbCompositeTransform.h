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
#ifndef __otbCompositeTransform_h
#define __otbCompositeTransform_h

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "itkTransform.h"
#include "itkMacro.h"
#include "otbMapProjection.h"

namespace otb
{


/** \class CompositeTransform

 * \brief Class which compose two itk::Transform to obtain one itk::Transform
 * 			 T1					T2												 T3
 * (x,y) -> (x',y') -> (x'',y'')   =>    (x,y) -> (x'',y'') 
 * \ingroup Transform 
 */
template <class TFirstTransform,
					class TSecondTransform,
					class TScalarType=double, 
					unsigned int NInputDimensions=2,
					unsigned int NOutputDimensions=2>
class ITK_EXPORT CompositeTransform: public itk::Transform<TScalarType,       // Data type for scalars 
																									NInputDimensions,  // Number of dimensions in the input space
																									NOutputDimensions> // Number of dimensions in the output space
{
public :
  

	/** Standard class typedefs */
  typedef itk::Transform< TScalarType,
                  				NInputDimensions,
				                  NOutputDimensions >  				Superclass;
  typedef CompositeTransform                            				Self;
  typedef itk::SmartPointer<Self>              				Pointer;
  typedef itk::SmartPointer<const Self>        				ConstPointer;

  typedef TFirstTransform					         						FirstTransformType;
  typedef typename TFirstTransform::Pointer 					FirstTransformPointerType;
  typedef TSecondTransform					       						SecondTransformType;
	typedef typename TSecondTransform::Pointer       		SecondTransformPointerType;

  /** Standard vector type for this class. */
  typedef typename Superclass::InputVectorType  InputVectorType;
  typedef typename Superclass::OutputVectorType OutputVectorType;
  
  /** Standard covariant vector type for this class */
  typedef typename Superclass::InputCovariantVectorType InputCovariantVectorType;
  typedef typename Superclass::OutputCovariantVectorType OutputCovariantVectorType;
  
  /** Standard vnl_vector type for this class. */
  typedef typename Superclass::InputVnlVectorType InputVnlVectorType;
  typedef typename Superclass::OutputVnlVectorType OutputVnlVectorType;
  
  /** Standard coordinate point type for this class */
  typedef typename Superclass::InputPointType InputPointType;
  typedef typename Superclass::OutputPointType OutputPointType;

	/** Method for creation through the object factory. */
	itkNewMacro( Self );
	
	/** Run-time type information (and related methods). */
	itkTypeMacro( CompositeTransform, itk::Transform );
	
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int,NInputDimensions*(NInputDimensions+1));

	/** Set first transformation */  
	itkSetObjectMacro(FirstTransform,FirstTransformType); 
	
	/** Set second transformation */ 
	itkSetObjectMacro(SecondTransform,SecondTransformType);


  /**  Method to transform a point. */
  virtual OutputPointType TransformPoint(const InputPointType  & ) const;

  /**  Method to transform a vector. */
  virtual OutputVectorType TransformVector(const InputVectorType &) const;

  /**  Method to transform a vnl_vector. */
  virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &) const;

  /**  Method to transform a CovariantVector. */
  virtual OutputCovariantVectorType TransformCovariantVector(const InputCovariantVectorType &) const;

	/** Compute MapProjection1 coordinates to MapProjection2 coordinates. */ 
	OutputPointType ComputeProjection1ToProjection2(const InputPointType &point1);

	/** Compute MapProjection1 coordinates to MapProjection2 coordinates. */ 
	InputPointType ComputeProjection2ToProjection1(const OutputPointType &point2);

protected:
	CompositeTransform();
	~CompositeTransform();
	
	FirstTransformPointerType m_FirstTransform;
	SecondTransformPointerType m_SecondTransform;  
	
private:
  CompositeTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
	
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCompositeTransform.txx"
#endif

#endif
