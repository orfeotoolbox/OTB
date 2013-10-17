//*******************************************************************
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Andrew Johnson
//
//*******************************************************************
//  $Id: ossimBuckeyeSensor.cpp  $
#include <sstream>
#include <ossim/projection/ossimBuckeyeSensor.h>
#include <ossim/base/ossimEllipsoid.h>
#include <ossim/base/ossimUnitConversionTool.h>
#include <ossim/base/ossimUnitTypeLut.h>
#include <ossim/base/ossimDatumFactory.h>
#include <ossim/base/ossimDatum.h>
#include <ossim/base/ossimException.h>
#include <ossim/base/ossimLsrSpace.h>
#include <ossim/base/ossimTrace.h>
#include <ossim/base/ossimGeoidManager.h>
#include <ossim/projection/ossimUtmProjection.h>
#include <ossim/base/ossimMatrix4x4.h>
#include <ossim/elevation/ossimElevManager.h>
#include <ossim/base/ossimCsvFile.h>
static ossimTrace traceDebug("ossimBuckeyeSensor:debug");

RTTI_DEF1(ossimBuckeyeSensor, "ossimBuckeyeSensor", ossimSensorModel);

ossimBuckeyeSensor::ossimBuckeyeSensor()
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::ossimBuckeyeSensor(): entering..." << std::endl;

	theCompositeMatrix			= ossimMatrix4x4::createIdentity();
	theCompositeMatrixInverse	= ossimMatrix4x4::createIdentity();
	theRoll						= 0.0;
	thePitch					= 0.0;
	theHeading					= 0.0;
	theFocalLength				= 195.1000;
	thePixelSize				= ossimDpt(.006, .006);
	thePrincipalPoint			= ossimDpt(0, 0);
	theEcefPlatformPosition		= ossimGpt(0.0,0.0, 1000.0);
	theAdjEcefPlatformPosition	= ossimGpt(0.0,0.0, 1000.0);
	theLensDistortion			= new ossimSmacCallibrationSystem();

	theGSD.makeNan();
	initAdjustableParameters();

	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::ossimBuckeyeSensor(): returning..." << std::endl;
}
ossimBuckeyeSensor::ossimBuckeyeSensor(const ossimDrect& imageRect,
	const ossimGpt& platformPosition,
	double roll,
	double pitch,
	double heading,
	const ossimDpt& /* principalPoint */, // in millimeters
	double focalLength, // in millimeters
	const ossimDpt& pixelSize) // in millimeters
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::ossimBuckeyeSensor(imageRect,platformPosition,roll,pitch,heading,ossimDpt,focalLength,pixelSize): entering..." << std::endl;

	theImageClipRect			= imageRect;
	theRefImgPt					= theImageClipRect.midPoint();
	theCompositeMatrix          = ossimMatrix4x4::createIdentity();
	theCompositeMatrixInverse   = ossimMatrix4x4::createIdentity();
	theRoll                     = roll;
	thePitch                    = pitch;
	theHeading                  = heading;
	theFocalLength              = focalLength;
	thePixelSize                = pixelSize;
	theEcefPlatformPosition     = platformPosition;
	theAdjEcefPlatformPosition  = platformPosition;
	theLensDistortion           = new ossimSmacCallibrationSystem();
	theGSD.makeNan();

	initAdjustableParameters();
	updateModel();
	
	try
	{
		// Method throws ossimException.
		computeGsd();
	}
	catch (const ossimException& e)
	{
		ossimNotify(ossimNotifyLevel_WARN)
			<< "ossimBuckeyeSensor Constructor caught Exception:\n"
			<< e.what() << std::endl;
	}

	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::ossimBuckeyeSensor(imageRect,platformPosition,roll,pitch,heading,ossimDpt,focalLength,pixelSize): returning..." << std::endl;
}

