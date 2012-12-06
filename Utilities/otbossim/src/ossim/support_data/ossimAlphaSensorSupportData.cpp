#include <ossim/support_data/ossimAlphaSensorSupportData.h>
#include <ossim/support_data/ossimEnviHeader.h>
#include <ossim/matrix/newmatio.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimTrace.h>
#include <fstream>

static ossimTrace traceExec ("ossimAlphaSensorSupportData:exec");
static ossimTrace traceDebug("ossimAlphaSensorSupportData:debug");


ossimAlphaSensorSupportData::ossimAlphaSensorSupportData()
{
   reset();
}

void ossimAlphaSensorSupportData::reset()
{
   m_imageSize.makeNan();
}


bool ossimAlphaSensorSupportData::readSupportFiles(const ossimFilename& hdrFile)
{
   bool readOK = false;

   // Read ENVI hdr file
   ossimEnviHeader hdr;
   if ( hdr.open(hdrFile) )
   {
      readOK= readSupportFiles( hdr );
   }
   return readOK;
}

bool ossimAlphaSensorSupportData::readSupportFiles(const ossimEnviHeader& hdr)
{
   bool readOK = readHdrFile(hdr);

   ossimFilename txtFile = hdr.getFile();
   
   if ( readOK )
   {
      // Derive associated insgps.txt file name
      if (m_isHSI)
      {
         // HSI:
         //  Associated file is located in /NavData, sub-directory of current container
         //   example:
         //   .hdr = "/data/AH/2012-06-15_20-00-29/HSI/Scan_00002/2012-06-15_20-00-29.HSI.Scan_00002.scene.corrected.hsi.hdr"
         //   .txt = "/data/AH/2012-06-15_20-00-29/HSI/Scan_00002/NavData/2012-06-15_20-00-29.HSI.Scan_00002.scene.insgps.txt"
         // txtFile.insert(hdr.getFile().find_last_of('/'), "/NavData");
         // txtFile.gsub("corrected.hsi.hdr", "insgps.txt");
         txtFile = hdr.getFile().path();
         txtFile = txtFile.dirCat("NavData");
         txtFile = txtFile.dirCat( hdr.getFile().file() );
         txtFile.gsub("corrected.hsi.hdr", "insgps.txt");
      }
      else
      {
         // HRI:
         //  Associated file is located in ../NavData, parallel to current container
         //  There is one insgps.txt file common to multiple hdr files
         //   example:
         //   .hdr = "/data/AH/2012-06-15_20-00-29/HRI/HRI_2/2012-06-15_20-00-29.HRI_2.Strip_00004.corrected.hri.hdr"
         //   .txt = "/data/AH/2012-06-15_20-00-29/HRI/NavData/2012-06-15_20-00-29.HRI.Strip_00004.insgps.txt"
         txtFile = hdr.getFile().path();
         if ( txtFile.empty() )
         {
            txtFile = txtFile.dirCat("../NavData");
         }
         else
         {
            txtFile.replace(txtFile.find("HRI_"), 5, "NavData");
         }  
         txtFile = txtFile.dirCat( hdr.getFile().file() );
         txtFile.replace(txtFile.find("HRI_"), 5, "HRI");
         txtFile.gsub("corrected.hri.hdr", "insgps.txt");
      }

      // Read .txt file
      readOK = readInsGpsFile(txtFile);
   }    
   
   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorSupportData::readSupportFiles DEBUG:"
         << "\n hdrFile = " << hdr.getFile()
         << "\n txtFile = " << txtFile
         << std::endl;
   }

   return readOK;
}

bool ossimAlphaSensorSupportData::readHdrFile(const ossimFilename& hdrFile)
{
   bool result = false;
   ossimEnviHeader hdr;
   if ( hdr.open(hdrFile) )
   {
      result = readHdrFile( hdr );
   }
   return result;
}

