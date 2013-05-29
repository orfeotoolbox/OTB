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

#ifndef __mvdImagePlacenameLoader_h
#define __mvdImagePlacenameLoader_h

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

//
// Internal class pre-declaration.
class VectorImageModel;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
  * \class ImagePlacenameLoader
  *
  * \brief 
  *
  */
class ImagePlacenameLoader : public QObject
{
    Q_OBJECT;

public:
  /** \brief Constructor */
  ImagePlacenameLoader(VectorImageModel* model);
    
public slots:
  /** \brief load the placename */
  void LoadPlacename();

signals:
  /** \brief Triggered after a non empty placename was loaded */
  void PlacenameLoaded( const QString& );

  /** \brief Triggered when processing is finished */
  void Finished();
  
private:

  VectorImageModel* m_Model;
};
  
} // end namespace 'mvd'

#endif // __mvdImageModel_h
