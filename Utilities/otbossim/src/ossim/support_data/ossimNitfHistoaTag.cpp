//----------------------------------------------------------------------------
//
// License:  See top level LICENSE.txt file.
//
// Author:  Garrett Potts
//
// Description: HISTOA tag class definition.
// 
//----------------------------------------------------------------------------
// $Id: ossimNitfHistoaTag.cpp 22013 2012-12-19 17:37:20Z dburken $

#include <ossim/support_data/ossimNitfHistoaTag.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimString.h>
#include <iomanip>

ossimNitfHistoaProcessingEvent::ossimNitfHistoaProcessingEvent()
{
   clearFields();
}

ossim_uint32 ossimNitfHistoaProcessingEvent::getSizeInBytes()const
{
   ossim_uint32 sizeInBytes = (PDATE_SIZE + PSITE_SIZE + PAS_SIZE + NIPCOM_SIZE);

   if ( (*m_nIpcom != '0') && m_comment.size() )
   {
      sizeInBytes += ( m_comment.size() * IPCOM_SIZE );
   }

   sizeInBytes += (IBPP_SIZE + IPVTYPE_SIZE + INBWC_SIZE + 
                   DISP_FLAG_SIZE + ROT_FLAG_SIZE);

   if(*m_rotFlag == '1')
   {
      sizeInBytes += ROT_ANGLE_SIZE;
   }
   
   sizeInBytes += ASYM_FLAG_SIZE;
   if(*m_asymFlag == '1')
   {
      sizeInBytes += (ZOOMROW_SIZE + ZOOMCOL_SIZE);
   }
   
   sizeInBytes += ( PROJ_FLAG_SIZE + SHARP_FLAG_SIZE );
   if(*m_sharpFlag == '1')
   {
      sizeInBytes += (SHARPFAM_SIZE + SHARPMEM_SIZE);
   }

   sizeInBytes += MAG_FLAG_SIZE;
   if(*m_magFlag == '1')
   {
      sizeInBytes += MAG_LEVEL_SIZE;
   }

   sizeInBytes += DRA_FLAG_SIZE;
   if(*m_draFlag == '1')
   {
      sizeInBytes += (DRA_MULT_SIZE + DRA_SUB_SIZE);
   }

   sizeInBytes += TTC_FLAG_SIZE;
   if(*m_ttcFlag == '1')
   {
      sizeInBytes += (TTCFAM_SIZE + TTCMEM_SIZE);
   }

   sizeInBytes += DEVLUT_FLAG_SIZE + OBPP_SIZE + OPVTYPE_SIZE + OUTBWC_SIZE;

   return sizeInBytes;
}

void ossimNitfHistoaProcessingEvent::parseStream(std::istream& in)
{
   clearFields();

   in.read(m_pdate, PDATE_SIZE);
   in.read(m_psite, PSITE_SIZE);
   in.read(m_pas, PAS_SIZE);
   in.read(m_nIpcom, NIPCOM_SIZE);
   ossim_uint32 nIpcom = ossimString(m_nIpcom).toUInt32();
   if(nIpcom < 10)
   {
      ossim_uint32 idx = 0;
      for(;idx < nIpcom;++idx)
      {
         char tempBuf[IPCOM_SIZE+1];
         in.read(tempBuf, IPCOM_SIZE);
         tempBuf[IPCOM_SIZE] = '\0';
      
         // m_comment += std::string(tempBuf, tempBuf + 80);
         
         m_comment.push_back( std::string(tempBuf) );
      }
   }
   in.read(m_ibpp, IBPP_SIZE);
   in.read(m_ipvType, IPVTYPE_SIZE);
   
   in.read(m_inbwc, INBWC_SIZE);
   in.read(m_dispFlag, DISP_FLAG_SIZE);
   in.read(m_rotFlag, ROT_FLAG_SIZE);
   if(*m_rotFlag =='1')
   {
      in.read(m_rotAngle, ROT_ANGLE_SIZE);
   }

   in.read(m_asymFlag, ASYM_FLAG_SIZE);
   if(ossimString(m_asymFlag).toBool())
   {
      in.read(m_zoomRow, ZOOMROW_SIZE);
      in.read(m_zoomCol, ZOOMCOL_SIZE);
   }

   in.read(m_projFlag, PROJ_FLAG_SIZE);   
   in.read(m_sharpFlag, SHARP_FLAG_SIZE);
   if(ossimString(m_sharpFlag).toBool())
   {
      in.read(m_sharpFam, SHARPFAM_SIZE);
      in.read(m_sharpMem, SHARPMEM_SIZE);
   }

   in.read(m_magFlag, MAG_FLAG_SIZE);
   if(ossimString(m_magFlag).toBool())
   {
      in.read(m_magLevel, MAG_LEVEL_SIZE);
   }

   in.read(m_draFlag, DRA_FLAG_SIZE);
   if(ossimString(m_draFlag).toBool())
   {
      in.read(m_draMult, DRA_MULT_SIZE);
      in.read(m_draSub, DRA_SUB_SIZE);
   }

   in.read(m_ttcFlag, TTC_FLAG_SIZE);
   if(ossimString(m_ttcFlag).toBool())
   {
      in.read(m_ttcFam, TTCFAM_SIZE);
      in.read(m_ttcMem, TTCMEM_SIZE);
   }

   in.read(m_devLutFlag, DEVLUT_FLAG_SIZE);
   in.read(m_obpp, OBPP_SIZE);
   in.read(m_opvType, OPVTYPE_SIZE);
   in.read(m_outBwc, OUTBWC_SIZE);
}

