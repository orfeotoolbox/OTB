# Dashboard is opened for submissions for a 24 hour period starting at
# the specified NIGHLY_START_TIME. Time is specified in 24 hour format.
SET (NIGHTLY_START_TIME "01:00:00 CET")

SET (BUILDNAME "${BUILDNAME}${CMAKE_BUILD_TYPE}")


# Dart server to submit results (used by client)
SET (DROP_METHOD "xmlrpc")
SET (DROP_SITE "http://nemo:8081")
SET (DROP_LOCATION "OTB")
SET (COMPRESS_SUBMISSION ON)
#SET (DROP_LOCATION "/incoming")
#SET (DROP_SITE_USER "ftpuser")
#SET (DROP_SITE_PASSWORD "public")

#SET (DROP_SITE_MODE "active")

SET (TRIGGER_SITE "http://${DROP_SITE}/cgi-bin/Submit-Orfeotoolbox-TestingResults.cgi")

# Project Home Page
SET (PROJECT_URL "http://smsc.cnes.fr/PLEIADES/")

# Dart server configuration 
SET (ROLLUP_URL "http://${DROP_SITE}/cgi-bin/OrfeotoolboxRollup.cgi")

#SET (ROLLUP_URL "http://${DROP_SITE}/cgi-bin/insight-rollup-dashboard.sh")
#SET (ROLLUP_URL "http://${DROP_SITE}/cgi-bin/orfeotoolbox-rollup-dashboard.sh")
#SET (CVS_WEB_URL "http://${DROP_SITE}/cgi-bin/viewcvs.cgi/")
#SET (CVS_WEB_CVSROOT "Insight")
#SET (CVS_WEB_CVSROOT "OrfeoToolbox")

OPTION(BUILD_DOXYGEN "Build source documentation using doxygen" "Off")
SET (DOXYGEN_CONFIG "${OTB_BINARY_DIR}/doxygen.config" )
SET (USE_DOXYGEN "On")
SET (DOXYGEN_URL "http://${DROP_SITE}/OrfeoToolbox/Doxygen/html/" )

SET (USE_GNATS "On")
SET (GNATS_WEB_URL "http://nemo/http://nemo/phpBugTracker/phpbt-1.0.1/")

# Continuous email delivery variables
#SET (CONTINUOUS_FROM "lorensen@crd.ge.com")
#SET (SMTP_MAILHOST "public.kitware.com")
#SET (CONTINUOUS_MONITOR_LIST "lorensen@crd.ge.com millerjv@crd.ge.com lorensen@nycap.rr.com")
#SET (CONTINUOUS_BASE_URL "http://www.itk.org/Testing")

MARK_AS_ADVANCED(BUILD_DOXYGEN)
