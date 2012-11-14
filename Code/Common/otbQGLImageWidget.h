#ifndef __otbQGLImageWidget_h__
#define __otbQGLImageWidget_h__

#include <QtOpenGL/qgl.h>

#include "itkRGBAPixel.h"
#include "itkAffineTransform.h"
#include "otbImage.h"

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


namespace otb
{

class QGLImageWidget : public QGLWidget
{
  Q_OBJECT
public:
  QGLImageWidget(QWidget *parent = 0);

  ~QGLImageWidget();

public:
  virtual void ReadBuffer(const ImageType * image, const RegionType& region);

  void ClearBuffer();

  /** Set/Get the Isotropic zoom */
  void SetIsotropicZoom(double value)
  {
    m_IsotropicZoom = value;
  }

  double GetIsotropicZoom() const
  {
    return m_IsotropicZoom;
  }

protected:

  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  /** Update the image to screen transform */
  //void UpdateTransforms(int w, int h);

  /** Compute the linear buffer index according to the 2D region and
  * its 2D index.This method is used when OTB_GL_USE_ACCEL is ON.
  * \param index 2D index
  * \param region 2D region
  */
  static inline unsigned int ComputeBufferIndex(const IndexType& index, const RegionType& region)
  {
    return (index[1] - region.GetIndex()[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
  }

  /** Compute the linear buffer index according to the 2D region and
   * its 2D index.This method is used when OTB_GL_USE_ACCEL is OFF.
   * The resulting buffer will be flipped over the X axis.
   * \param index 2D index
   * \param region 2D region
   */
  static inline unsigned int ComputeXAxisFlippedBufferIndex(const IndexType& index, const RegionType& region)
  {
    return (region.GetSize()[1] - 1 + region.GetIndex()[1] -
            index[1]) * 3 * region.GetSize()[0] + 3 * (index[0] - region.GetIndex()[0]);
  }

private:

  /** OpenGl zoom factor */
  double m_IsotropicZoom;

  /** OpenGl buffer      */
  unsigned char * m_OpenGlBuffer;

  /** OpenGl buffered region */
  RegionType m_OpenGlBufferedRegion;

  /** The display extent */
  RegionType m_Extent;

  GLint m_W, m_H;

  /** Space to screen transform */
  //AffineTransformType::Pointer m_ImageToScreenTransform;
  //AffineTransformType::Pointer m_ScreenToImageTransform;

};

}

#endif

