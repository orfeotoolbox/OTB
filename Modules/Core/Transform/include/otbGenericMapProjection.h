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
#ifndef __otbGenericMapProjection_h
#define __otbGenericMapProjection_h

#include <iostream>
#include <sstream>

#include "otbTransform.h"
#include "itkMacro.h"
#include "otbMapProjectionAdapter.h"

namespace otb
{

namespace TransformDirection
{
enum TransformationDirection
  {
  FORWARD = 0,
  INVERSE = 1
  };
}

/** \class GenericMapProjection
 *  \brief This is the base class for generic map projection transformation
 *
 * The forward transformation will transform the coordinate for the geographic to the
 * cartographic. The inverse model will transform from the cartographic to the geographic.
 *
 * All derived class assume that the latitude and longitude are given according to the
 * WGS84 ellipsoid model.
 *
 * \ingroup Projection
 *
 *
 * \ingroup OTBTransform
 **/

template <TransformDirection::TransformationDirection TDirectionOfMapping,
    class TScalarType = double,
    unsigned int NInputDimensions = 2,
    unsigned int NOutputDimensions = 2>
class ITK_EXPORT GenericMapProjection : public Transform<TScalarType,       // Data type for scalars
      NInputDimensions,                                                         // Number of dimensions in the input space
      NOutputDimensions>                                                         // Number of dimensions in the output space
{
public:
  /** Standard class typedefs. */
  typedef Transform<TScalarType,
      NInputDimensions,
      NOutputDimensions>                Superclass;
  typedef GenericMapProjection          Self;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef typename Superclass::ScalarType           ScalarType;
  typedef MapProjectionAdapter                      MapProjectionType;
  typedef itk::Point<ScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<ScalarType, NOutputDimensions> OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GenericMapProjection, Transform);

  static const TransformDirection::TransformationDirection DirectionOfMapping = TDirectionOfMapping;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions * (NInputDimensions + 1));

  /** Return the Wkt representation of the projection*/
  virtual std::string GetWkt();

  /** Instanciate the projection according to the Wkt specification*/
  virtual void SetWkt(const std::string& projectionRefWkt);

  virtual void PrintMap() const;

  OutputPointType TransformPoint(const InputPointType& point) const;

  virtual bool InstanciateProjection();

  const MapProjectionAdapter* GetMapProjection() const;

  virtual bool IsProjectionDefined() const;

  void SetParameter(const std::string& key, const std::string& value);
  std::string GetParameter(const std::string& key) const;

protected:
  GenericMapProjection();
  virtual ~GenericMapProjection();

  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  MapProjectionAdapter::Pointer m_MapProjection;

private:
  GenericMapProjection(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGenericMapProjection.txx"
#endif

#endif
