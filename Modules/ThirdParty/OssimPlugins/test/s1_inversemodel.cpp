#include <vector>

#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/tuple/tuple.hpp>

#include <ossim/base/ossimXmlDocument.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimXmlNode.h>
#include <ossim/base/ossimEcefPoint.h>
#include <ossim/base/ossimEcefVector.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>


typedef boost::tuple<boost::posix_time::ptime,ossimEcefPoint,ossimEcefVector>                       RecordType;
typedef boost::tuple<boost::posix_time::ptime,double,ossimDpt,ossimGpt>                             GCPType;
typedef std::vector<RecordType>                                                                     RecordVectorType;
typedef std::vector<GCPType>                                                                        GCPVectorType;
typedef boost::tuple<boost::posix_time::ptime,unsigned long,boost::posix_time::ptime,unsigned long> BurstRecordType;
typedef std::vector<BurstRecordType>                                                                BurstRecordVectorType;

typedef boost::tuple<boost::posix_time::ptime,double,std::vector<double> >                          SRGRRecordType;
typedef std::vector<SRGRRecordType>                                                                 SRGRRecordVectorType;

const double C = 299792458;

double compute_doppler(const double& radarFreq,const ossimEcefVector& vel, const ossimEcefPoint& sensorPos, const ossimEcefPoint& inputPos)
{
  // Eq. 19, p. 25
  const ossimEcefVector s2gVec = inputPos - sensorPos;

  return 2 * radarFreq * vel.dot(s2gVec);
}


void interpolatePosVel(const boost::posix_time::ptime t, const RecordVectorType& records, ossimEcefPoint& pos, ossimEcefVector & vel, unsigned int deg = 8)
{
  unsigned int nBegin(0), nEnd(0);

  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;

  vel[0] = 0;
  vel[1] = 0;
  vel[2] = 0;

  if(records.size()<deg)
    {
    nEnd = records.size()-1;
    }
  else
    {

    unsigned int t_min_idx = 0;
    boost::posix_time::time_duration t_min = t - boost::get<0>(records[0]);

    if(t_min.is_negative())
      t_min = t_min.invert_sign();

    unsigned int count = 0;

    for (RecordVectorType::const_iterator b = records.begin(), e = records.end()
            ; b != e
            ; ++b, ++count
        )
      {
      boost::posix_time::time_duration current_time = t-boost::get<0>(*b);

      if(current_time.is_negative())
        current_time = current_time.invert_sign();

      if(t_min > current_time)
        {
        t_min_idx = count;
        t_min = current_time;
        }
      }
    nBegin = std::max((int)t_min_idx-(int)deg/2+1,(int)0);
    nEnd = std::min(nBegin+deg-1,(unsigned int)records.size());
    nBegin = nEnd<records.size()-1 ? nBegin : nEnd-deg+1;
    }

  for(unsigned int i = nBegin; i < nEnd; ++i)
    {

    double w = 1.;

    for(unsigned int j = nBegin; j < nEnd; ++j)
      {
      if(j!=i)
        {
        const double td1 = (t - boost::get<0>(records[j])).total_microseconds();
        const double td2 = (boost::get<0>(records[i]) - boost::get<0>(records[j])).total_microseconds();
        w*=td1/td2;
        }
      }

    pos[0]+=w*boost::get<1>(records[i])[0];
    pos[1]+=w*boost::get<1>(records[i])[1];
    pos[2]+=w*boost::get<1>(records[i])[2];

    vel[0]+=w*boost::get<2>(records[i])[0];
    vel[1]+=w*boost::get<2>(records[i])[1];
    vel[2]+=w*boost::get<2>(records[i])[2];
    }
}


