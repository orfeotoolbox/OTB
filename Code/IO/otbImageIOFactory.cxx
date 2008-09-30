/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbImageIOFactory.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

#include "otbONERAImageIOFactory.h"
#include "otbMSTARImageIOFactory.h"
#include "otbGDALImageIOFactory.h"
#include "otbLUMImageIOFactory.h"
#include "otbBSQImageIOFactory.h"
#include "otbRADImageIOFactory.h"

#ifdef OTB_COMPILE_JPEG2000
#include "otbJPEG2000ImageIOFactory.h"
#endif

#ifdef OTB_USE_CURL
#include "otbTileMapImageIOFactory.h"
#endif

namespace otb
{

  itk::ImageIOBase::Pointer
      ImageIOFactory::CreateImageIO(const char* path, FileModeType mode)
      {

        RegisterBuiltInFactories();
        return (Superclass::CreateImageIO(path,mode) );

      }
  
      void
          ImageIOFactory::RegisterBuiltInFactories()
      {
        static bool firstTime = true;

        static itk::SimpleMutexLock mutex;
        {
                // This helper class makes sure the Mutex is unlocked 
                // in the event an exception is thrown.
          itk::MutexLockHolder<itk::SimpleMutexLock> mutexHolder( mutex );
          if( firstTime )
          {
          
	    // RAD Format for OTB
            itk::ObjectFactoryBase::RegisterFactory( RADImageIOFactory::New() );            
                      
	    // BSQ format for OTB
            itk::ObjectFactoryBase::RegisterFactory( BSQImageIOFactory::New() );			
                        
	    // LUM format for OTB
            itk::ObjectFactoryBase::RegisterFactory( LUMImageIOFactory::New() );			

#ifdef OTB_COMPILE_JPEG2000
	    // JPEG2000 : New format for OTB
            itk::ObjectFactoryBase::RegisterFactory( JPEG2000ImageIOFactory::New() );
#endif
	    
	    // GDAL : New format for OTB
            itk::ObjectFactoryBase::RegisterFactory( GDALImageIOFactory::New() );            
           
	    // ONERA format for OTB
            itk::ObjectFactoryBase::RegisterFactory( ONERAImageIOFactory::New() );			
                        
	    // MSTAR Format for OTB
            itk::ObjectFactoryBase::RegisterFactory( MSTARImageIOFactory::New() );
            
#ifdef OTB_USE_CURL
	    // TileMap : New format for OTB
            itk::ObjectFactoryBase::RegisterFactory( TileMapImageIOFactory::New() );
#endif                        
            firstTime = false;
          }
        }

      }

} // end namespace otb
