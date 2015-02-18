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
#include "otbTransform.h"
#include "itkMacro.h"

namespace otb
{

/** \class CompositeTransform

* \brief Class which compose two itk::Transform to obtain one itk::Transform
*    T1    T2      T3
* (x, y) -> (x', y') -> (x'', y'')   =>    (x, y) -> (x'', y'')
*
 *
* \ingroup Transform
 *
 * \ingroup OTBTransform
*/

//typedef enum
//{
//  PROJDEFAULT=0,
//  PROJIDENTITY=1,
//  PROJMAPFORWARD=2,
//  PROJMAPINVERSE=3,
//  PROJSENSORFORWARD=4,
//  PROJSENSORINVERSE=5
//} ProjectionTypeEnum;

template <class TFirstTransform,
    class TSecondTransform,
    class TScalarType = typename TFirstTransform::ScalarType,
    unsigned int NInputDimensions = TFirstTransform::InputSpaceDimension,
    unsigned int NOutputDimensions = TSecondTransform::OutputSpaceDimension>
class ITK_EXPORT CompositeTransform : public Transform<TScalarType,  // Data type for scalars
      NInputDimensions,                                                       // Number of dimensions in the input space
      NOutputDimensions>                                                       // Number of dimensions in the output space
{
public:

  /** Standard class typedefs */
  typedef Transform<TScalarType,
      NInputDimensions,
      NOutputDimensions>         Superclass;
  typedef CompositeTransform            Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TFirstTransform                    FirstTransformType;
  typedef typename TFirstTransform::Pointer  FirstTransformPointerType;
  typedef TSecondTransform                   SecondTransformType;
  typedef typename TSecondTransform::Pointer SecondTransformPointerType;

  /** Standard vector type for this class. */
  //  typedef typename TFirstTransform::InputVectorType  FirstTransformInputVectorType;
  //  typedef typename TFirstTransform::OutputVectorType FirstTransformOutputVectorType;

  /** Standard covariant vector type for this class */
  //  typedef typename TFirstTransform::InputCovariantVectorType FirstTransformInputCovariantVectorType;
  //  typedef typename TFirstTransform::OutputCovariantVectorType FirstTransformOutputCovariantVectorType;

  /** Standard vnl_vector type for this class. */
  //  typedef typename TFirstTransform::InputVnlVectorType FirstTransformInputVnlVectorType;
  //  typedef typename TFirstTransform::OutputVnlVectorType FirstTransformOutputVnlVectorType;

  /** Standard coordinate point type for this class */
  typedef typename Superclass::InputPointType       FirstTransformInputPointType;
  typedef typename TFirstTransform::OutputPointType FirstTransformOutputPointType;
  /** TSecondTransform::InputPointType is purposely not defined
   *  It contrains user to choose First Transform Output compatible
   *  with Second Transform Input */
  typedef typename Superclass::OutputPointType SecondTransformOutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(CompositeTransform, Transform);

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension,      unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions * (NInputDimensions + 1));

  /** Set first transformation */
  itkSetObjectMacro(FirstTransform, FirstTransformType);

  /** Set second transformation */
  itkSetObjectMacro(SecondTransform, SecondTransformType);

/** Get First and second transform*/
  itkGetConstReferenceMacro(FirstTransform, FirstTransformPointerType);
  itkGetConstReferenceMacro(SecondTransform, SecondTransformPointerType);

  /**  Method to transform a point. */
  virtual SecondTransformOutputPointType TransformPoint(const FirstTransformInputPointType&) const;

  /**  Method to transform a vector. */
  //  virtual OutputVectorType TransformVector(const InputVectorType &) const;

  /**  Method to transform a vnl_vector. */
  //  virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &) const;

  /**  Method to transform a CovariantVector. */
  //  virtual OutputCovariantVectorType TransformCovariantVector(const InputCovariantVectorType &) const;

protected:
  CompositeTransform();
  virtual ~CompositeTransform();

  FirstTransformPointerType  m_FirstTransform;
  SecondTransformPointerType m_SecondTransform;

private:
  CompositeTransform(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCompositeTransform.txx"
#endif

#endif
