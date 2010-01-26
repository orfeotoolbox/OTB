//*******************************************************************
//
// License:  See top level LICENSE.txt file.
// 
// Author:  David Burken
//
//**********************************************************************
// $Id: ossimPoolObject.h 9968 2006-11-29 14:01:53Z gpotts $
#ifndef ossimPoolObject_HEADER
#define ossimPoolObject_HEADER


/**
 * Templated container for an object pointer and a flag.
 */
template <class T>
class ossimPoolObject
{
 public:
   ~ossimPoolObject()
   {
      if(theObjectPtr)
      {
         cout << "deleting obj:  " << (hex) << theObjectPtr << endl;
         delete theObjectPtr;
         theObjectPtr = NULL;
      }
      theAvailableFlag = false;  
   }


   bool theAvailableFlag;
   T*   theObjectPtr;
};
#endif /* #ifndef ossimPoolObject_HEADER */
