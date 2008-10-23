SET(WRAP_SOURCES
  wrap_itkExpImageFilter
  wrap_itkExpNegativeImageFilter
  wrap_itkGradientMagnitudeRecursiveGaussianImageFilter
  wrap_itkGradientRecursiveGaussianImageFilter
  wrap_itkMeanImageFilter
  wrap_itkMedianImageFilter
  wrap_itkMinimumMaximumImageFilter
  wrap_itkNaryAddImageFilter
  wrap_itkNormalizeImageFilter
  wrap_itkPermuteAxesImageFilter
  wrap_itkRandomImageSource
  wrap_itkRecursiveGaussianImageFilter
  wrap_itkRecursiveSeparableImageFilter
  wrap_itkRegionOfInterestImageFilter
  wrap_itkResampleImageFilter
  wrap_itkRescaleIntensityImageFilter
  wrap_itkShiftScaleImageFilter
  wrap_itkSigmoidImageFilter
  wrap_itkSmoothingRecursiveGaussianImageFilter
  wrap_itkStatisticsImageFilter
  wrap_itkSubtractImageFilter
  wrap_itkThresholdImageFilter
  wrap_itkVTKImageExport
  wrap_itkVTKImageImport
)

SET(MASTER_INDEX_FILES "${WrapOTB_BINARY_DIR}/VXLNumerics/VXLNumerics.mdx"
  "${WrapOTB_BINARY_DIR}/Numerics/ITKNumerics.mdx"
  "${WrapOTB_BINARY_DIR}/CommonA/ITKCommonA.mdx"
  "${WrapOTB_BINARY_DIR}/CommonB/ITKCommonB.mdx"
  "${WrapOTB_BINARY_DIR}/BasicFiltersB/ITKBasicFiltersB.mdx"
  )

ITK_WRAP_LIBRARY("${WRAP_SOURCES}" ITKBasicFiltersB BasicFiltersB "ITKNumerics;ITKCommonB;ITKCommonA" "" "ITKBasicFilters")
