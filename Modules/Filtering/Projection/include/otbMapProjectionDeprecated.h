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
#ifndef __otbMapProjectionDeprecated_h
#define __otbMapProjectionDeprecated_h

#include "otbGenericMapProjection.h"


namespace otb
{

namespace MapProjectionType
{
enum MapProj
{
  ALBERS,
  AZIMEQUDIST,
  BNG,
  BONNE,
  CADRG,
  CASSINI,
  CYLEQUAREA,
  ECKERT6,
  GNOMONIC,
  LLXY,
  EQUDISTCYL,
  MERCATOR,
  MILLER,
  NEWZEALANDMAPGRID,
  OBLIQUEMERCATOR,
  ORTHOGRAPHIC,
  POLARSTEREO,
  POLYCONIC,
  SPACEOBLIQUEMERCATOR,
  STEREOGRAPHIC,
  TRANSCYLEQUAREA,
  UPS,
  VANDERGRINTEN,
};
}

/** \class MapProjectionDeprecated
 *  \brief The only purpose of this class is to maintain backward compatibility.
 *
 *  GenericMapProjection should be used instead.
 *
 *
 *
 * \ingroup OTBProjection
 */
template <MapProjectionType::MapProj TMapProj,
          TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT MapProjectionDeprecated : public GenericMapProjection<TTransform>
{
public:

  /** Standard class typedefs. */
  typedef MapProjectionDeprecated           Self;
  typedef GenericMapProjection<TTransform>  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MapProjectionDeprecated, GenericMapProjection);

protected:
  MapProjectionDeprecated();
  virtual ~MapProjectionDeprecated() {};

private:
  MapProjectionDeprecated(const Self &); //purposely not implemented
  void operator =(const Self&);   //purposely not implemented
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMapProjectionDeprecated.txx"
#endif

#endif
