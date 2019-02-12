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


#include "erssar/ErsSarLeaderFactory.h"

#include "erssar/ErsSarDataSetSummary.h"
#include "erssar/ErsSarFileDescriptor.h"
#include "erssar/ErsSarMapProjectionData.h"
#include "erssar/ErsSarPlatformPositionData.h"
#include "erssar/ErsSarFacilityData.h"

namespace ossimplugins
{

ErsSarLeaderFactory::ErsSarLeaderFactory()
{
  RegisterRecord(5, new ErsSarFacilityData());
  RegisterRecord(4, new ErsSarPlatformPositionData());
  RegisterRecord(3, new ErsSarMapProjectionData());
  RegisterRecord(2, new ErsSarDataSetSummary());
  RegisterRecord(1, new ErsSarFileDescriptor());
}

ErsSarLeaderFactory::~ErsSarLeaderFactory()
{
  std::map<int, ErsSarRecord*>::iterator i = _availableRecords.begin();

  while (i != _availableRecords.end())
  {
    if ((*i).second)
    {
      delete(*i).second;
      (*i).second = 0;
    }
    ++i;
  }
  _availableRecords.clear();
}
}
