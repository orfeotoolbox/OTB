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
#ifndef __otbDEMReader_cxx
#define __otbDEMReader_cxx

#include "otbDEMReader.h"
#include "otbMacro.h"

namespace otb
{



DEMReader
::DEMReader()
{
	m_ElevManager=ossimElevManager::instance();
}
  
	

DEMReader
::~DEMReader()
{
	// not needed, m_ElevManager created with instance() method
	// delete m_ElevManager;
}



bool 
DEMReader
::OpenDEMDirectory(const char* DEMDirectory)
{
	ossimFilename ossimDEMDir;
	ossimDEMDir=ossimFilename(DEMDirectory);
	bool result= false;
	if (m_ElevManager->openDirectory(ossimDEMDir))
	{
		result= true;
	}
	return result;
}


double 
DEMReader
::GetHeightAboveMSL(const PointType& geoPoint)
{
	float height;
	ossimGpt ossimWorldPoint;
	ossimWorldPoint.lat=geoPoint[0];
	ossimWorldPoint.lon=geoPoint[1];
	height=m_ElevManager->getHeightAboveMSL(ossimWorldPoint);
	return height;
}


void 
DEMReader
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
 
  os << indent << "DEMReader" << std::endl;
}

} // namespace otb

#endif
