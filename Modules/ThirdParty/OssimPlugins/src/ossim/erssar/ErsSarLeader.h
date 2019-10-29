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


#ifndef ErsSarLeader_h
#define ErsSarLeader_h

#include "ErsSarFileDescriptor.h"
#include "ErsSarDataSetSummary.h"
#include "ErsSarMapProjectionData.h"
#include "ErsSarPlatformPositionData.h"
#include "ErsSarFacilityData.h"
#include <map>

class ossimKeywordlist;


namespace ossimplugins
{

class ErsSarPlatformPositionData;
class ErsSarMapProjectionData;
class ErsSarDataSetSummary;
class ErsSarFileDescriptor;
class ErsSarFacilityData;

/**
 * @ingroup ErsSarLeaderFile
 * @brief This class is able to read the Leader file of the ErsSar file structure
 *
 */
class ErsSarLeader
{
public:
  /**
   * @brief Constructor
   */
  ErsSarLeader();

  /**
   * @brief Destructor
   */
  virtual ~ErsSarLeader();

  /**
   * @brief This function write the ErsSarLeader in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ErsSarLeader& data);

  /**
   * @brief This function read a ErsSarLeader from a stream
   */
  friend std::istream& operator>>(std::istream& is, ErsSarLeader& data);

  /**
   * @brief Copy constructor
   */
  ErsSarLeader(const ErsSarLeader& rhs);

  /**
   * @brief Copy operator
   */
  ErsSarLeader& operator=(const ErsSarLeader& rhs);

  /**
   * @brief Remove all the previous records from the ErsSarLeader
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

  const ErsSarFacilityData * get_ErsSarFacilityData() const;
  const ErsSarPlatformPositionData * get_ErsSarPlatformPositionData() const;
  const ErsSarMapProjectionData * get_ErsSarMapProjectionData() const;
  const ErsSarDataSetSummary * get_ErsSarDataSetSummary() const;
  const ErsSarFileDescriptor * get_ErsSarFileDescriptor() const;

protected:
  // Made mutable because the get_* methods need to access it.
  // (modifying the definition of the map might be another solution)
  mutable std::map<int, ErsSarRecord*> theRecords;

  static const int ErsSarFacilityDataID;
  static const int ErsSarPlatformPositionDataID;
  static const int ErsSarMapProjectionDataID;
  static const int ErsSarDataSetSummaryID;
  static const int ErsSarFileDescriptorID;
private:

};
}

#endif
