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

#include "otbShapeFileData.h"



namespace otb
{



ShapeFileData::ShapeFileData()
{
	m_OGRDataSource = NULL;
}

ShapeFileData::~ShapeFileData()
{
	if (m_OGRDataSource != NULL)
		OGRDataSource::DestroyDataSource(m_OGRDataSource);
}



void 
ShapeFileData::PrintSelf(std::ostream& os, itk::Indent indent) const
{
	Superclass::PrintSelf(os, indent);
  os << indent << "ShapeFileData" << std::endl;
}


} // end namespace otb
