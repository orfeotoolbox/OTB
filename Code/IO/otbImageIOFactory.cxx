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

#include "otbGDALImageIOFactory.h"
#include "otbONERAImageIOFactory.h"
#include "otbMSTARImageIOFactory.h"

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
                        // ONERA format for OTB
                        itk::ObjectFactoryBase::RegisterFactory( ONERAImageIOFactory::New() );			
                        // MSTAR Format for OTB
			itk::ObjectFactoryBase::RegisterFactory( MSTARImageIOFactory::New() );
                        // GDAL : New format for OTB
                        itk::ObjectFactoryBase::RegisterFactory( GDALImageIOFactory::New() );
                        firstTime = false;
                }
        }

}

} // end namespace otb
