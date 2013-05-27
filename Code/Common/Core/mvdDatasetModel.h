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

#ifndef __mvdDatasetModel_h
#define __mvdDatasetModel_h

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

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractImageModel.h"
#include "mvdTypes.h"


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
class DatasetDescriptor;

/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/** \class DatasetModel
 *
 */
class Monteverdi2_EXPORT DatasetModel :
    public AbstractModel
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public types.
public:
  /**
   */
  typedef QList< const AbstractImageModel* > ConstAbstractImageModelList;
  typedef QList< AbstractImageModel* > AbstractImageModelList;

  /**
   * \class BuildContext
   * \brief WIP.
   */
  class BuildContext
  {
    //
    // Public methods.
  public:
    BuildContext( const QString& path,
		  const QString& name,
                  const QString& alias,
		  bool isLoadSubModelsEnabled,
		  int width =-1,
		  int height =-1 ) :
      m_Path( path ),
      m_Name( name ),
      m_Alias( alias),
      m_Width( width ),
      m_Height( height ),
      m_IsLoadSubModelsEnabled( isLoadSubModelsEnabled )
    {
    }

    BuildContext( int width, int height ) :
      m_Path(),
      m_Name(),
      m_Alias(),
      m_Width( width ),
      m_Height( height ),
      m_IsLoadSubModelsEnabled( true )
    {
    }

    //
    // Public attributes
  public:
    QString m_Path;
    QString m_Name;
    QString m_Alias;
    int m_Width;
    int m_Height;
    bool m_IsLoadSubModelsEnabled;
  };

//
// Public methods.
public:

  /** \brief Constructor. */
  DatasetModel( QObject* parent =NULL );

  /** \brief Destructor. */
  virtual ~DatasetModel();

  /**
   */
  bool IsConsistent() const;

  /** */
  void SetAlias(const QString & alias);

  /**
   */
  inline const QDir& GetDirectory() const;

  /**
   */
  inline const QString& GetName() const;

  /**
   */
  inline const QString& GetAlias() const;

  /**
   */
  inline const QString& GetPath() const;

  /** */
  void ImportImage( const QString& filename, int w, int h );

  /**
   */
  void LoadImageModels( int width, int height );

  /** */
  inline bool HasSelectedImageModel() const;

  /**
   */
  inline const AbstractImageModel* GetSelectedImageModel() const;

  /**
   */
  inline AbstractImageModel* GetSelectedImageModel();

  /**
   */
  template< typename T >
  inline
    const T* GetSelectedImageModel() const;

  /**
   */
  template< typename T >
  inline
    T* GetSelectedImageModel();

  /**
   */
  inline ConstAbstractImageModelList GetImageModels() const;

  /**
   */
  inline QFileInfo HistogramFileInfo( const QString& imageFilename ) const;

  /**
   */
  inline QFileInfo QuicklookFileInfo( const QString& imageFilename ) const;

  /**
   */
  const PropertiesContainer& QueryDatasetProperties();

  //
  // AbstractModel overrides.

  virtual bool IsModified() const;

  virtual void ClearModified();

  //
  // accessors to last rendering information

  inline const PointType& GetLastPhysicalCenter() const ;

  inline const double GetLastIsotropicZoom() const ;


  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  //
  // AbstractModel overrides.

  void virtual_BuildModel( void* context =NULL );

  void virtual_Save();

//
// Protected attributes.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Private attributes.
private:
  /**
   */
  static const char* DESCRIPTOR_FILENAME;

  /**
   * \brief The quicklook image-file extension
   * (e.g. '/tmp/my_source_image.ql.tif').
   */
  static const char* QUICKLOOK_FILE_EXT;

  /**
   * \brief The histogram file extension '.txt'
   * (e.g. '/tmp/my_source_image.hg.tif.txt').
   */
  static const char* HISTOGRAM_FILE_EXT;

  /**
   * \brief The histogram file marker-suffix '.hg'
   * (e.g. '/tmp/my_source_image.hg.tif.txt').
   */
  static const char* HISTOGRAM_FILE_MARKER;


//
// Private methods.
private:

  /**
   */
  inline AbstractImageModelList GetImageModels();

  /** */
  AbstractImageModel* LoadImage( AbstractImageModel::BuildContext& context,
				 int width,
				 int height );

  /** */
  void ParseDescriptor( BuildContext* context );

  /** */
  void ParseAlias();


  /** */
  void ParseImageViewContext();

  /** */
  void WriteDescriptor() const;

