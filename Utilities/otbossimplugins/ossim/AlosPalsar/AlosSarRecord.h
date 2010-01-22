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

#ifndef AlosSarRecord_h
#define AlosSarRecord_h


#include <AlosPalsar/AlosSarRecordHeader.h>
#include <iostream>

namespace ossimplugins
{

/**
 * @ingroup AlosSar
 * @brief This class is the base class of all the record classes
 */
class AlosSarRecord
{
public:
  /**
   * @brief Constructor
   * @param mnemonic Name of the record
   */
  AlosSarRecord(std::string mnemonic);
  /**
   * @brief Destructor
   */
  ~AlosSarRecord();

  /**
   * @brief Copy constructor
   */
  AlosSarRecord(const AlosSarRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  virtual AlosSarRecord* Instanciate() =0;

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  virtual AlosSarRecord* Clone()=0;

  /**
   * @brief Read the class data from a stream
   */
  virtual void Read(std::istream& is) =0;

  /**
   * @brief Write the class to a stream
   */
  virtual void Write(std::ostream& os)=0;

  std::string get_mnemonic()
  {
    return _mnemonic;
  };

protected:

  std::string _mnemonic;

private:
};
}
#endif
