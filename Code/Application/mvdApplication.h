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

#ifndef __mvdApplication_h
#define __mvdApplication_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)
#include <QApplication>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

//
// Class pre-declaration.

//
// Class declaration.
namespace mvd
{
/** \class Application
 *
 */
class Application
  : public QApplication
{
  Q_OBJECT;

//
// Public methods.
public:

  /** Constructor */
  Application( int& argc, char** argv );

  /** Destructor */
  virtual ~Application();

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
  void InitializeCore();

  /**
   */
  void InitializeDataDir();

  /**
   */
  void InitializeLocale();

//
// Private attributes.
private:
  /**
   */
  bool m_IsRunningFromBuildDir;

//
// SLOTS
private slots:
  /**
   *
   */
  void OnAboutToQuit();
};

} // end namespace 'mvd'

#endif // __Application_h