double slantRangeToGroundRange(const double & slantRange, const boost::posix_time::ptime& azimuthTime, const SRGRRecordVectorType & srgrRecords)
{
    SRGRRecordVectorType::const_iterator it = srgrRecords.begin();

    SRGRRecordType srgrRecord  = *it;

    if(azimuthTime > boost::get<0>(srgrRecord))
    {
        SRGRRecordVectorType::const_iterator previousRecord = it;

        ++it;

        SRGRRecordVectorType::const_iterator nextrecord = it;

        bool found = false;

        // Look for the correct record
        while(it!=srgrRecords.end() && !found)
        {
            if(azimuthTime > boost::get<0>(*previousRecord)
                    && azimuthTime < boost::get<0>(*nextrecord))
            {
                found = true;
            }
            else
            {
                previousRecord = nextrecord;
                ++it;
                nextrecord = it;
            }
        }
        if(!found)
        {
            srgrRecord = *previousRecord;
        }
        else
        {
            // If azimuth time is between 2 records, interpolate
            const double interp = (azimuthTime-boost::get<0>(*previousRecord)).total_microseconds()/static_cast<double>((boost::get<0>(*nextrecord)-boost::get<0>(*previousRecord)).total_microseconds());

            const double sr0 = (1-interp) * boost::get<1>(*previousRecord)+ interp*boost::get<1>(*nextrecord);

            std::vector<double> coefs;
            std::vector<double>::const_iterator pIt = boost::get<2>(*previousRecord).begin();
            std::vector<double>::const_iterator nIt = boost::get<2>(*nextrecord).begin();
            for(;pIt != boost::get<2>(*previousRecord).end() && nIt != boost::get<2>(*nextrecord).end();
                    ++pIt,++nIt)
            {
                coefs.push_back(interp*(*nIt)+(1-interp)*(*pIt));
            }

            srgrRecord = boost::make_tuple(azimuthTime,sr0,coefs);
        }
    }

    // Now that we have the interpolated coefs, compute ground range
    // from slant range
    const double sr_minus_sr0 = slantRange - boost::get<1>(srgrRecord);

    double ground_range = 0;

    for(std::vector<double>::const_reverse_iterator cIt = boost::get<2>(srgrRecord).rbegin();cIt!=boost::get<2>(srgrRecord).rend();++cIt)
    {
        ground_range = *cIt + sr_minus_sr0*ground_range;
    }

    return ground_range;
}

