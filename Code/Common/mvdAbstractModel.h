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

#ifndef __mvdAbstractModel_h
#define __mvdAbstractModel_h

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
#include "mvdTypes.h"

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.

/** \class AbstractModel
 *
 */
class Monteverdi2_EXPORT AbstractModel :
    public QObject
{
  Q_OBJECT;

//
// Public methods.
public:

  /** Constructor */
  AbstractModel( QObject* parent =NULL );

  /** Destructor */
  virtual ~AbstractModel();

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

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdAbstractModel_h
