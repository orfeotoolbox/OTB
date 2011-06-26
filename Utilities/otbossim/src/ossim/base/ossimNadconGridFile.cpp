#include <ossim/base/ossimNadconGridFile.h>
#include <ossim/base/ossimEndian.h>

using namespace std;

ossimNadconGridFile::~ossimNadconGridFile()
{
   close();
}

bool ossimNadconGridFile::open(const ossimFilename& file)
{
   if(theHeader.readHeader(file))
   {
      theInputFile.close();
      theInputFile.clear();
      theInputFile.open(file.c_str(), ios::in|ios::binary);

      theBoundingRect = theHeader.getBoundingRect();
      theLatLonOrigin.lat = theHeader.getMinY();
      theLatLonOrigin.lon = theHeader.getMinX();
      theFilename = file;
      theFileOkFlag = true;
      return theInputFile.good();
   }
   theFileOkFlag = false;
   theFilename   = "";

   return false;
}

void ossimNadconGridFile::close()
{
   theInputFile.close();
}

double ossimNadconGridFile::getShiftAtLatLon(double lat, double lon)const
{
   double result = ossim::nan();
   if(pointWithin(lat, lon))
   {
      double x = (lon - theLatLonOrigin.lon)/(double)theHeader.getDeltaX();
      double y = (lat - theLatLonOrigin.lat)/(double)theHeader.getDeltaY();

      int lat0 = (int)y;
      int lat1 = lat0 + 1;
      int lon0 = (int)x;
      int lon1 = lon0 + 1;

      int rows = theHeader.getNumberOfRows();
      int cols = theHeader.getNumberOfCols();

      if(lat1 >= rows) lat1 = lat0;
      if(lon1 >= cols) lon1 = lon0;

      double tLat = y - lat0;
      double tLon = x - lon0;

      int offset00 = lat0*theHeader.getBytesPerRow() + lon0*4 + theHeader.getStartOffset();
      int offset01 = lat0*theHeader.getBytesPerRow() + lon1*4 + theHeader.getStartOffset();
      int offset11 = lat1*theHeader.getBytesPerRow() + lon1*4 + theHeader.getStartOffset();
      int offset10 = lat1*theHeader.getBytesPerRow() + lon0*4 + theHeader.getStartOffset();
      
      
      double v00 = 0.0;
      double v01 = 0.0;
      double v11 = 0.0;
      double v10 = 0.0;
      
      theInputFile.seekg((std::streampos)offset00);
      theInputFile.read((char*)&v00, 4);
      theInputFile.seekg((std::streampos)offset01);
      theInputFile.read((char*)&v01, 4);
      theInputFile.seekg((std::streampos)offset11);
      theInputFile.read((char*)&v11, 4);
      theInputFile.seekg((std::streampos)offset10);
      theInputFile.read((char*)&v10, 4);
      ossimEndian anEndian;

      if(anEndian.getSystemEndianType() != OSSIM_LITTLE_ENDIAN)
      {
         anEndian.swap(v00);
         anEndian.swap(v01);
         anEndian.swap(v11);
         anEndian.swap(v10);
      }
      
      double top    = (double)v00 + ((double)v01 - (double)v00)*tLon;
      double bottom = (double)v10 + ((double)v11 - (double)v10)*tLon;
      
      result = top + (bottom-top)*tLat;
   }
   
   return result;
}

bool ossimNadconGridFile::pointWithin(double lat, double lon)const
{
  return theBoundingRect.pointWithin(ossimDpt(lon, lat));
}
