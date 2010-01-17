//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  Frank Warmerdam (warmerdam@pobox.com)
//
// Description:
//
// Contains class declaration for ossimImageWriter
//*******************************************************************
//  $Id: ossimImageDisplayWriter.cpp 9963 2006-11-28 21:11:01Z gpotts $
#include <ossim/imaging/ossimImageDisplayWriter.h>

RTTI_DEF3(ossimImageDisplayWriter, "ossimImageDisplayWriter", ossimImageWriter, ossimDisplayInterface, ossimConnectableObjectListener);

ossimImageDisplayWriter::ossimImageDisplayWriter(ossimObject* owner)
   :ossimImageWriter(owner,
                     1,
                     0,
                     true,
                     true),
    ossimConnectableObjectListener()
{
}

ossimImageDisplayWriter::ossimImageDisplayWriter(ossimObject* owner,
                                                 ossim_uint32 numberOfInputs,
                                                 ossim_uint32 numberOfOutputs,
                                                 bool inputListIsFixed,
                                                 bool outputListIsFixed)
   : ossimImageWriter(owner,
                      numberOfInputs,
                      numberOfOutputs,
                      inputListIsFixed,
                      outputListIsFixed),
     ossimConnectableObjectListener()
{
}
