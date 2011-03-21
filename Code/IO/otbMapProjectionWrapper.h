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
#ifndef __otbMapProjectionWrapper_h
#define __otbMapProjectionWrapper_h

#include "itkObject.h"
#include "itkObjectFactory.h"

class ossimProjection;

namespace otb
{

/**
 * \class MapProjectionWrapper
 * \brief Wrapper class to group all dependencies to ossim for map projection
 *
 * This class is NOT intented to be used outside of OTB. Use the
 * GenericMapProjection.
 *
 * \sa GenericMapProjection
 * \ingroup Projection
 *
 **/

class MapProjectionWrapper: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef MapProjectionWrapper          Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef ossimProjection* InternalMapProjectionPointer;
  typedef const ossimProjection* InternalMapProjectionConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MapProjectionWrapper, itk::Object);

  InternalMapProjectionPointer GetMapProjection();
  InternalMapProjectionConstPointer GetMapProjection() const;

  std::string GetWkt();
  void SetWkt(std::string projectionRefWkt);
  bool InstanciateProjection();

  void InverseTransform(double x, double y, double z,
                        double& lon, double& lat, double& h);
  void ForwardTransform(double lon, double lat, double h,
                        double& x, double& y, double& z);

  void PrintMap() const;
protected:
  MapProjectionWrapper();
  virtual ~MapProjectionWrapper();


private:
  MapProjectionWrapper(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  InternalMapProjectionPointer m_MapProjection;
  std::string                  m_ProjectionRefWkt;

  bool reinstanciateProjection;
};
} // namespace otb

#endif
