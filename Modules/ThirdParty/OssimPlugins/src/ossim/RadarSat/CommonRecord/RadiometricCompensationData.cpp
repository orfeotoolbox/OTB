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


#include "RadiometricCompensationData.h"

namespace ossimplugins
{
   RadiometricCompensationData::RadiometricCompensationData() : RadarSatRecord("radi_comp_rec")
   {
   }

   RadiometricCompensationData::~RadiometricCompensationData()
   {
   }

   std::ostream& operator<<(std::ostream& os, const RadiometricCompensationData& data)
   {
      os<<"seq_num:"<<data._seq_num<<std::endl;

      os<<"chan_ind:"<<data._chan_ind<<std::endl;

      os<<"n_dset:"<<data._n_dset<<std::endl;

      os<<"dset_size:"<<data._dset_size<<std::endl;

      for (int i=0;i<4;i++)
      {
         os<<"dset["<<i<<"]:"<<data._dset[i]<<std::endl;
      }
      return os;
   }

   std::istream& operator>>(std::istream& is, RadiometricCompensationData& data)
   {
      char buf[9];
      buf[8] = '\0';

      is.read(buf,4);
      buf[4] = '\0';
      data._seq_num = atoi(buf);

      is.read(buf,4);
      buf[4] = '\0';
      data._chan_ind = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._n_dset = atoi(buf);

      is.read(buf,8);
      buf[8] = '\0';
      data._dset_size = atoi(buf);

      for (int i=0;i<4;i++)
      {
         is>> data._dset[i];
      }

      return is;
   }

   RadiometricCompensationData::RadiometricCompensationData(const RadiometricCompensationData& rhs):
      RadarSatRecord(rhs)
   {
      _seq_num = rhs._seq_num;

      _chan_ind = rhs._chan_ind;

      _n_dset = rhs._n_dset;

      _dset_size = rhs._dset_size;

      for (int i=0;i<4;i++)
      {
         _dset[i] = rhs._dset[i];
      }
   }

   RadiometricCompensationData& RadiometricCompensationData::operator=(const RadiometricCompensationData& /* rhs */)
   {
      return *this;
   }
}