void ossimNitfHistoaProcessingEvent::writeStream(std::ostream& out)
{
   out.write(m_pdate, PDATE_SIZE);
   out.write(m_psite, PSITE_SIZE);
   out.write(m_pas, PAS_SIZE);
   out.write(m_nIpcom, NIPCOM_SIZE);
   ossim_uint32 nIpcom = ossimString(m_nIpcom).toUInt32();
   if( (nIpcom > 0) && (nIpcom < 10) )
   {
      for( ossim_uint32 i = 0; i < nIpcom; ++i )
      {
         // Should size check std::string or make set method that ensures 80 chars.
         out.write( m_comment[i].data(), IPCOM_SIZE );
      }
   }
   out.write(m_ibpp, IBPP_SIZE);
   out.write(m_ipvType, IPVTYPE_SIZE);
   
   out.write(m_inbwc, INBWC_SIZE);
   out.write(m_dispFlag, DISP_FLAG_SIZE);
   out.write(m_rotFlag, ROT_FLAG_SIZE);
   if(*m_rotFlag =='1')
   {
      out.write(m_rotAngle, ROT_ANGLE_SIZE);
   }
   out.write(m_asymFlag, ASYM_FLAG_SIZE);
   if(*m_asymFlag == '1')
   {
      out.write(m_zoomRow, ZOOMROW_SIZE);
      out.write(m_zoomCol, ZOOMCOL_SIZE);
   }
   out.write(m_projFlag, PROJ_FLAG_SIZE);
   out.write(m_sharpFlag, SHARP_FLAG_SIZE);
   if(*m_sharpFlag == '1')
   {
      out.write(m_sharpFam, SHARPFAM_SIZE);
      out.write(m_sharpMem, SHARPMEM_SIZE);
   }
   out.write(m_magFlag, MAG_FLAG_SIZE);
   if(*m_magFlag == '1')
   {
      out.write(m_magLevel, MAG_LEVEL_SIZE);
   }
   out.write(m_draFlag, DRA_FLAG_SIZE);
   if(*m_draFlag == '1')
   {
      out.write(m_draMult, DRA_MULT_SIZE);
      out.write(m_draSub, DRA_SUB_SIZE);
   }
   out.write(m_ttcFlag, TTC_FLAG_SIZE);
   if(*m_ttcFlag == '1')
   {
      out.write(m_ttcFam, TTCFAM_SIZE);
      out.write(m_ttcMem, TTCMEM_SIZE);
   }
   out.write(m_devLutFlag, DEVLUT_FLAG_SIZE);
   out.write(m_obpp, OBPP_SIZE);
   out.write(m_opvType, OPVTYPE_SIZE);
   out.write(m_outBwc, OUTBWC_SIZE);
}

