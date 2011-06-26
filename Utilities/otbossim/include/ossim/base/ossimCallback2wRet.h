//----------------------------------------------------------------------------
//
// File: ossimCallback2wRet.h
// 
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//
// Description:
//
// Simple templated callback class that takes a two parameters(arguments) "P1"
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
 * @class ossimCallback2wRet
 *
 * Usage example, note template params can be anything:
 *
 * Caller interface to register callback:
 *
 * @code
 * void registerCallback(ossimCallback2wRet<const ossimFilename&, bool&, bool>* cb);
 * @endcode
 *
 * Callee creation of call back mechanism:
 *
 * @code
 * class ProcessFileCB: public ossimCallback2wRet<const ossimFilename&, bool&, bool>
 * {
 * public:
 *    ProcessFileCB(
 *       ossimImageElevationDatabase* obj,
 *       bool (ossimImageElevationDatabase::*func)(const ossimFilename&, bool&))
 *       :
 *       m_obj(obj),
 *       m_func(func)
 *    {}
 *       
 *    virtual bool operator()(const ossimFilename& file, bool& flag) const
 *    {
 *       return ( m_obj->*m_func)(file, flag);
 *    }
 * 
 * private:
 *    ossimImageElevationDatabase* m_obj;
 *    bool (ossimImageElevationDatabase::*m_func)(const ossimFilename& file, bool& flag);
 * };
 * @endcode
 *
 * Making/registering a callback.
 *
 * @code
 * ossimFileWalker* fw = new ossimFileWalker();
 *
 * ossimCallback2wRet<const ossimFilename&, bool&, bool>* cb =
 *    new ProcessFileCB(this, &ossimImageElevationDatabase::processFile);
 *
 * fw->registerProcessFileCallback(cb);
 *
 * @endcode
 */

template <class P1, class P2, class RT> class ossimCallback2wRet
{
public:
   ossimCallback2wRet(){}
   virtual RT operator()(P1 p1, P2 p2) const = 0;
};

#endif
