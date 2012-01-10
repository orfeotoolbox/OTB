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
#ifndef __otbPlatformPositionAdapter_h
#define __otbPlatformPositionAdapter_h

#include <vector>

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace ossimplugins
{
class ossimGeometricSarSensorModel;
}

namespace otb
{

class ImageKeywordlist;

/**
 * \class PlatformPositionAdapter
 * \brief Wrapper class to access the platform position from ossim
 *
 * This class is intended to be used in an InSAR setting to get
 * information relative to the baseline. If necessary, it could be
 * extended to non SAR models, but the optical ossim classes would
 * need to be modified.
 **/

class PlatformPositionAdapter: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef PlatformPositionAdapter       Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(PlatformPositionAdapter, itk::Object);

  void CreateSensorModel(const ImageKeywordlist& image_kwl);

  void GetPlatformPosition(
      double line, std::vector<double>& position, std::vector<double>& speed);

protected:
  PlatformPositionAdapter();
  virtual ~PlatformPositionAdapter();

private:
  PlatformPositionAdapter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  ossimplugins::ossimGeometricSarSensorModel* m_SensorModel;

};

} // namespace otb

#endif
