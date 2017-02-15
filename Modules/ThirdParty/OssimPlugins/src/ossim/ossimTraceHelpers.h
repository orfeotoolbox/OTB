//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$


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
