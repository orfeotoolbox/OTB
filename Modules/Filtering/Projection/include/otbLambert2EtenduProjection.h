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
#ifndef otbLambert2EtenduProjection_h
#define otbLambert2EtenduProjection_h

#include "otbLambertConformalConicMapProjection.h"

namespace otb
{
/** \class Lambert2EtenduProjection
* \brief This class implements the Lambert 2 Etendu map projection.
* It converts coordinates in longitude, latitude (WGS84) to Lambert 2 Etendu map coordinates.
*
 *
 * \ingroup OTBProjection
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT Lambert2EtenduProjection : public LambertConformalConicMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef Lambert2EtenduProjection                       Self;
  typedef LambertConformalConicMapProjection<TTransform> Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Lambert2EtenduProjection, LambertConformalConicMapProjection);

protected:
  Lambert2EtenduProjection()
  {
    this->SetParameter("OriginX", "2.33722778");
    this->SetParameter("OriginY", "46.8");
    this->SetParameter("Datum", "NTF");
    this->SetParameter("FalseNorthing", "2200000");
    this->SetParameter("FalseEasting", "600000");
    this->SetParameter("StandardParallel1", "45.89891944");
    this->SetParameter("StandardParallel2", "47.69601389");
  }

  ~Lambert2EtenduProjection() ITK_OVERRIDE {}

private:
  Lambert2EtenduProjection(const Self &); //purposely not implemented
  void operator =(const Self&);           //purposely not implemented
};

} // namespace otb

#endif
