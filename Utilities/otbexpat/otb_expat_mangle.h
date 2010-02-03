/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.
  
  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otb_expat_mangle_h
#define otb_expat_mangle_h

/*


This header file mangles all symbols exported from the openjpeg library.
It is included in all files while building the expat library.  Due to
namespace pollution, no openjpeg headers should be included in .h files in
OTB.

The following commands were used to obtain the symbol list:

nm libotbexpat.a |grep " T "
//nm libotbexpat.a |grep " D "
//nm libotbexpat.a |grep " R "

*/

#define  XmlPrologStateInit    otb_XmlPrologStateInit
#define  XmlGetUtf16InternalEncoding    otb_XmlGetUtf16InternalEncoding
#define  XmlGetUtf8InternalEncoding    otb_XmlGetUtf8InternalEncoding
#define  XmlInitEncoding    otb_XmlInitEncoding
#define  XmlInitUnknownEncoding    otb_XmlInitUnknownEncoding
#define  XmlParseXmlDecl    otb_XmlParseXmlDecl
#define  XmlSizeOfUnknownEncoding    otb_XmlSizeOfUnknownEncoding
#define  XmlUtf16Encode    otb_XmlUtf16Encode
#define  XmlUtf8Encode    otb_XmlUtf8Encode
#define  XML_DefaultCurrent    otb_XML_DefaultCurrent
#define  XML_ErrorString    otb_XML_ErrorString
#define  XML_ExpatVersion    otb_XML_ExpatVersion
#define  XML_ExpatVersionInfo    otb_XML_ExpatVersionInfo
#define  XML_ExternalEntityParserCreate    otb_XML_ExternalEntityParserCreate
#define  XML_FreeContentModel    otb_XML_FreeContentModel
#define  XML_GetBase    otb_XML_GetBase
#define  XML_GetBuffer    otb_XML_GetBuffer
#define  XML_GetCurrentByteCount    otb_XML_GetCurrentByteCount
#define  XML_GetCurrentByteIndex    otb_XML_GetCurrentByteIndex
#define  XML_GetCurrentColumnNumber    otb_XML_GetCurrentColumnNumber
#define  XML_GetCurrentLineNumber    otb_XML_GetCurrentLineNumber
#define  XML_GetErrorCode    otb_XML_GetErrorCode
#define  XML_GetFeatureList    otb_XML_GetFeatureList
#define  XML_GetIdAttributeIndex    otb_XML_GetIdAttributeIndex
#define  XML_GetInputContext    otb_XML_GetInputContext
#define  XML_GetParsingStatus    otb_XML_GetParsingStatus
#define  XML_GetSpecifiedAttributeCount    otb_XML_GetSpecifiedAttributeCount
#define  XML_MemFree    otb_XML_MemFree
#define  XML_MemMalloc    otb_XML_MemMalloc
#define  XML_MemRealloc    otb_XML_MemRealloc
#define  XML_Parse    otb_XML_Parse
#define  XML_ParseBuffer    otb_XML_ParseBuffer
#define  XML_ParserCreate    otb_XML_ParserCreate
#define  XML_ParserCreateNS    otb_XML_ParserCreateNS
#define  XML_ParserCreate_MM    otb_XML_ParserCreate_MM
#define  XML_ParserFree    otb_XML_ParserFree
#define  XML_ParserReset    otb_XML_ParserReset
#define  XML_ResumeParser    otb_XML_ResumeParser
#define  XML_SetAttlistDeclHandler    otb_XML_SetAttlistDeclHandler
#define  XML_SetBase    otb_XML_SetBase
#define  XML_SetCdataSectionHandler    otb_XML_SetCdataSectionHandler
#define  XML_SetCharacterDataHandler    otb_XML_SetCharacterDataHandler
#define  XML_SetCommentHandler    otb_XML_SetCommentHandler
#define  XML_SetDefaultHandler    otb_XML_SetDefaultHandler
#define  XML_SetDefaultHandlerExpand    otb_XML_SetDefaultHandlerExpand
#define  XML_SetDoctypeDeclHandler    otb_XML_SetDoctypeDeclHandler
#define  XML_SetElementDeclHandler    otb_XML_SetElementDeclHandler
#define  XML_SetElementHandler    otb_XML_SetElementHandler
#define  XML_SetEncoding    otb_XML_SetEncoding
#define  XML_SetEndCdataSectionHandler    otb_XML_SetEndCdataSectionHandler
#define  XML_SetEndDoctypeDeclHandler    otb_XML_SetEndDoctypeDeclHandler
#define  XML_SetEndElementHandler    otb_XML_SetEndElementHandler
#define  XML_SetEndNamespaceDeclHandler    otb_XML_SetEndNamespaceDeclHandler
#define  XML_SetEntityDeclHandler    otb_XML_SetEntityDeclHandler
#define  XML_SetExternalEntityRefHandler    otb_XML_SetExternalEntityRefHandler
#define  XML_SetExternalEntityRefHandlerArg    otb_XML_SetExternalEntityRefHandlerArg
#define  XML_SetNamespaceDeclHandler    otb_XML_SetNamespaceDeclHandler
#define  XML_SetNotStandaloneHandler    otb_XML_SetNotStandaloneHandler
#define  XML_SetNotationDeclHandler    otb_XML_SetNotationDeclHandler
#define  XML_SetParamEntityParsing    otb_XML_SetParamEntityParsing
#define  XML_SetProcessingInstructionHandler    otb_XML_SetProcessingInstructionHandler
#define  XML_SetReturnNSTriplet    otb_XML_SetReturnNSTriplet
#define  XML_SetSkippedEntityHandler    otb_XML_SetSkippedEntityHandler
#define  XML_SetStartCdataSectionHandler    otb_XML_SetStartCdataSectionHandler
#define  XML_SetStartDoctypeDeclHandler    otb_XML_SetStartDoctypeDeclHandler
#define  XML_SetStartElementHandler    otb_XML_SetStartElementHandler
#define  XML_SetStartNamespaceDeclHandler    otb_XML_SetStartNamespaceDeclHandler
#define  XML_SetUnknownEncodingHandler    otb_XML_SetUnknownEncodingHandler
#define  XML_SetUnparsedEntityDeclHandler    otb_XML_SetUnparsedEntityDeclHandler
#define  XML_SetUserData    otb_XML_SetUserData
#define  XML_SetXmlDeclHandler    otb_XML_SetXmlDeclHandler
#define  XML_StopParser    otb_XML_StopParser
#define  XML_UseForeignDTD    otb_XML_UseForeignDTD
#define  XML_UseParserAsHandlerArg    otb_XML_UseParserAsHandlerArg

#endif
