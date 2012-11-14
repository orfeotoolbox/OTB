/*=========================================================================

  Program:   qTutor
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) 
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __Application_h
#define __Application_h

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
//
public:

/** Constructor */
  Application( int& argc, char** argv );

/** Destructor */
  virtual ~Application();

//
//
protected:

//
// SLOTS
private slots:
/**
*
*/
  void OnAboutToQuit();

//
//
private:
};

} // end namespace 'mvd'

#endif // __Application_h
