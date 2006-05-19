#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbImageIOFactory.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

#include "otbCAIImageIOFactory.h"
#include "otbGDALImageIOFactory.h"
#include "otbONERAImageIOFactory.h"

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
                        //For an SPOT5TIF image, if the user set only the directory, this image is read with CAI.
                        //Otherwise, if the user set the name of the image ('IMAGERY.TIF') it read with GDAL.
                        
                         // ONERA : New format for OTB
                        itk::ObjectFactoryBase::RegisterFactory( ONERAImageIOFactory::New() );
                        // GDAL : New format for OTB
                        itk::ObjectFactoryBase::RegisterFactory( GDALImageIOFactory::New() );
                        // CAI : New format for OTB
                        itk::ObjectFactoryBase::RegisterFactory( CAIImageIOFactory::New() );
                       firstTime = false;
                }
        }

}

} // end namespace otb
