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
#ifndef otbTileMapImageIOHelper_h
#define otbTileMapImageIOHelper_h


#include "itkObject.h"
#include "itkObjectFactory.h"

#include <string>
#include <iostream>
#include <sstream>

#include "OTBIOTileMapExport.h"

namespace otb
{

/** \class TileMapImageIOHelper
 *  \brief This class IO TileMapImage
 *
 *
 * \ingroup OTBIOTileMap
 */

class OTBIOTileMap_EXPORT TileMapImageIOHelper: public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef TileMapImageIOHelper           Self;
  typedef itk::SmartPointer<Self>        Pointer;
  typedef itk::SmartPointer<const Self>  ConstPointer;
  typedef itk::Object                    Superclass;

  itkTypeMacro(TileMapImageIOHelper, itk::Object);
  itkNewMacro(Self);

  /** Correspondence between depth and scale for the quad tree paradigm*/
  const std::string ConvertDepthToScale(const unsigned int depth) const;

protected:
  TileMapImageIOHelper() {}
  ~TileMapImageIOHelper() ITK_OVERRIDE {}

private:
  TileMapImageIOHelper(const Self &);  //purposely not implemented
  void operator =(const Self&);  //purposely not implemented


}; // end class TileMapImageIOHelper

} // end namespace otb

#endif