bool ossimAlphaSensorSupportData::readHdrFile(const ossimEnviHeader& hdr)
{
   bool result = false;

   while( 1 )
   {
      // Required stuff will break from loop if not found/valid.

      m_sensorType = hdr.getSensorType();
      if ( m_sensorType.empty() ) break;
      
      // m_imageID = hdr.getFile().afterPos(hdr.getFile().find_last_of('/'));
      m_imageID = hdr.getFile().file();

      if ( m_sensorType == "Unknown" )
      {
         // Make an assumption from file name...
         if ( hdr.getFile().file().contains( ossimString("HSI") ) )
         {
            m_sensorType = "ACES_YOGI-HSI";
         }
         else if (  hdr.getFile().file().contains( ossimString("HRI") ) )
         {
            m_sensorType = "ACES_YOGI-HRI2";
         }
      }

      if ( m_sensorType == "Unknown" )
      {
         break; // Get out...
      }
      
      if (m_sensorType.contains("HSI"))
      {
         m_imageID  = m_imageID.beforePos(m_imageID.find(".scene"));
         m_isHSI = true;
      }
      else 
      {
         m_imageID  = m_imageID.beforePos(m_imageID.find(".corrected"));
         m_isHSI = false;
      }

      m_imageSize.x = hdr.getSamples();
      m_imageSize.y = hdr.getLines();
      if ( !m_imageSize.x || !m_imageSize.y ) break;
      
      ossimString value;
 
      hdr.getValue("roll bias", value);
      if ( value.size() )
      {
         m_rollBias = ossimString::toDouble(value);
      }
      else
      {
         break;
      }

      hdr.getValue("pitch bias", value);
      if ( value.size() )
      {
         m_pitchBias = ossimString::toDouble(value);
      }
      else
      {
         break;
      }

      hdr.getValue("heading bias", value);
      if ( value.size() )
      {
         m_headingBias = ossimString::toDouble(value);
      }
      else
      {
         break;
      }

      hdr.getValue("fpa fov deg", value);
      if ( value.size() )
      {
         m_fov = ossimString::toDouble(value);
      }
      else
      {
         break;
      }

      hdr.getValue("slit rotation deg", value);
      if ( value.size() )
      {
         m_slitRot = ossimString::toDouble(value);
      }
      else
      {
         break;
      }

      // Last two lines of while forever.  If we get here, set status true and break out.
      result = true;
      break;

      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimAlphaSensorSupportData::readHdrFile DEBUG:"
            << "\n getSensorType = " << m_sensorType
            << "\n getSamples = " << m_imageSize.x
            << "\n getLines = " << m_imageSize.y
            << "\n roll bias = " << m_rollBias
            << "\n pitch bias = " << m_pitchBias
            << "\n heading bias = " << m_headingBias
            << "\n fpa fov deg = " << m_fov
            << "\n slit rotation deg = " << m_slitRot
            << std::endl;
      }
   }

   return result;

} // End: bool ossimAlphaSensorSupportData::readHdrFile(const ossimEnviHeader& )

