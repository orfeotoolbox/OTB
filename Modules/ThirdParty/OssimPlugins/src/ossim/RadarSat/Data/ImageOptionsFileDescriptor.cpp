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

#include <RadarSat/Data/ImageOptionsFileDescriptor.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimTrace.h>

namespace ossimplugins
{
   static ossimTrace traceDebug("ImageOptionsFileDescriptor:debug");

   ImageOptionsFileDescriptor::ImageOptionsFileDescriptor() : RadarSatRecord("imop_desc_rec")
   {
   }

   ImageOptionsFileDescriptor::~ImageOptionsFileDescriptor()
   {
   }

   std::ostream& operator<<(std::ostream& os, const ImageOptionsFileDescriptor& data)
   {
      os<<"ascii_flag:"<<data._ascii_flag.c_str()<<std::endl;

      os<<"format_doc:"<<data._format_doc.c_str()<<std::endl;

      os<<"format_rev:"<<data._format_rev.c_str()<<std::endl;

      os<<"design_rev:"<<data._design_rev.c_str()<<std::endl;

      os<<"software_id:"<<data._software_id.c_str()<<std::endl;

      os<<"file_num:"<<data._file_num<<std::endl;

      os<<"file_name:"<<data._file_name.c_str()<<std::endl;

      os<<"rec_seq:"<<data._rec_seq.c_str()<<std::endl;

      os<<"seq_loc:"<<data._seq_loc<<std::endl;

      os<<"seq_len:"<<data._seq_len<<std::endl;

      os<<"rec_code:"<<data._rec_code.c_str()<<std::endl;

      os<<"code_loc:"<<data._code_loc<<std::endl;

      os<<"code_len:"<<data._code_len<<std::endl;

      os<<"rec_len:"<<data._rec_len.c_str()<<std::endl;

      os<<"rlen_loc:"<<data._rlen_loc<<std::endl;

      os<<"rlen_len:"<<data._rlen_len<<std::endl;

      os<<"n_dataset:"<<data._n_dataset<<std::endl;

      os<<"l_dataset:"<<data._l_dataset<<std::endl;

      os<<"nbit:"<<data._nbit<<std::endl;

      os<<"nsamp:"<<data._nsamp<<std::endl;

      os<<"nbyte:"<<data._nbyte<<std::endl;

      os<<"justify:"<<data._justify.c_str()<<std::endl;

      os<<"nchn:"<<data._nchn<<std::endl;

      os<<"nlin:"<<data._nlin<<std::endl;

      os<<"nleft:"<<data._nleft<<std::endl;

      os<<"ngrp:"<<data._ngrp<<std::endl;

      os<<"nright:"<<data._nright<<std::endl;

      os<<"ntop:"<<data._ntop<<std::endl;

      os<<"nbott:"<<data._nbott<<std::endl;

      os<<"intleav:"<<data._intleav.c_str()<<std::endl;

      os<<"nrec_lin:"<<data._nrec_lin<<std::endl;

      os<<"nrec_chn:"<<data._nrec_chn<<std::endl;

      os<<"n_prefix:"<<data._n_prefix<<std::endl;

      os<<"n_sar:"<<data._n_sar<<std::endl;

      os<<"n_suffix:"<<data._n_suffix<<std::endl;

      os<<"lin_loc:"<<data._lin_loc.c_str()<<std::endl;

      os<<"chn_loc:"<<data._chn_loc.c_str()<<std::endl;

      os<<"tim_loc:"<<data._tim_loc.c_str()<<std::endl;

      os<<"left_loc:"<<data._left_loc.c_str()<<std::endl;

      os<<"right_loc:"<<data._right_loc.c_str()<<std::endl;

      os<<"pad_ind:"<<data._pad_ind.c_str()<<std::endl;

      os<<"qual_loc:"<<data._qual_loc.c_str()<<std::endl;

      os<<"cali_loc:"<<data._cali_loc.c_str()<<std::endl;

      os<<"gain_loc:"<<data._gain_loc.c_str()<<std::endl;

      os<<"bias_loc:"<<data._bias_loc.c_str()<<std::endl;

      os<<"type_id:"<<data._type_id.c_str()<<std::endl;

      os<<"type_code:"<<data._type_code.c_str()<<std::endl;

      os<<"left_fill:"<<data._left_fill<<std::endl;

      os<<"right_fill:"<<data._right_fill<<std::endl;

      os<<"pix_rng:"<<data._pix_rng<<std::endl;

      return os;
   }

