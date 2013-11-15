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
#ifndef __otbGeocentricTransform_h
#define __otbGeocentricTransform_h

#include "otbTransform.h"
#include "otbGenericMapProjection.h"
#include "otbEllipsoidAdapter.h"

namespace otb
{
/** \class GeocentricTransform
 *
 * \brief Convert coordinates from geocentric (X, Y, Z) to geographic (lon, lat, h)
 *
 */
template <TransformDirection::TransformationDirection TDirectionOfMapping,
    class TScalarType = double,
    unsigned int NInputDimensions = 3,
    unsigned int NOutputDimensions = 3>
class ITK_EXPORT GeocentricTransform :
    public Transform<TScalarType,  // Data type for scalars
                     NInputDimensions,  // Number of dimensions in the input space
                     NOutputDimensions>  // Number of dimensions in the output space
{
public:
  /** Standard class typedefs. */
  typedef GeocentricTransform                                              Self;
  typedef Transform<TScalarType, NInputDimensions, NOutputDimensions>      Superclass;
  typedef itk::SmartPointer<Self>                                          Pointer;
  typedef itk::SmartPointer<const Self>                                    ConstPointer;

  typedef typename Superclass::ScalarType           ScalarType;
  typedef itk::Point<ScalarType, NInputDimensions>  InputPointType;
  typedef itk::Point<ScalarType, NOutputDimensions> OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeocentricTransform, Transform);

  static const TransformDirection::TransformationDirection DirectionOfMapping = TDirectionOfMapping;

  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(SpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(ParametersDimension, unsigned int, NInputDimensions * (NInputDimensions + 1));

  OutputPointType TransformPoint(const InputPointType& point) const;

protected:
  GeocentricTransform();
  virtual ~GeocentricTransform();
  EllipsoidAdapter::Pointer m_Ellipsoid;

private:
  GeocentricTransform(const Self &);        //purposely not implemented
  void operator =(const Self&);        //purposely not implemented

};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGeocentricTransform.txx"
#endif

#endif
