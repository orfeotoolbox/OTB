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

#ifndef AlosPalsarData_h
#define AlosPalsarData_h

#include <iostream>
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
