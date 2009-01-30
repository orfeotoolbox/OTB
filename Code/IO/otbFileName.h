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
#ifndef __otbFileName_h
#define __otbFileName_h

#include "itkDataObject.h"
#include "ossim/base/ossimFilename.h"


namespace otb
{
/** \class FileName
 *  \brief This class represents a file name
 *
 * It is derived from the ossimFilename class, which allows to manipulate a
 *  file name the appropriate methods to use it
 */

class ITK_EXPORT FileName : public ossimFilename
{
public:

  FileName();
  FileName(const FileName& src);
  FileName(const ossimFilename& src);
  FileName(const ossimString& src);
  FileName(const char* src);

  /**
   *  Return "t1" if file is "/data/images/t1.tif".
   */
  FileName ObtainFileNameWithNoExtension() const;

};

}

#endif

