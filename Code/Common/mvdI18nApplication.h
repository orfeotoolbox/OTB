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

#ifndef __mvdI18nApplication_h
#define __mvdI18nApplication_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdTypes.h"

//
// Class pre-declaration.

//
// Class declaration.
namespace mvd
{
/** \class I18nApplication
 *
 */
class Monteverdi2_EXPORT I18nApplication
  : public QApplication
{
  Q_OBJECT;

//
// Public methods.
public:

  /** Constructor */
  I18nApplication( int& argc, char** argv );

  /** Destructor */
  virtual ~I18nApplication();

//
// SIGNALS.
signals:

//
// Protected methods.
protected:

//
// Private methods.
private:
  /**
   */
  void InitializeLocale();

  /**
   */
  bool LoadAndInstallTranslator( const QString& filename,
				 const QString& directory =QString(),
				 const QString& searchDelimiters =QString(),
				 const QString& suffix =QString() );

//
// Private attributes.
private:
  /**
   */
  bool m_IsRunningFromBuildDir;

//
// SLOTS
private slots:
};

} // end namespace 'mvd'

#endif // __I18nApplication_h