   std::istream& operator>>(std::istream& is, ImageOptionsFileDescriptor& data)
   {
      char buf[15805];
      buf[15804] = '\0';

      is.read(buf,2);
      buf[2] = '\0';
      data._ascii_flag = buf;

      is.read(buf,2);

      is.read(buf,12);
      buf[12] = '\0';
      data._format_doc = buf;

      is.read(buf,2);
      buf[2] = '\0';
      data._format_rev = buf;

      is.read(buf,2);
      buf[2] = '\0';
      data._design_rev = buf;

      is.read(buf,12);
      buf[12] = '\0';
      data._software_id = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._file_num = atoi(buf);

      is.read(buf,16);
      buf[16] = '\0';
      data._file_name = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._rec_seq = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._seq_loc = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._seq_len = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._rec_code = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._code_loc = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._code_len = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._rec_len = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._rlen_loc = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._rlen_len = atoi(buf);

      for (int i=0;i<4;i++)
      {
         is.read(buf,1);
      }

      is.read(buf,64);

      is.read(buf,6);
      buf[6] = '\0';
      data._n_dataset = atoi(buf);

      is.read(buf,6);
      buf[6] = '\0';
      data._l_dataset = atoi(buf);

      is.read(buf,24);

      is.read(buf,4);
      buf[4] = '\0';
      data._nbit = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._nsamp = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._nbyte = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._justify = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._nchn = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';

      // We should use strtol() to avoid wrong conversion with atoi()
      char* p;
      int result = strtol(buf, &p, 10);
      if ( *p != 0 || p == buf)
      {
         if(traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "WARNING: strtol() try to convert an empty tab of characters. It may be possible in case of SCN and SCW format" << buf << "!" << std::endl;
            ossimNotify(ossimNotifyLevel_DEBUG) << "=> _nlin = -1" << std::endl;
         }
         data._nlin = -1;
      }
      else
      {
         data._nlin = result;
      };

      is.read(buf,4);
      buf[4] = '\0';
      data._nleft = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._ngrp = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._nright = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._ntop = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._nbott = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._intleav = buf;

      is.read(buf,2);
      buf[2] = '\0';
      data._nrec_lin = atoi(buf);

      is.read(buf,2);
      buf[2] = '\0';
      data._nrec_chn = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._n_prefix = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._n_sar = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._n_suffix = atoi(buf);

      is.read(buf,4);

      is.read(buf,8);
      buf[8] = '\0';
      data._lin_loc = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._chn_loc = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._tim_loc = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._left_loc = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._right_loc = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._pad_ind = buf;

      is.read(buf,28);

      is.read(buf,8);
      buf[8] = '\0';
      data._qual_loc = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._cali_loc = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._gain_loc = buf;

      is.read(buf,8);
      buf[8] = '\0';
      data._bias_loc = buf;

      is.read(buf,28);
      buf[28] = '\0';
      data._type_id = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._type_code = buf;

      is.read(buf,4);
      buf[4] = '\0';
      data._left_fill = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._right_fill = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._pix_rng = atoi(buf);

      is.read(buf,15804);

      return is;
   }

   ImageOptionsFileDescriptor::ImageOptionsFileDescriptor(const ImageOptionsFileDescriptor& rhs):
      RadarSatRecord(rhs)
   {


   }

   ImageOptionsFileDescriptor& ImageOptionsFileDescriptor::operator=(const ImageOptionsFileDescriptor& /* rhs */)
   {
      return *this;
   }
}