std::ostream& ossimNitfHistoaProcessingEvent::print(std::ostream& out,
                                                    const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += ".";
   
   out << setiosflags(std::ios::left)
   << pfx << std::setw(24) << "PDATE:"     << m_pdate << "\n"
   << pfx << std::setw(24) << "PSITE:"  << m_psite << "\n"
   << pfx << std::setw(24) << "PAS:"  << m_pas << "\n"
   << pfx << std::setw(24) << "NIPCOM:"   << m_nIpcom << "\n";
   ossim_uint32 nIpcom = ossimString(m_nIpcom).toUInt32();
   if( ( nIpcom > 0 ) && ( nIpcom < 10 ) )
   {
      for( ossim_uint32 i = 0; i < nIpcom; ++i )
      {
         std::string s = "IPCOM";
         
         //---
         // NOTE:
         // Doing  s += ossimString::toString( i ).string() core dumping linux gcc version 4.7.0.
         //---
         std::string s2 = ossimString::toString( i ).string();
         s += s2;
         s += ":";
         out << pfx << std::setw(24) << s.c_str();
         out.write( m_comment[i].data(), IPCOM_SIZE );
         out << "\n";
      }
   }
   
   out << setiosflags(std::ios::left)
   << pfx << std::setw(24) << "IBPP:"     << m_ibpp << "\n"
   << pfx << std::setw(24) << "IPVTYPE:"     << m_ipvType << "\n"
   << pfx << std::setw(24) << "INBWC:"     << m_inbwc << "\n"
   << pfx << std::setw(24) << "DISP_FLAG:"     << m_dispFlag << "\n"
   << pfx << std::setw(24) << "ROT_FLAG:"     << m_rotFlag << "\n";
   if(*m_rotFlag == '1')
   {
      out<< pfx << std::setw(24) << "ROT_ANGLE:" << m_rotAngle << "\n";
   }

   out<< pfx << std::setw(24) << "ASYM_FLAG:" << m_asymFlag << "\n";
   
   if(*m_asymFlag=='1')
   {
      out<< pfx << std::setw(24) << "ZOOMROW:" << m_zoomRow << "\n";
      out<< pfx << std::setw(24) << "ZOOMCOL:" << m_zoomCol << "\n";
   }
   out<< pfx << std::setw(24) << "PROJ_FLAG:" << m_projFlag << "\n";
   out<< pfx << std::setw(24) << "SHARP_FLAG:" << *m_sharpFlag << "\n";
   if(*m_sharpFlag == '1')
   {
      out<< pfx << std::setw(24) << "SHARPFAM:" << m_sharpFam << "\n";
      out<< pfx << std::setw(24) << "SHARPMEM:" << m_sharpMem << "\n";
   }
   out<< pfx << std::setw(24) << "MAG_FLAG:" << *m_magFlag << "\n";
   if(*m_magFlag  == '1')
   {
      out<< pfx << std::setw(24) << "MAG_LEVEL:" << m_magLevel << "\n";
   }
   out<< pfx << std::setw(24) << "DRA_FLAG:" << *m_draFlag << "\n";
   if(*m_draFlag=='1')
   {
      out<< pfx << std::setw(24) << "DRA_MULT:" << m_draMult << "\n";
      out<< pfx << std::setw(24) << "DRA_SUB:" << m_draSub << "\n";
   }
   out<< pfx << std::setw(24) << "TTC_FLAG:" << *m_ttcFlag << "\n";
   if(*m_ttcFlag == '1')
   {
      out<< pfx << std::setw(24) << "TTCFAM:" << m_ttcFam << "\n";
      out<< pfx << std::setw(24) << "TTCMEM:" << m_ttcMem << "\n";
   }
   out<< pfx << std::setw(24) << "DEVLUT_FLAG:" << *m_devLutFlag << "\n";
   out<< pfx << std::setw(24) << "OBPP:" << m_obpp << "\n";
   out<< pfx << std::setw(24) << "OPVTYPE:" << m_opvType << "\n";
   out<< pfx << std::setw(24) << "OUTBWC:" << m_outBwc << "\n";
   
   return out;
}

