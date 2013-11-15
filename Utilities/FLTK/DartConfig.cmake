# Dashboard is opened for submissions for a 24 hour period starting at
# the specified NIGHLY_START_TIME. Time is specified in 24 hour format.
set(NIGHTLY_START_TIME "21:00:00 EDT")

# Dart server to submit results (used by client)
set(DROP_METHOD "http")
set(DROP_SITE "public.kitware.com")
set(DROP_LOCATION "/cgi-bin/HTTPUploadDartFile.cgi")
set(TRIGGER_SITE "http://${DROP_SITE}/cgi-bin/Submit-Fltk-TestingResults.pl")

# Project Home Page
set(PROJECT_URL "http://www.fltk.org")

# Dart server configuration
set(ROLLUP_URL "http://${DROP_SITE}/cgi-bin/fltk-rollup-dashboard.sh")
set(CVS_WEB_URL "http://cvs.sourceforge.net/viewcvs.py/fltk/fltk/")
set(CVS_WEB_CVSROOT "fltk")

set(USE_GNATS "On")
set(GNATS_WEB_URL "http://www.fltk.org/str.php")

# Continuous email delivery variables
set(CONTINUOUS_FROM "fltk-dashboard@public.kitware.com")
set(SMTP_MAILHOST "public.kitware.com")
set(CONTINUOUS_MONITOR_LIST "fltk-dashboard@public.kitware.com")
set(CONTINUOUS_BASE_URL "http://public.kitware.com/Fltk/Testing")

set(DELIVER_BROKEN_BUILD_EMAIL_WITH_TEST_FAILURES ON)
set(DELIVER_BROKEN_BUILD_EMAIL "Continuous Nightly")
set(EMAIL_FROM "fltk-dashboard@public.kitware.com")
set(DARTBOARD_BASE_URL "http://public.kitware.com/Fltk/Testing")

set(DELIVER_BROKEN_BUILD_EMAIL_WITH_CONFIGURE_FAILURES 1)
set(DELIVER_BROKEN_BUILD_EMAIL_WITH_BUILD_ERRORS 1)
set(DELIVER_BROKEN_BUILD_EMAIL_WITH_BUILD_WARNINGS 1)
set(DELIVER_BROKEN_BUILD_EMAIL_WITH_TEST_NOT_RUNS 1)
set(DELIVER_BROKEN_BUILD_EMAIL_WITH_TEST_FAILURES 1)

