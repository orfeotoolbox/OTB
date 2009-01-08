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
  * 	 T1    T2	    T3
  * (x,y) -> (x',y') -> (x'',y'')   =>    (x,y) -> (x'',y'')
  *
  * The class can also be used as a generic class templated on itk::Transform. In this case,
   * the user need to specify the transform type using methods SetFirstTransformProjectionType()
   * and SetSecondTransformProjectionType(). This information is passed using the ProjectionTypeEnum.
   *
   * According to this information a dynamic_cast will occur to call the desired transform method. By
   * default, no cast is performed.
   *
  * \ingroup Transform
  */

  typedef enum {
    PROJDEFAULT=0,
    PROJIDENTITY=1,
    PROJMAPFORWARD=2,
    PROJMAPINVERSE=3,
    PROJSENSORFORWARD=4,
    PROJSENSORINVERSE=5
    } ProjectionTypeEnum;

  template <class TFirstTransform,
            class TSecondTransform,
            class TScalarType=double,
            unsigned int NInputDimensions=2,
            unsigned int NOutputDimensions=2>
    class ITK_EXPORT CompositeTransform: public itk::Transform<TScalarType,  // Data type for scalars
                                                     NInputDimensions,       // Number of dimensions in the input space
                                                     NOutputDimensions>      // Number of dimensions in the output space
    {
      public :


      /** Standard class typedefs */
      typedef itk::Transform< TScalarType,
      NInputDimensions,
      NOutputDimensions >  			 Superclass;
      typedef CompositeTransform                 Self;
      typedef itk::SmartPointer<Self>            Pointer;
      typedef itk::SmartPointer<const Self>      ConstPointer;

      typedef TFirstTransform			 FirstTransformType;
      typedef typename TFirstTransform::Pointer  FirstTransformPointerType;
      typedef TSecondTransform			 SecondTransformType;
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
       *	with Second Transform Input */
      typedef typename Superclass::OutputPointType      SecondTransformOutputPointType;

      /** Method for creation through the object factory. */
      itkNewMacro( Self );

      /** Run-time type information (and related methods). */
      itkTypeMacro( CompositeTransform, itk::Transform );

      itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
      itkStaticConstMacro(OutputSpaceDimension,unsigned int, NOutputDimensions);
      itkStaticConstMacro(SpaceDimension,      unsigned int, NInputDimensions);
      itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions*(NInputDimensions+1));

      /** Set first transformation */
      itkSetObjectMacro(FirstTransform,FirstTransformType);

      /** Set second transformation */
      itkSetObjectMacro(SecondTransform,SecondTransformType);


      itkSetMacro(FirstTransformProjectionType,ProjectionTypeEnum);
      itkGetMacro(FirstTransformProjectionType,ProjectionTypeEnum);
      itkSetMacro(SecondTransformProjectionType,ProjectionTypeEnum);
      itkGetMacro(SecondTransformProjectionType,ProjectionTypeEnum);

      /**  Method to transform a point. */
      virtual SecondTransformOutputPointType TransformPoint(const FirstTransformInputPointType  & ) const;

      /**  Method to transform a vector. */
      //  virtual OutputVectorType TransformVector(const InputVectorType &) const;

      /**  Method to transform a vnl_vector. */
      //  virtual OutputVnlVectorType TransformVector(const InputVnlVectorType &) const;

      /**  Method to transform a CovariantVector. */
      //  virtual OutputCovariantVectorType TransformCovariantVector(const InputCovariantVectorType &) const;

      protected:
      CompositeTransform();
      ~CompositeTransform();

//       FirstTransformPointerType  m_FirstTransform;
//       SecondTransformPointerType m_SecondTransform;
      FirstTransformType*  m_FirstTransform;
      SecondTransformType* m_SecondTransform;

      ProjectionTypeEnum m_FirstTransformProjectionType;
      ProjectionTypeEnum m_SecondTransformProjectionType;

      private:
      CompositeTransform(const Self&); //purposely not implemented
      void operator=(const Self&); //purposely not implemented

    };

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbCompositeTransform.txx"
#endif

#endif
