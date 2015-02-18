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
#ifndef __otbUtmMapProjection_h
#define __otbUtmMapProjection_h

#include "otbGenericMapProjection.h"

namespace otb
{
/** \class UtmMapProjection
 *  \brief This class implements the UTM map projection.
 *
 * It converts coordinates in longitude, latitude (WGS84) to UTM map coordinates.
 *
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT UtmMapProjection : public GenericMapProjection<TTransform>
{
public:

  /** Standard class typedefs. */
  typedef UtmMapProjection                  Self;
  typedef GenericMapProjection<TTransform>  Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(UtmMapProjection, GenericMapProjection);

  virtual void SetZone(long zone);
  virtual void SetHemisphere(char hemisphere);
  virtual int GetZone() const;
  virtual char GetHemisphere() const;

  virtual void SetZoneAndHemisphereFromGeoPoint(const InputPointType& geoPoint);

protected:
  UtmMapProjection();
  virtual ~UtmMapProjection() {};

private:
  UtmMapProjection(const Self &); //purposely not implemented
  void operator =(const Self&);   //purposely not implemented
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbUtmMapProjection.txx"
#endif

#endif
