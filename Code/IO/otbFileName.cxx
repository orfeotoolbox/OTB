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

#include "otbFileName.h"

namespace otb
{

FileName::FileName() : ossimFilename()
{}

FileName::FileName(const FileName& src)
 : ossimFilename(src)
{}

FileName::FileName(const ossimFilename& src)
 : ossimFilename(src)
{}

FileName::FileName(const ossimString& src)
 : ossimFilename(src)
{}

FileName::FileName(const char* src)
 : ossimFilename(src)
{}


FileName FileName::ObtainFileNameWithNoExtension() const
{
	return this->fileNoExtension();
}

} // end namespace otb
