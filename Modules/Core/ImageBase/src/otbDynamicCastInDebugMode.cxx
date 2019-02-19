
#include "otbImage.h"
#include "otbVectorImage.h"

// otb::Image
template otb::Image<unsigned char, 2u>* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<unsigned char, 2u>*, itk::DataObject*>(itk::DataObject*);
template otb::Image<unsigned int, 2u>* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<unsigned int, 2u>*, itk::DataObject*>(itk::DataObject*);
template otb::Image<unsigned short, 2u>* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<unsigned short, 2u>*, itk::DataObject*>(itk::DataObject*);
template otb::Image<float, 2u>* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<float, 2u>*, itk::DataObject*>(itk::DataObject*);
template otb::Image<double, 2u>* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<double, 2u>*, itk::DataObject*>(itk::DataObject*);

template otb::Image<unsigned char, 2u> const* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<unsigned char, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::Image<unsigned int, 2u> const* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<unsigned int, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::Image<unsigned short, 2u> const* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<unsigned short, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::Image<float, 2u> const* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<float, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::Image<double, 2u> const* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::Image<double, 2u> const*, itk::DataObject const*>(itk::DataObject const*);

// otb::VectorImage
template otb::VectorImage<unsigned char, 2u>* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned char, 2u>*, itk::DataObject*>(itk::DataObject*);
template otb::VectorImage<float, 2u>* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<float, 2u>*, itk::DataObject*>(itk::DataObject*);
template otb::VectorImage<double, 2u>* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<double, 2u>*, itk::DataObject*>(itk::DataObject*);

template otb::VectorImage<unsigned char, 2u> const* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<unsigned char, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<float, 2u> const* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<float, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
template otb::VectorImage<double, 2u> const* OTBImageBase_EXPORT_EXPLICIT_TEMPLATE
  itkDynamicCastInDebugMode<otb::VectorImage<double, 2u> const*, itk::DataObject const*>(itk::DataObject const*);
