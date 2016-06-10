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
#ifndef __otbEllipsoidAdapter_h
#define __otbEllipsoidAdapter_h

#include "itkObject.h"
#include "itkObjectFactory.h"

class ossimEllipsoid;

namespace otb
{
/** \class EllipsoidAdapter
 *  \brief class providing the interface to the ossimEllipsoid
 *
 * This is a class to be used internally instead of introducing a dependence on
 * ossimEllipsoid
 *
 * \todo{Add the support for different ellipsoid models}
 *
 *
 * \ingroup OTBOSSIMAdapters
 **/

class EllipsoidAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef EllipsoidAdapter              Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(EllipsoidAdapter, itk::Object);

  /** Convert a XYZ coordinate into a lon, lat, heigh on the ellipsoid */
  void XYZToLonLatHeight(double x, double y, double z,
                         double& lon, double& lat, double& h) const;

  /** Convert a lon, lat, heigh on the ellipsoid into a XYZ geocentric system*/
  void LonLatHeightToXYZ(double lon, double lat, double h,
                         double& x, double& y, double& z) const;

protected:
  EllipsoidAdapter();
  ~EllipsoidAdapter() ITK_OVERRIDE;

private:
  EllipsoidAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  ossimEllipsoid* m_Ellipsoid;
};

} // namespace otb

#endif