ossimDpt inverse_loc(const double & radarFreq, const boost::posix_time::ptime acqStartTime, const double & azimythTimeIntervalInMicroSeconds, const double & nearRangeTime, const double & rangeSamplingRate, const double & rangeRes, const RecordVectorType & records, const BurstRecordVectorType& burstRecords, const SRGRRecordVectorType& srgrRecords, const ossimGpt& worldPoint, boost::posix_time::ptime& estimatedTime, double & estimatedSlantRangeTime)
{
  // First convert lat/lon to ECEF
  const ossimEcefPoint inputPt(worldPoint);

  RecordVectorType::const_iterator it = records.begin();

  double lastDoppler = compute_doppler(radarFreq, boost::get<2>(*it),boost::get<1>(*it),inputPt);
  RecordVectorType::const_iterator lastRecord = it;

  const bool lastDopplerSign = lastDoppler < 0;
  bool found = false;

  ++it;

  RecordVectorType::const_iterator currentRecord = it;
  double currentDoppler = lastDoppler;

  // Look for the consecutive records where doppler freq changes sign
  while(it!=records.end() && !found)
    {
    currentRecord = it;
    currentDoppler = compute_doppler(radarFreq, boost::get<2>(*it),boost::get<1>(*it),inputPt);

    const bool currentDopplerSign = currentDoppler< 0;

    //std::cout<<"last doppler: "<<lastDoppler<<", current doppler: "<<currentDoppler<<std::endl;

    if(currentDopplerSign != lastDopplerSign)
      {
      found = true;
      }
    else
      {
      lastDoppler = currentDoppler;
      lastRecord = currentRecord;
      ++it;
      }
    }

  // TODO: Handle not found case here

  // now interpolate time and sensor position
  const double interp = std::abs(lastDoppler)/(std::abs(lastDoppler)+std::abs(currentDoppler));

  //std::cout<<"Interpolation coef: "<<interp<<std::endl;

  // Here I do not now if microsecond is sufficient
  const boost::posix_time::time_duration delta_td = boost::get<0>(*currentRecord) - boost::get<0>(*lastRecord);
  const double deltat = delta_td.total_microseconds();

  //std::cout<<"Corresponding deltat: "<<deltat<<" µs\n";

  const boost::posix_time::time_duration td = boost::posix_time::microseconds(static_cast<unsigned long>(floor(interp * deltat+0.5)));

  estimatedTime = boost::get<0>(*lastRecord)+td;

  //std::cout<<"Times: "<<boost::posix_time::to_simple_string(boost::get<0>(*lastRecord))<<" < "<<boost::posix_time::to_simple_string(estimatedTime)<<" < "<<boost::posix_time::to_simple_string(boost::get<0>(*currentRecord))<<std::endl;



  // ossimEcefVector deltaPos = (boost::get<1>(*currentRecord)-boost::get<1>(*lastRecord));
  // deltaPos = deltaPos * interp;
  // ossimEcefPoint interSensorPos = boost::get<1>(*lastRecord) + deltaPos;

  ossimEcefPoint interSensorPos;
  ossimEcefVector interSensorVel;

  interpolatePosVel(estimatedTime,records,interSensorPos,interSensorVel);


  //std::cout<<"Sensor positions: "<<boost::get<1>(*lastRecord)<<" <
  //"<<interSensorPos<<" < "<<boost::get<1>(*currentRecord)<<std::endl;


  // TODO: Handle possible bistatic bias correction here (not needed
  // for S1)

  // Bistatic correction (eq 25, p 28)


  // TODO: Look this in metadata (if it should be applied)
  const bool bistatic_correction = false;

  if(bistatic_correction)
    {
    const double halftrange = 1000000 * (interSensorPos-inputPt).magnitude()/C;
    const boost::posix_time::time_duration bistatic_td = boost::posix_time::microseconds(static_cast<unsigned long>(floor(halftrange+0.5)));
    estimatedTime = estimatedTime + bistatic_td;

    std::cout<<"Bistatic td: "<<boost::posix_time::to_simple_string(bistatic_td)<<std::endl;

    // interp = static_cast<double>((td+bistatic_td).total_microseconds())/static_cast<double>(delta_td.total_microseconds());

    // deltaPos = (boost::get<1>(*currentRecord)-boost::get<1>(*lastRecord));
    // deltaPos = deltaPos * interp;
    // interSensorPos = boost::get<1>(*lastRecord) + deltaPos;

    interpolatePosVel(estimatedTime,records,interSensorPos,interSensorVel);
    }

  ossimDpt resp;

  if(burstRecords.empty())
    {
    // Now compute fractional line index
    // Eq 22 p 27

    const boost::posix_time::time_duration timeSinceStart = (estimatedTime-acqStartTime);

    const double timeSinceStartInMicroSeconds = timeSinceStart.total_microseconds();

    //std::cout<<"timeSinceStartInMicroSeconds: "<<timeSinceStartInMicroSeconds<<" µs\n";

    resp.y = timeSinceStartInMicroSeconds/azimythTimeIntervalInMicroSeconds;
    }
  else
    {
    boost::posix_time::ptime acqStart;
    bool burstFound(false);
    unsigned long acqStartLine(0);

    for (BurstRecordVectorType::const_reverse_iterator bIt = burstRecords.rbegin(), e = burstRecords.rend()
            ; bIt != e && !burstFound
            ; ++bIt
        )
      {
      if(estimatedTime > boost::get<0>(*bIt) && estimatedTime < boost::get<2>(*bIt))
        {
        burstFound = true;
        acqStart = boost::get<0>(*bIt);
        acqStartLine = boost::get<1>(*bIt);
        }
      }

    if(!burstFound)
      {
      if(estimatedTime < boost::get<0>(burstRecords.front()))
        {
        acqStart = boost::get<0>(burstRecords.front());
        acqStartLine = boost::get<1>(burstRecords.front());
        }
      else if (estimatedTime > boost::get<0>(burstRecords.back()))
        {
        acqStart = boost::get<0>(burstRecords.back());
        acqStartLine = boost::get<1>(burstRecords.back());
        }
      }

    std::cout<<boost::posix_time::to_simple_string(acqStart)<<" "<<acqStartLine<<std::endl;

    const boost::posix_time::time_duration timeSinceStart = (estimatedTime-acqStart);

    const double timeSinceStartInMicroSeconds = timeSinceStart.total_microseconds();

    std::cout<<"timeSinceStartInMicroSeconds: "<<timeSinceStartInMicroSeconds<<" µs "<< timeSinceStartInMicroSeconds/azimythTimeIntervalInMicroSeconds <<std::endl;

    resp.y = timeSinceStartInMicroSeconds/azimythTimeIntervalInMicroSeconds + acqStartLine;
    }

  // TODO: Here this is different for products with burst

  //std::cout<<"Estimated line position: "<<resp.y<<std::endl;

  // Now compute fractional sample index
  // Eq 23 and 24 p. 28

  const double range_distance = (interSensorPos-inputPt).magnitude();
  estimatedSlantRangeTime = 2*range_distance/C;

  if(srgrRecords.empty())
    {
    // SLC product case
    resp.x = (estimatedSlantRangeTime - nearRangeTime)*rangeSamplingRate;
    }
  else
    {
    const double ground_range = slantRangeToGroundRange(range_distance,estimatedTime,srgrRecords);
    const double near_ground_range = slantRangeToGroundRange(nearRangeTime*C/2,estimatedTime,srgrRecords);

    resp.x = (ground_range - near_ground_range)/rangeRes;
    }

  //std::cout<<"Estimated sample position: "<<resp.x<<std::endl;

  return resp;
}



