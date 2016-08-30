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
#ifndef __mvdAboutDialog_h
#define __mvdAboutDialog_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi.h"

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

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
// Internal classes pre-declaration.
namespace Ui
{
class AboutDialog;
}

/** \class AboutDialog
 *
 * \ingroup OTBMonteverdiGui
 *
 */
class ITK_EXPORT AboutDialog :
    public QDialog
{
  Q_OBJECT;

//
// Public methods.
public:

  /** Constructor */
  AboutDialog( QWidget* p =NULL, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~AboutDialog();

//
// SIGNALS.
signals:

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

//
// Private methods.
private:


//
// Private attributes.
private:
  /**
   */
  Ui::AboutDialog* m_UI;

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdAboutDialog_h
