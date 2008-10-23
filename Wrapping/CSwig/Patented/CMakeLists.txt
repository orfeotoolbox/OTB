# create the ITKPatentedTcl libraries
SET(WRAP_SOURCES
    wrap_itkSimpleFuzzyConnectednessImageFilterBase
    wrap_itkSimpleFuzzyConnectednessScalarImageFilter

)

SET(MASTER_INDEX_FILES "${WrapOTB_BINARY_DIR}/VXLNumerics/VXLNumerics.mdx"
                       "${WrapOTB_BINARY_DIR}/Numerics/ITKNumerics.mdx"
                       "${WrapOTB_BINARY_DIR}/CommonA/ITKCommonA.mdx"
                       "${WrapOTB_BINARY_DIR}/CommonB/ITKCommonB.mdx"
                       "${WrapOTB_BINARY_DIR}/BasicFiltersA/ITKBasicFiltersA.mdx"
                       "${WrapOTB_BINARY_DIR}/BasicFiltersB/ITKBasicFiltersB.mdx"
                       "${WrapOTB_BINARY_DIR}/Algorithms/ITKAlgorithms.mdx"
                       "${WrapOTB_BINARY_DIR}/Patented/ITKPatented.mdx"
)

ITK_WRAP_LIBRARY("${WRAP_SOURCES}" ITKPatented Patented 
  "ITKNumerics;ITKCommonA;ITKCommonB;ITKBasicFiltersA;ITKBasicFiltersB" "" "")