ossimBuckeyeSensor::ossimBuckeyeSensor(const ossimBuckeyeSensor& src)
	:ossimSensorModel(src)
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::ossimBuckeyeSensor(src): entering..." << std::endl;

	initAdjustableParameters();

	if(src.theLensDistortion.valid())
	{
		theLensDistortion = new ossimSmacCallibrationSystem(*(src.theLensDistortion.get()));
	}
	else
	{
		theLensDistortion = new ossimSmacCallibrationSystem();
	}
	theGSD.makeNan();

	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::ossimBuckeyeSensor(src): returning..." << std::endl;
}

ossimObject* ossimBuckeyeSensor::dup()const
{
	return new ossimBuckeyeSensor(*this);
}

void ossimBuckeyeSensor::imagingRay(const ossimDpt& image_point,
	ossimEcefRay&   image_ray) const
{
	if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG) << "ossimBuckeyeSensor::imagingRay: ..... entered" << std::endl;

	ossimDpt f1 ((image_point) - theRefImgPt);
	f1.x *= thePixelSize.x;
	f1.y *= -thePixelSize.y;
	ossimDpt film (f1 - thePrincipalPoint);

	if(traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG) << "pixel size   = " << thePixelSize << std::endl;
		ossimNotify(ossimNotifyLevel_DEBUG) << "principal pt = " << thePrincipalPoint << std::endl;
		ossimNotify(ossimNotifyLevel_DEBUG) << "film pt      = " << film << std::endl;
	}

	if (theLensDistortion.valid())
	{
		ossimDpt filmOut;
		theLensDistortion->undistort(film, filmOut);
		film = filmOut;
	}

	ossimColumnVector3d cam_ray_dir (film.x,
		film.y,
		-theFocalLength);
	ossimEcefVector     ecf_ray_dir (theCompositeMatrix*cam_ray_dir);
	ecf_ray_dir = ecf_ray_dir*(1.0/ecf_ray_dir.magnitude());

	image_ray.setOrigin(theAdjEcefPlatformPosition);
	image_ray.setDirection(ecf_ray_dir);

	if(traceDebug()) ossimNotify(ossimNotifyLevel_DEBUG) << "ossimBuckeyeSensor::imagingRay: ..... leaving" << std::endl;
}

void ossimBuckeyeSensor::lineSampleToWorld(const ossimDpt& image_point,
	ossimGpt&       gpt) const
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::lineSampleToWorld:entering..." << std::endl;

	if(image_point.hasNans())
	{
		gpt.makeNan();
		return;
	}

	//***
	// Determine imaging ray and invoke elevation source object's services to
	// intersect ray with terrain model:
	//***

	ossimEcefRay ray;
	imagingRay(image_point, ray);
	ossimElevManager::instance()->intersectRay(ray, gpt);

	if (traceDebug())
	{
		ossimNotify(ossimNotifyLevel_DEBUG) << "image_point = " << image_point << std::endl;
		ossimNotify(ossimNotifyLevel_DEBUG) << "ray = " << ray << std::endl;
		ossimNotify(ossimNotifyLevel_DEBUG) << "gpt = " << gpt << std::endl;
	}

	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::lineSampleToWorld: returning..." << std::endl;
	return;
}

void ossimBuckeyeSensor::lineSampleHeightToWorld(const ossimDpt& image_point,
	const double&   heightEllipsoid,
	ossimGpt&       worldPoint) const
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::lineSampleHeightToWorld: entering..." << std::endl;
	if (!insideImage(image_point))
	{
		worldPoint.makeNan();
	}
	else
	{
		//***
		// First establish imaging ray from image point:
		//***
		ossimEcefRay ray;
		imagingRay(image_point, ray);
		ossimEcefPoint Pecf (ray.intersectAboveEarthEllipsoid(heightEllipsoid));
		worldPoint = ossimGpt(Pecf);
	}
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::lineSampleHeightToWorld: returning..." << std::endl;
}

