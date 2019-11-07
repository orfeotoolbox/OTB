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


#ifndef AlosPalsarData_h
#define AlosPalsarData_h

#include <map>

class ossimKeywordlist;

namespace ossimplugins
{

class AlosPalsarRecord;
class AlosPalsarDataFileDescriptor;
class AlosPalsarSignalData;

/**
 * @ingroup AlosPalsarDataFile
 * @brief This class is able to read the Leader file of the AlosPalsar file structure
 */
class AlosPalsarData
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarData();

  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarData();

  /**
   * @brief This function write the AlosPalsarData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarData& data);

  /**
   * @brief This function read a AlosPalsarData from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarData& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarData(const AlosPalsarData& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarData& operator=(const AlosPalsarData& rhs);

  /**
   * @brief Remove all the previous records from the AlosPalsarData
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


  const AlosPalsarDataFileDescriptor * get_AlosPalsarDataFileDescriptor() const;
  const AlosPalsarSignalData * get_AlosPalsarSignalData() const;

protected:
  typedef std::map<int, AlosPalsarRecord*> RecordType;
  RecordType _records;


  static const int AlosPalsarDataFileDescriptorID;
  static const int AlosPalsarSignalDataID;
private:

};
}
#endif
