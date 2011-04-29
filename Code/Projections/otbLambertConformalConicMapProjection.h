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
#ifndef __otbLambertConformalConicMapProjection_h
#define __otbLambertConformalConicMapProjection_h

#include "otbGenericMapProjection.h"

namespace otb
{
/** \class LambertConformalConicMapProjection
 * \brief This class implements the Lambert Conformal Conic map projection.
 * It converts coordinates in longitude, latitude (WGS84) to Lambert Conformal Conic map coordinates.
 *
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT LambertConformalConicMapProjection : public GenericMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef LambertConformalConicMapProjection  Self;
  typedef GenericMapProjection<TTransform>    Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LambertConformalConicMapProjection, GenericMapProjection);

  void   SetStandardParallel1(double degree);
  void   SetStandardParallel2(double degree);
  void   SetStandardParallels(double parallel1Degree, double parallel2Degree);
  void   SetFalseEasting(double falseEasting);
  void   SetFalseNorthing(double falseNorthing);
  double GetFalseNorthing() const;
  double GetFalseEasting() const;
  void  SetParameters(double parallel1Degree, double parallel2Degree,
                      double falseEasting, double falseNorthing);

protected:
  LambertConformalConicMapProjection();
  virtual ~LambertConformalConicMapProjection() {};

private:
  LambertConformalConicMapProjection(const Self &); //purposely not implemented
  void operator =(const Self&);                       //purposely not implemented
};

} // namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLambertConformalConicMapProjection.txx"
#endif

#endif
