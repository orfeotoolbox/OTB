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
#ifndef __otbLambert93Projection_h
#define __otbLambert93Projection_h

#include "otbLambertConformalConicMapProjection.h"

namespace otb
{
/** \class Lambert93Projection
* \brief This class implements the Lambert93 map projection (RGF93 french geodesic system).
* It converts coordinates in longitude, latitude (WGS84) to Lambert 93 map coordinates.
*
 *
 * \ingroup OTBProjection
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT Lambert93Projection : public LambertConformalConicMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef Lambert93Projection                            Self;
  typedef LambertConformalConicMapProjection<TTransform> Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Lambert93Projection, LambertConformalConicMapProjection);

protected:
  Lambert93Projection()
  {
    this->SetParameter("OriginX", "3");
    this->SetParameter("OriginY", "46.5");
    this->SetParameter("Datum", "WE");
    this->SetParameter("FalseNorthing", "6600000");
    this->SetParameter("FalseEasting", "700000");
    this->SetParameter("StandardParallel1", "44");
    this->SetParameter("StandardParallel2", "49");
  }

  virtual ~Lambert93Projection() {}

private:
  Lambert93Projection(const Self &); //purposely not implemented
  void operator =(const Self&);      //purposely not implemented
};

} // namespace otb

#endif