void ossimBuckeyeSensor::worldToLineSample(const ossimGpt& world_point,
	ossimDpt&       image_point) const
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::worldToLineSample: entering..." << std::endl;
	if((theBoundGndPolygon.getNumberOfVertices() > 0)&&
		(!theBoundGndPolygon.hasNans()))
	{
		if (!(theBoundGndPolygon.pointWithin(world_point)))
		{
			image_point.makeNan();
			return;
		}         
	}
	ossimEcefPoint g_ecf(world_point);
	ossimEcefVector ecfRayDir(g_ecf - theAdjEcefPlatformPosition);
	ossimColumnVector3d camRayDir (theCompositeMatrixInverse*ecfRayDir.data());


	double scale = -theFocalLength/camRayDir[2];
	ossimDpt film (scale*camRayDir[0], scale*camRayDir[1]);

	if (theLensDistortion.valid())
	{
		ossimDpt filmOut;
		theLensDistortion->distort(film, filmOut);
		film = filmOut;
	}

	ossimDpt f1(film + thePrincipalPoint);
	ossimDpt p1(f1.x/thePixelSize.x,
		-f1.y/thePixelSize.y);

	ossimDpt p0 (p1.x + theRefImgPt.x,
		p1.y + theRefImgPt.y);

	image_point = p0;
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::worldToLineSample: returning..." << std::endl;
}

void ossimBuckeyeSensor::updateModel()
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::updateModel: entering..." << std::endl;
	ossimGpt gpt;
	ossimGpt wgs84Pt;
	double metersPerDegree = wgs84Pt.metersPerDegree().x;
	double degreePerMeter = 1.0/metersPerDegree;
	double latShift = -computeParameterOffset(1)*degreePerMeter;
	double lonShift = computeParameterOffset(0)*degreePerMeter;

	gpt = theEcefPlatformPosition;
	double height = gpt.height();
	gpt.height(height + computeParameterOffset(5));
	gpt.latd(gpt.latd() + latShift);
	gpt.lond(gpt.lond() + lonShift);
	theAdjEcefPlatformPosition = gpt;
	ossimLsrSpace lsrSpace(theAdjEcefPlatformPosition, theHeading+computeParameterOffset(4));

	// make a left handed roational matrix;
	ossimMatrix4x4 lsrMatrix(lsrSpace.lsrToEcefRotMatrix());
	NEWMAT::Matrix orientation = (ossimMatrix4x4::createRotationXMatrix(thePitch+computeParameterOffset(3), OSSIM_LEFT_HANDED)*
		ossimMatrix4x4::createRotationYMatrix(theRoll+computeParameterOffset(2), OSSIM_LEFT_HANDED));
	theCompositeMatrix        = (lsrMatrix.getData()*orientation);
	theCompositeMatrixInverse = theCompositeMatrix.i();

	theBoundGndPolygon.resize(4);
	// ossim_float64 w = theImageClipRect.width()*2.0;
	// ossim_float64 h = theImageClipRect.height()*2.0;
	theExtrapolateImageFlag = false;
	theExtrapolateGroundFlag = false;

	lineSampleToWorld(theImageClipRect.ul(),gpt);//+ossimDpt(-w, -h), gpt);
	theBoundGndPolygon[0] = gpt;
	lineSampleToWorld(theImageClipRect.ur(),gpt);//+ossimDpt(w, -h), gpt);
	theBoundGndPolygon[1] = gpt;
	lineSampleToWorld(theImageClipRect.lr(),gpt);//+ossimDpt(w, h), gpt);
	theBoundGndPolygon[2] = gpt;
	lineSampleToWorld(theImageClipRect.ll(),gpt);//+ossimDpt(-w, h), gpt);
	theBoundGndPolygon[3] = gpt;
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::updateModel: returning..." << std::endl;
}