int main(int argc, char * argv[])
{
  std::cout.precision(9);

  if(argc != 2)
    return EXIT_FAILURE;

  try {
      std::string annotationXml = argv[1];

      unsigned int nbLines = 0;
      unsigned int nbSamples = 0;
      boost::posix_time::ptime acqStartTime;
      boost::posix_time::ptime acqStopTime;
      double nearRangeTime(0.);
      double rangeSamplingRate(0.);

      RecordVectorType records;
      SRGRRecordVectorType srgrRecords;

      ossimRefPtr<ossimXmlDocument> xmlDoc = new ossimXmlDocument(annotationXml);

      const std::string product_type = xmlDoc->getRoot()->findFirstNode("adsHeader/productType")->getText();
      const std::string mode = xmlDoc->getRoot()->findFirstNode("adsHeader/mode")->getText();
      const std::string swath = xmlDoc->getRoot()->findFirstNode("adsHeader/swath")->getText();
      const std::string polarisation = xmlDoc->getRoot()->findFirstNode("adsHeader/polarisation")->getText();

      const bool isGrd = (product_type == "GRD");

      std::cout<<"Product type: "<<product_type<<std::endl;
      std::cout<<"Mode: "<<mode<<", swath: "<<swath<<", polarisation: "<<polarisation<<std::endl<<std::endl;

      // First, lookup position/velocity records
      std::vector<ossimRefPtr<ossimXmlNode> > xnodes;
      xmlDoc->findNodes("/product/generalAnnotation/orbitList/orbit",xnodes);

      std::cout<<"Reading orbit records ...\n";
      std::cout<<"Number of orbit records found: "<<xnodes.size()<<std::endl;

      for (std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator itNode = xnodes.begin(), e = xnodes.end()
              ; itNode != e
              ; ++itNode
          )
      {
          // Retrieve acquisition time
          ossimString att1 = "time";
          ossimString s = (*itNode)->findFirstNode(att1)->getText();
          std::replace(s.begin(), s.end(), 'T', ' ');
          std::cout<<s;
          boost::posix_time::ptime acqTime(boost::posix_time::time_from_string(s));

          // Retrieve ECEF position
          ossimEcefPoint pos;
          att1 = "position";
          ossimString att2 = "x";
          pos[0] = atof((*itNode)->findFirstNode(att1)->findFirstNode(att2)->getText().c_str());
          att2 = "y";
          pos[1] = atof((*itNode)->findFirstNode(att1)->findFirstNode(att2)->getText().c_str());
          att2 = "z";
          pos[2] = atof((*itNode)->findFirstNode(att1)->findFirstNode(att2)->getText().c_str());

          std::cout<<", ECEF pos: "<<pos[0]<<", "<<pos[1]<<", "<<pos[2];

          // Retrieve ECEF velocity
          ossimEcefVector vel;
          att1 = "velocity";
          att2 = "x";
          vel[0] = atof((*itNode)->findFirstNode(att1)->findFirstNode(att2)->getText().c_str());
          att2 = "y";
          vel[1] = atof((*itNode)->findFirstNode(att1)->findFirstNode(att2)->getText().c_str());
          att2 = "z";
          vel[2] = atof((*itNode)->findFirstNode(att1)->findFirstNode(att2)->getText().c_str());

          std::cout<<", ECEF vel: "<<vel[0]<<", "<<vel[1]<<", "<<vel[2]<<std::endl;

          records.push_back(make_tuple(acqTime,pos,vel));
      }
      std::cout<<"done.\n\n";


      std::cout<<"Reading other useful values ...\n";
      ossimString s = xmlDoc->getRoot()->findFirstNode("imageAnnotation/imageInformation/productFirstLineUtcTime")->getText();
      //ossimString s = xmlDoc->getRoot()->findFirstNode("generalAnnotation/downlinkInformationList/downlinkInformation/firstLineSensingTime")->getText();
      std::replace(s.begin(), s.end(), 'T', ' ');
      std::cout<<"Acquisition start time: "<<s<<std::endl;
      acqStartTime = boost::posix_time::time_from_string(s);

      s = xmlDoc->getRoot()->findFirstNode("imageAnnotation/imageInformation/productLastLineUtcTime")->getText();
      //s = xmlDoc->getRoot()->findFirstNode("generalAnnotation/downlinkInformationList/downlinkInformation/lastLineSensingTime")->getText();
      std::replace(s.begin(), s.end(), 'T', ' ');
      std::cout<<"Acquisition stop time: "<<s<<std::endl;
      acqStopTime = boost::posix_time::time_from_string(s);

      nbLines = xmlDoc->getRoot()->findFirstNode("imageAnnotation/imageInformation/numberOfLines")->getText().toUInt16();
      nbSamples = xmlDoc->getRoot()->findFirstNode("imageAnnotation/imageInformation/numberOfSamples")->getText().toUInt16();

      std::cout<<"Image size: "<<nbSamples<<" x "<<nbLines<<std::endl;

      nearRangeTime = xmlDoc->getRoot()->findFirstNode("imageAnnotation/imageInformation/slantRangeTime")->getText().toDouble();
      std::cout<<"Near range time: "<<nearRangeTime<<" s\n";

      const double nearRangeDistance = nearRangeTime * C /2;

      std::cout<<"Near range distance: "<<nearRangeDistance<< "m\n";

      rangeSamplingRate = xmlDoc->getRoot()->findFirstNode("generalAnnotation/productInformation/rangeSamplingRate")->getText().toDouble();
      const double estimatedRangeRes = (1/rangeSamplingRate)*C/2;
      std::cout<<"Range sampling rate: "<<rangeSamplingRate<<" Hz (estimated range res: "<<estimatedRangeRes<<" m)\n";

      const double rangeRes = xmlDoc->getRoot()->findFirstNode("imageAnnotation/imageInformation/rangePixelSpacing")->getText().toDouble();
      std::cout<<"Range res from product: "<<rangeRes<<" m\n";

      const double radarFrequency = xmlDoc->getRoot()->findFirstNode("generalAnnotation/productInformation/radarFrequency")->getText().toDouble();
      std::cout<<"Radar frequency: "<<radarFrequency<<" Hz\n";

      const boost::posix_time::time_duration td = (acqStopTime - acqStartTime);

      const double acquisition_duration = td.total_microseconds();

      std::cout<<"Acquisition duration: "<<boost::posix_time::to_simple_string(td)<<" ("<<acquisition_duration/1000000<<" s)\n";

      const double estimatedAzimuthTimeIntervalInMicroSeconds = acquisition_duration/nbLines;
      const double prf = 1000000/estimatedAzimuthTimeIntervalInMicroSeconds;

      const double azimuthTimeIntervalInMicroSeconds = xmlDoc->getRoot()->findFirstNode("imageAnnotation/imageInformation/azimuthTimeInterval")->getText().toDouble()*1000000;

      std::cout<<"Estimated prf: "<<prf<<" Hz ("<<estimatedAzimuthTimeIntervalInMicroSeconds/1000000<<" s)\n";
      std::cout<<"Azimuth time interval from product: "<<azimuthTimeIntervalInMicroSeconds/1000000<<" s\n";

      std::cout<<"Done.\n\n";

      // Now read burst records as well
      BurstRecordVectorType burstRecords;

      std::cout<<"Reading burst records ...\n";
      xnodes.clear();
      xmlDoc->findNodes("/product/swathTiming/burstList/burst",xnodes);

      if(xnodes.empty())
      {
          std::cout<<"No burst records found, skipping\n";
      }
      else
      {
          std::cout<<"Number of burst records found: "<<xnodes.size()<<std::endl;

          const unsigned int linesPerBurst = xmlDoc->getRoot()->findFirstNode("swathTiming/linesPerBurst")->getText().toUInt16();

          std::cout<<"LinesPerBurst: "<<linesPerBurst<<std::endl;

          unsigned int burstId(0);

          for (std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator itNode = xnodes.begin(), e = xnodes.end()
                  ; itNode != e
                  ; ++itNode, ++burstId
              )
          {
              ossimString att1 = "azimuthTime";
              ossimString s2 = (*itNode)->findFirstNode(att1)->getText();
              std::replace(s2.begin(), s2.end(), 'T', ' ');
              boost::posix_time::ptime azTime(boost::posix_time::time_from_string(s2));

              att1 = "firstValidSample";
              s2 = (*itNode)->findFirstNode(att1)->getText();

              long first_valid(0), last_valid(0);
              bool begin_found(false), end_found(false);

              std::vector<ossimString> ssp = s2.split(" ");

              for (std::vector<ossimString>::const_iterator sIt = ssp.begin(), sEnd = ssp.end()
                      ; sIt != sEnd && !end_found
                      ; ++sIt
                  )
              {
                  if(!begin_found)
                  {
                      if(*sIt!="-1")
                      {
                          begin_found = true;
                      }
                      else
                      {
                          ++first_valid;
                      }
                      ++last_valid;
                  }
                  else
                  {
                      if(!end_found && *sIt=="-1")
                      {
                          end_found = true;
                      }
                      else
                      {
                          ++last_valid;
                      }
                  }
              }

              const unsigned long burstFirstValidLine = burstId*linesPerBurst + first_valid;
              const unsigned long burstLastValidLine  = burstId*linesPerBurst + last_valid;

              const boost::posix_time::ptime burstFirstValidTime = azTime + boost::posix_time::microseconds(first_valid*azimuthTimeIntervalInMicroSeconds);
              const boost::posix_time::ptime burstLastValidTime = azTime + boost::posix_time::microseconds(last_valid*azimuthTimeIntervalInMicroSeconds);

              std::cout<<"Burst #"<<burstId<<std::endl;
              std::cout<<"FirstValidSample: "<<burstFirstValidLine<<" ("<<burstFirstValidTime<<")\n";
              std::cout<<"LastValidSample: "<<burstLastValidLine<<" ("<<burstLastValidTime<<")\n";

              burstRecords.push_back(boost::make_tuple(burstFirstValidTime, burstFirstValidLine,burstLastValidTime,burstLastValidLine));
          }
      }
      std::cout<<"Done.\n\n";

      if(isGrd)
      {
          std::cout<<"Reading Slant range to Ground range coefficients ...\n";

          xnodes.clear();
          xmlDoc->findNodes("/product/coordinateConversion/coordinateConversionList/coordinateConversion",xnodes);
          std::cout<<"Number of records found: "<<xnodes.size()<<std::endl;

          for (std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator itNode = xnodes.begin(), e = xnodes.end()
                  ; itNode != e
                  ; ++itNode
              )
          {
              ossimString att1 = "azimuthTime";
              ossimString s2 = (*itNode)->findFirstNode(att1)->getText();
              std::replace(s2.begin(), s2.end(), 'T', ' ');
              boost::posix_time::ptime azTime(boost::posix_time::time_from_string(s2));

              att1 = "sr0";
              double sr0 = (*itNode)->findFirstNode(att1)->getText().toDouble();

              att1 = "srgrCoefficients";
              s2 = (*itNode)->findFirstNode(att1)->getText();
              std::vector<ossimString> ssplit = s2.split(" ");

              std::vector<double> coefs;

              for (std::vector<ossimString>::const_iterator cIt = ssplit.begin(), cEnd = ssplit.end()
                      ; cIt != cEnd
                      ; ++cIt
                  )
              {
                  coefs.push_back(cIt->toDouble());
              }

              srgrRecords.push_back(boost::make_tuple(azTime,sr0,coefs));
          }

          std::cout<<"Done.\n\n";
      }


      std::cout<<"Reading ground control points ...\n";
      GCPVectorType gcps;
      xnodes.clear();
      xmlDoc->findNodes("/product/geolocationGrid/geolocationGridPointList/geolocationGridPoint",xnodes);
      std::cout<<"Number of GCPs found: "<<xnodes.size()<<std::endl;

      for (std::vector<ossimRefPtr<ossimXmlNode> >::const_iterator itNode = xnodes.begin(), eNode = xnodes.end()
              ; itNode != eNode
              ; ++itNode
          )
      {
          // Retrieve acquisition time
          ossimString att1 = "azimuthTime";
          ossimString s2 = (*itNode)->findFirstNode(att1)->getText();
          std::replace(s2.begin(), s2.end(), 'T', ' ');
          boost::posix_time::ptime azTime(boost::posix_time::time_from_string(s2));

          att1 = "slantRangeTime";
          double slantRangeTime = (*itNode)->findFirstNode(att1)->getText().toDouble();

          ossimDpt imPoint;
          att1 = "pixel";
          imPoint.x = (*itNode)->findFirstNode(att1)->getText().toDouble();
          // att1 = "line";
          // imPoint.y = (*itNode)->findFirstNode(att1)->getText().toDouble();

          if(!burstRecords.empty())
          {
              boost::posix_time::ptime acqStart;
              bool burstFound(false);
              unsigned long acqStartLine(0);

              for (BurstRecordVectorType::const_reverse_iterator bIt = burstRecords.rbegin(), bEnd = burstRecords.rend()
                      ; bIt != bEnd && !burstFound
                      ; ++bIt
                  )
              {
                  if(azTime > boost::get<0>(*bIt) && azTime < boost::get<2>(*bIt))
                  {
                      burstFound = true;
                      acqStart = boost::get<0>(*bIt);
                      acqStartLine = boost::get<1>(*bIt);
                  }
              }

              if(!burstFound)
              {
                  if(azTime < boost::get<0>(burstRecords.front()))
                  {
                      acqStart = boost::get<0>(burstRecords.front());
                      acqStartLine = boost::get<1>(burstRecords.front());
                  }
                  else if (azTime > boost::get<0>(burstRecords.back()))
                  {
                      acqStart = boost::get<0>(burstRecords.back());
                      acqStartLine = boost::get<1>(burstRecords.back());
                  }
              }
              boost::posix_time::time_duration timeSinceStart = (azTime-acqStart);

              const double timeSinceStartInMicroSeconds = timeSinceStart.total_microseconds();
              imPoint.y = timeSinceStartInMicroSeconds/azimuthTimeIntervalInMicroSeconds + acqStartLine;
          }

          else
          {
              att1 = "line";
              imPoint.y = (*itNode)->findFirstNode(att1)->getText().toDouble();
          }
          ossimGpt geoPoint;
          att1 = "latitude";
          geoPoint.lat = (*itNode)->findFirstNode(att1)->getText().toDouble();
          att1 = "longitude";
          geoPoint.lon = (*itNode)->findFirstNode(att1)->getText().toDouble();
          att1 = "height";
          geoPoint.hgt = (*itNode)->findFirstNode(att1)->getText().toDouble();

          gcps.push_back(make_tuple(azTime,slantRangeTime,imPoint,geoPoint));
      }
      std::cout<<"Done.\n\n";

      unsigned int count = 1;

      for (GCPVectorType::const_iterator itGcp = gcps.begin(), e = gcps.end()
              ; itGcp != e
              ; ++itGcp, ++count
          )
      {
          boost::posix_time::ptime estimatedTime;
          double estimatedSlantRangeTime;

          const ossimDpt estimatedPos = inverse_loc(radarFrequency,acqStartTime,azimuthTimeIntervalInMicroSeconds, nearRangeTime, rangeSamplingRate, rangeRes, records, burstRecords,srgrRecords,boost::get<3>(*itGcp),estimatedTime,estimatedSlantRangeTime);

          std::cout<<"ProcessingGCP #"<<count<<":\n";
          std::cout<<"Position: "<<boost::get<2>(*itGcp)<<", estimated: "<<estimatedPos<<", residual: "<<estimatedPos-boost::get<2>(*itGcp)<<std::endl;
          std::cout<<"Azimuth time: "<<boost::posix_time::to_simple_string(boost::get<0>(*itGcp))<<", estimated: "<<boost::posix_time::to_simple_string(estimatedTime)<<", residual: "<<boost::posix_time::to_simple_string(estimatedTime-boost::get<0>(*itGcp))<<std::endl;
          std::cout<<"Slant range time: "<<boost::get<1>(*itGcp)<<", estimated: "<<estimatedSlantRangeTime<<", residual: "<<boost::get<1>(*itGcp)-estimatedSlantRangeTime<<std::endl;
          std::cout<<std::endl;
          return EXIT_SUCCESS;
      }
  } catch (std::exception const& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return EXIT_FAILURE;
  } 
}
