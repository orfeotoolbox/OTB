/*=========================================================================

  Program:   Monteverdi
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdMainWindowTitleLoader_h
#define __mvdMainWindowTitleLoader_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

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
 * \class MainWindowTitleLoader
 *
 * \ingroup OTBMonteverdiGui
 *
 * \brief Imports an image into a Monteverdi dataset
 *
 * This class is used to deport the dataset creation
 * in a separate thread
 */
class ITK_EXPORT MainWindowTitleLoader : public QObject
{
    Q_OBJECT;

public:
  /** \brief Constructor */
  MainWindowTitleLoader(VectorImageModel* model);
    
public slots:
  /** \brief Trigger the dataset creation */
  void LoadTitle();

signals:
  /** \brief Triggered after model is ready */
  void TitleLoaded( const QString& );

  /** \brief Triggered when processing is finished */
  void Finished();
  
private:

  VectorImageModel* m_Model;
};
  
} // end namespace 'mvd'

#endif // __mvdImageModel_h
