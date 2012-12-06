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
// with void return.
//
// See link for more detail description:
// http://www.tutok.sk/fastgl/callback.html
// 
//----------------------------------------------------------------------------
// $Id$

#ifndef ossimCallback1_HEADER
#define ossimCallback1_HEADER 1

/**
 * @class ossimCallback1
 *
 * Usage example, note template params can be anything:
 *
 * Caller interface to register callback:
 *
 * @code
 * void registerCallback(ossimCallback1<const ossimFilename&>* cb);
 * @endcode
 *
 * Callee creation of call back mechanism:
 *
 * @code
 * class ProcessFileCB: public ossimCallback1<const ossimFilename&>
 * {
 * public:
 *    ProcessFileCB(
 *       ossimImageElevationDatabase* obj,
 *       void (ossimImageElevationDatabase::*func)(const ossimFilename&))
 *       :
 *       m_obj(obj),
 *       m_func(func)
 *    {}
 *       
 *    virtual void operator()(const ossimFilename& file) const
 *    {
 *       ( m_obj->*m_func)(file);
 *    }
 * 
 * private:
 *    ossimImageElevationDatabase* m_obj;
 *    void (ossimImageElevationDatabase::*m_func)(const ossimFilename& file);
 * };
 * @endcode
 *
 * Making/registering a callback.
 *
 * @code
 * ossimFileWalker* fw = new ossimFileWalker();
 *
 * ossimCallback1wRet<const ossimFilename&>* cb =
 *    new ProcessFileCB(this, &ossimImageElevationDatabase::processFile);
 *
 * fw->registerProcessFileCallback(cb);
 *
 * @endcode
 */
template <class P1> class ossimCallback1
{
public:
   ossimCallback1(){}
   virtual ~ossimCallback1(){}
   virtual void operator()(P1 p1) const = 0;
};

#endif /* #ifndef ossimCallback1_HEADER */