void ossimBuckeyeSensor::initAdjustableParameters()
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::initAdjustableParameters: entering..." << std::endl;
	resizeAdjustableParameterArray(6);

	setAdjustableParameter(0, 0.0);
	setParameterDescription(0, "x_offset");
	setParameterUnit(0, "pixels");

	setAdjustableParameter(1, 0.0);
	setParameterDescription(1, "y_offset");
	setParameterUnit(1, "pixels");

	setAdjustableParameter(2, 0.0);
	setParameterDescription(2, "roll");
	setParameterUnit(2, "degrees");

	setAdjustableParameter(3, 0.0);
	setParameterDescription(3, "pitch");
	setParameterUnit(3, "degrees");

	setAdjustableParameter(4, 0.0);
	setParameterDescription(4, "heading");
	setParameterUnit(4, "degrees");

	setAdjustableParameter(5, 0.0);
	setParameterDescription(5, "altitude");
	setParameterUnit(5, "meters");

	// TODO: default to correct default values, or just leave it up to the input file, since we have different offsets for the B100, 182, and Metroliner, also need a z offset
	setParameterSigma(0, 1.0);
	setParameterSigma(1, 1.0);
	setParameterSigma(2, 0);
	setParameterSigma(3, 0);
	setParameterSigma(4, 0);
	setParameterSigma(5, 1000);
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::initAdjustableParameters: returning..." << std::endl;
}


bool ossimBuckeyeSensor::saveState(ossimKeywordlist& kwl,
	const char* prefix) const
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::saveState: entering..." << std::endl;
	ossimSensorModel::saveState(kwl, prefix);

	kwl.add(prefix, "type", "ossimBuckeyeSensor", true);

	kwl.add(prefix, "roll", theRoll, true);
	kwl.add(prefix, "pitch", thePitch, true);
	kwl.add(prefix, "heading", theHeading, true);
	kwl.add(prefix, "principal_point", "("+ossimString::toString(thePrincipalPoint.x) + "," + ossimString::toString(thePrincipalPoint.y)+")");
	kwl.add(prefix, "pixel_size", "("+ossimString::toString(thePixelSize.x) + "," + ossimString::toString(thePixelSize.y)+")");
	kwl.add(prefix, "focal_length", theFocalLength);
	kwl.add(prefix, "ecef_platform_position",
		ossimString::toString(theEcefPlatformPosition.x()) + " " +
		ossimString::toString(theEcefPlatformPosition.y()) + " " +
		ossimString::toString(theEcefPlatformPosition.z()));

	if(theLensDistortion.valid())
	{
		ossimString lensPrefix = ossimString(prefix)+"distortion.";
		theLensDistortion->saveState(kwl,
			lensPrefix.c_str());
	}
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::saveState: returning..." << std::endl;
	return true;
}

