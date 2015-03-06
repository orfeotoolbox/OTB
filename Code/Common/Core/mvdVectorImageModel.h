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

#ifndef __mvdVectorImageModel_h
#define __mvdVectorImageModel_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)
#include "itkExtractImageFilter.h"

//
// OTB includes (sorted by alphabetic order)
#include "otbGenericRSTransform.h"

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAbstractImageModel.h"
#include "Core/mvdVectorImageSettings.h"
#include "Core/mvdTypes.h"
#include "Gui/mvdColorSetupWidget.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
class DatasetModel;


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class VectorImageModel
 *
 */
class Monteverdi2_EXPORT VectorImageModel :
    public AbstractImageModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   * In-memory storage type of source image (from file).
   */
  typedef DefaultImageType SourceImageType;

//
// Public methods.
public:

  /** Constructor */
  VectorImageModel( QObject* parent =NULL );

  /** Destructor */
  virtual ~VectorImageModel();

  /**
   * \brief Get the parent DatasetModel.
   *
   * \return The parent DatasetModel.
   */
  virtual const DatasetModel* GetDatasetModel() const;

  /**
   * \brief Get the parent DatasetModel.
   *
   * \return The parent DatasetModel.
   */
  virtual DatasetModel* GetDatasetModel();

  /** */
  static void EnsureValidImage( const QString& filename );

  /** */
  // TODO: Move into template wrapper base-class.
  SourceImageType::ConstPointer ToImage() const;

  /** */
  // TODO: Move into template wrapper base-class.
  const SourceImageType::Pointer& ToImage();

  /**
   */
  inline const VectorImageSettings & GetSettings() const;

  /** */
  inline VectorImageSettings & GetSettings();

  /** */
  inline void SetSettings( const VectorImageSettings & settings );

  /** */
  inline otb::GenericRSTransform<> * GetGenericRSTransform();

  /**
   * Width and height are added to compute the best level of detail to
   * load from the image if multi-resolution image.
   */
  void SetFilename( const QString& filename, int width, int height );

  /** Get input filename */
  inline QString GetFilename() const;

  /**
   * Following the zoom factor, get the best level of detail
   */
  CountType ComputeBestLod( double ZoomFactor ) const;

  /**
   * Following the zoom factor, get the best level of detail
   */
  CountType ComputeBestLod( int width, int height ) const;

  //
  // AbstractImageModel overrides.

  /** 
   * Get the number of available LOD.
   */
  virtual CountType GetNbLod() const;

  /**
   * Get a smart-pointer to the current LOD image-base.
   */
  virtual ImageBaseType::ConstPointer ToImageBase() const;

  /**
   * Get a smart-pointer to the current LOD image-base.
   */
  virtual ImageBaseType::Pointer ToImageBase();

  /**
   * Get the placename from the center pixel
   */
  std::string GetCenterPixelPlaceName();

  //
  // AbstractModel overrides.

  virtual bool IsModified() const;

  virtual void ClearModified();

  // get image size in byte
  std::streamoff GetImageSizeInBytes()
  {
    return m_ImageFileReader->GetImageIO()->GetImageSizeInBytes();
  }

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public slots.
public slots:
  /**
   */
  void OnPhysicalCursorPositionChanged( const QPoint &,
                                        const PointType &,
                                        const PointType &,
                                        const DefaultImageType::PixelType & );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void SettingsUpdated( AbstractImageModel* imageModel =NULL );

  /**
   */
  void PropertiesUpdated( AbstractImageModel* imageModel =NULL );

  /**   */
  void ViewportRegionChanged(double, double);

  /** */
  void CurrentIndexUpdated( const IndexType&, bool isInside );
  void CurrentIndexUpdated(const QString&);
  void CurrentPhysicalUpdated(const QStringList&);
  void CurrentGeographicUpdated(const QStringList&);
  void CurrentRadioUpdated(const QString&);
  void CurrentPixelValueUpdated(const VectorImageType::PixelType &, 
                                const QStringList& );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModel overrides.

  virtual void virtual_BuildModel( void* context =NULL );

  //
  // AbstractImageModel overrides.

  void InitializeColorSetupSettings();

  void InitializeColorDynamicsSettings();

//
// Protected attributes.
protected:

  /** */
  SourceImageType::Pointer m_Image;

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private types.
private:

//
// Private methods.
private:

  /**
    * Helper method to get the best closest Jpeg2K level of detail.
    */
  static
    unsigned int
    Closest(double invZoomfactor,
	    const std::vector<unsigned int> & res);

  /** 
    * helper to setup the lod image using a width/height or a zoom factor
    */
  void SetupCurrentLodImage(int w, int h);

  /**
   */
  void ApplySettings();

  /**
   */
  void BuildGdalOverviews();

  //
  // AbstractImageModel overrides.

  virtual void virtual_SetCurrentLod( CountType lod );

  virtual void virtual_RefreshHistogram();

//
// Private attributes.
private:
  // Default image reader
  DefaultImageFileReaderType::Pointer m_ImageFileReader;

  /**
   * User-configurable settings of image-model.
   */
  VectorImageSettings m_Settings;

  // store the input image filename
  QString m_Filename;

  /** List of all Level of detail (Resolution) available from the file */
  UIntVector m_AvailableLod;
  
  //  Generic RS Transform to get lat/long coordinates 
  otb::GenericRSTransform<>::Pointer m_GenericRSTransform;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /** */
  void OnModelUpdated();
};

} // end namespace 'mvd'.

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdDatasetModel.h"


namespace mvd
{

/*****************************************************************************/
inline
VectorImageModel::SourceImageType::ConstPointer
VectorImageModel
::ToImage() const
{
  // TODO: Fix unsafe weak-pointer dereferencing.
  return otb::ConstCast< VectorImageModel::SourceImageType >( m_Image );
}

/*****************************************************************************/
inline
const VectorImageModel::SourceImageType::Pointer&
VectorImageModel
::ToImage()
{
  return m_Image;
}

/*****************************************************************************/
inline
void
VectorImageModel
::SetSettings( const VectorImageSettings & settings )
{
  m_Settings = settings;
}

/*****************************************************************************/
inline
const VectorImageSettings &
VectorImageModel
::GetSettings() const
{
  return m_Settings;
}

/*****************************************************************************/
inline
VectorImageSettings &
VectorImageModel
::GetSettings()
{
  return m_Settings;
}

/*****************************************************************************/
inline
otb::GenericRSTransform<>*
VectorImageModel
::GetGenericRSTransform()
{
  return m_GenericRSTransform;
}

/*****************************************************************************/
inline
QString
VectorImageModel
::GetFilename() const
{
  return m_Filename;
}

/*****************************************************************************/

} // end namespace 'mvd'

#endif // __mvdVectorImageModel_h