void ossimNitfHistoaProcessingEvent::clearFields()
{
   memset(m_pdate, ' ', PDATE_SIZE+1);
   memset(m_psite, ' ', PSITE_SIZE+1);
   memset(m_pas, ' ', PAS_SIZE+1);
   memset(m_nIpcom, ' ', NIPCOM_SIZE+1);
   memset(m_ibpp, ' ', IBPP_SIZE+1);
   memset(m_ipvType, ' ', IPVTYPE_SIZE+1);
   
   memset(m_inbwc, ' ', INBWC_SIZE);
   memset(m_dispFlag, '0', DISP_FLAG_SIZE);
   memset(m_rotFlag, '0', DISP_FLAG_SIZE);
   memset(m_rotAngle, '0', ROT_ANGLE_SIZE);
   m_rotAngle[3]='.';
   memset(m_projFlag, '0', PROJ_FLAG_SIZE);
   memset(m_asymFlag, '0', ASYM_FLAG_SIZE);
   memset(m_zoomRow, '0', ZOOMROW_SIZE);
   m_zoomRow[2]='.';
   memset(m_zoomCol, '0', ZOOMCOL_SIZE);
   m_zoomCol[2]='.';
   memset(m_sharpFlag, '0', SHARP_FLAG_SIZE);
   memset(m_sharpFam, ' ', SHARPFAM_SIZE);
   memset(m_sharpMem, ' ', SHARPMEM_SIZE);
   memset(m_magFlag, '0', MAG_FLAG_SIZE);
   memset(m_magLevel, ' ', MAG_LEVEL_SIZE);
   memset(m_draFlag, '0', DRA_FLAG_SIZE);
   memset(m_draMult, ' ', DRA_MULT_SIZE);
   
   memset(m_draSub, ' ', DRA_SUB_SIZE);
   memset(m_ttcFlag, '0', TTC_FLAG_SIZE);
   memset(m_ttcFam, ' ', TTCFAM_SIZE);
   memset(m_ttcMem, ' ', TTCMEM_SIZE);
   memset(m_obpp, ' ', OBPP_SIZE);
   memset(m_opvType, ' ', OPVTYPE_SIZE);
   memset(m_outBwc, ' ', OUTBWC_SIZE);
   
   m_pdate[PDATE_SIZE]     = '\0';
   m_psite[PSITE_SIZE]     = '\0';
   m_pas[PAS_SIZE]         = '\0';
   m_nIpcom[NIPCOM_SIZE]   = '\0';
   m_ibpp[IBPP_SIZE]       = '\0';
   m_ipvType[IPVTYPE_SIZE] = '\0';
   
   m_comment.clear();

   m_inbwc[INBWC_SIZE] = '\0';
   m_dispFlag[DISP_FLAG_SIZE] = '\0';
   m_rotFlag[ROT_FLAG_SIZE] = '\0';
   m_rotAngle[ROT_ANGLE_SIZE] = '\0';
   m_projFlag[PROJ_FLAG_SIZE] = '\0';
   m_asymFlag[ASYM_FLAG_SIZE] = '\0';
   m_zoomRow[ZOOMROW_SIZE] = '\0';
   m_zoomCol[ZOOMCOL_SIZE] = '\0';
   m_sharpFlag[SHARP_FLAG_SIZE] = '\0';
   m_sharpFam[SHARPFAM_SIZE] = '\0';
   m_sharpMem[SHARPMEM_SIZE] = '\0';
   m_magFlag[MAG_FLAG_SIZE] = '\0';
   m_magLevel[MAG_LEVEL_SIZE] = '\0';
   m_draFlag[DRA_FLAG_SIZE] = '\0';
   m_draMult[DRA_MULT_SIZE] = '\0';
   m_draSub[DRA_SUB_SIZE] = '\0';
   m_ttcFlag[TTC_FLAG_SIZE] = '\0';
   m_ttcFam[TTCFAM_SIZE] = '\0';
   m_ttcMem[TTCMEM_SIZE] = '\0';
   m_obpp[OBPP_SIZE] = '\0';
   m_opvType[OPVTYPE_SIZE] = '\0';
   m_outBwc[OUTBWC_SIZE] = '\0';
}

ossimNitfHistoaTag::ossimNitfHistoaTag()
   : ossimNitfRegisteredTag( std::string("HISTOA"), 0 )
{
}

