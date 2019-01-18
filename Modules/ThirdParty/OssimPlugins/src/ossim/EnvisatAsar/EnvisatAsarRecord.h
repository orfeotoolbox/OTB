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


#ifndef EnvisatAsarRecord_h
#define EnvisatAsarRecord_h

#include <cstdlib>
#include <cstring>
#include <ossim/base/ossimCommon.h>
#include <string>

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
  virtual EnvisatAsarRecord* Instantiate() =0;

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
