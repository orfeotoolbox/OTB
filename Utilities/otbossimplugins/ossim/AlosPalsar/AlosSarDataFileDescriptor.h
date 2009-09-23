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

#ifndef AlosSarDataFileDescriptor_h
#define AlosSarDataFileDescriptor_h


#include<iostream>
#include <cstdlib>
#include <AlosSarRecordHeader.h>
#include <AlosSarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup SARLeaderAlosSarDataFileDescriptorRecord
 * @brief This class is able to read the SAR leader file descriptor record of the leader file
 */
class AlosSarDataFileDescriptor : public AlosSarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosSarDataFileDescriptor();
  /**
   * @brief Destructor
   */
  ~AlosSarDataFileDescriptor();

  /**
   * @brief This function write the AlosSarDataFileDescriptor in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosSarDataFileDescriptor& data);

  /**
   * @brief This function read a AlosSarDataFileDescriptor from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosSarDataFileDescriptor& data);

  /**
   * @brief Copy constructor
   */
  AlosSarDataFileDescriptor(const AlosSarDataFileDescriptor& rhs);

  /**
   * @brief Copy operator
   */
  AlosSarDataFileDescriptor& operator=(const AlosSarDataFileDescriptor& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosSarRecord* Instanciate()
  {
    return new AlosSarDataFileDescriptor();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosSarRecord* Clone()
  {
    return new AlosSarDataFileDescriptor(*this);
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


  int get_num_pix_in_line() const
  {
    return _num_pix_in_line;
  };

  int get_num_lines() const
  {
    return _num_lines;
  };

protected:
  /**
  * @brief num_pix_in_line
  */
  int   _num_pix_in_line;
  /**
  * @brief num_lines
  */
  int   _num_lines;


private:
};
}
#endif
