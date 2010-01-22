//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
// "Copyright Centre for Remote Imaging, Sensing and Processing"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef AlosSarData_h
#define AlosSarData_h

#include <iostream>
#include <map>

class ossimKeywordlist;

namespace ossimplugins
{

class AlosSarRecord;
class AlosSarDataFileDescriptor;
class AlosSarSignalData;

/**
 * @ingroup AlosSarDataFile
 * @brief This class is able to read the Leader file of the AlosSar file structure
 */
class AlosSarData
{
public:
  /**
   * @brief Constructor
   */
  AlosSarData();

  /**
   * @brief Destructor
   */
  ~AlosSarData();

  /**
   * @brief This function write the AlosSarData in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosSarData& data);

  /**
   * @brief This function read a AlosSarData from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosSarData& data);

  /**
   * @brief Copy constructor
   */
  AlosSarData(const AlosSarData& rhs);

  /**
   * @brief Copy operator
   */
  AlosSarData& operator=(const AlosSarData& rhs);

  /**
   * @brief Remove all the previous records from the AlosSarData
   */
  void ClearRecords();


  /**
   * @brief Method to save object state to a keyword list.
   * @param kwl Keyword list to save to.
   * @param prefix added to keys when saved.
   * @return true on success, false on error.
   */
  virtual bool saveState(ossimKeywordlist& kwl,
                         const char* prefix=0) const;


  const AlosSarDataFileDescriptor * get_AlosSarDataFileDescriptor() const;
  const AlosSarSignalData * get_AlosSarSignalData() const;

protected:
  typedef std::map<int, AlosSarRecord*> RecordType;
  RecordType _records;


  static const int AlosSarDataFileDescriptorID;
  static const int AlosSarSignalDataID;
private:

};
}
#endif
