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


#include <AlosPalsar/AlosPalsarPositionVectorRecord.h>

namespace ossimplugins
{


AlosPalsarPositionVectorRecord::AlosPalsarPositionVectorRecord()
{
}

AlosPalsarPositionVectorRecord::~AlosPalsarPositionVectorRecord()
{
}

std::ostream& operator<<(std::ostream& os, const AlosPalsarPositionVectorRecord& data)
{
  for (int i = 0; i < 3; i++)
  {
    os << "pos[" << i << "]:" << data._pos[i] << std::endl;
  }

  for (int i = 0; i < 3; i++)
  {
    os << "vel[" << i << "]:" << data._vel[i] << std::endl;
  }
  return os;
}

std::istream& operator>>(std::istream& is, AlosPalsarPositionVectorRecord& data)
{
  char buf[23];
  buf[22] = '\0';

  for (int i = 0; i < 3; i++)
  {
    is.read(buf, 22);
    data._pos[i] = atof(buf);
  }

  for (int i = 0; i < 3; i++)
  {
    is.read(buf, 22);
    data._vel[i] = atof(buf);
  }
  return is;
}

AlosPalsarPositionVectorRecord::AlosPalsarPositionVectorRecord(const AlosPalsarPositionVectorRecord& rhs)
{
  for (int i = 0; i < 3; i++)
  {
    _pos[i] = rhs._pos[i];
  }

  for (int i = 0; i < 3; i++)
  {
    _vel[i] = rhs._vel[i];
  }
}

AlosPalsarPositionVectorRecord& AlosPalsarPositionVectorRecord::operator=(const AlosPalsarPositionVectorRecord& rhs)
{
  for (int i = 0; i < 3; i++)
  {
    _pos[i] = rhs._pos[i];
  }

  for (int i = 0; i < 3; i++)
  {
    _vel[i] = rhs._vel[i];
  }
  return *this;
}

}
