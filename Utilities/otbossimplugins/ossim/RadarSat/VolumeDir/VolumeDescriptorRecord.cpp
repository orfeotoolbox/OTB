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

#include "VolumeDescriptorRecord.h"

namespace ossimplugins
{
VolumeDescriptorRecord::VolumeDescriptorRecord() : RadarSatRecord("vol_desc_rec")
{
}

VolumeDescriptorRecord::~VolumeDescriptorRecord()
{
}

std::ostream& operator<<(std::ostream& os, const VolumeDescriptorRecord& data)
{
  os<<"ascii_flag:"<<data._ascii_flag.c_str()<<std::endl;
  os<<"format_doc:"<<data._format_doc.c_str()<<std::endl;
  os<<"format_ver:"<<data._format_ver.c_str()<<std::endl;
  os<<"format_rev:"<<data._format_rev.c_str()<<std::endl;
  os<<"software_id:"<<data._software_id.c_str()<<std::endl;
  os<<"phyvol_id:"<<data._phyvol_id.c_str()<<std::endl;
  os<<"logvol_id:"<<data._logvol_id.c_str()<<std::endl;
  os<<"volset_id:"<<data._volset_id.c_str()<<std::endl;
  os<<"phyvol_cnt:"<<data._phyvol_cnt<<std::endl;
  os<<"first_phyvol:"<<data._first_phyvol<<std::endl;
  os<<"last_phyvol:"<<data._last_phyvol<<std::endl;
  os<<"curr_phyvol:"<<data._curr_phyvol<<std::endl;
  os<<"first_file:"<<data._first_file<<std::endl;
  os<<"volset_log:"<<data._volset_log<<std::endl;
  os<<"phyvol_log:"<<data._phyvol_log<<std::endl;
  os<<"logvol_date:"<<data._logvol_date.c_str()<<std::endl;
  os<<"logvol_time:"<<data._logvol_time.c_str()<<std::endl;
  os<<"logvol_country:"<<data._logvol_country.c_str()<<std::endl;
  os<<"logvol_agency:"<<data._logvol_agency.c_str()<<std::endl;
  os<<"logvol_facility:"<<data._logvol_facility.c_str()<<std::endl;
  os<<"n_filepoint:"<<data._n_filepoint<<std::endl;
  os<<"n_voldir:"<<data._n_voldir<<std::endl;
  os<<"product_id:"<<data._product_id.c_str()<<std::endl;


  return os;
}

std::istream& operator>>(std::istream& is, VolumeDescriptorRecord& data)
{
  char buf[349]; // 361-12
    buf[348] = '\0';

    is.read(buf,2);
    buf[2] = '\0';
    data._ascii_flag = buf;

    is.read(buf,2); // spare1

    is.read(buf,12);
    buf[12] = '\0';
    data._format_doc = buf;

    is.read(buf,2);
    buf[2] = '\0';
    data._format_ver = buf;

    is.read(buf,2);
    buf[2] = '\0';
    data._format_rev = buf;

    is.read(buf,12);
    buf[12] = '\0';
    data._software_id = buf;

    is.read(buf,16);
    buf[16] = '\0';
    data._phyvol_id = buf;

    is.read(buf,16);
    buf[16] = '\0';
    data._logvol_id = buf;

    is.read(buf,16);
    buf[16] = '\0';
    data._volset_id = buf;

    is.read(buf,2);
    buf[2] = '\0';
    data._phyvol_cnt = atoi(buf);

    is.read(buf,2);
    buf[2] = '\0';
    data._first_phyvol = atoi(buf);

    is.read(buf,2);
    buf[2] = '\0';
    data._last_phyvol = atoi(buf);

    is.read(buf,2);
    buf[2] = '\0';
    data._curr_phyvol = atoi(buf);

    is.read(buf,4);
    buf[4] = '\0';
    data._first_file = atoi(buf);

    is.read(buf,4);
    buf[4] = '\0';
    data._volset_log = atoi(buf);

    is.read(buf,4);
    buf[4] = '\0';
    data._phyvol_log = atoi(buf);

    is.read(buf,8);
    buf[8] = '\0';
    data._logvol_date = buf;

    is.read(buf,8);
    buf[8] = '\0';
    data._logvol_time = buf;

    is.read(buf,12);
    buf[12] = '\0';
    data._logvol_country = buf;


    is.read(buf,8);
    buf[8] = '\0';
    data._logvol_agency = buf;

    is.read(buf,12);
    buf[12] = '\0';
    data._logvol_facility = buf;

    is.read(buf,4);
    buf[4] = '\0';
    data._n_filepoint = atoi(buf);

    is.read(buf,4);
    buf[4] = '\0';
    data._n_voldir = atoi(buf);

    is.read(buf,92); // spare2

    is.read(buf,8);
    buf[8] = '\0';
    data._product_id = buf;

    is.read(buf,92); // spare3

  return is;
}

VolumeDescriptorRecord::VolumeDescriptorRecord(const VolumeDescriptorRecord& rhs):
  RadarSatRecord(rhs),
  _ascii_flag(rhs._ascii_flag),
  _format_doc(rhs._format_doc),
  _format_ver(rhs._format_ver),
  _format_rev(rhs._format_rev),
  _software_id(rhs._software_id),
  _phyvol_id(rhs._phyvol_id),
  _logvol_id(rhs._logvol_id),
  _volset_id(rhs._volset_id),
  _phyvol_cnt(rhs._phyvol_cnt),
  _first_phyvol(rhs._first_phyvol),
  _last_phyvol(rhs._last_phyvol),
  _curr_phyvol(rhs._curr_phyvol),
  _first_file(rhs._first_file),
  _volset_log(rhs._volset_log),
  _logvol_date(rhs._logvol_date),
  _logvol_time(rhs._logvol_time),
  _logvol_country(rhs._logvol_country),
  _logvol_agency(rhs._logvol_agency),
  _logvol_facility(rhs._logvol_facility),
  _n_filepoint(rhs._n_filepoint),
  _n_voldir(rhs._n_voldir),
  _product_id(rhs._product_id)
{
}

VolumeDescriptorRecord& VolumeDescriptorRecord::operator=(const VolumeDescriptorRecord& rhs)
{
  _ascii_flag = rhs._ascii_flag;
  _format_doc = rhs._format_doc;
  _format_ver = rhs._format_ver;
  _format_rev = rhs._format_rev;
  _software_id = rhs._software_id;
  _phyvol_id = rhs._phyvol_id;
  _logvol_id = rhs._logvol_id;
  _volset_id = rhs._volset_id;
  _phyvol_cnt = rhs._phyvol_cnt;
  _first_phyvol = rhs._first_phyvol;
  _last_phyvol = rhs._last_phyvol;
  _curr_phyvol = rhs._curr_phyvol;
  _first_file = rhs._first_file;
  _volset_log = rhs._volset_log;
  _logvol_date = rhs._logvol_date;
  _logvol_time = rhs._logvol_time;
  _logvol_country = rhs._logvol_country;
  _logvol_agency = rhs._logvol_agency;
  _logvol_facility = rhs._logvol_facility;
  _n_filepoint = rhs._n_filepoint;
  _n_voldir = rhs._n_voldir;
  _product_id = rhs._product_id;
  return *this;
}
}
