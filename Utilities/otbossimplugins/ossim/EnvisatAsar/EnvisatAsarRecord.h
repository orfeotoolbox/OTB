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

#ifndef EnvisatAsarRecord_h
#define EnvisatAsarRecord_h

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ossim/base/ossimCommon.h>

namespace ossimplugins
{
/**
 * @ingroup EnvisatAsar
 * @brief This class is the base class of all the Envisat ASAR record classes
 */




class EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   * @param mnemonic Name of the record
   */
  EnvisatAsarRecord(std::string mnemonic);
  /**
   * @brief Destructor
   */
  virtual ~EnvisatAsarRecord();

  /**
   * @brief Copy constructor
   */
  EnvisatAsarRecord(const EnvisatAsarRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  virtual EnvisatAsarRecord* Instanciate() =0;

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  virtual EnvisatAsarRecord* Clone()=0;

  /**
   * @brief Reads the class data from a stream
   */
  virtual void Read(std::istream& is) =0;

  /**
   * @brief Writes the class to a stream
   */
  virtual void Write(std::ostream& os)=0;

  /**
   * @brief Returns the mnemonic of the record
   */
  std::string get_mnemonic()
  {
    return _mnemonic;
  };



protected:
  /**
   * @brief This function switches the LSB value and the MSB value of the parameter
   */
  template<typename T>
  void SwitchEndian(T &value) {

    char *buffer;
    char *res;
    int nb_octets = sizeof(T);

    if (ossim::byteOrder() == OSSIM_LITTLE_ENDIAN) {
      buffer = (char* )malloc(nb_octets);
      res = (char* )malloc(nb_octets);

      memcpy(buffer,&value,nb_octets);

      for (int i=0; i<nb_octets/2; i++) {
        res[i] = buffer[nb_octets-1-i];
        res[nb_octets-1-i] = buffer[i];
      }
      memcpy(&value,res,nb_octets);

      free(buffer);
      free(res);
    }
  }


  std::string _mnemonic;

private:
};
}
#endif
