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

#ifndef TextRecord_h
#define TextRecord_h


#include<iostream>
#include <RadarSat/RadarSatRecordHeader.h>
#include <RadarSat/RadarSatRecord.h>

namespace ossimplugins
{
/**
 * @ingroup TextRecordRecord
 * @brief This class is able to read the Text record of the volume directory file
 */
class TextRecord : public RadarSatRecord
{
public:
  /**
   * @brief Constructor
   */
  TextRecord();
  /**
   * @brief Destructor
   */
  virtual ~TextRecord();

  /**
   * @brief This function writes the TextRecord in a stream
   */
  friend std::ostream& operator<<(std::ostream& os, const TextRecord& data);

  /**
   * @brief This function reads a TextRecord from a stream
   */
  friend std::istream& operator>>(std::istream& is, TextRecord& data);

  /**
   * @brief Copy constructor
   */
  TextRecord(const TextRecord& rhs);

  /**
   * @brief Copy operator
   */
  TextRecord& operator=(const TextRecord& rhs);

  /**
   * @brief This function is able to create a new instance of the class
   */
  RadarSatRecord* Instanciate()
  {
    return new TextRecord();
  };

  /**
   * @brief This function is able to create a new instance of the class initialised with the data of the calling instance
   */
  RadarSatRecord* Clone()
  {
    return new TextRecord(*this);
  };

  /**
   * @brief Reads the class data from a stream
   */
  void Read(std::istream& is)
  {
    is>>*this;
  };

  /**
   * @brief Writes the class to a stream
   */
  void Write(std::ostream& os)
  {
    os<<*this;
  };

  /**
   * @brief ASCII flag
   */
  std::string get_ascii_flag()
  {
    return _ascii_flag;
  };

  /**
   * @brief Continuation flag
   */
  std::string get_cont_flag()
  {
    return _cont_flag;
  };

  /**
   * @brief   Product type specifier
   */
  std::string get_product_type()
  {
    return _product_type;
  };

  /**
   * @brief Product creation info
   */
  std::string get_product_create()
  {
    return _product_create;
  };

  /**
   * @brief Volume Description Record
   */
  std::string get_phyvol_id()
  {
    return _phyvol_id;
  };

  /**
   * @brief Scene identifier
   */
  std::string get_scene_id()
  {
    return _scene_id;
  };

  /**
   * @brief Scene location
   */
  std::string get_scene_loc()
  {
    return _scene_loc;
  };

  /**
   * @brief Copyright
   */
  std::string get_copyright_info()
  {
    return _copyright_info;
  };


protected:
  /**
   * @brief ASCII flag
   */
  std::string _ascii_flag;

  /**
   * @brief Continuation flag
   */
  std::string _cont_flag;

  /**
   * @brief   Product type specifier
   */
  std::string _product_type;

  /**
   * @brief Product creation info
   */
  std::string _product_create ;

  /**
   * @brief Volume Description Record
   */
  std::string _phyvol_id;

  /**
   * @brief Scene identifier
   */
  std::string _scene_id;

  /**
   * @brief Scene location
   */
  std::string _scene_loc;

  /**
   * @brief Copyright
   */
  std::string _copyright_info;

private:
};
}
#endif
