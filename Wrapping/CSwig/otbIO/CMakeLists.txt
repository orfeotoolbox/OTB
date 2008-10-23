# create the OTBIOTcl libraries
SET(WRAP_SOURCES
    wrap_otbIOBase
    wrap_otbImageFileReader
    wrap_otbImageFileWriter
)
SET(MASTER_INDEX_FILES "${WrapOTB_BINARY_DIR}/VXLNumerics/VXLNumerics.mdx"
#                       "${WrapOTB_BINARY_DIR}/Numerics/ITKNumerics.mdx"
                       "${WrapOTB_BINARY_DIR}/CommonA/ITKCommonA.mdx"
                       "${WrapOTB_BINARY_DIR}/CommonB/ITKCommonB.mdx"
#                       "${WrapOTB_BINARY_DIR}/BasicFiltersA/ITKBasicFiltersA.mdx"
#                       "${WrapOTB_BINARY_DIR}/BasicFiltersB/ITKBasicFiltersB.mdx"
		       "${WrapOTB_BINARY_DIR}/IO/ITKIO.mdx"
                       "${WrapOTB_BINARY_DIR}/otbIO/OTBIO.mdx"
                       "${WrapOTB_BINARY_DIR}/otbCommon/OTBCommon.mdx"
)
ITK_WRAP_LIBRARY("${WRAP_SOURCES}" OTBIO otbIO "OTBCommon;ITKIO;ITKCommonA;ITKCommonB" "" "OTBIO;ITKIO;ITKCommon;OTBCommon")

