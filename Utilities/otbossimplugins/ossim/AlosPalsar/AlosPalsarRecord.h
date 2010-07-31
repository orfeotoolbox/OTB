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

#ifndef AlosPalsarRecord_h
#define AlosPalsarRecord_h


#include <AlosPalsar/AlosPalsarRecordHeader.h>
#include <iostream>

namespace ossimplugins
{

/**
 * @ingroup AlosPalsar
 * @brief This class is the base class of all the record classes
 */
class AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   * @param mnemonic Name of the record
   */
  AlosPalsarRecord(std::string mnemonic);
  /**
   * @brief Destructor
   */
  virtual ~AlosPalsarRecord();

  /**
   * @brief Copy constructor
   */
  AlosPalsarRecord(const AlosPalsarRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  virtual AlosPalsarRecord* Instanciate() = 0;

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  virtual AlosPalsarRecord* Clone() = 0;

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
