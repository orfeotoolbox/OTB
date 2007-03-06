# create the OTBIOTcl libraries
SET(WRAP_SOURCES
    wrap_otbImageViewer
)
SET(MASTER_INDEX_FILES "${WrapOTB_BINARY_DIR}/VXLNumerics/VXLNumerics.mdx"
#                       "${WrapOTB_BINARY_DIR}/Numerics/ITKNumerics.mdx"
#                       "${WrapOTB_BINARY_DIR}/CommonA/ITKCommonA.mdx"
#                       "${WrapOTB_BINARY_DIR}/CommonB/ITKCommonB.mdx"
#                       "${WrapOTB_BINARY_DIR}/BasicFiltersA/ITKBasicFiltersA.mdx"
#                       "${WrapOTB_BINARY_DIR}/BasicFiltersB/ITKBasicFiltersB.mdx"
#		       "${WrapOTB_BINARY_DIR}/IO/ITKIO.mdx"
                       "${WrapOTB_BINARY_DIR}/otbVisu/OTBVisu.mdx"
)
ITK_WRAP_LIBRARY("${WRAP_SOURCES}" OTBVisu otbVisu "" ""
"OTBVisu;OTBGui;OTBIO;OTBCommon")

