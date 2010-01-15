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

#ifndef dsd_h
#define dsd_h

#include <iostream>
#include <EnvisatAsar/EnvisatAsarRecord.h>

namespace ossimplugins
{
/**
 * @ingroup dsdRecord
 * @brief This class is able to read the ASAR dsd record
 */
class dsd : public EnvisatAsarRecord
{
public:
  /**
   * @brief Constructor
   */
  dsd();

  /**
   * @brief Destructor
   */
  virtual ~dsd();

  /**
   * @brief This function write the dsd in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const dsd& data);

  /**
   * @brief This function read a dsd from a stream
   */
  friend std::istream& operator>>(std::istream& is, dsd& data);

  /**
   * @brief Copy constructor
   */
  dsd(const dsd& rhs);

  /**
   * @brief Copy operator
   */
  dsd& operator=(const dsd& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  EnvisatAsarRecord* Instanciate()
  {
    return new dsd();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  EnvisatAsarRecord* Clone()
  {
    return new dsd(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is>>*this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os<<*this;
  };


  /**
  * @brief Data set Name
  */
  std::string   get_ds_name()
  {
    return _ds_name;
  }
  /**
  * @brief Data set Type
  */
  std::string   get_ds_type()
  {
    return _ds_type;
  }
  /**
  * @brief Data set Filename
  */
  std::string   get_filename()
  {
    return _filename;
  }
  /**
  * @brief Data set Offset
  */
  double   get_ds_offset()
  {
    return _ds_offset;
  }
  /**
  * @brief Data set Size
  */
  double   get_ds_size()
  {
    return _ds_size;
  }
  /**
  * @brief Number of data set record
  */
  int   get_num_dsr()
  {
    return _num_dsr;
  }
  /**
  * @brief Data set record size
  */
  int   get_dsr_size()
  {
    return _dsr_size;
  }

  /**
  * @brief Data set Name
  */
  std::string   _ds_name;
  /**
  * @brief Data set Type
  */
  std::string   _ds_type;
  /**
  * @brief Data set Filename
  */
  std::string   _filename;
  /**
  * @brief Data set Offset
  */
  double  _ds_offset;
  /**
  * @brief Data set Size
  */
  double  _ds_size;
  /**
  * @brief Number of data set record
  */
  int  _num_dsr;
  /**
  * @brief Data set record size
  */
  int  _dsr_size;

private:

};
}
#endif


