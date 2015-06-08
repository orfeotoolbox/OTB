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
#ifndef __otbNoDataHelper_h
#define __otbNoDataHelper_h
#include <vector>
#include <cassert>
#include "vnl/vnl_math.h"
#include <itkVariableLengthVector.h>

namespace otb
{
/**
* Test if the pixel corresponds to a no data pixel according to a
* vector of no data flags, and a vector of no data values.
* \param pixel The pixel to test
* \param flags A vector of size > 1 containing a flag per band to
* indicate if a no data value is available for this band
* \param values A vector of size > 1 corresponding to the no data
* value for each band. If flag is 0, the value will be ignored.
*/
template<typename T> bool IsNoData(const T & pixel, const
std::vector<bool> & flags, const std::vector<double> & values) {
  assert(flags.size()>0);
  assert(values.size()>0);

  if(vnl_math_isnan((int)flags[0]))
    return true;


  if(flags[0])
    {
    return (pixel == values[0]);
    }
  else
    {
    return false;
    }
}
/**
 * Specialization of IsNoData function to handle itk::VariableLengthVector
 */
template <typename T> bool IsNoData(const itk::VariableLengthVector<T> & pixel, const std::vector<bool> & flags, const std::vector<double> & values)
{
  assert(flags.size()>=pixel.Size());
  assert(values.size()>=pixel.Size());

  for(unsigned int i = 0; i < pixel.Size();++i)
    {
    if(vnl_math_isnan(pixel[i]) || (flags[i] && (pixel[i] == values[i])))
      {
      return true;
      }
    }
  return false;
}


} // End namespace otb

#endif
