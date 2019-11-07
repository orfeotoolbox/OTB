/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2008-2010 by Centre for Remote Imaging, Sensing and Processing (CRISP)
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


#ifndef AlosPalsarLeader_h
#define AlosPalsarLeader_h

#include "AlosPalsar/AlosPalsarFileDescriptor.h"
#include "AlosPalsar/AlosPalsarDataSetSummary.h"
#include "AlosPalsar/AlosPalsarMapProjectionData.h"
#include "AlosPalsar/AlosPalsarPlatformPositionData.h"
#include "AlosPalsar/AlosPalsarRadiometricData.h"
#include "AlosPalsar/AlosPalsarFacilityData.h"
#include <map>

class ossimKeywordlist;

namespace ossimplugins
{

class AlosPalsarPlatformPositionData;
class AlosPalsarMapProjectionData;
class AlosPalsarDataSetSummary;
class AlosPalsarFileDescriptor;
class AlosPalsarFacilityData;
class AlosPalsarRadiometricData;

/**
 * @ingroup AlosPalsarLeaderFile
 * @brief This class is able to read the Leader file of the AlosPalsar file structure
 */
class AlosPalsarLeader
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarLeader();

  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarLeader();

  /**
   * @brief This function write the AlosPalsarLeader in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarLeader& data);

  /**
   * @brief This function read a AlosPalsarLeader from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarLeader& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarLeader(const AlosPalsarLeader& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarLeader& operator=(const AlosPalsarLeader& rhs);

  /**
   * @brief Remove all the previous records from the AlosPalsarLeader
   */
  void ClearRecords();


  /**
   * @brief Method to save object state to a keyword list.
   * @param kwl Keyword list to save to.
   * @param prefix added to keys when saved.
   * @return true on success, false on error.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix = 0) const;


  const AlosPalsarFacilityData * get_AlosPalsarFacilityData() const;
  const AlosPalsarRadiometricData * get_AlosPalsarRadiometricData() const;
  const AlosPalsarPlatformPositionData * get_AlosPalsarPlatformPositionData() const;
//  const AlosPalsarMapProjectionData * get_AlosPalsarMapProjectionData() const;
  const AlosPalsarDataSetSummary * get_AlosPalsarDataSetSummary() const;
  const AlosPalsarFileDescriptor * get_AlosPalsarFileDescriptor() const;

protected:
  typedef std::map<int, AlosPalsarRecord*> RecordType;
  RecordType _records;

  static const int AlosPalsarFacilityDataID;
  static const int AlosPalsarRadiometricDataID;
  static const int AlosPalsarPlatformPositionDataID;
//  static const int AlosPalsarMapProjectionDataID;
  static const int AlosPalsarDataSetSummaryID;
  static const int AlosPalsarFileDescriptorID;
private:

};
}
#endif
