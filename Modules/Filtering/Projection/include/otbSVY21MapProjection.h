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
#ifndef __otbSVY21MapProjection_h
#define __otbSVY21MapProjection_h

#include "otbTransMercatorMapProjection.h"

namespace otb
{
/** \class SVY21MapProjection
* \brief This class implements the SVY21 map projection (Singapore geodesic system).
* It converts coordinates in longitude, latitude (WGS84) to SVY21 map coordinates.
*
 */
template <TransformDirection::TransformationDirection TTransform>
class ITK_EXPORT SVY21MapProjection : public TransMercatorMapProjection<TTransform>
{
public:
  /** Standard class typedefs. */
  typedef SVY21MapProjection                     Self;
  typedef TransMercatorMapProjection<TTransform> Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;

  typedef typename Superclass::ScalarType ScalarType;
  typedef itk::Point<ScalarType, 2>       InputPointType;
  typedef itk::Point<ScalarType, 2>       OutputPointType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SVY21MapProjection, TransMercatorMapProjection);

protected:
  SVY21MapProjection()
  {
    this->SetParameter("OriginX", "103.83333333333333");
    this->SetParameter("OriginY", "1.3666666666666667");
    this->SetParameter("Datum", "WE");
    this->SetParameter("FalseNorthing", "38744.572");
    this->SetParameter("FalseEasting", "28001.642");
    this->SetParameter("ScaleFactor", "1.00");
  }
  virtual ~SVY21MapProjection() {}

private:
  SVY21MapProjection(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented
};

} // namespace otb

#endif
