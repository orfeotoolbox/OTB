//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

#ifndef ErsSarRecord_h
#define ErsSarRecord_h


#include "erssar/ErsSarRecordHeader.h"
#include <iostream>

namespace ossimplugins
{

/**
 * @ingroup ErsSar
 * @brief This class is the base class of all the record classes
 *
 */
class ErsSarRecord
{
public:
  /**
   * @brief Constructor
   * @param mnemonic Name of the record
   */
  ErsSarRecord(std::string mnemonic);
  /**
   * @brief Destructor
   */
  virtual ~ErsSarRecord();

  /**
   * @brief Copy constructor
   */
  ErsSarRecord(const ErsSarRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  virtual ErsSarRecord* Instanciate() = 0;

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  virtual ErsSarRecord* Clone() = 0;

  /**
   * @brief Read the class data from a stream
   */
  virtual void Read(std::istream& is) = 0;

  /**
   * @brief Write the class to a stream
   */
  virtual void Write(std::ostream& os) = 0;

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
