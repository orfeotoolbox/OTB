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
#ifndef __otbLambert3CartoSudProjection_h
#define __otbLambert3CartoSudProjection_h

#include "projection/ossimMapProjection.h"
#include "projection/ossimLambertConformalConicProjection.h"
#include "otbMapProjection.h"

namespace otb
{
/** \class Lambert3CartoSudProjection
* \brief This class implements the Lambert3CartoSud map projection (RGF93 french geodesic system).
* It converts coordinates in longitude, latitude (WGS84) to Lambert 3 map coordinates.
*
 */
template <Transform::TransformationDirection transform>
class ITK_EXPORT Lambert3CartoSudProjection : public LambertConformalConicMapProjection<transform>
{
public:
  /** Standard class typedefs. */
  typedef Lambert3CartoSudProjection                    Self;
  typedef LambertConformalConicMapProjection<transform> Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Lambert3CartoSudProjection, LambertConformalConicMapProjection);

protected:
  Lambert3CartoSudProjection();
  virtual ~Lambert3CartoSudProjection();

private:
  Lambert3CartoSudProjection(const Self &); //purposely not implemented
  void operator =(const Self&);                       //purposely not implemented
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLambert3CartoSudProjection.txx"
#endif

#endif
