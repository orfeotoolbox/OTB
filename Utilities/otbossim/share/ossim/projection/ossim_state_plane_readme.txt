// $Id: ossim_state_plane_readme.txt 2644 2011-05-26 15:20:11Z oscar.kramer $

This is a readme to document the state_plane.csv file keys.

State Plane support can be picked up via ossimPreferences by adding the 
key "state_plane_csv_file" to your preferences file.  Auto detection from 
some common place is on the to do list.
Example preference entry:

state_plane_csv_file1: /usr/local/share/ossim/projection/ossim_state_plane.csv
state_plane_csv_file2: /usr/local/share/ossim/projection/ossim_harn_state_plane.csv


The state_plane.csv (comma separated value) file is used by the 
ossimStatePlaneProjectionFactory to create projections from line entries
contained in the csv file.  Each line entry should represent some Projection
Coordinate System (PCS) for a given state plane projection.

All values in a line must be comma separated.  It does not matter to our code
whether or not the values are quoted or not.

Example key line (first line):
name,pcs_code,projection_code,param1,param2,param3,param4,false_easting,false_northing,linear_units,datum_code

Example line with Lambert Conformal Conic projection (lcc):
NAD27_Arkansas_North,26751,lcc,34 20 n,92 w,34 56 n,36 14 n,2000000,0,us_survey_feet,NAS-C

Example line with Transverse Mercator Projection (tm):
NAD27_Alabama_East,26729,tm,30 30 n,85 50 w,25000,0,500000,0,us_survey_feet,NAS-C

Key/value: description

1) name/NAD27_Alabama_East: The long name or human readable.

2) pcs_code/26729:  PCS code for this projection.
   See:  http://www.remotesensing.org/geotiff/spec/geotiff6.html#6.3.3

3) projection_code/tm: This is the underlying projection of the PCS.  This
   will either be the abbreviated tm or lcc, or the full ossim projection 
   class name of ossimTransMercatorProjection or 
   ossimLambertConformalConicProjection.

4) param1/30 30 n: The origin of latitude.  String format = "dd mm h" where
   dd = degrees, mm = minutes and h is the hemisphere either n or s.
   (see note 1)

5) param2/85 50 w,  The central meridian.  String format = "dd mm h" where
   dd = degrees, mm = minutes and h is the hemisphere either n or s.
   (see note 1)

6) param3/25000: If Lambert this will be the 1st parallel(seen note 1),
   If Transverse Mercator (this case) it will be the scale factor in the form
   of "scale_factor = 1.0 - (1.0 / param3).
   NOTE: For Transverse Mercator units = meters.

7) param4/0: If Lambert this will be the 2st parallel(seen note 1),
   If Transverse Mercator (this case) it will be 0.

8) false_easting/500000: The false easting in linear_units which should be 
   either meters or us_survey_feet.

9) false_northing/0: The false northing in linear_units which should be 
   either meters or us_survey_feet.

10) linear_units/us_survey_feet: This can be either meters or us_survey_feet.

11) datum_code/NAS-C  The ossim datum code corresponding to the PCS.  
    Run the ossim application "datums" to see a list of valid codes.

NOTES: 

1) Internally this string is fed to an ossimDms class that is pretty good at interpreting strings representing degrees minutes seconds, but if you stick with the "dd mm h" format you can't go wrong.

2) Key line must be present and have the valid keys or file will not be loaded.
   This is case sensitive so your keys should be in lower case.

