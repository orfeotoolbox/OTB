#include "otbQGLImageWidget.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{

QGLImageWidget::QGLImageWidget(QWidget *parent)
: QGLWidget(parent),
  m_IsotropicZoom(1.0),
  m_OpenGlBuffer(NULL),
  m_OpenGlBufferedRegion(),
  m_Extent()
/*,  m_ImageToScreenTransform(AffineTransformType::New()),
  m_ScreenToImageTransform(AffineTransformType::New()) */
{
}

QGLImageWidget::~QGLImageWidget()
{
  this->ClearBuffer();
}

void QGLImageWidget::ReadBuffer(const ImageType * image, const RegionType& region)
{
  // Before doing anything, check if region is inside the buffered
  // region of image
  if (!image->GetBufferedRegion().IsInside(region))
    {
      //itkExceptionMacro(<< "Region to read is oustside of the buffered region.");
    }
  // Delete previous buffer if needed
  this->ClearBuffer();

  // Allocate new memory
  m_OpenGlBuffer = new unsigned char[3 * region.GetNumberOfPixels()];

  // Declare the iterator
  itk::ImageRegionConstIteratorWithIndex<ImageType> it(image, region);

  // Go to begin
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    // Fill the buffer
    unsigned int index = 0;
    index = ComputeXAxisFlippedBufferIndex(it.GetIndex(), region);

    // Fill the buffer
    m_OpenGlBuffer[index]  = it.Get()[0];
    m_OpenGlBuffer[index + 1] = it.Get()[1];
    m_OpenGlBuffer[index + 2] = it.Get()[2];
    ++it;
    }
  // Last, updating buffer size
  m_OpenGlBufferedRegion = region;
}

void QGLImageWidget::ClearBuffer()
{
  // Delete previous buffer if needed
  if (m_OpenGlBuffer != NULL)
    {
    delete[] m_OpenGlBuffer;
    m_OpenGlBuffer = NULL;
    }

  // Last, updating buffer size
  m_OpenGlBufferedRegion = RegionType();
}

/*
void QGLImageWidget::UpdateTransforms(int w, int h)
{
  std::cout << "UpdateTransforms" << std::endl;

  if (m_IsotropicZoom <= 0)
    {
    //itkExceptionMacro(<< "Internal error: Isotropic zoom should be non null positive.");
    }

  RegionType::IndexType index;
  RegionType::SizeType  size;
  // Update image extent
  size[0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[0]));
  size[1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[1]));
  index[0] = (w - size[0]) / 2;
  index[1] = (h - size[1]) / 2;
  m_Extent.SetIndex(index);
  m_Extent.SetSize(size);

  // Image to screen matrix
  AffineTransformType::MatrixType s2iMatrix;
  s2iMatrix.Fill(0);
  const double s2iSpacing = (m_SubsamplingRate) / m_IsotropicZoom;
  s2iMatrix(0, 0) = s2iSpacing;
  s2iMatrix(1, 1) = -s2iSpacing;
  m_ScreenToImageTransform->SetMatrix(s2iMatrix);

  // Image to screen translation
  AffineTransformType::OutputVectorType translation;
  translation[0] = m_SubsamplingRate * (m_OpenGlBufferedRegion.GetIndex()[0] - m_Extent.GetIndex()[0] / m_IsotropicZoom);
  translation[1] = m_SubsamplingRate *
                   (((m_Extent.GetIndex()[1] +
                      m_Extent.GetSize()[1]) / m_IsotropicZoom) + m_OpenGlBufferedRegion.GetIndex()[1]);
  m_ScreenToImageTransform->SetTranslation(translation);


  // Compute the inverse transform
  bool couldInvert = m_ScreenToImageTransform->GetInverse(m_ImageToScreenTransform);
  if (couldInvert == false)
    {
    itkExceptionMacro(<< "Internal error: Could not invert ScreenToImageTransform.");
    }

}
*/

void QGLImageWidget::initializeGL()
 {
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
 }

 void QGLImageWidget::resizeGL(int w, int h)
 {
   RegionType::SizeType size;
   size [0] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[0]));
   size [1] = static_cast<unsigned int>(m_IsotropicZoom * static_cast<double>(m_OpenGlBufferedRegion.GetSize()[1]));

   RegionType::IndexType index;
   index[0] = (w - static_cast<int>(size[0])) / 2;
   index[1] = (h - static_cast<int>(size[1])) / 2;

   m_Extent.SetIndex(index);
   m_Extent.SetSize(size);

   m_W = (GLint)w;
   m_H =  (GLint)h;

  glViewport(0, 0, m_W, m_H);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, m_W, 0, m_H, -1, 1);

}

 void QGLImageWidget::paintGL()
 {
   unsigned int nb_displayed_rows;
   unsigned int nb_displayed_cols;
   unsigned int first_displayed_row;
   unsigned int first_displayed_col;

   if( m_Extent.GetIndex()[0] >= 0 )
    {
    nb_displayed_cols = m_OpenGlBufferedRegion.GetSize()[0];
    first_displayed_col = 0;
    }
   else
    {
    nb_displayed_cols = m_W / m_IsotropicZoom;
    first_displayed_col = (m_OpenGlBufferedRegion.GetSize()[0] - nb_displayed_cols) / 2;
    }

   if( m_Extent.GetIndex()[1] >= 0 )
    {
    nb_displayed_rows = m_OpenGlBufferedRegion.GetSize()[1];
    first_displayed_row = 0;
    }
   else
    {
    nb_displayed_rows = m_H / m_IsotropicZoom;
    first_displayed_row = (m_OpenGlBufferedRegion.GetSize()[1] - nb_displayed_rows) / 2;
    }


   RegionType::IndexType startPosition = m_Extent.GetIndex();
   startPosition[0] = startPosition[0] < 0 ? 0 : startPosition[0];
   startPosition[1] = startPosition[1] < 0 ? 0 : startPosition[1];

   glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
   glPixelStorei(GL_UNPACK_ROW_LENGTH, m_OpenGlBufferedRegion.GetSize()[0]);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS, first_displayed_col);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,first_displayed_row);

   glClear(GL_COLOR_BUFFER_BIT);
   glPixelZoom(m_IsotropicZoom,m_IsotropicZoom);

   glRasterPos2f(startPosition[0], startPosition[1]);
   glDrawPixels(nb_displayed_cols,
                nb_displayed_rows,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                m_OpenGlBuffer);


   glFlush();
 }

}