void ossimNitfHistoaTag::parseStream(std::istream& in)
{
   clearFields();

   in.read(m_systype, SYSTYPE_SIZE);
   in.read(m_pc, PC_SIZE);
   in.read(m_pe, PE_SIZE);
   in.read(m_remapFlag, REMAP_FLAG_SIZE);
   in.read(m_lutid, LUTID_SIZE);
   in.read(m_nEvents, NEVENTS_SIZE);

   ossim_uint32 nEvents = ossimString(m_nEvents).toUInt32();
   if(nEvents < 100)
   {
      for(ossim_uint32 idx = 0; idx < nEvents; ++idx)
      {
         ossimNitfHistoaProcessingEvent evt;
         evt.parseStream(in);
         m_eventList.push_back(evt);
      }
   }

   // Set the tag length in base for the getTagLenth method.
   setTagLength( getSizeInBytes() );
}

void ossimNitfHistoaTag::writeStream(std::ostream& out)
{
   out.write(m_systype, SYSTYPE_SIZE);
   out.write(m_pc, PC_SIZE);
   out.write(m_pe, PE_SIZE);
   out.write(m_remapFlag, REMAP_FLAG_SIZE);
   out.write(m_lutid, LUTID_SIZE);
   out.write(m_nEvents, NEVENTS_SIZE);
   ossim_uint32 idx = 0;
   ossim_uint32 nEvents = ossimString(m_nEvents).toUInt32();
   if(nEvents < 100)
   {
      for(;idx < nEvents; ++idx)
      {
         m_eventList[idx].writeStream(out);
      }
   }
}

ossim_uint32 ossimNitfHistoaTag::getSizeInBytes()const
{
   // Make this dynamic for constructed HISTOA tags instead of using theTagLength.
   ossim_uint32 sizeInBytes = (SYSTYPE_SIZE + PC_SIZE + PE_SIZE + 
                               REMAP_FLAG_SIZE + LUTID_SIZE + NEVENTS_SIZE);

   for ( ossim_uint32 idx = 0; idx < m_eventList.size(); ++idx)
   {
      sizeInBytes += m_eventList[idx].getSizeInBytes();
   }

   return sizeInBytes;
}

void ossimNitfHistoaTag::clearFields()
{
   memset(m_systype,     ' ', SYSTYPE_SIZE);
   memset(m_pc,     ' ', PC_SIZE);
   memset(m_pe,     ' ', PE_SIZE);
   memset(m_remapFlag,     ' ', REMAP_FLAG_SIZE);
   memset(m_lutid,     '0', LUTID_SIZE);
   memset(m_nEvents,     '0', NEVENTS_SIZE);

   m_systype[SYSTYPE_SIZE] = '\0';
   m_pc[PC_SIZE] = '\0';
   m_pe[PE_SIZE] = '\0';
   m_remapFlag[REMAP_FLAG_SIZE] = '\0';
   m_lutid[LUTID_SIZE] = '\0';
   m_nEvents[NEVENTS_SIZE] = '\0';
   m_eventList.clear();

   // Clear the tag length in base.
   setTagLength(0);
}

std::ostream& ossimNitfHistoaTag::print(std::ostream& out,
                                        const std::string& prefix) const
{
   std::string pfx = prefix;
   pfx += getTagName();
   pfx += ".";
   
   out << setiosflags(std::ios::left)
   << pfx << std::setw(24) << "CETAG:"
   << getTagName() << "\n"
   << pfx << std::setw(24) << "CEL:"
   << getSizeInBytes() << "\n"
   << pfx << std::setw(24) << "SYSTYPE:"     << m_systype << "\n"
   << pfx << std::setw(24) << "PC:"  << m_pc << "\n"
   << pfx << std::setw(24) << "PE:"  << m_pe << "\n"
   << pfx << std::setw(24) << "REMAP_FLAG:"   << m_remapFlag << "\n"
   << pfx << std::setw(24) << "LUTID:"     << m_lutid << "\n"
   << pfx << std::setw(24) << "NEVENTS:"     << m_nEvents << "\n";
   for ( ossim_uint32 idx = 0; idx < m_eventList.size(); ++idx )
   {
      m_eventList[idx].print(out, pfx+"EVENT"+ossimString::toString(idx));
   }
   
   return out;
}

