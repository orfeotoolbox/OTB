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
#ifndef otbLambert3CartoSudProjection_h
#define otbLambert3CartoSudProjection_h

#include "otbLambertConformalConicMapProjection.h"

namespace otb
{
/** \class Lambert3CartoSudProjection
* \brief This class implements the Lambert3CartoSud map projection (RGF93 french geodesic system).
* It converts coordinates in longitude, latitude (WGS84) to Lambert 3 map coordinates.
*
 *
 * \ingroup OTBProjection
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT Lambert3CartoSudProjection : public LambertConformalConicMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef Lambert3CartoSudProjection                     Self;
  typedef LambertConformalConicMapProjection<TTransform> Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Lambert3CartoSudProjection, LambertConformalConicMapProjection);

protected:
  Lambert3CartoSudProjection()
  {
    this->SetParameter("OriginX", "3");
    this->SetParameter("OriginY", "46.5");
    this->SetParameter("Datum", "WE");
    this->SetParameter("FalseNorthing", "3200000");
    this->SetParameter("FalseEasting", "600000");
    this->SetParameter("StandardParallel1", "43.1992913888888888888888888889");
    this->SetParameter("StandardParallel2", "44.9960938888888888888888888888");
  }

  virtual ~Lambert3CartoSudProjection() {}

private:
  Lambert3CartoSudProjection(const Self &); //purposely not implemented
  void operator =(const Self&);                       //purposely not implemented
};

} // namespace otb

#endif