//
// Private attributes.
private:
  /**
   */
  DatasetDescriptor* m_Descriptor;

  /**
   */
  QString m_Path;

  /**
   */
  QString m_Name;

  /**
   */
  QString m_Alias;

  /**
   */
  QDir m_Directory;
  
  /**
    */
  PropertiesContainer m_DatasetProperties;

  /**
   * \brief Last physical center used for viewport rendering
   * 
   */
  PointType      m_LastPhysicalCenter;

  /**
   * \brief Last zoom level used for viewport rendering
   * 
   */
  double         m_LastIsotropicZoom;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   */
  void OnSettingsUpdated( AbstractImageModel* );

  /** */
  void OnRenderingContextChanged(const PointType&, double );
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "mvdAbstractImageModel.h"

namespace mvd
{

/*****************************************************************************/
inline
QFileInfo
DatasetModel
::HistogramFileInfo( const QString& imageFilename ) const
{
  // Source image file information.
  QFileInfo imageFileInfo( imageFilename );

  // Histogram file information.
  return QFileInfo(
    m_Directory.path(),
    imageFileInfo.completeBaseName()
    + DatasetModel::HISTOGRAM_FILE_MARKER
    + "." + imageFileInfo.suffix()
    + DatasetModel::HISTOGRAM_FILE_EXT
  );
}

/*****************************************************************************/
inline
QFileInfo
DatasetModel
::QuicklookFileInfo( const QString& imageFilename ) const
{
  // Source image file information.
  QFileInfo imageFileInfo( imageFilename );

  // Histogram file information.
  return QFileInfo(
    m_Directory.path(),
    imageFileInfo.completeBaseName() + DatasetModel::QUICKLOOK_FILE_EXT
  );
}

/*****************************************************************************/
inline
const QDir&
DatasetModel
::GetDirectory() const
{
  return m_Directory;
}

/*****************************************************************************/
inline
const QString&
DatasetModel
::GetPath() const
{
  return m_Path;
}

/*****************************************************************************/
inline
const QString&
DatasetModel
::GetName() const
{
  return m_Name;
}

/*****************************************************************************/
inline
const QString&
DatasetModel
::GetAlias() const
{
  return m_Alias;
}

/*****************************************************************************/
inline
bool
DatasetModel
::HasSelectedImageModel() const
{
  return !GetImageModels().empty();
}

/*****************************************************************************/
inline
const AbstractImageModel*
DatasetModel
::GetSelectedImageModel() const
{
  DatasetModel::ConstAbstractImageModelList imageModels( GetImageModels() );

  return
    imageModels.isEmpty()
    ? NULL
    : GetImageModels().first();
}

/*****************************************************************************/
inline
AbstractImageModel*
DatasetModel
::GetSelectedImageModel()
{
  DatasetModel::AbstractImageModelList imageModels( GetImageModels() );

  return
    imageModels.isEmpty()
    ? NULL
    : GetImageModels().first();
}

/*****************************************************************************/
template< typename T >
inline
const T*
DatasetModel
::GetSelectedImageModel() const
{
  return qobject_cast< const T* >( GetSelectedImageModel() );
}

/*****************************************************************************/
template< typename T >
inline
T*
DatasetModel
::GetSelectedImageModel()
{
  return qobject_cast< T* >( GetSelectedImageModel() );
}

/*****************************************************************************/
inline
DatasetModel::ConstAbstractImageModelList
DatasetModel
::GetImageModels() const
{
  return findChildren< const AbstractImageModel* >();
}

/*****************************************************************************/
inline
DatasetModel::AbstractImageModelList
DatasetModel
::GetImageModels()
{
  return findChildren< AbstractImageModel* >();
}

/*****************************************************************************/
inline
const 
PointType&
DatasetModel
::GetLastPhysicalCenter() const
{
  return m_LastPhysicalCenter;
}

/*****************************************************************************/
inline
const
double
DatasetModel
::GetLastIsotropicZoom() const
{
  return m_LastIsotropicZoom;
}

} // end namespace 'mvd'

#endif // __mvdDatasetModel_h