bool ossimAlphaSensorSupportData::readInsGpsFile(const ossimFilename& file)
{
   bool result = true;
   std::ifstream in(file.c_str(), std::ios::in);

   ossimString line1;
   // int nLines = 0;

   ossim_float64 inum;
   ossim_float64 roll;
   ossim_float64 pitch;
   ossim_float64 heading;
   ossim_float64 lon;
   ossim_float64 lat;
   ossim_float64 alt;
   ossim_float64 scanAng;
   int res;
   int frm;
   ossim_float64 timeCode;


   std::vector< ossim_uint32 > lines;

   if (traceDebug())
   {
      ossimNotify(ossimNotifyLevel_DEBUG)
         << "ossimAlphaSensorSupportData::readInsGpsFile DEBUG:" << std::endl;
      ossimNotify(ossimNotifyLevel_DEBUG)<<std::setprecision(8);
      ossimNotify(ossimNotifyLevel_DEBUG)<<std::setiosflags(std::ios_base::scientific);

   }

   // Format: line,roll,pitch,heading,lon,lat,alt,scanAngle,reserved,frame,time
   if (in)
   {
      in >> line1;
      while(!in.eof())
      {
         in >> inum >> roll >> pitch >> heading >> lon >> lat >> alt >> scanAng >> res >> frm >> timeCode;

         lines.push_back(inum);
         m_roll.push_back(roll);
         m_pitch.push_back(pitch);
         m_heading.push_back(heading);
         m_lon.push_back(lon);
         m_lat.push_back(lat);
         m_alt.push_back(alt);
         m_scanAng.push_back(scanAng);
      }

      // Load independent variable (line number, referenced to first line)
      vector< ossimPolynom< ossim_float64 , 1 >::VAR_TUPLE >::iterator pit;
      m_imgLine.resize(lines.size());
      int j;
      for (pit=m_imgLine.begin(), j=0; pit!=m_imgLine.end(); ++pit,++j)
      {
         pit->push_back(lines[j]-lines[0]);
      }


      // Dependent variable solutions follow...
      double rms=0.0;

      // Exponent sets
      std::vector< ossimPolynom<ossim_float64,1>::EXPT_SET > expSet;
      expSet.resize(MAX_TERMS);
      ossimPolynom<ossim_float64,1>::EXP_TUPLE et(1);
      for (int numExp=1; numExp<=MAX_TERMS; ++numExp)
      {
         for(int o=0; o<numExp ; ++o)
         {
            et[0]=o;
            expSet[numExp-1].insert(et);
         }
      }

      // Polynomial classes
      //  scanAngle
      ossimPolynom<ossim_float64,1> saPoly;
      //  orientation angles (roll, pitch, heading)
      ossimPolynom<ossim_float64,1> rPoly;
      ossimPolynom<ossim_float64,1> pPoly;
      ossimPolynom<ossim_float64,1> hPoly;
      //  position (longitude, latitude, altitude)
      ossimPolynom<ossim_float64,1> lonPoly;
      ossimPolynom<ossim_float64,1> latPoly;
      ossimPolynom<ossim_float64,1> altPoly;

      // Solve for polynomial coefficients
      if (m_isHSI)
      {
         result = saPoly.SLSfit(expSet[SCAN_ANGLE_DEG], m_imgLine, m_scanAng, &rms);
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "\n SA  poly deg, rms error = "
               << SCAN_ANGLE_DEG << ", " << ossim::radiansToDegrees(rms) << " deg";
         }
      }
      if (result)
      {
         result = rPoly.SLSfit(expSet[ROLL_DEG], m_imgLine, m_roll, &rms);
         if (traceDebug())
         {
            ossimNotify(ossimNotifyLevel_DEBUG) << "\n R   poly deg, rms error = "
               << ROLL_DEG << ", " << ossim::radiansToDegrees(rms) << " deg";
         }

         if (result)
         {
            result = pPoly.SLSfit(expSet[PITCH_DEG], m_imgLine, m_pitch, &rms);
            if (traceDebug())
            {
               ossimNotify(ossimNotifyLevel_DEBUG) << "\n P   poly deg, rms error = "
                  << PITCH_DEG << ", " << ossim::radiansToDegrees(rms) << " deg";
            }

            if (result)
            {
               result = hPoly.SLSfit(expSet[HEADING_DEG], m_imgLine, m_heading, &rms);
               if (traceDebug())
               {
                  ossimNotify(ossimNotifyLevel_DEBUG) << "\n H   poly deg, rms error = "
                     << HEADING_DEG << ", " << ossim::radiansToDegrees(rms) << " deg";
               }

               if (result)
               {
                  result = lonPoly.SLSfit(expSet[LON_DEG], m_imgLine, m_lon, &rms);
                  if (traceDebug())
                  {
                     ossimNotify(ossimNotifyLevel_DEBUG) << "\n Lon poly deg, rms error = "
                        << LON_DEG << ", " << ossim::radiansToDegrees(rms) << " deg";
                  }

                  if (result)
                  {
                     result = latPoly.SLSfit(expSet[LAT_DEG], m_imgLine, m_lat, &rms);
                     if (traceDebug())
                     {
                        ossimNotify(ossimNotifyLevel_DEBUG) << "\n Lat poly deg, rms error = "
                           << LAT_DEG << ", " << ossim::radiansToDegrees(rms) << " deg";
                     }

                     if (result)
                     {
                        result = altPoly.SLSfit(expSet[ALT_DEG], m_imgLine, m_alt, &rms);
                        if (traceDebug())
                        {
                           ossimNotify(ossimNotifyLevel_DEBUG) << "\n Alt poly deg, rms error = "
                              << ALT_DEG << ", " << rms << " mtr";
                        }
                     }
                  }
               }
            }
         }
      }

      if (result)
      {
         // Save coefficients
         ossimPolynom<ossim_float64,1>::MONOM_MAP::const_iterator it;
         for (it = rPoly.getMonoms().begin(); it != rPoly.getMonoms().end() ; ++it )
         {
            m_rollPolyCoef.push_back(it->second);
         }
         for (it = pPoly.getMonoms().begin(); it != pPoly.getMonoms().end() ; ++it )
         {
            m_pitchPolyCoef.push_back(it->second);
         }
         for (it = hPoly.getMonoms().begin(); it != hPoly.getMonoms().end() ; ++it )
         {
            m_headingPolyCoef.push_back(it->second);
         }
         for (it = lonPoly.getMonoms().begin(); it != lonPoly.getMonoms().end() ; ++it )
         {
            m_lonPolyCoef.push_back(it->second);
         }
         for (it = latPoly.getMonoms().begin(); it != latPoly.getMonoms().end() ; ++it )
         {
            m_latPolyCoef.push_back(it->second);
         }
         for (it = altPoly.getMonoms().begin(); it != altPoly.getMonoms().end() ; ++it )
         {
            m_altPolyCoef.push_back(it->second);
         }
         if (m_isHSI)
         {
            for (it = saPoly.getMonoms().begin(); it != saPoly.getMonoms().end() ; ++it )
            {
               m_scanAngPolyCoef.push_back(it->second);
            }
         }
         else
         {
            m_scanAngPolyCoef.push_back(0.0);
         }
      }


      if (traceDebug())
      {
         if (result)
         {
            vector<ossim_float64> inp(1);
            inp[0] = 510.0;
            ossimNotify(ossimNotifyLevel_DEBUG) << "\n Test eval line number = " << inp[0] << endl;
            if (m_isHSI)
            {
               saPoly.print(ossimNotify(ossimNotifyLevel_DEBUG));
               ossim_float64 sa = saPoly.eval(inp);
               ossimNotify(ossimNotifyLevel_DEBUG)<<"\n sa="<<sa<<endl;
            }
            rPoly.print(ossimNotify(ossimNotifyLevel_DEBUG));
            ossim_float64 rr = rPoly.eval(inp);
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n r="<<rr<<endl;

            pPoly.print(ossimNotify(ossimNotifyLevel_DEBUG));
            ossim_float64 pp = pPoly.eval(inp);
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n p="<<pp<<endl;

            hPoly.print(ossimNotify(ossimNotifyLevel_DEBUG));
            ossim_float64 hh = hPoly.eval(inp);
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n h="<<hh<<endl;

            lonPoly.print(ossimNotify(ossimNotifyLevel_DEBUG));
            ossim_float64 lon = lonPoly.eval(inp);
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n lon="<<lon<<endl;

            latPoly.print(ossimNotify(ossimNotifyLevel_DEBUG));
            ossim_float64 lat = latPoly.eval(inp);
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n lat="<<lat<<endl;

            altPoly.print(ossimNotify(ossimNotifyLevel_DEBUG));
            ossim_float64 alt = altPoly.eval(inp);
            ossimNotify(ossimNotifyLevel_DEBUG)<<"\n alt="<<alt<<endl;
         }
         else
         {
            ossimNotify(ossimNotifyLevel_DEBUG)
               << "  ...polynomial solution error for file: " << file << std::endl;
         }
      }

   }
   else
   {
      result = false;
      if (traceDebug())
      {
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "ossimAlphaSensorSupportData::readInsGpsFile DEBUG:" << std::endl;
         ossimNotify(ossimNotifyLevel_DEBUG)
            << "  ...ifstream error for file: " << file << std::endl;
      }
   }

   return result;
}

