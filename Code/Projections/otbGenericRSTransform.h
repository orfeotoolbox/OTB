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
#ifndef __otbGenericRSTransform_h
#define __otbGenericRSTransform_h

#include <iostream>
#include <sstream>
#include <stdio.h>
#include "projection/ossimProjection.h"
#include "itkTransform.h"
#include "itkExceptionObject.h"
#include "itkMacro.h"
// #include "base/ossimGpt.h"
// #include "base/ossimDpt.h"
// #include "projection/ossimProjection.h"
// #include "base/ossimEllipsoid.h"
// #include "base/ossimEllipsoidFactory.h"
// #include "base/ossimString.h"
// #include "ossimOgcWktTranslator.h"
#include "otbCompositeTransform.h"

namespace otb
{

/** \class GenericRSTransform
 *  \brief This is the class to handle generic remote sensing transform
 *
 * The transformation will transform the coordinate from any map projection or sensor
 * model to any map projection or sensor model.
 *
 *
 * \ingroup Projection
 *
 **/

template <class TScalarType = double,
unsigned int NInputDimensions=2,
unsigned int NOutputDimensions=2>
class ITK_EXPORT GenericRSTransform: public itk::Transform<TScalarType,       // Data type for scalars
      NInputDimensions,  // Number of dimensions in the input space
      NOutputDimensions> // Number of dimensions in the output space
{
public :
  /** Standard class typedefs. */
  typedef itk::Transform< TScalarType,
  NInputDimensions,
  NOutputDimensions >       Superclass;
  typedef GenericRSTransform                      Self;
  typedef itk::SmartPointer<Self>                   Pointer;
  typedef itk::SmartPointer<const Self>             ConstPointer;

  typedef typename Superclass::ScalarType           ScalarType;
  typedef ossimProjection                           OssimMapProjectionType;
  typedef itk::Point<ScalarType,NInputDimensions >  InputPointType;
  typedef itk::Point<ScalarType,NOutputDimensions > OutputPointType;

  typedef itk::Vector<double, 2> SpacingType;
  typedef itk::Point<double, 2> OriginType;

  typedef itk::Transform<double, 2, 2> GenericTransformType;
  typedef typename GenericTransformType::Pointer GenericTransformPointerType;
  typedef otb::CompositeTransform<GenericTransformType, GenericTransformType, double, 2, 2> TransformType;

  /** Method for creation through the object factory. */
  itkNewMacro( Self );

  /** Run-time type information (and related methods). */
  itkTypeMacro( GenericRSTransform, itk::Transform );

  typedef InverseOrForwardTransformationEnum DirectionOfMappingEnumType;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int,NInputDimensions*(NInputDimensions+1));

  virtual const TransformType* GetTransform() const;


  OutputPointType TransformPoint(const InputPointType &point) const;

  virtual bool InstanciateTransform();

protected:
  GenericRSTransform();
  virtual ~GenericRSTransform(){};

private :
  GenericRSTransform(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ImageKeywordlist m_InputKeywordList;
  ImageKeywordlist m_OutputKeywordList;
//   const itk::MetaDataDictionary & m_InputDictionary;
//   const itk::MetaDataDictionary & m_OutputDictionary;
  const itk::MetaDataDictionary m_InputDictionary;
  const itk::MetaDataDictionary m_OutputDictionary;
  std::string m_InputProjectionRef;
  std::string m_OutputProjectionRef;
  std::string m_DEMDirectory;

  SpacingType         m_InputSpacing;
  OriginType          m_InputOrigin;
  SpacingType         m_OutputSpacing;
  OriginType          m_OutputOrigin;


  TransformType* m_Transform;

  GenericTransformPointerType m_InputTransform;
  GenericTransformPointerType m_OutputTransform;
  bool reinstanciateTransform;
};



} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericRSTransform.txx"
#endif

#endif
