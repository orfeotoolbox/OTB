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

#ifndef __mvdAbstractImageModel_h
#define __mvdAbstractImageModel_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)
#include <otbImage.h>
#include <otbImageFileReader.h>
#include <otbVectorImage.h>

//
// Monteverdi includes (sorted by alphabetic order)
#include "ConfigureMonteverdi2.h"
#include "mvdAbstractModel.h"

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.

/** \class AbstractImageModel
 *
 */
class Monteverdi2_EXPORT AbstractImageModel :
    public AbstractModel
{
  Q_OBJECT;

//
// Public methods.
public:
  /**
   */
  typedef otb::Image< Monteverdi2_FLOATING_TYPE, Monteverdi2_DIMENSION_TYPE >
    TImage2f;
  /**
   */
  typedef otb::ImageFileReader< TImage2f >
    TImageFileReader2f;
  /**
   */
  typedef
    otb::VectorImage< Monteverdi2_FLOATING_TYPE, Monteverdi2_DIMENSION_TYPE >
    TVectorImage2f;
  /**
   */
  typedef otb::ImageFileReader< TVectorImage2f >
    TVectorImageFileReader2f;


  /** Constructor */
  AbstractImageModel( QObject* parent =NULL );

  /** Destructor */
  virtual ~AbstractImageModel();

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

#endif // __mvdAbstractImageModel_h
