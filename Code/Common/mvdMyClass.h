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

#ifndef __mvdMyClass_h
#define __mvdMyClass_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)
#include <QObject>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

//
// Class pre-declaration.
namespace
{
}

namespace mvd
{
/** \class MyClass
 *
 */
class MyClass :
    public QObject
{
  Q_OBJECT;

public:

/** Constructor */
  MyClass();

/** Destructor */
  virtual ~MyClass();

signals:

protected:

private:

private slots:
};

} // end namespace 'mvd'

#endif // __mvdMyClass_h
