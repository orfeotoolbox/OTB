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
#ifndef __mvdGLImageWidget_h
#define __mvdGLImageWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"

//
// Qt includes (sorted by alphabetic order)
#include <QtOpenGL>

//
// System includes (sorted by alphabetic order)

// this include must be added here to avoid compilation problem
#include "mvdTypes.h"

//
// ITK includes (sorted by alphabetic order)
#include "itkRGBAPixel.h"
#include "itkAffineTransform.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbImage.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractImageModel.h"
#include "mvdImageModelRenderer.h"
#include "mvdImageViewManipulator.h"

//
// Global definitions.
const unsigned int Dimension = 2;
typedef itk::RGBAPixel<unsigned char>     PixelType;
typedef otb::Image<PixelType, Dimension>  ImageType;
typedef ImageType::RegionType             RegionType;
typedef ImageType::IndexType              IndexType;
typedef ImageType::SizeType               SizeType;

// affine transform
typedef itk::AffineTransform<double, 2>      AffineTransformType;
typedef AffineTransformType::InputPointType  PointType;
typedef AffineTransformType::InputVectorType VectorType;
typedef itk::FixedArray<double, 4>           ColorType;

namespace mvd
{

/** \class GLImageWidget
 *
 */
class Monteverdi2_EXPORT GLImageWidget : public QGLWidget
{
  Q_OBJECT;

public:
  
  typedef ImageViewManipulator        ImageViewEventHandlerType;
  typedef ImageModelRenderer          ImageModelRendererType;

  GLImageWidget(QWidget *parent = 0);

  virtual ~GLImageWidget();

public:

  /** Set/Get the Isotropic zoom */
  inline void SetIsotropicZoom(double value)
  {
    //TODO: Implement setter.
  }

  double GetIsotropicZoom() const
  {
    // TODO: Implement getter.
    return 1.0;
  }

public slots:
  void onLargestPossibleRegionChanged(const ImageRegionType& largestRegion);

protected:

  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();
  void mouseMoveEvent ( QMouseEvent * event );
  void mousePressEvent ( QMouseEvent * event );
  void resizeEvent(QResizeEvent* event);

  /** Update the image to screen transform */
  //void UpdateTransforms(int w, int h);

private:

  /** Space to screen transform */
  //AffineTransformType::Pointer m_ImageToScreenTransform;
  //AffineTransformType::Pointer m_ScreenToImageTransform;

  /** Event handler pointer */
  ImageViewEventHandlerType* m_ImageViewManipulator;

  /** Model Renderer pointer */
  ImageModelRendererType   * m_ImageModelRenderer;
};

}

#endif
