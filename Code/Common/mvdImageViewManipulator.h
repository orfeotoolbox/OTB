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

#ifndef __mvdImageViewManipulator_h
#define __mvdImageViewManipulator_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)
#include <QtGui>

//
// ITK includes (sorted by alphabetic order)
#include "itkImageRegion.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbImageFileReader.h"

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

/** \class ImageViewManipulator
 *  Brief this class handles the event related to a QGLWidget. It also
 *  handles :
 *    - NavigationContext : to store the region of the image to be
 *      rendered.
 *    - MouseContext : to remember the user action as a mouse press,
 *                     mouse drag
 *  
 *   WIP
 */
class ImageViewManipulator :
    public QObject
{
  Q_OBJECT;

//
// Public methods.
public:

  typedef itk::ImageRegion<2>     ImageRegionType;

  /** Navigation context  */
  typedef struct 
  {
    ImageRegionType   bufferedRegion;
  } NavigationContextType;

  /** Mouse context */
  typedef struct 
  {
    int x;  // mousePress x
    int y;  // mousePress y
    int dx; // mouseMove in x (Drag)
    int dy; // mouseMove in y (Drag)
  } MouseContextType;

  /** Constructor */
  ImageViewManipulator( QObject* parent =NULL );

  /** Destructor */
  virtual ~ImageViewManipulator();

  /** */
  void mouseMoveEvent ( QMouseEvent * event );
  void mousePressEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );
  void resizeEvent ( QResizeEvent * event );

  /** Methods to access to the context */
  const NavigationContextType& GetNavigationContext() const
  {
    return m_NavigationContext;
  }

  const MouseContextType&  GetMouseContextType() const
  {
    return m_MouseContextType;
  }

//
// SIGNALS.
signals:

public slots:
  void InitializeContext(int width, int height);

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
  NavigationContextType  m_NavigationContext;
  MouseContextType       m_MouseContextType;

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdImageViewManipulator_h
