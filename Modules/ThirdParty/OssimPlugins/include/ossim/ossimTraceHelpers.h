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



#ifndef ossimTraceHelpers_h
#define ossimTraceHelpers_h

#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimNotify.h>

namespace ossimplugins {
   /** Helper class to log automatically entering and leaving scopes.
    * @warning Not meant to be used directly. Use \c SCOPED_LOG instead.
    */
   struct ScopedLogger
   {
      ScopedLogger(ossimTrace & channel, char const* module, ossimNotifyLevel level = ossimNotifyLevel_DEBUG)
         : m_channel(channel)
         , MODULE(module)
         {
            if (m_channel()) {
               ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " entered...\n";
            }
         }

      ~ScopedLogger() {
         if (m_channel()) {
            ossimNotify(ossimNotifyLevel_DEBUG) << MODULE << " left...\n";
         }
      }
   private:
      ScopedLogger(ScopedLogger const&);
      ScopedLogger& operator=(ScopedLogger const&);

      ossimTrace & m_channel;
      char const* MODULE;
   };


#define SCOPED_LOG(channel, msg) \
   SCOPED_LOG_NAME(__LINE__)(channel, msg)

#define SCOPED_LOG_NAME(x) \
   SCOPED_LOG_NAME0(x)

#define SCOPED_LOG_NAME0(x) \
   ossimplugins::ScopedLogger slog ## x
} // ossimplugins namespace


#endif // ossimTraceHelpers_h
