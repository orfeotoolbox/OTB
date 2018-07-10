/*
 * Copyright (C) 2005-2017 by Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2008-2010 by Centre for Remote Imaging, Sensing and Processing (CRISP)
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


#include <AlosPalsar/AlosPalsarFileDescriptor.h>


namespace ossimplugins
{

AlosPalsarFileDescriptor::AlosPalsarFileDescriptor() : AlosPalsarRecord("sar_desc_rec")
{
}

AlosPalsarFileDescriptor::~AlosPalsarFileDescriptor()
{
}

std::ostream& operator<<(std::ostream& os, const AlosPalsarFileDescriptor& data)
{
  os << "ascii_flag:" << data._ascii_flag << std::endl;
  os << "continuation_flag:" << data._continuation_flag << std::endl;
  os << "format_doc:" << data._format_doc << std::endl;
  os << "format_ver:" << data._format_ver << std::endl;
  os << "design_rev:" << data._design_rev << std::endl;
  os << "software_id:" << data._software_id << std::endl;
  os << "file_num:" << data._file_num << std::endl;
  os << "file_name:" << data._file_name << std::endl;
  os << "rec_seq:" << data._rec_seq << std::endl;
  os << "seq_loc:" << data._seq_loc << std::endl;
  os << "seq_len:" << data._seq_len << std::endl;
  os << "rec_code:" << data._rec_code << std::endl;
  os << "code_loc:" << data._code_loc << std::endl;
  os << "code_len:" << data._code_len << std::endl;
  os << "rec_len:" << data._rec_len << std::endl;
  os << "rlen_loc:" << data._rlen_loc << std::endl;
  os << "rlen_len:" << data._rlen_len << std::endl;
  os << "n_dataset:" << data._n_dataset << std::endl;
  os << "l_dataset:" << data._l_dataset << std::endl;
  os << "n_map_proj:" << data._n_map_proj << std::endl;
  os << "l_map_proj:" << data._l_map_proj << std::endl;
  os << "n_plat_pos:" << data._n_plat_pos << std::endl;
  os << "l_plat_pos:" << data._l_plat_pos << std::endl;
  os << "n_att_data:" << data._n_att_data << std::endl;
  os << "l_att_data:" << data._l_att_data << std::endl;
  os << "n_radi_data:" << data._n_radi_data << std::endl;
  os << "l_radi_data:" << data._l_radi_data << std::endl;
  os << "n_radi_comp:" << data._n_radi_comp << std::endl;
  os << "l_radi_comp:" << data._l_radi_comp << std::endl;
  os << "n_qual_sum:" << data._n_qual_sum << std::endl;
  os << "l_qual_sum:" << data._l_qual_sum << std::endl;
  os << "n_data_his:" << data._n_data_his << std::endl;
  os << "l_data_his:" << data._l_data_his << std::endl;
  os << "n_rang_spec:" << data._n_rang_spec << std::endl;
  os << "l_rang_spec:" << data._l_rang_spec << std::endl;
  os << "n_dem_desc:" << data._n_dem_desc << std::endl;
  os << "l_dem_desc:" << data._l_dem_desc << std::endl;
  os << "n_radar_par:" << data._n_radar_par << std::endl;
  os << "l_radar_par:" << data._l_radar_par << std::endl;
  os << "n_anno_data:" << data._n_anno_data << std::endl;
  os << "l_anno_data:" << data._l_anno_data << std::endl;
  os << "n_det_proc:" << data._n_det_proc << std::endl;
  os << "l_det_proc:" << data._l_det_proc << std::endl;
  os << "n_cal:" << data._n_cal << std::endl;
  os << "l_cal:" << data._l_cal << std::endl;
  os << "n_gcp:" << data._n_gcp << std::endl;
  os << "l_gcp:" << data._l_gcp << std::endl;
  os << "n_fac_data:" << data._n_fac_data << std::endl;
  os << "l_fac_data:" << data._l_fac_data << std::endl;
  os << "n_fac_data2:" << data._n_fac_data2 << std::endl;
  os << "l_fac_data2:" << data._l_fac_data2 << std::endl;
  os << "n_fac_data3:" << data._n_fac_data3 << std::endl;
  os << "l_fac_data3:" << data._l_fac_data3 << std::endl;
  os << "n_fac_data4:" << data._n_fac_data4 << std::endl;
  os << "l_fac_data4:" << data._l_fac_data4 << std::endl;
  os << "n_fac_data5:" << data._n_fac_data5 << std::endl;
  os << "l_fac_data5:" << data._l_fac_data5 << std::endl;
  os << "n_fac_data6:" << data._n_fac_data6 << std::endl;
  os << "l_fac_data6:" << data._l_fac_data6 << std::endl;
  os << "n_fac_data7:" << data._n_fac_data7 << std::endl;
  os << "l_fac_data7:" << data._l_fac_data7 << std::endl;
  os << "n_fac_data8:" << data._n_fac_data8 << std::endl;
  os << "l_fac_data8:" << data._l_fac_data8 << std::endl;
  os << "n_fac_data9:" << data._n_fac_data9 << std::endl;
  os << "l_fac_data9:" << data._l_fac_data9 << std::endl;
  os << "n_fac_data10:" << data._n_fac_data10 << std::endl;
  os << "l_fac_data10:" << data._l_fac_data10 << std::endl;
  os << "n_fac_data11:" << data._n_fac_data11 << std::endl;
  os << "l_fac_data11:" << data._l_fac_data11 << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, AlosPalsarFileDescriptor& data)
{
  char buf2[3];
  buf2[2] = '\0';
  char buf12[13];
  buf12[12] = '\0';
  char buf16[17];
  buf16[16] = '\0';
  char buf4[5];
  buf4[4] = '\0';
  char buf6[7];
  buf6[6] = '\0';
  char buf8[9];
  buf8[8] = '\0';
  char buf64[65];
  buf64[64] = '\0';

  char buf60[61];
  buf60[60] = '\0';

  char buf146[147];
  buf146[146] = '\0';

  is.read(buf2, 2);
  data._ascii_flag = buf2;

  is.read(buf2, 2);
  data._continuation_flag = buf2;

  is.read(buf12, 12);
  data._format_doc = buf12;

  is.read(buf2, 2);
  data._format_ver = buf2;

  is.read(buf2, 2);
  data._design_rev = buf2;

  is.read(buf12, 12);
  data._software_id = buf12;

  is.read(buf4, 4);
  data._file_num = atoi(buf4);

  is.read(buf16, 16);
  data._file_name = buf16;

  is.read(buf4, 4);
  data._rec_seq = buf4;

  is.read(buf8, 8);
  data._seq_loc = atoi(buf8);

  is.read(buf4, 4);
  data._seq_len = atoi(buf4);

  is.read(buf4, 4);
  data._rec_code = buf4;

  is.read(buf8, 8);
  data._code_loc = atoi(buf8);

  is.read(buf4, 4);
  data._code_len = atoi(buf4);

  is.read(buf4, 4);
  data._rec_len = buf4;

  is.read(buf8, 8);
  data._rlen_loc = atoi(buf8);

  is.read(buf4, 4);
  data._rlen_len = atoi(buf4);

  is.read(buf4, 4);

  is.read(buf64, 64);

  is.read(buf6, 6);
  data._n_dataset = atoi(buf6);

  is.read(buf6, 6);
  data._l_dataset = atoi(buf6);

  is.read(buf6, 6);
  data._n_map_proj = atoi(buf6);

  is.read(buf6, 6);
  data._l_map_proj = atoi(buf6);

  is.read(buf6, 6);
  data._n_plat_pos = atoi(buf6);
  is.read(buf6, 6);
  data._l_plat_pos = atoi(buf6);
  is.read(buf6, 6);
  data._n_att_data = atoi(buf6);
  is.read(buf6, 6);
  data._l_att_data = atoi(buf6);
  is.read(buf6, 6);
  data._n_radi_data = atoi(buf6);
  is.read(buf6, 6);
  data._l_radi_data = atoi(buf6);
  is.read(buf6, 6);
  data._n_radi_comp = atoi(buf6);
  is.read(buf6, 6);
  data._l_radi_comp = atoi(buf6);
  is.read(buf6, 6);
  data._n_qual_sum = atoi(buf6);
  is.read(buf6, 6);
  data._l_qual_sum = atoi(buf6);
  is.read(buf6, 6);
  data._n_data_his = atoi(buf6);
  is.read(buf6, 6);
  data._l_data_his = atoi(buf6);



  is.read(buf6, 6);
  data._n_rang_spec = atoi(buf6);
  is.read(buf6, 6);
  data._l_rang_spec = atoi(buf6);
  is.read(buf6, 6);
  data._n_dem_desc = atoi(buf6);
  is.read(buf6, 6);
  data._l_dem_desc = atoi(buf6);
  is.read(buf6, 6);
  data._n_radar_par = atoi(buf6);
  is.read(buf6, 6);
  data._l_radar_par = atoi(buf6);
  is.read(buf6, 6);
  data._n_anno_data = atoi(buf6);
  is.read(buf6, 6);
  data._l_anno_data = atoi(buf6);
  is.read(buf6, 6);
  data._n_det_proc = atoi(buf6);
  is.read(buf6, 6);
  data._l_det_proc = atoi(buf6);
  is.read(buf6, 6);
  data._n_cal = atoi(buf6);
  is.read(buf6, 6);
  data._l_cal = atoi(buf6);
  is.read(buf6, 6);
  data._n_gcp = atoi(buf6);
  is.read(buf6, 6);
  data._l_gcp = atoi(buf6);
  is.read(buf60, 60);
  is.read(buf6, 6);
  data._n_fac_data = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data2 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data2 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data3 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data3 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data4 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data4 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data5 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data5 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data6 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data6 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data7 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data7 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data8 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data8 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data9 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data9 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data10 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data10 = atoi(buf8);
  is.read(buf6, 6);
  data._n_fac_data11 = atoi(buf6);
  is.read(buf8, 8);
  data._l_fac_data11 = atoi(buf8);

  is.read(buf146, 146);
  return is;
}

AlosPalsarFileDescriptor::AlosPalsarFileDescriptor(const AlosPalsarFileDescriptor& rhs):
    AlosPalsarRecord(rhs),
    _ascii_flag(rhs._ascii_flag),
    _continuation_flag(rhs._continuation_flag),
    _format_doc(rhs._format_doc),
    _format_ver(rhs._format_ver),
    _design_rev(rhs._design_rev),
    _software_id(rhs._software_id),
    _file_num(rhs._file_num),
    _file_name(rhs._file_name),
    _rec_seq(rhs._rec_seq),
    _seq_loc(rhs._seq_loc),
    _seq_len(rhs._seq_len),
    _rec_code(rhs._rec_code),
    _code_loc(rhs._code_loc),
    _code_len(rhs._code_len),
    _rec_len(rhs._rec_len),
    _rlen_loc(rhs._rlen_loc),
    _rlen_len(rhs._rlen_len),
    _n_dataset(rhs._n_dataset),
    _l_dataset(rhs._l_dataset),
    _n_map_proj(rhs._n_map_proj),
    _l_map_proj(rhs._l_map_proj),
    _n_plat_pos(rhs._n_plat_pos),
    _l_plat_pos(rhs._l_plat_pos),
    _n_att_data(rhs._n_att_data),
    _l_att_data(rhs._l_att_data),
    _n_radi_data(rhs._n_radi_data),
    _l_radi_data(rhs._l_radi_data),
    _n_radi_comp(rhs._n_radi_comp),
    _l_radi_comp(rhs._l_radi_comp),
    _n_qual_sum(rhs._n_qual_sum),
    _l_qual_sum(rhs._l_qual_sum),
    _n_data_his(rhs._n_data_his),
    _l_data_his(rhs._l_data_his),
    _n_rang_spec(rhs._n_rang_spec),
    _l_rang_spec(rhs._l_rang_spec),
    _n_dem_desc(rhs._n_dem_desc),
    _l_dem_desc(rhs._l_dem_desc),
    _n_radar_par(rhs._n_radar_par),
    _l_radar_par(rhs._l_radar_par),
    _n_anno_data(rhs._n_anno_data),
    _l_anno_data(rhs._l_anno_data),
    _n_det_proc(rhs._n_det_proc),
    _l_det_proc(rhs._l_det_proc),
    _n_cal(rhs._n_cal),
    _l_cal(rhs._l_cal),
    _n_gcp(rhs._n_gcp),
    _l_gcp(rhs._l_gcp),
    _n_fac_data(rhs._n_fac_data),
    _l_fac_data(rhs._l_fac_data),
    _n_fac_data2(rhs._n_fac_data2),
    _l_fac_data2(rhs._l_fac_data2),
    _n_fac_data3(rhs._n_fac_data3),
    _l_fac_data3(rhs._l_fac_data3),
    _n_fac_data4(rhs._n_fac_data4),
    _l_fac_data4(rhs._l_fac_data4),
    _n_fac_data5(rhs._n_fac_data5),
    _l_fac_data5(rhs._l_fac_data5),
    _n_fac_data6(rhs._n_fac_data6),
    _l_fac_data6(rhs._l_fac_data6),
    _n_fac_data7(rhs._n_fac_data7),
    _l_fac_data7(rhs._l_fac_data7),
    _n_fac_data8(rhs._n_fac_data8),
    _l_fac_data8(rhs._l_fac_data8),
    _n_fac_data9(rhs._n_fac_data9),
    _l_fac_data9(rhs._l_fac_data9),
    _n_fac_data10(rhs._n_fac_data10),
    _l_fac_data10(rhs._l_fac_data10),
    _n_fac_data11(rhs._n_fac_data11),
    _l_fac_data11(rhs._l_fac_data11)

{
}

AlosPalsarFileDescriptor& AlosPalsarFileDescriptor::operator=(const AlosPalsarFileDescriptor& rhs)
{
  _ascii_flag = rhs._ascii_flag;
  _continuation_flag = rhs._continuation_flag;
  _format_doc = rhs._format_doc;
  _format_ver = rhs._format_ver;
  _design_rev = rhs._design_rev;
  _software_id = rhs._software_id;
  _file_num = rhs._file_num;
  _file_name = rhs._file_name;
  _rec_seq = rhs._rec_seq;
  _seq_loc = rhs._seq_loc;
  _seq_len = rhs._seq_len;
  _rec_code = rhs._rec_code;
  _code_loc = rhs._code_loc;
  _code_len = rhs._code_len;
  _rec_len = rhs._rec_len;
  _rlen_loc = rhs._rlen_loc;
  _rlen_len = rhs._rlen_len;
  _n_dataset = rhs._n_dataset;
  _l_dataset = rhs._l_dataset;
  _n_map_proj = rhs._n_map_proj;
  _l_map_proj = rhs._l_map_proj;
  _n_plat_pos = rhs._n_plat_pos;
  _l_plat_pos = rhs._l_plat_pos;
  _n_att_data = rhs._n_att_data;
  _l_att_data = rhs._l_att_data;
  _n_radi_data = rhs._n_radi_data;
  _l_radi_data = rhs._l_radi_data;
  _n_radi_comp = rhs._n_radi_comp;
  _l_radi_comp = rhs._l_radi_comp;
  _n_qual_sum = rhs._n_qual_sum ;
  _l_qual_sum = rhs._l_qual_sum;
  _n_data_his = rhs._n_data_his;
  _l_data_his = rhs._l_data_his;
  _n_rang_spec = rhs._n_rang_spec;
  _l_rang_spec = rhs._l_rang_spec;
  _n_dem_desc = rhs._n_dem_desc;
  _l_dem_desc = rhs._l_dem_desc;
  _n_radar_par = rhs._n_radar_par;
  _l_radar_par = rhs._l_radar_par;
  _n_anno_data = rhs._n_anno_data;
  _l_anno_data = rhs._l_anno_data;
  _n_det_proc = rhs._n_det_proc;
  _l_det_proc = rhs._l_det_proc;
  _n_cal = rhs._n_cal;
  _l_cal = rhs._l_cal;
  _n_gcp = rhs._n_gcp;
  _l_gcp = rhs._l_gcp;
  _n_fac_data = rhs._n_fac_data;
  _l_fac_data = rhs._l_fac_data;
  _n_fac_data2 = rhs._n_fac_data2;
  _l_fac_data2 = rhs._l_fac_data2;
  _n_fac_data3 = rhs._n_fac_data3;
  _l_fac_data3 = rhs._l_fac_data3;
  _n_fac_data4 = rhs._n_fac_data4;
  _l_fac_data4 = rhs._l_fac_data4;
  _n_fac_data5 = rhs._n_fac_data5;
  _l_fac_data5 = rhs._l_fac_data5;
  _n_fac_data6 = rhs._n_fac_data6;
  _l_fac_data6 = rhs._l_fac_data6;
  _n_fac_data7 = rhs._n_fac_data7;
  _l_fac_data7 = rhs._l_fac_data7;
  _n_fac_data8 = rhs._n_fac_data8;
  _l_fac_data8 = rhs._l_fac_data8;
  _n_fac_data9 = rhs._n_fac_data9;
  _l_fac_data9 = rhs._l_fac_data9;
  _n_fac_data10 = rhs._n_fac_data10;
  _l_fac_data10 = rhs._l_fac_data10;
  _n_fac_data11 = rhs._n_fac_data11;
  _l_fac_data11 = rhs._l_fac_data11;
  return *this;
}
}
