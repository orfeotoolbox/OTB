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

namespace itk
{
class MetaDataDictionary;
}

namespace otb
{

/** 
 * Reads no data flag from the MetaDataDictionnary dict to flags and values
 * vectors. Returns true upon success. 
 */
bool ReadNoDataFlags(const itk::MetaDataDictionary& dict, std::vector<bool> & flags, std::vector<double> & values);

/** 
 * Write no data flags to the MetaDataDictionnary dict from flags and values
 * vectors. Returns true upon success. 
 */
void WriteNoDataFlags(const std::vector<bool> & flags, const std::vector<double> & values, itk::MetaDataDictionary& dict);

/**
* Test if the pixel corresponds to a no data pixel according to a
* vector of no data flags, and a vector of no data values.
* \param pixel The pixel to test
* \param flags A vector of size > 1 containing a flag per band to
* indicate if a no data value is available for this band
* \param values A vector of size > 1 corresponding to the no data
* value for each band. If flag is 0, the value will be ignored.
* \param nanIsNoData If true, NaN values will be reported as no-data.
*/
template<typename T> bool IsNoData(const T & pixel, const
                                   std::vector<bool> & flags, const std::vector<double> & values, bool nanIsNoData = false) {
  assert(flags.size()>0);
  assert(values.size()>0);

  if(nanIsNoData && vnl_math_isnan(values[0]))
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
* Reads a pixel and change the no data value if it is found. No data
* value is changed either if the pixel value is NaN or if the pixel
* value equals the no data value and flag is true.
* 
* \param pixel The pixel to process \param flags A vector of size > 1
* containing a flag per band to indicate if a no data value is
* available for this band
* \param values A vector of size > 1 corresponding to the current no data
* value for each band. If flag is 0, the value will be ignored.
* \param newValues A vector of size > 1 corresponding to the new no data
* value for each band. If flag is 0, the value will be ignored 
* \param nanIsNoData If true, NaN values will be considered as no-data
* and changed as well.
*
*/
template<typename T> T ChangeNoData(const T & pixel, const
                                    std::vector<bool> & flags,
                                    const std::vector<double> & values,
                                    const std::vector<double> & newValues,
                                    bool nanIsNoData = false) {
  assert(flags.size()>0);
  assert(values.size()>0);
  assert(newValues.size()>0);
  
  if(nanIsNoData && vnl_math_isnan(pixel))
    {
    return static_cast<T>(newValues[0]);
    }

  if(flags[0] && pixel == values[0])
    {
    return static_cast<T>(newValues[0]);
    }
  return pixel;
}


/**
 * Specialization of IsNoData function to handle itk::VariableLengthVector
 */
template <typename T> bool IsNoData(const itk::VariableLengthVector<T> & pixel, const std::vector<bool> & flags, const std::vector<double> & values, bool nanIsNoData = false)
{
  assert(flags.size()>=pixel.Size());
  assert(values.size()>=pixel.Size());

  for(unsigned int i = 0; i < pixel.Size();++i)
    {
    if((nanIsNoData && vnl_math_isnan(pixel[i])) || (flags[i] && (pixel[i] == values[i])))
      {
      return true;
      }
    }
  return false;
}


/**
 * Specialization of ChangeNoData function to handle itk::VariableLengthVector
 */
template <typename T> itk::VariableLengthVector<T> ChangeNoData(const itk::VariableLengthVector<T> & pixel,
                                                                const std::vector<bool> & flags,
                                                                const std::vector<double> & values,
                                                                const std::vector<double> & newValues,
                                                                bool nanIsNoData = false)
{
  assert(flags.size()>=pixel.Size());
  assert(values.size()>=pixel.Size());
  assert(newValues.size()>=pixel.Size());

  itk::VariableLengthVector<T> outPixel(pixel.Size());
  
  for(unsigned int i = 0; i < pixel.Size();++i)
    {
    if((nanIsNoData && vnl_math_isnan(pixel[i])) || (flags[i] && (pixel[i] == values[i])))
      {
      outPixel[i] = newValues[i];
      }
    else
      {
      outPixel[i] = pixel[i];
      }
    }

  return outPixel;
}



} // End namespace otb

#endif
