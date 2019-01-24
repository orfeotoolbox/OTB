/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef EnvisatAsarData_h
#define EnvisatAsarData_h

#include "sph.h"
#include "mph.h"
#include "SQ_ADSR.h"
#include "MainProcessingParameters.h"
#include "AntennaElevationPatterns.h"
#include "DopplerCentroidParameters.h"
#include "SRGRConversionParameters.h"
#include "ChirpParameters.h"
#include "GeolocationGrid.h"
#include <map>
#include <list>
#include <vector>

namespace ossimplugins
{
/**
 * @ingroup EnvisatAsarDataFile
 * @brief This class is able to read the Envisat file structure
 */
class EnvisatAsarData
{
public:
  /**
   * @brief Constructor
   */
  EnvisatAsarData();

  /**
   * @brief Destructor
   */
  ~EnvisatAsarData();

  /**
   * @brief This function writes the EnvisatAsarData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const EnvisatAsarData& data);

  /**
   * @brief This function reads a EnvisatAsarData from a stream
   */
  friend std::istream& operator>>(std::istream& is, EnvisatAsarData& data);

  /**
   * @brief Copy constructor
   */
  EnvisatAsarData(const EnvisatAsarData& rhs);

  /**
   * @brief Copy operator
   */
  EnvisatAsarData& operator=(const EnvisatAsarData& rhs);

  /**
   * @brief Removes all the previous records from the EnvisatAsarData
   */
  void ClearRecords();

  mph * get_mph();
  sph * get_sph();
  SQ_ADSR * get_SQ_ADSR();
  MainProcessingParameters * get_MainProcessingParameters();
  AntennaElevationPatterns * get_AntennaElevationPatterns();
  DopplerCentroidParameters * get_DopplerCentroidParameters();
  SRGRConversionParameters * get_SRGRConversionParameters(int pos = 0);
  ChirpParameters * get_ChirpParameters();
  GeolocationGrid * get_GeolocationGrid(int num_ds = 0);

  int get_num_ds(EnvisatAsarRecord * record);

protected:
  std::list<EnvisatAsarRecord*> _records;

private:

};
}
#endif