bool ossimBuckeyeSensor::loadState(const ossimKeywordlist& kwl,
	const char* prefix)
{
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::loadState: entering..." << std::endl;

	theImageClipRect = ossimDrect(0,0,8984,6732);
	theRefImgPt      = ossimDpt(4492, 3366);

	ossimSensorModel::loadState(kwl, prefix);
	if(getNumberOfAdjustableParameters() < 1)
	{
		initAdjustableParameters();
	}
	theEcefPlatformPosition    = ossimGpt(0.0,0.0,1000.0);
	theAdjEcefPlatformPosition = ossimGpt(0.0,0.0,1000.0);
	theRoll    = 0.0;
	thePitch   = 0.0;
	theHeading = 0.0;

	ossimString framemeta_gsti = kwl.find(prefix, "framemeta_gsti");
	ossimString framemeta = kwl.find(prefix,"framemeta");
	ossimString frame_number = kwl.find(prefix, "frame_number");
	ossimString pixel_size = kwl.find(prefix, "pixel_size");
	ossimString principal_point = kwl.find(prefix, "principal_point");
	ossimString focal_length = kwl.find(prefix, "focal_length");
	ossimString smac_radial = kwl.find(prefix, "smac_radial");
	ossimString smac_decent = kwl.find(prefix, "smac_decent");
	ossimString roll;
	ossimString pitch;
	ossimString yaw;
	ossimString platform_position;
	ossimFilename file_to_load;

	ossimString FRAME_STRING	= "Frame#";
	ossimString ROLL_STRING		= "Roll(deg)";
	ossimString PITCH_STRING	= "Pitch(deg)";
	ossimString YAW_STRING		= "Yaw(deg)";
	ossimString LAT_STRING		= "Lat(deg)";
	ossimString LON_STRING		= "Lon(deg)";
	ossimString HAE_STRING		= "HAE(m)";

	// Deal with the fact that there are 3 different types of 'FrameMeta' file
	if (framemeta_gsti.empty() && !framemeta.empty() && !frame_number.empty())
	{
		file_to_load.setFile(framemeta);
		YAW_STRING	= "Azimuth(deg)";
	}
	else if (!framemeta_gsti.empty() && framemeta.empty() && !frame_number.empty())
	{
		file_to_load.setFile(framemeta_gsti);
	}

	if (file_to_load.exists() && !frame_number.empty())
	{
		ossimCsvFile csv(" \t"); // we will use tab or spaces as seaparator
		if(csv.open(file_to_load))
		{
			if(csv.readHeader())
			{
				ossimCsvFile::StringListType heads = csv.fieldHeaderList();
				// Try to see if you can find the first header item, if not, then you either have a file that doesn't work in this case, or it's uppercase
				if (std::find(heads.begin(), heads.end(), FRAME_STRING) == heads.end())
				{
					FRAME_STRING	= FRAME_STRING.upcase();
					ROLL_STRING		= ROLL_STRING.upcase();
					PITCH_STRING	= PITCH_STRING.upcase();
					YAW_STRING		= YAW_STRING.upcase();
					LAT_STRING		= LAT_STRING.upcase();
					LON_STRING		= LON_STRING.upcase();
					HAE_STRING		= HAE_STRING.upcase();
				}

				ossimRefPtr<ossimCsvFile::Record> record;
				bool foundFrameNumber = false;
				while( ((record = csv.nextRecord()).valid()) && !foundFrameNumber)
				{
					if( (*record)[FRAME_STRING] == frame_number)
					{
						foundFrameNumber = true;
						roll = (*record)[ROLL_STRING];
						pitch = (*record)[PITCH_STRING];
						yaw = (*record)[YAW_STRING];
						platform_position = (*record)[LAT_STRING] + " " 
							+ (*record)[LON_STRING]+ " "
							+ (*record)[HAE_STRING] + " WGE";
					}
				}
			}
		}
		csv.close();
	}
	else
	{
		roll = kwl.find(prefix, "roll"); 
		pitch = kwl.find(prefix, "pitch"); 
		yaw = kwl.find(prefix, "heading"); 
		platform_position = kwl.find(prefix, "ecef_platform_position");
	}

	bool result = (!pixel_size.empty()&&
		!principal_point.empty()&&
		!focal_length.empty()&&
		!platform_position.empty());

	if(!focal_length.empty())
	{
		theFocalLength = focal_length.toDouble();
	}
	if(!pixel_size.empty())
	{
		thePixelSize.toPoint(pixel_size);
	}
	if(!roll.empty())
	{
		theRoll = roll.toDouble();
	}
	if(!pitch.empty())
	{
		thePitch = pitch.toDouble();
	}
	if(!yaw.empty())
	{
		theHeading   = yaw.toDouble();
	}
	if(!principal_point.empty())
	{
		thePrincipalPoint.toPoint(principal_point);
	}
	if(platform_position.contains("WGE"))
	{
		std::vector<ossimString> splitString;
		ossimString tempString(platform_position);
		tempString.split(splitString, ossimString(" "));
		std::string datumString;
		double lat=0.0, lon=0.0, h=0.0;
		if(splitString.size() > 2)
		{
			lat = splitString[0].toDouble();
			lon = splitString[1].toDouble();
			h = splitString[2].toDouble();
		}

		theEcefPlatformPosition = ossimGpt(lat,lon,h);
	} else {
		std::vector<ossimString> splitString;
		ossimString tempString(platform_position);
		tempString.split(splitString, ossimString(" "));
		std::string datumString;
		double x=0.0, y=0.0, z=0.0;
		if(splitString.size() > 2)
		{
			x = splitString[0].toDouble();
			y = splitString[1].toDouble();
			z = splitString[2].toDouble();
		}
		theEcefPlatformPosition = ossimEcefPoint(x, y, z);
	}
	theLensDistortion = 0;
	if(!smac_radial.empty()&&
		!smac_decent.empty())
	{
		std::vector<ossimString> radial;
		std::vector<ossimString> decent;
		smac_radial.split(radial, " ");
		smac_decent.split(decent, " ");
		if((radial.size() == 5)&&
			(decent.size() == 4))
		{
			// Just for debugging really.. optomization will make this sleeker
			double k0 = radial[0].toDouble();
			double k1 = radial[1].toDouble();
			double k2 = radial[2].toDouble();
			double k3 = radial[3].toDouble();
			double k4 = radial[4].toDouble();

			double p0 = decent[0].toDouble();
			double p1 = decent[1].toDouble();
			double p2 = decent[2].toDouble();
			double p3 = decent[3].toDouble();

			//theLensDistortion = new ossimSmacCallibrationSystem(radial[0].toDouble(),
			//	radial[1].toDouble(),
			//	radial[2].toDouble(),
			//	radial[3].toDouble(),
			//	radial[4].toDouble(),
			//	decent[0].toDouble(),
			//	decent[1].toDouble(),
			//	decent[2].toDouble(),
			//	decent[3].toDouble());

			theLensDistortion = new ossimSmacCallibrationSystem(k0,k1,k2,k3,k4,p0,p1,p2,p3);
		}
	}
	theImageSize = ossimDpt(theImageClipRect.width(),
		theImageClipRect.height());

	updateModel();

	if(theGSD.isNan())
	{
		try
		{
			// This will set theGSD and theMeanGSD. Method throws ossimException.
			computeGsd();
		}
		catch (const ossimException& e)
		{
			if (traceDebug())
			{
				ossimNotify(ossimNotifyLevel_DEBUG)
					<< "ossimBuckeyeSensor::loadState Caught Exception:\n"
					<< e.what() << std::endl;
			}
		}
	}
	if (traceDebug())  ossimNotify(ossimNotifyLevel_DEBUG) << "DEBUG ossimBuckeyeSensor::loadState: returning..." << std::endl;
	return result;
}

bool ossimBuckeyeSensor::setupOptimizer(const ossimString& init_file)
{
	ossimKeywordlist kwl;
	kwl.addFile(init_file.c_str());

	return loadState(kwl);
}

void ossimBuckeyeSensor::setPrincipalPoint(ossimDpt principalPoint)
{
	thePrincipalPoint = principalPoint;
}

void ossimBuckeyeSensor::setRollPitchHeading(double roll,
	double pitch,
	double heading)
{
	theRoll    = roll;
	thePitch   = pitch;
	theHeading = heading;

	updateModel();
}

void ossimBuckeyeSensor::setPixelSize(const ossimDpt& pixelSize)
{
	thePixelSize = pixelSize;
}

void ossimBuckeyeSensor::setImageRect(const ossimDrect& rect)
{
	theImageClipRect = rect;
	theRefImgPt = rect.midPoint();
}

void ossimBuckeyeSensor::setFocalLength(double focalLength)
{
	theFocalLength = focalLength;
}

void ossimBuckeyeSensor::setPlatformPosition(const ossimGpt& gpt)
{
	theRefGndPt            = gpt;
	theEcefPlatformPosition = gpt;
	updateModel();

}