const ossimDpt& ossimAlphaSensorSupportData::getImageSize()const
{
   return m_imageSize;
}

const ossim_float64& ossimAlphaSensorSupportData::getRollBias()const
{
   return m_rollBias;
}

const ossim_float64& ossimAlphaSensorSupportData::getPitchBias()const
{
   return m_pitchBias;
}

const ossim_float64& ossimAlphaSensorSupportData::getHeadingBias()const
{
   return m_headingBias;
}

const ossim_float64& ossimAlphaSensorSupportData::getFov()const
{
   return m_fov;
}

const ossim_float64& ossimAlphaSensorSupportData::getSlitRot()const
{
   return m_slitRot;
}

const std::vector< ossim_float64 >& ossimAlphaSensorSupportData::getRollPoly()const
{
   return m_rollPolyCoef;
}

const std::vector< ossim_float64 >& ossimAlphaSensorSupportData::getPitchPoly()const
{
   return m_pitchPolyCoef;
}

const std::vector< ossim_float64 >& ossimAlphaSensorSupportData::getHeadingPoly()const
{
   return m_headingPolyCoef;
}

const std::vector< ossim_float64 >& ossimAlphaSensorSupportData::getLonPoly()const
{
   return m_lonPolyCoef;
}

const std::vector< ossim_float64 >& ossimAlphaSensorSupportData::getLatPoly()const
{
   return m_latPolyCoef;
}

const std::vector< ossim_float64 >& ossimAlphaSensorSupportData::getAltPoly()const
{
   return m_altPolyCoef;
}

const std::vector< ossim_float64 >& ossimAlphaSensorSupportData::getScanPoly()const
{
   return m_scanAngPolyCoef;
}


// Hidden from use...
ossimAlphaSensorSupportData::ossimAlphaSensorSupportData(const ossimAlphaSensorSupportData& /* src */)
{
}

ossimAlphaSensorSupportData& ossimAlphaSensorSupportData::operator=(
   const ossimAlphaSensorSupportData& /* src */)
{
   return *this;
}

