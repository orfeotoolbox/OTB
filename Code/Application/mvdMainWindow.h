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

#ifndef __mvdMainWindow_h
#define __mvdMainWindow_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)
#include <QtGui>

//
// OTB includes (sorted by alphabetic order)
#include "mvdImageView.h"
#include "itkRGBAPixel.h"
#include "otbVectorImage.h"
#include "otbImageLayerRenderingModel.h"
#include "otbImageFileReader.h"

//
// Monteverdi includes (sorted by alphabetic order)
// Warning! it's generally not a good idea to use namespaces in header files.
using namespace otb;
using namespace itk;

//
// External class pre-declaration.

namespace mvd
{
//
// Internal class pre-declaration.
namespace Ui
{
class MainWindow;
}

/** \class MainWindow
 *
 */
class MainWindow
  : public QMainWindow
{
  Q_OBJECT;

public:

  typedef otb::VectorImage<double, 2>     VectorImageType;
  typedef itk::RGBAPixel<unsigned char>   PixelType;
  typedef otb::Image<PixelType, 2>        ImageType;
  typedef ImageType::RegionType           RegionType;

  typedef otb::ImageFileReader<VectorImageType>    ReaderType;
  typedef otb::ImageLayerRenderingModel<ImageType> RenderingModelType;
  typedef mvd::ImageView<RenderingModelType>       ImageViewType;
  
  /** Constructor */
  MainWindow( QWidget* Parent =0, Qt::WindowFlags flags =0 );

  /** Destructor */
  virtual ~MainWindow();

protected:

private:
  void Initialize();

  Ui::MainWindow* m_UI;
  ImageViewType::Pointer m_ImageView;

private slots:
  void on_action_Open_activated();
};

} // end namespace 'mvd'

#endif // __MainWindow_h
