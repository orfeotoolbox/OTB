#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "otbImageIOFactory.h"
#include "itkMutexLock.h"
#include "itkMutexLockHolder.h"

#include "otbCAIImageIOFactory.h"
#include "otbGDALImageIOFactory.h"

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
                        // CAI : Nouveau format de l'OTB
                        itk::ObjectFactoryBase::RegisterFactory( CAIImageIOFactory::New() );
                        // GDAL : Nouveau format de l'OTB
//                        itk::ObjectFactoryBase::RegisterFactory( GDALImageIOFactory::New() );
                        firstTime = false;
                }
        }

}

} // end namespace otb
