namespace otb
{
namespace Wrapper
{
  enum DefaultValueMode
  {
    DefaultValueMode_UNKNOWN,
    DefaultValueMode_RELATIVE,
    DefaultValueMode_ABSOLUTE
  };

  typedef enum
  {
    ParameterType_Empty,
    ParameterType_Int,
    ParameterType_Float,
    ParameterType_String,
    ParameterType_StringList,
    ParameterType_InputFilename,
    ParameterType_InputFilenameList,
    ParameterType_OutputFilename,
    ParameterType_Directory,
    ParameterType_Choice,
    ParameterType_InputImage,
    ParameterType_InputImageList,
    ParameterType_InputVectorData,
    ParameterType_InputVectorDataList,
    ParameterType_OutputImage,
    ParameterType_OutputVectorData,
    ParameterType_Radius,
    ParameterType_Group,
    ParameterType_ListView,
    ParameterType_ComplexInputImage,
    ParameterType_ComplexOutputImage,
    ParameterType_RAM,
    ParameterType_OutputProcessXML,
    ParameterType_InputProcessXML
  } ParameterType;

  typedef enum
  {
    UserLevel_Basic,
    UserLevel_Advanced
  } UserLevel;

  typedef enum
  {
    ImagePixelType_uint8,
    ImagePixelType_int16,
    ImagePixelType_uint16,
    ImagePixelType_int32,
    ImagePixelType_uint32,
    ImagePixelType_float,
    ImagePixelType_double,
  } ImagePixelType;

  typedef enum
  {
    Role_Input = 0,
    Role_Output
  } Role;

  typedef enum
  {
    ComplexImagePixelType_float,
    ComplexImagePixelType_double,
  } ComplexImagePixelType;

} /* end namespace wapper */

} /* end namespace otb */
