//----------------------------------------------------------------------------
//
// File: ossimCallback1wRet.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Simple templated callback class that takes a single parameter(argument) "P1"
// and has a return "RT".
//
// See link for more detail description:
// http://www.tutok.sk/fastgl/callback.html
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimCallback1wRet_HEADER
#define ossimCallback1wRet_HEADER 1

/**
 * @class ossimCallback1wRet
 *
 * Usage example, note template params can be anything:
 *
 * Caller interface to register callback:
 *
 * @code
 * void registerCallback(ossimCallback1wRet<const ossimFilename&, bool>* cb);
 * @endcode
 *
 * Callee creation of call back mechanism:
 *
 * @code
 * class ProcessFileCB: public ossimCallback1wRet<const ossimFilename&, bool>
 * {
 * public:
 *    ProcessFileCB(
 *       ossimImageElevationDatabase* obj,
 *       bool (ossimImageElevationDatabase::*func)(const ossimFilename&))
 *       :
 *       m_obj(obj),
 *       m_func(func)
 *    {}
 *       
 *    virtual bool operator()(const ossimFilename& file) const
 *    {
 *       return ( m_obj->*m_func)(file);
 *    }
 * 
 * private:
 *    ossimImageElevationDatabase* m_obj;
 *    bool (ossimImageElevationDatabase::*m_func)(const ossimFilename& file);
 * };
 * @endcode
 *
 * Making/registering a callback.
 *
 * @code
 * ossimFileWalker* fw = new ossimFileWalker();
 *
 * ossimCallback1wRet<const ossimFilename&, bool>* cb =
 *    new ProcessFileCB(this, &ossimImageElevationDatabase::processFile);
 *
 * fw->registerProcessFileCallback(cb);
 *
 * @endcode
 */
template <class P1, class RT> class ossimCallback1wRet
{
public:
   ossimCallback1wRet(){}
   virtual RT operator()(P1 p1) const = 0;
};

#endif
