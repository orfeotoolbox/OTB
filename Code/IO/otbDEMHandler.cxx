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
#include "otbDEMHandler.h"
#include "otbMacro.h"

namespace otb
{


  DEMHandler
  ::DEMHandler()
  {
    m_ElevManager=ossimElevManager::instance();
    m_Mutex.Unlock();
  }


  DEMHandler
  ::~DEMHandler()
  {
    // not needed, m_ElevManager created with instance() method
    // delete m_ElevManager;
  }



  void
  DEMHandler
  ::OpenDEMDirectory(const char* DEMDirectory)
  {
    m_Mutex.Lock();
    ossimFilename ossimDEMDir;
    ossimDEMDir=ossimFilename(DEMDirectory);

    if (!m_ElevManager->openDirectory(ossimDEMDir))
      {
	m_Mutex.Unlock();
	itkExceptionMacro("Not possible to open DEM Directory");
      }
    m_Mutex.Unlock();
  }


  double 
  DEMHandler
  ::GetHeightAboveMSL(const PointType& geoPoint)
  {
    double height;
    ossimGpt ossimWorldPoint;
    ossimWorldPoint.lon=geoPoint[0];
    ossimWorldPoint.lat=geoPoint[1];
    m_Mutex.Lock();
    height=m_ElevManager->getHeightAboveMSL(ossimWorldPoint);
    m_Mutex.Unlock();
    return height;
  }


  void 
  DEMHandler
  ::PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
    os << indent << "DEMHandler" << std::endl;
  }

} // namespace otb
