//********************************************************************
// Copyright (C) 2004 Intelligence-data, Inc.  All rights reserved.
//
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author:  Garrett Potts
//
//**********************************************************************
// $Id: ossimPool.h,v 1.2 2004/04/20 21:26:05 dburken Exp $
#ifndef ossimPool_HEADER
#define ossimPool_HEADER
#include <iostream>
#include <vector>
#include <exception>
#include <stdexcept>
#include <base/common/ossimCommon.h>
#include <base/common/ossimConstants.h>
#include <base/common/ossimPoolObject.h>
#include <base/context/ossimNotifyContext.h>

#define OSSIM_DEFAULT_POOL_SIZE (ossim_uint32)10

/**
 * Container pool to be used for things like threads.
 */
template <class T>
class ossimPool
{
 public:
   /**
    * Contructor
    * @param n Number of pooled objects to create.
    */
   ossimPool(ossim_uint32 n=OSSIM_DEFAULT_POOL_SIZE) throw(std::bad_alloc)
      : thePool(n)
   {
      for(ossim_uint32 idx = 0; idx < n; ++idx)
      {
         try
         {
               ossimPoolObject<T>* poolObject = new ossimPoolObject<T>();
               poolObject->theAvailableFlag = true;
               poolObject->theObjectPtr = new T();
               thePool[idx] = poolObject;
         }
         catch(std::bad_alloc)
         {
            ossimNotify(ossimNotifyLevel_FATAL)
               << "FATAL ossimPool(): Bad allocation" << endl;
         }
      }
   }

   /**
    * Destructor
    */
   ~ossimPool()
   {
      thePool.clear();
   }

   /**
    * @return A pooled object of type T*.  Will return null if the pool is
    * used up or pool size is zero.
    */
   T* checkout()
   {
      T* t = NULL;
      
      theCriticalSectionMutex.lock();
      for (ossim_uint32 idx = 0; idx < thePool.size(); ++idx)
      {
         if (thePool[idx]->theAvailableFlag == true)
         {
            thePool[idx]->theAvailableFlag = false;
            t = thePool[idx]->theObjectPtr;
            break;
         }
      }
      theCriticalSectionMutex.unlock();
      return t;
   }

   /**
    * Resets and eturns object to the pool.
    * @param obj Object to be returned to the pool.
    * 
    * @note The object is required to have a "reset()" method.
    *
    * @return Returns void unless object was not from pool then throws
    * a std::out_of_range error.
    */
   void checkin(T* obj) throw(std::out_of_range)
   {
      theCriticalSectionMutex.lock();
      for (ossim_uint32 idx = 0; idx < thePool.size(); ++idx)
      {
         if (obj == thePool[idx]->theObjectPtr)
         {
            obj->reset();
            thePool[idx]->theAvailableFlag = true;
            theCriticalSectionMutex.unlock();
            return;
         }
      }
      
      theCriticalSectionMutex.unlock();
      throw std::out_of_range(string("ossimPool<T>::checkin Error Object not managed by this pool!"));
   }

   /**
    * Outputs object addresses and theAvailableFlag.
    */
   std::ostream& print(std::ostream& out) const
   {
      for (ossim_uint32 idx = 0; idx < thePool.size(); ++idx)
      {
         out << "obj address:  " << (hex) << thePool[idx]->theObjectPtr
             << " is "
             << (thePool[idx]->theAvailableFlag ? "available":"unavailable")
             << endl;
      } 
      return out;
   }
   
   template <class C> friend std::ostream& operator<<(std::ostream& out,
                                                      const ossimPool<C>& obj)
   {
      return obj.print(out);
   }

protected:
   
   std::vector<ossimPoolObject<T>*> thePool;
};

#endif /* #ifndef ossimPool_HEADER */

