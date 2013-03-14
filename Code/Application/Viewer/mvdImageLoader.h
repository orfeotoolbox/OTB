/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdImageLoader_h
#define __mvdImageLoader_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtCore>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{

namespace core
{
//
// Internal class pre-declaration.
class AbstractModel;
}

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
  * \class ImageLoader
  *
  * \brief Imports an image into a Monteverdi2 dataset
  *
  * This class is used to deport the dataset creation
  * in a separate thread
  */
class ImageLoader : public QObject
{
    Q_OBJECT;

public:
  /** \brief Constructor */
  ImageLoader( const QString& filename, int width, int height );
    
public slots:
  /** \brief Trigger the dataset creation */
  void OpenImage();

signals:
  /** \brief Triggered after model is ready */
  void ModelLoaded( core::AbstractModel* );

  /** \brief Triggered when processing is finished */
  void Finished();
  
  /** \brief Triggered when an error occurs during dataset creation */
  void Error( QString err );
  
private:
  QString m_Filename;
  int m_Width;
  int m_Height;
};
  
} // end namespace 'mvd'

#endif // __mvdImageModel_h
