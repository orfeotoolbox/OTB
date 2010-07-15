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
#ifndef __otbTileRecord_h
#define __otbTileRecord_h

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbImage.h"
#include "time.h"

namespace otb
{
/** \class TileRecord
 * \brief Manages Tiles from OSM.
 *
 */
class ITK_EXPORT TileRecord
{
public:
  /** Standard typedefs */
  typedef TileRecord Self;

  /** Constructor */
  TileRecord();
  ~TileRecord();

  /** Accessors */
  void SetFileName(std::string _arg);
  std::string GetFileName();

  void SetDate();
  time_t GetDate();

private:
  /** File name */
  std::string m_FileName;

  /** Date time */
  time_t m_Date;
};
}
#endif
