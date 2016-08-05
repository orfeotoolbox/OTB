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
#include "otbTileMapImageIOHelper.h"
namespace otb
{
/** get scale from depth */
const std::string TileMapImageIOHelper::ConvertDepthToScale(const unsigned int depth) const
{
  std::string res;
  res += "level ";
  std::stringstream out;
  out << depth;
  res += out.str();
  res += " (around 1 : ";
  switch (depth)
    {
    case 1:
      res += "222 million)";
      break;
    case 2:
      res += "111 million)";
      break;
    case 3:
      res += "55 million)";
      break;
    case 4:
      res += "28 million";
      break;
    case 5:
      res += "14 million";
      break;
    case 6:
      res += "7 million)";
      break;
    case 7:
      res += "3 million)";
      break;
    case 8:
      res += "2 million)";
      break;
    case 9:
      res += "867, 000)";
      break;
    case 10:
      res += "433, 000)";
      break;
    case 11:
      res += "217, 000)";
      break;
    case 12:
      res += "108, 000)";
      break;
    case 13:
      res += "54, 000)";
      break;
    case 14:
      res += "27, 000)";
      break;
    case 15:
      res += "14, 000)";
      break;
    case 16:
      res += "6, 771)";
      break;
    case 17:
      res += "3, 385)";
      break;
    case 18:
      res += "1, 693)";
      break;
    default:
      res += "222 million)";
      break;
    }
  return res;
}

} // end namespace otb
