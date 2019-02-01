/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
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


#include <AlosPalsar/AlosPalsarDataFileDescriptor.h>


namespace ossimplugins
{

AlosPalsarDataFileDescriptor::AlosPalsarDataFileDescriptor() : AlosPalsarRecord("sar_desc_rec")
{
}

AlosPalsarDataFileDescriptor::~AlosPalsarDataFileDescriptor()
{
}

std::ostream& operator<<(std::ostream& os, const AlosPalsarDataFileDescriptor& data)
{
  os << "_num_lines:" << data._num_lines << std::endl;
  os << "_num_pix_in_line:" << data._num_pix_in_line << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, AlosPalsarDataFileDescriptor& data)
{
  char buf6[7];
  buf6[6] = '\0';

  char buf168[169];
  buf168[168] = '\0';

  char buf94[95];
  buf94[94] = '\0';

  char buf8[9];
  buf8[8] = '\0';

  is.read(buf168, 168);

  is.read(buf6, 6);
  data._num_lines = atoi(buf6);

  is.read(buf94, 94);

  int tmpval;
  is.read(buf8, 8);
  tmpval = atoi(buf8);
  data._num_pix_in_line = tmpval / 8; // Assume data always in 8-byte complex format

  char buf432[433];
  buf432[432] = '\0';

  is.read(buf432, 432);
  return is;
}

AlosPalsarDataFileDescriptor::AlosPalsarDataFileDescriptor(const AlosPalsarDataFileDescriptor& rhs):
    AlosPalsarRecord(rhs),
    _num_pix_in_line(rhs._num_pix_in_line),
    _num_lines(rhs._num_lines)
{
}

AlosPalsarDataFileDescriptor& AlosPalsarDataFileDescriptor::operator=(const AlosPalsarDataFileDescriptor& rhs)
{
  _num_lines = rhs._num_lines;
  _num_pix_in_line = rhs._num_pix_in_line;
  return *this;
}
}
