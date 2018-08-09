/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbTileMapImageIOHelper.h"
namespace otb
{
/** get scale from depth */
OTBIOTileMap_DEPRECATED const std::string TileMapImageIOHelper::ConvertDepthToScale(const unsigned int depth) const
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
