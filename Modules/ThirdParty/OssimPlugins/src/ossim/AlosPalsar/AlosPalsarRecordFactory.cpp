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


#include <AlosPalsar/AlosPalsarRecordFactory.h>
#include <ossim/base/ossimTrace.h>

// Static trace for debugging
static ossimTrace traceDebug("AlosPalsarRecordFactory:debug");

namespace ossimplugins
{

AlosPalsarRecordFactory::AlosPalsarRecordFactory()
{
}

AlosPalsarRecordFactory::~AlosPalsarRecordFactory()
{
}

AlosPalsarRecord* AlosPalsarRecordFactory::Instantiate(int id)
{
  if (traceDebug())
  {
    ossimNotify(ossimNotifyLevel_DEBUG) << "Intanciate AlosPalsar record:" << id << "\n";
  }
  AlosPalsarRecord* record = _availableRecords[id];
  if (record == NULL)
  {
    return NULL;
  }
  else
  {
    return record->Instantiate();
  }
}

void AlosPalsarRecordFactory::RegisterRecord(int id, AlosPalsarRecord * record)
{
  _availableRecords[id] = record;
}

}
