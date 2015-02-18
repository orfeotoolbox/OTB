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

#ifndef AlosPalsarDataFileDescriptor_h
#define AlosPalsarDataFileDescriptor_h


#include<iostream>
#include <cstdlib>
#include <AlosPalsar/AlosPalsarRecordHeader.h>
#include <AlosPalsar/AlosPalsarRecord.h>

namespace ossimplugins
{

/**
 * @ingroup SARLeaderAlosPalsarDataFileDescriptorRecord
 * @brief This class is able to read the SAR leader file descriptor record of the leader file
 */
class AlosPalsarDataFileDescriptor : public AlosPalsarRecord
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarDataFileDescriptor();
  /**
   * @brief Destructor
   */
  ~AlosPalsarDataFileDescriptor();

  /**
   * @brief This function write the AlosPalsarDataFileDescriptor in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarDataFileDescriptor& data);

  /**
   * @brief This function read a AlosPalsarDataFileDescriptor from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarDataFileDescriptor& data);

  /**
   * @brief Copy constructor
   */
  AlosPalsarDataFileDescriptor(const AlosPalsarDataFileDescriptor& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarDataFileDescriptor& operator=(const AlosPalsarDataFileDescriptor& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  AlosPalsarRecord* Instanciate()
  {
    return new AlosPalsarDataFileDescriptor();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  AlosPalsarRecord* Clone()
  {
    return new AlosPalsarDataFileDescriptor(*this);
  };

  /**
   * @brief Read the class data from a stream
   */
  void Read(std::istream& is)
  {
    is >> *this;
  };

  /**
   * @brief Write the class to a stream
   */
  void Write(std::ostream& os)
  {
    os << *this;
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
