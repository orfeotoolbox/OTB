//----------------------------------------------------------------------------
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description: Base class for Info object.
// 
//----------------------------------------------------------------------------
// $Id$

#include <ossim/support_data/ossimInfoBase.h>

ossimInfoBase::ossimInfoBase()
   : theOverviewFlag(true)
{}

ossimInfoBase::~ossimInfoBase()
{}

void ossimInfoBase::setProcessOverviewFlag(bool flag)
{
   theOverviewFlag = flag;
}

bool ossimInfoBase::getProcessOverviewFlag() const
{
   return theOverviewFlag;
}
