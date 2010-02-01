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

#ifndef AlosPalsarRecordHeader_h
#define AlosPalsarRecordHeader_h

#include<iostream>
#include<cstring>

namespace ossimplugins
{

/**
 * @ingroup AlosPalsar
 * @brief This class is able to read a record header
 */
class AlosPalsarRecordHeader
{
public:
  /**
   * @brief Constructor
   */
  AlosPalsarRecordHeader();

  /**
   * @brief Destructor
   */
  ~AlosPalsarRecordHeader();

  /**
   * @brief Copy constructor
   */
  AlosPalsarRecordHeader(const AlosPalsarRecordHeader& rhs);

  /**
   * @brief Copy operator
   */
  AlosPalsarRecordHeader& operator=(const AlosPalsarRecordHeader& rhs);
  /**
   * @brief This function write the RecordHeader in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const AlosPalsarRecordHeader& data);

  /**
   * @brief This function read a RecordHeader from a stream
   */
  friend std::istream& operator>>(std::istream& is, AlosPalsarRecordHeader& data);

  /**
   * @brief Record sequence number
   */
  unsigned int  get_rec_seq()
  {
    return _rec_seq;
  };

  /**
   * @brief First record sub-type code
   */
  unsigned char   get_rec_sub1()
  {
    return _rec_sub1;
  };

  /**
   * @brief Record type code
   */
  unsigned char   get_rec_type()
  {
    return _rec_type;
  };

  /**
   * @brief Second record sub-type code
   */
  unsigned char   get_rec_sub2()
  {
    return _rec_sub2;
  };

  /**
   * @brief Third record sub-type code
   */
  unsigned char   get_rec_sub3()
  {
    return _rec_sub3;
  };

  /**
   * @brief Length of this record (in bytes)
   */
  unsigned int get_length()
  {
    return _length;
  };
protected:

  /**
   * @brief This function switch the LSB value and the MSB value of the parameter
   */
  void SwitchEndian(unsigned int& value);

  /**
   * @brief Record sequence number
   */
  unsigned int  _rec_seq;
  /**
   * @brief First record sub-type code
   */
  unsigned char   _rec_sub1;
  /**
   * @brief Record type code
   */
  unsigned char   _rec_type;
  /**
   * @brief Second record sub-type code
   */
  unsigned char   _rec_sub2;
  /**
   * @brief Third record sub-type code
   */
  unsigned char   _rec_sub3;
  /**
   * @brief Length of this record (in bytes)
   */
  unsigned int  _length;
private:


};
}
#endif
