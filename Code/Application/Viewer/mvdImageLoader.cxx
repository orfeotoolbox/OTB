/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdApplication.h"
#include "mvdImageLoader.h"
#include "Core/mvdDatasetModel.h"

namespace mvd
{

ImageLoader
::ImageLoader( const QString& filename, int width, int height )
: m_Filename(filename),
  m_Width(width),
  m_Height(height)
{
}

void
ImageLoader
::OpenImage()
{
  // This method is executed in a separate thread

#if 0
  class Sleeper : public QThread
  {
    public:
    static void sleep(unsigned long secs) {
      QThread::sleep(secs);
      }
  };

  Sleeper::sleep( 5 );
#endif

  try
    {
    DatasetModel* model = Application::LoadDatasetModel(
      m_Filename, m_Width, m_Height, false
    );

    // We can only push to another thread,
    // so thread affinity must be set here,
    // and not in the slot that receives the object
    model->moveToThread(Application::Instance()->thread());
    
    emit ModelLoaded(model);
    }
  catch( std::exception& exc )
    {
    emit Error( exc.what() );
    }
  emit Finished();
}

} // end namespace 'mvd'
