//----------------------------------------------------------------------------
//
// File ossim-batch-test.cpp
// 
// License:  See top level LICENSE.txt file.
//
// Author:  David Burken, Oscar Kramer
//
// Description: Test code application ossim batch test.
//
//----------------------------------------------------------------------------
// $Id: ossim-batch-test.cpp 3112 2012-01-26 17:28:00Z david.burken $

#include <ossim/util/ossimBatchTest.h>
#include <ossim/base/ossimArgumentParser.h>
#include <ossim/base/ossimApplicationUsage.h>
#include <ossim/base/ossimEnvironmentUtility.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimNotify.h>
#include <ossim/base/ossimString.h>
#include <ossim/base/ossimTimer.h>
#include <ossim/init/ossimInit.h>

#include <cstdlib> /* for system() */
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

//**************************************************************************************************
// Constructor
//**************************************************************************************************
ossimBatchTest::ossimBatchTest()
   :
   m_allIsDisabled(false),
   m_templateModeActive(false)
{

}

//**************************************************************************************************
// Initialize parses the command line. Returns true if status OK.
//**************************************************************************************************
bool ossimBatchTest::initialize(ossimArgumentParser& ap)
{
   if ( (ap.argc() == 1) || ap.read("-h") || ap.read("--help") )
   {
      usage(ap);

      // continue_after_init to false
      return false;
   }

   // Initialize environment:
#if defined(WIN32) || defined(_MSC_VER) && !defined(__CYGWIN__) && !defined(__MWERKS__)
   _putenv("DEL_CMD=del /Q"); // For backwards compatiblity.
   _putenv("DIFF_CMD=fc /W");
   _putenv("COPY_CMD=copy /Y");
   _putenv("MKDIR_CMD=mkdir");
   _putenv("RM_CMD=del /Q");
   _putenv("RMDIR_CMD=rmdir /S /Q");
   
#else
   setenv("DEL_CMD",  "rm -rf",    1); // For backwards compatiblity.
   setenv("DIFF_CMD",  "diff -w",  1);
   setenv("COPY_CMD",  "cp",       1);
   setenv("MKDIR_CMD", "mkdir -p", 1);
   setenv("RM_CMD",    "rm -f",    1);
   setenv("RMDIR_CMD", "rm -rf",   1);
   
#endif
   
   std::string tempString;
   ossimArgumentParser::ossimParameter stringParam(tempString);
   
   if (ap.read("-W", stringParam) || ap.read("-W"))
   {
      ossimFilename templateFile (tempString.c_str());
      if (templateFile.empty())
         templateFile = "obt_config_long_template.kwl";
      m_templateModeActive = true;
      writeTemplate(templateFile, true);
      return true;
   }
   if (ap.read("-w", stringParam) || ap.read("-w"))
   {
      ossimFilename templateFile(tempString.c_str());
      if (templateFile.empty())
         templateFile = "obt_config_short_template.kwl";
      m_templateModeActive = true;
      writeTemplate(templateFile, false);
      return true;
   }
   
   while ( ap.read("-a", stringParam) || ap.read("--accept-test", stringParam) )
   {
      if ( tempString.size() )
      {
         m_allIsDisabled = true;
         ossimString os = tempString;
         ossimString separatorList = " ";
         std::vector<ossimString> result;
         os.split(result, separatorList);
         std::vector<ossimString>::const_iterator i = result.begin();
         while ( i != result.end() )
         {
            m_acceptTestList.push_back((*i));
            ++i;
         }
      }
   }

   while ( ap.read("-c", stringParam) || ap.read("--clean-test", stringParam) )
   {
      if ( tempString.size() )
      {
         m_allIsDisabled = true;
         ossimString os = tempString;
         ossimString separatorList = " ";
         std::vector<ossimString> result;
         os.split(result, separatorList);
         std::vector<ossimString>::const_iterator i = result.begin();
         while ( i != result.end() )
         {
            m_cleanTestList.push_back((*i));
            ++i;
         }
      }
   }

   while ( ap.read("-p", stringParam) || ap.read("--preprocess-test", stringParam) )
   {
      if ( tempString.size() )
      {
         m_allIsDisabled = true;
         ossimString os = tempString;
         ossimString separatorList = " ";
         std::vector<ossimString> result;
         os.split(result, separatorList);
         std::vector<ossimString>::const_iterator i = result.begin();
         while ( i != result.end() )
         {
            m_preprocessTestList.push_back((*i));
            ++i;
         }
      }
   }

   while ( ap.read("-r", stringParam) || ap.read("--run-test", stringParam) )
   {
      if ( tempString.size() )
      {
         m_allIsDisabled = true;
         ossimString os = tempString;
         ossimString separatorList = " ";
         std::vector<ossimString> result;
         os.split(result, separatorList);
         std::vector<ossimString>::const_iterator i = result.begin();
         while ( i != result.end() )
         {
            m_runTestList.push_back((*i));
            ++i;
         }
      }
   }

   // End of arg parsing.
   ap.reportRemainingOptionsAsUnrecognized();
   if ( ap.errors() )
   {
      ap.writeErrorMessages(ossimNotify(ossimNotifyLevel_NOTICE));
      return false;
   }

   if (ap.argc() > 1 )
   {
      m_configFileName = ap.argv()[1];
   }
   else
   {
      usage(ap);

      // continue_after_init to false
      return false;
   }

   // Special command line case: When only a config filename is provided, this implies "run all 
   // tests".
   if (m_cleanTestList.empty()  && m_preprocessTestList.empty() &&
       m_acceptTestList.empty() && m_runTestList.empty() && m_configFileName.size())
   {
      m_runTestList.push_back("all");
   }

   return true;
}

//**************************************************************************************************
// Writes a template (either long form or short) to the file name specified.
//**************************************************************************************************
void ossimBatchTest::writeTemplate(const ossimFilename& templateFile, bool write_long_form)
{
   if (templateFile.exists())
   {
      ossimString del_cmd ("$(RM_CMD) ");
      del_cmd += templateFile;
      del_cmd = del_cmd.expandEnvironmentVariable();
      if (system(del_cmd.chars()) != 0)
      {
         cerr << "ERROR: Could not delete existing template file before writing new template."
            " Please delete the existing and rerun the command."<< endl;
         return;
      }
   }

   std::ofstream out(templateFile.c_str());
   if ( !out.good() )
   {
      ossimNotify(ossimNotifyLevel_NOTICE)
         << "Could not open: " << templateFile.c_str() << std::endl;
      return;
   }

   if (write_long_form)
   {
      out<< "//===================================================================================\n"
         << "// \n"
         << "// File: " << templateFile.c_str() << "\n"
         << "// Generated by command: ossim-batch-test -W " << templateFile.c_str() << "\n"
         << "// Description: ossim-batch-test template config file.\n"
         << "// \n"
         << "// Control flags:\n"
         << "// Use 1 or true, 0 or false to turn on and off sections.\n"
         << "// Typically preprocess and expected results only turned on for first run.\n"
         << "// Clean commands should erase anything created by this test.\n"
         << "// Command order if flag turned on:\n"
         << "// 1) clean, 2) pre-process, 3) expected, 4) test, 5) post-process\n"
         << "// \n"
         << "// NOTES:\n"
         << "// * The following environment variables must be set before running batch test:\n"
         << "//     OSSIM_BATCH_TEST_DATA     Top-level dir containing all test source data\n"
         << "//     OSSIM_BATCH_TEST_RESULTS  Top-level dir containing all test results (exp, \n"
         << "//                               out and log),\n"
         << "// \n"
         << "// * The variables OBT_EXP_DIR and OBT_OUT_DIR are assigned during run-time to\n"
         << "//   the proper paths according to the config filename. It isn't required to replace\n"
         << "//   these nor predefine them in the environment. You can leave them as is or modify\n"
         << "//   the paths to your unconventional file paths.\n"
         << "// \n"
         << "// * In order to preserve platform independence, please utilize the following \n"
         << "//   variables for the common OS commands when adding new commands to the test\n"
         << "//   configuration KWL file. Examples are provided in this template\n"
         << "//       $(DIFF_CMD) Use this var for windows \"fc\" or linux \"diff\"\n"
         << "//       $(COPY_CMD) Use this var for windows \"copy\" or linux \"cp\"\n"
         << "//       $(MKDIR_CMD) Use this var for windows \"mkdir\" or linux \"mkdir\"\n"
         << "//       $(RM_CMD) Use this var for windows \"del /Q\" or linux \"rm -f\"\n"
         << "//       $(RMDIR_CMD) Use this var for windows \"rmdir /S /Q\" or linux \"rm -rf\"\n"
         << "//   This application will set the environment variables to the proper values at \n"
         << "//   runtime, so no need to worry about defining these.\n"
         << "// \n"
         << "// * Use forward slashes \"//\", at beginning of line for comments.\n"
         << "// \n"
         << "// * You can use existing environment variables as $(YOUR_VARIABLE). They will be \n"
         << "//   expanded at run time if valid.\n"
         << "// \n"
         << "// * Clean, preprocess, run, and accept test sections can be switched on or off\n"
         << "//   at run time.  These override config file flags.  Optional arguments are the \n"
         << "//   the following: \n"
         << "//      -c or --clean-test \n"
         << "//      -p or --preprocess-test\n"
         << "//      -r or --run-test \n"
         << "//      -a or --accept-test \n"
         << "//   Multiple tests can be entered by quoting string of space-separated\n"
         << "//   test names, e.g. \"test1 test2\". To do all tests use \"all\" or leave blank.\n"
         << "// \n"
         << "//===================================================================================\n"
         << "\n"
         << "// If the config file will contain only a list of subordinate config files to be\n"
         << "// run as one consolidated \"super-test\", then use only the following suffixed\n"
         << "// keywords. Otherwise, REMOVE THESE:\n"
         << "test_config_file1: <my_first_test_config.kwl>\n"
         << "test_config_file2: <my_second_test_config.kwl>\n"
         << "// ... etc.\n"
         << "\n"
         << "// The remaining keywords are used exclusive of \"test_config_fileX\" above. The\n"
         << "// config files must be either a list of subordinate configs, or a concrete, \n"
         << "// low-level config file.\n"
         << "\n"
         << "// Where you want the top-level (inter-test) log files to go:\n"
         << "log_directory: $(OBT_OUT_DIR)\\..\\log\n"
         << "\n"
         << "//===================================================================================\n"
         << "// Begin Test 1\n"
         << "// NOTE: If the config file contains just a single test, then the use of the \"test1\"\n"
         << "// prefix is optional. Make sure the results directory spec reflects the proper path.\n"
         << "\n"
         << "test1.name: <YOUR_TEST_NAME>\n"
         << "test1.description: Test height for the center of the image for test2 and test3.\n"
         << "\n"
         << "// Controls/turns on/off whole test (all sections):\n"
         << "test1.enabled: 1\n"
         << "\n"
         << "// Individual control flags:\n"
         << "test1.run_clean_commands: 0\n"
         << "test1.run_preprocessing_commands: 0\n"
         << "test1.run_expected_results_commands: 0\n"
         << "test1.run_test_commands: 1\n"
         << "test1.run_postprocessing_commands: 1\n"
         << "\n"
         << "// Temp file to catch diff output.\n"
         << "test1.temp_file: $(TEMP)\\tmp.txt\n"
         << "\n"
         << "// Clean up commands\n"
         << "test1.clean_command1: $(RMDIR_CMD) $(OBT_OUT_DIR)\n"
         << "\n"
         << "// Pre-process commands\n"
         << "test1.preprocess_command1: $(MKDIR_CMD) $(OBT_OUT_DIR)\n"
         << "test1.preprocess_command1: $(MKDIR_CMD) $(OBT_OUT_DIR)\\..\\log\n"
         << "\n"
         << "//---------------------------------------\n"
         << "// Commands to generate expected results\n"
         << "//---------------------------------------\n"
         << "// Since test2 and test3 are dependent on elevation test the center of the image.\n"
         << "test1.expected_results_command0: ossim-info --height -42.8508 147.2537 > $(OBT_EXP_DIR)\\height.txt\n"
         << "test1.expected_results_command1: $(COPY_CMD) $(OBT_OUT_DIR)\\height.txt $(OBT_EXP_DIR)\\height.txt\n"
         << "\n"
         << "//---------------------------------------\n"
         << "// The actual commands to test\n"
         << "//---------------------------------------\n"
         << "test1.test_command0: ossim-info --height -42.8508 147.2537 > $(OBT_OUT_DIR)\\height.txt\n"
         << "\n"
         << "//-------------------------------------------\n"
         << "// Post process commands for diffs, etc.\n"
         << "//-------------------------------------------\n"
         << "test1.postprocess_command0: $(DIFF_CMD) $(OBT_EXP_DIR)\\height.txt $(OBT_OUT_DIR)\\height.txt\n"
         << " \n"
         << "\n"
         << "// End <TEST_NAME>\n"
         << "//===================================================================================\n"
         << "// Begin <TEST2_NAME>...\n"
         << "\n"
         << "// You can specify additional tests by copying the test1 keywords and changing the\n"
         << "// prefix indices to be unique. It is not necessary to be consecutive.\n"
         << "\n"
         << std::endl;
   }
   else 
   {
      out<< "//===================================================================================\n"
         << "// \n"
         << "// File: " << templateFile.c_str() << "\n"
         << "// Generated by command: ossim-batch-test -w " << templateFile.c_str() << "\n"
         << "// Description: ossim-batch-test template config file.\n"
         << "// \n"
         << "// NOTES:\n"
         << "// * The following environment variables must be set before running batch test:\n"
         << "//     OSSIM_BATCH_TEST_DATA     Top-level dir containing all test source data\n"
         << "//     OSSIM_BATCH_TEST_RESULTS  Top-level dir containing all test results (exp, \n"
         << "//                               out and log),\n"
         << "// \n"
         << "// * You can use existing environment variables as $(YOUR_VARIABLE). They will be \n"
         << "//   expanded at run time if valid.\n"
         << "// \n"
         << "// * If the config file contains more than a single test, then it will be necessary \n" 
         << "//   to prefix all test-specific keywords with \"testN.\"\n"
         << "// \n"
         << "// * The variables OBT_EXP_DIR and OBT_OUT_DIR are assigned during runtime to\n"
         << "//   the proper paths according to the config filename. It isn't required to replace\n"
         << "//   these nor predefine them in the environment. You can leave them as is or modify\n"
         << "//   the paths to your unconventional file paths.\n"
         << "// \n"
         << "//===================================================================================\n"
         << "\n"
         << "description: Test height for the center of the image for test2 and test3.\n"
         << "\n"
         << "//---------------------------------------\n"
         << "// Commands to generate expected results.\n"
         << "//---------------------------------------\n"
         << "expected_results_command0: ossim-info --height -42.8508 147.2537 > $(OBT_OUT_DIR)\\height.txt\n"
         << "expected_results_command1: $(COPY_CMD) $(OBT_OUT_DIR)\\height.txt $(OBT_EXP_DIR)\\height.txt\n"
         << "\n"
         << "//---------------------------------------\n"
         << "// The actual commands to test.\n"
         << "//---------------------------------------\n"
         << "test_command0: ossim-info --height -42.8508 147.2537 > $(OBT_OUT_DIR)\\height.txt\n"
         << "\n"
         << "//---------------------------------------\n"
         << "// The post-processing (comparison) commands. Typically these involve a file \n"
         << "// comparison of last run command against expected results.\n"
         << "//---------------------------------------\n"
         << "postprocess_command0: $(DIFF_CMD) $(OBT_EXP_DIR)\\height.txt $(OBT_OUT_DIR)\\height.txt\n"
         << std::endl;
   }

   out.close();
   ossimNotify(ossimNotifyLevel_NOTICE) << "Wrote file: " << templateFile.c_str() << std::endl;

}

//**************************************************************************************************
// Processes a test config file
//**************************************************************************************************
ossim_uint8 ossimBatchTest::execute()
{
   if (m_templateModeActive)
      return (ossim_uint8) TEST_PASSED;

   // Establish the top-level test directory that will contain log, exp and out subdirs:
   ossimFilename base_output_dir = ossimEnvironmentUtility::instance()->getEnvironmentVariable(
         ossimString("OSSIM_BATCH_TEST_RESULTS") );
   if (base_output_dir.empty())
   {
      cout<<"\nossimBatchTest WARNING: The environment variable OSSIM_BATCH_TEST_RESULTS is not "
         "defined. Results will be written relative to the current working directory."<<endl;
   }
   base_output_dir = base_output_dir.expand().dirCat(m_configFileName.fileNoExtension());
   
   // The following env vars permits the user to specify the test directory as a variable in the KWL
   // config file:
   m_outDir = base_output_dir.dirCat("out");
   m_expDir = base_output_dir.dirCat("exp");
#if defined(WIN32) || defined(_MSC_VER) && !defined(__CYGWIN__) && !defined(__MWERKS__)
   ossimString env_spec = ossimString("OBT_OUT_DIR=") + m_outDir;
   _putenv(env_spec.chars());
   env_spec = ossimString("OBT_EXP_DIR=") + m_expDir;
   _putenv(env_spec.chars());
#else
   setenv("OBT_OUT_DIR", m_outDir.chars(), 1);
   setenv("OBT_EXP_DIR", m_expDir.chars(), 1);
#endif

   // Turn expansion of for like: $(OBT_TEST_RESULTS)
   ossimKeywordlist kwl;
   kwl.setExpandEnvVarsFlag(true);
   if (!kwl.addFile(m_configFileName))
      return (ossim_uint8) TEST_ERROR;

   // The KWL may contain names of other test config files. Is this a list of config files? If the
   // status returns anything other than TBD, then a list was present and processed:
   ossim_uint8 status = processConfigList(kwl);
   if (status != TEST_TBD)
      return status;

   if ( m_allIsDisabled )
      disableAllKwl(kwl);

   // Pick up individual test options passed in by user.  These will adjust the keyword list
   // flags loaded in memory.
   if ( m_cleanTestList.size() ) // Do this first always...
   {
      std::string testCommand = "run_clean_commands";
      preprocessKwl(m_cleanTestList, testCommand, kwl);
   }

   if ( m_preprocessTestList.size() )
   {
      std::string testCommand = "run_preprocessing_commands";
      preprocessKwl(m_preprocessTestList, testCommand, kwl);
   }

   if ( m_acceptTestList.size() )
   {
      std::string testCommand = "run_expected_results_commands";
      preprocessKwl(m_acceptTestList, testCommand, kwl);
   }

   if ( m_runTestList.size() ) // Do this last always...
   {
      std::string testCommand = "run_test_commands";
      preprocessKwl(m_runTestList, testCommand, kwl);
   }

   ossimFilename logDir = base_output_dir.dirCat("log");
   const char* lookup = kwl.find("log_directory");
   if ( lookup )
   {
      logDir = convertToNative( lookup ).c_str();
   }

   if ( logDir.exists() == false ) 
   {
      if ( logDir.createDirectory() == false )
      {
         cerr << "Could not create: " << logDir << endl;
         return (ossim_uint8) TEST_ERROR;
      }
   }

   // Establish path to and active stream for the log file:
   ossimFilename logFile;
   getLogFilename(logFile);
   logFile = logDir.dirCat(logFile);
   ossimSetLogFilename(logFile);
   m_logStr.open(logFile.c_str());
   if ( m_logStr.fail() )
   {
      cerr << "Could not open: " << logFile << endl;
      return (ossim_uint8) TEST_ERROR;
   }
   else
   {
      cout << "Logging to file: " << logFile << "\n";
   }

   ossimString date;
   getDateString(date);
   m_logStr << "// ---\n"
      << "// ossim-batch-test log:\n"
      << "// date format = yyyymmddhhmmss\n"
      << "//---\n"
      << "start_time: " << date << "\n"
      << "config_file: " << m_configFileName<< "\n";
   // Start the timer.
   ossimTimer::instance()->setStartTick();

   double startTime = ossimTimer::instance()->time_s();

   // Get the number of test:
   ossimString regExpStr = "test[0-9]+\\.";
   ossim_uint32 num_tests = kwl.getNumberOfSubstringKeys(regExpStr);
   const ossim_uint32 MAX_INDEX = num_tests + 1000;
   ossimString prefixBase = "test";
   ossim_uint32 index = 0;
   ossim_uint32 processedIndexes = 0;
   ossimString prefix;

   // If no test prefix is used, this implies a single test:
   bool is_single_test = (num_tests == 0);
   if (is_single_test)
   {
      num_tests = 1;
      prefix = "";
   }
   
   ossim_uint8 overall_test_status = (ossim_uint8) TEST_TBD;
   while ( processedIndexes < num_tests )
   {
      if (!is_single_test)
         prefix = prefixBase + ossimString::toString(index) + ".";

      ossim_uint8 individual_test_status = processTest( prefix, kwl);
      if ( individual_test_status != (ossim_uint8) TEST_TBD)
         ++processedIndexes;

      overall_test_status |= individual_test_status;
      ++index;
      if ( index >= MAX_INDEX ) 
         break; 
   }

   getDateString(date);
   m_logStr << "\nstop_time: " << date << "\n";
   double stopTime = ossimTimer::instance()->time_s();
   m_logStr << "total elapsed time in seconds: "
      << std::setiosflags(ios::fixed) << std::setprecision(4)
      << (stopTime-startTime)
      << endl; // flush
   m_logStr.close();

   cout << "Wrote: " << logFile << endl;
   return overall_test_status;
}

//**************************************************************************************************
// Special handler for KWL containing list of test config files.
//**************************************************************************************************
ossim_uint8 ossimBatchTest::processConfigList(const ossimKeywordlist& kwl)
{
   ossim_uint8 overall_test_status = (ossim_uint8) TEST_TBD;
   ossimFilename config_list_path = m_configFileName.path();

   // Look for keywords for config file name:
   ossimString prefixBase = "test_config_file";
   ossimString regExpStr = prefixBase + "[0-9]+";
   const ossim_uint32 count = kwl.getNumberOfSubstringKeys(regExpStr);
   if (count != 0)
   {
      // This is indeed a list of config files:
      ossim_uint32 index = 0;
      ossim_uint32 processedIndexes = 0;
      while (processedIndexes < count)
      {
         // Looping over each config file listed, performing an execute() on each:
         ossimString kw = prefixBase + ossimString::toString(index);
         m_configFileName = ossimFilename(kwl.find(kw.chars()));
         
         if (!m_configFileName.empty())
         {
            // Expand any environment variable:
            if (m_configFileName.contains("$("))
               m_configFileName = m_configFileName.expand();

            // Handle paths relative to the master config list file:
            if (m_configFileName.path().empty())
               m_configFileName = m_configFileName.setPath(config_list_path);
         }

         // Execute this config file:
         if (m_configFileName.isReadable())
         {
            cout << "\n\nProcessing config file: " << m_configFileName.c_str() << "\n";
            overall_test_status |= execute();
            ++processedIndexes;
         }

         ++index;
      }
   }

   return overall_test_status;
}

//**************************************************************************************************
//! Fetches string from OS for naming and tagging the log file.
//**************************************************************************************************
void ossimBatchTest::getDateString(ossimString& date)
{
   time_t t;
   time(&t);
   tm* lt;
   lt = localtime(&t);
   std::string frmt = "%Y%m%d%H%M%S";
   // yyyymmddhhmmss
   char s[15];
   size_t count = strftime(s, 15, frmt.c_str(), lt);
   if ( count && (count < 15) )
      date = s;
   else
      date.clear();

}

//**************************************************************************************************
//! Establishes name of output log file.
//**************************************************************************************************
void ossimBatchTest::getLogFilename(ossimFilename& logFile)
{
   logFile = "obt-log-";
   ossimString date;
   getDateString(date);
   logFile += date;
   logFile += ".txt";
}

//**************************************************************************************************
//! Within a single config file can be multiple tests, distinguished by the "test*." prefix. This
//! method manages the execution of a single test.
//**************************************************************************************************
ossim_uint8 ossimBatchTest::processTest(const ossimString& prefix, const ossimKeywordlist& kwl)
{
   ossim_uint8 testStatus = TEST_TBD;

   // Determine first if this test prefix is represented in the KWL:
   if (kwl.getNumberOfSubstringKeys(prefix) == 0)
      return testStatus;

   ossimString testName;
   const char* lookup = kwl.find( prefix, "name" );
   if ( lookup )
   {
      testName = lookup;
   }
   if (testName.empty())
   {
      if (prefix.empty())
         testName = m_configFileName.fileNoExtension();
      else
         testName = prefix.trim(".");
   }

   m_logStr << "\n----------------------------------------------------------------------\n";

   // See if test is disabled/enabled:
   bool enabled = true;
   lookup = kwl.find( prefix, "enabled" );
   if ( lookup )
      enabled = ossimString(lookup).toBool();

   if ( !enabled )
   {
      testStatus = TEST_DISABLED;
      ossimString statusString;
      if ( prefix.size() )
      {
         statusString = prefix.trim(ossimString(".")) + ossimString(": disabled");
      }
      else
      {
         statusString = "test: disabled";
      }
      cout << "test name: " << testName << "\n" << statusString << endl;
      m_logStr << "test name: " << testName << "\n" << statusString << endl;
      return testStatus;
   }

   cout << "\n\nProcessing Test: " << testName << "\n";
   m_logStr << "\n\nProcessing Test: " << testName << "\n";   
   lookup = kwl.find( prefix, "description" );
   if ( lookup )
      m_logStr << "description: " << lookup << "\n";

   bool preProcessFlag  = false;
   bool expectedFlag    = false;
   bool testFlag        = false;
   bool postProcessFlag = false;
   bool cleanFlag       = false;
   std::string date; 

   lookup = kwl.find(prefix.c_str(), "run_clean_commands");
   if ( lookup )
      cleanFlag = ossimString(lookup).toBool();

   lookup = kwl.find(prefix.c_str(), "run_preprocessing_commands");
   if ( lookup )
      preProcessFlag = ossimString(lookup).toBool();

   lookup = kwl.find(prefix.c_str(), "run_expected_results_commands");
   if ( lookup )
      expectedFlag = ossimString(lookup).toBool();

   lookup = kwl.find(prefix.c_str(), "run_test_commands");
   if ( lookup )
      testFlag = ossimString(lookup).toBool();

   lookup = kwl.find(prefix.c_str(), "run_postprocessing_commands");
   if ( lookup )
      postProcessFlag = ossimString(lookup).toBool();

   m_logStr << "preProcessFlag:  " << preProcessFlag
      << "\nexpectedFlag:    " <<expectedFlag
      << "\ntestFlag:        " <<testFlag
      << "\npostProcessFlag: " <<postProcessFlag
      << "\ncleanFlag:       " <<cleanFlag
      << "\n";

   // Run the clean first if set...
   if ( cleanFlag )
   {
      ossimString prefixBase = prefix + "clean_command";
      testStatus |= processCommands( prefixBase, kwl, testName, false );
   }

   if ( preProcessFlag && !(testStatus & TEST_ERROR))
   {
      ossimString prefixBase = prefix + "preprocess_command";
      testStatus |= processCommands( prefixBase, kwl, testName, false );
   }
   if ( expectedFlag  && !(testStatus & TEST_ERROR))
   {
      ossimString prefixBase = prefix + "expected_results_command";
      testStatus |= processCommands( prefixBase, kwl, testName, true );
   }
   if ( testFlag  && !(testStatus & TEST_ERROR))
   {
      ossimString prefixBase = prefix + "test_command";
      testStatus |= processCommands( prefixBase, kwl, testName, true );
   }
   if ( postProcessFlag  && !(testStatus & TEST_ERROR))
   {
      ossimFilename tempFile;

      if ( getTempFileName( prefix, kwl, tempFile ) )
      {
         ossimString prefixBase = prefix + "postprocess_command";
         testStatus |= processCommands( prefixBase, kwl, testName, false, tempFile );
      }
      else
      {
         m_logStr << testName << ": ERROR temp file could not be derived..." << endl;
         testStatus |= TEST_ERROR; // Set an error bit...
      }
   }

   m_logStr << "end_test: " << testName << "\n";

   m_logStr << "----------------------------------------------------------------------\n"; 
   return testStatus; 
}

//**************************************************************************************************
ossim_uint8 ossimBatchTest::processCommands(const ossimString& prefixBase,
                                            const ossimKeywordlist& kwl,
                                            const ossimString& testName,
                                            bool logTime,
                                            const ossimFilename& tempFileName)
{
   ossim_uint8 result = TEST_TBD;

   //---
   // We must do these in order so we will use the
   // ossimKeywordlist::getNumberOfSubstringKeys
   // instead of: ossimKeywordlist::getSubstringKeyList
   //
   // MAX_INDEX is just so people can skip numbers in their config file like:
   // test1.command
   // test3.command oops...
   //---
   ossimString regExpStr = prefixBase + "[0-9]+";
   const ossim_uint32 num_commands = kwl.getNumberOfSubstringKeys(regExpStr);
   const ossim_uint32 MAX_INDEX = num_commands + 1000;

   // Hack to permit defaulted results paths and default clean operation:
   if (num_commands == 0)
   {
      bool rtn_ok = true;
      if (prefixBase.contains("preprocess"))
         rtn_ok = makeDefaultResultsDir();
      if (prefixBase.contains("clean"))
         rtn_ok = doDefaultClean();
      if (rtn_ok)
         result = TEST_PASSED;
      else
         result = TEST_ERROR;
      return result;
   }

   ossimString date;
   double startTime;
   double stopTime;
   
   ossimString command;
   ossim_uint32 index = 0;
   ossim_uint32 processedIndexes = 0;
   const char* lookup = 0;
   
   bool postprocessing = false;
   if (prefixBase.contains("postprocess"))
      postprocessing = true;

   while (  processedIndexes < num_commands )
   {
      ossimString cmd_kw =  prefixBase + ossimString::toString(index);
      lookup = kwl.find( cmd_kw );
      if ( lookup )
      {
         ossimString command_line = convertToNative( lookup ).c_str();
         if ( tempFileName.size() )
         {
            command_line += " > " + tempFileName;
         }

         m_logStr << "executing command: " << command_line << "\n";

         if ( logTime )
         {
            getDateString(date);
            m_logStr << "begin: " << date << "\n";

            // Start the clock:
            startTime = ossimTimer::instance()->time_s();
         }
            
         // Launch the command:
         int status = system(command_line.chars());
         if (status == 0)
            result |= TEST_PASSED;
         else if (postprocessing)
            result |= TEST_FAILED;
         else
            result |= TEST_ERROR;

         if ( logTime )
         {
            // Log the time and status:
            stopTime = ossimTimer::instance()->time_s();
            getDateString(date);
            m_logStr << "end: " << date << "\n"
                   << testName << "[" << index << "]: elapsed time in seconds: "
                   << std::setiosflags(ios::fixed)
                   << std::setprecision(4)
                   << (stopTime-startTime) << "\n";
         }
         
         m_logStr << "return status: " << status << "\n";
         
         // Output the status.
         // If failed write the temp file to the log.  This should have the diffs in it.
         ostringstream statusString;
         if ( prefixBase.size() )
         {
            statusString << prefixBase;
         }
         else
         {
            statusString << "test";
         }
         statusString << "[" << index << "]: ";

         if ( status != 0 )
         {
            // A bad return status can be a test fail if the command was a postprocess:
            if ( postprocessing )
            {
               statusString << "FAILED";
            }
            else
            {
               statusString << "ERROR";
            }

            cout << statusString.str() << endl;
            m_logStr << statusString.str() << endl;
            m_logStr << command << "\noutput follows:\n";
            std::ifstream in;
            if (!tempFileName.empty())
            {
               in.open(tempFileName.c_str(), ios::in | ios::binary);
               if ( in.is_open() )
               {
                  char ch;
                  while ( in.get(ch) ) m_logStr.put(ch);
                  m_logStr << "\n";
                  in.close();
               }
            }
         }
         else
         {
            statusString << "PASSED";
            cout << statusString.str() << endl;
            m_logStr << statusString.str() << endl;
         }

         ++processedIndexes;
      }
      ++index;
      
      if ( index >= MAX_INDEX ) 
         break; // Config file has bad numbering...
   }
   return result;
}

//**************************************************************************************************
void ossimBatchTest::preprocessKwl(const std::vector<std::string>& testList,
                                   const std::string& testCommand,
                                   ossimKeywordlist& kwl)
{
   if ( testList.empty() || testCommand.empty() || (kwl.getSize()==0) )
      return;

   // Check for user passed in "all" to option.
   std::vector<std::string>::const_iterator testIter = testList.begin();
   bool enableAllTestFlag = false;
   ossimString firstTest(*testIter);
   firstTest.downcase();
   if ( firstTest == "all" )
      enableAllTestFlag = true;

   while ( testIter != testList.end() )
   {
      // Get the number of test:
      ossimString regExpStr = "test[0-9]+\\.";
      ossim_uint32 num_tests = kwl.getNumberOfSubstringKeys(regExpStr);
      const ossim_uint32 MAX_INDEX = num_tests + 1000;
      ossimString prefixBase = "test";
      ossim_uint32 index = 0;
      ossim_uint32 processedIndexes = 0;

      // A count of 0 may indicate that the config file consists of a single, unprefixed test spec:
      bool is_single_test = false;
      if (num_tests == 0)
      {
         regExpStr = "test_command[0-9]+";
         const ossim_uint32 cmd_count = kwl.getNumberOfSubstringKeys(regExpStr);
         if (cmd_count != 0)
         {
            ++num_tests;
            is_single_test = true;
         }
         else
            break; // Nothing to do here:
      }

      while ( processedIndexes < num_tests )
      {
         ossimString prefix ("");
         bool test_name_matches = true;
         bool test_exists = true;
         
         if (!is_single_test)
         {
            // The tests are prefixed with "test*". Assign the prefix for this test set:
            prefix = prefixBase + ossimString::toString(index);
            test_name_matches = (prefix == (*testIter).c_str());
            prefix += ".";
            
            // With a valid prefix, check if this test even exists before doing any more 
            // preprocessing:
            regExpStr = prefix + "test_command[0-9]+";
            const ossim_uint32 cmd_count = kwl.getNumberOfSubstringKeys(regExpStr);
            test_exists = cmd_count > 0;
         }

         if ( test_exists )
         {
            if ( test_name_matches || enableAllTestFlag )
            {
               kwl.add(prefix.c_str(), "enabled", "1", true);
               kwl.add(prefix.c_str(), testCommand.c_str(), "1", true);
               if ( testCommand == "run_expected_results_commands" )
               {
                  // Need pre-processing if clean performed.
                  kwl.add(prefix.c_str(), "run_preprocessing_commands", "1", true);
               }
               if ( testCommand == "run_test_commands" )
               {
                  // Need pre/post-processing:
                  kwl.add(prefix.c_str(), "run_preprocessing_commands", "1", true);
                  kwl.add(prefix.c_str(), "run_postprocessing_commands", "1", true);
               }
            }
            ++processedIndexes;
         }
         ++index;
         if ( index >= MAX_INDEX ) break; 
      }
      ++testIter;

   } // End: while ( testIter != testList.end() )
   
} // End: preprocessKwl method

//**************************************************************************************************
void ossimBatchTest::disableAllKwl(ossimKeywordlist& kwl)
{
   if ( kwl.getSize() )
   {
      // Get the number of test:
      ossimString regExpStr = "test[0-9]+\\.";
      const ossim_uint32 COUNT = kwl.getNumberOfSubstringKeys(regExpStr);
      const ossim_uint32 MAX_INDEX = COUNT + 1000;
      ossimString prefixBase = "test";
      ossim_uint32 index = 0;
      ossim_uint32 processedIndexes = 0;
      
      while ( processedIndexes < COUNT )
      {
         ossimString prefix =  prefixBase + ossimString::toString(index) + ".";
         const char* lookup = kwl.find( prefix, "name" );
         if (lookup)
         {
            kwl.add(prefix.c_str(), "enabled", "0", true);
            kwl.add(prefix.c_str(), "run_clean_commands", "0", true);
            kwl.add(prefix.c_str(), "run_expected_results_commands", "0", true);
            kwl.add(prefix.c_str(), "run_test_commands", "0", true);
            kwl.add(prefix.c_str(), "run_postprocessing_commands", "0", true);
            ++processedIndexes;
         }
         ++index;
         if ( index >= MAX_INDEX ) break; 
      }
   }
   
} // End: disableAllKwl(ossimKeywordlist& kwl)


//**************************************************************************************************
//! Default preprocessing step makes expected and output results directories. Returns TRUE if
//! successful.
//**************************************************************************************************
bool ossimBatchTest::makeDefaultResultsDir()
{
   if ( !m_outDir.exists() && !m_outDir.createDirectory() )
   {
      cerr << "Could not create: " << m_outDir << endl;
      return false;
   }
   if ( !m_expDir.exists() &&  !m_expDir.createDirectory() )
   {
      cerr << "Could not create: " << m_expDir << endl;
      return false;
   }
   return true;
}

//**************************************************************************************************
//! Default clean step deletes all files in out and exp dirs. Returns TRUE if successful.
//**************************************************************************************************
bool ossimBatchTest::doDefaultClean()
{
   bool result = true;

   ossimString del_cmd = ossimEnvironmentUtility::instance()->getEnvironmentVariable(
      ossimString("RMDIR_CMD") );
   ossimString command_line;
   
   if ( m_outDir.exists() )
   {
      command_line = del_cmd + " " + m_outDir;
      m_logStr << "executing command: " << command_line << "\n";
      if (system(command_line) != 0)
      {
         m_logStr << "ERROR: execution failed!\n";
         cerr << "ERROR: Could not delete <"<<m_outDir<<">. Clean operation failed."<< endl;
         result = false;
      }
   }
   
   ossimFilename tmpDir;
   if ( getDefaultTempFileDir( tmpDir ) )
   {
      if ( tmpDir.exists() )
      {
         command_line = del_cmd + " " + tmpDir;
         m_logStr << "executing command: " << command_line << "\n";
         if (system(command_line) != 0)
         {
            m_logStr << "ERROR: execution failed!\n";
            cerr << "ERROR: Could not delete <"<<m_outDir<<">. Clean operation failed."<< endl;
            result = false;
         }
      }
   }
   
   return result;
}

bool ossimBatchTest::getTempFileName( const ossimString& prefix,
                                      const ossimKeywordlist& kwl,
                                      ossimFilename& tempFile ) const
{
   const char* lookup = kwl.find(prefix.c_str(), "temp_file");
   if ( lookup )
   {
      tempFile = ossimFilename(lookup);
      if (tempFile.contains("$("))
         tempFile = tempFile.expand();
   }
   else
   {
      if ( getDefaultTempFileDir( tempFile ) )
      {
         if ( tempFile.createDirectory( true, 0775 ) )
         {
            // Tack on the file name.
            tempFile = tempFile.dirCat("tmp.txt");
         }
         else
         {
            tempFile.clear();
         }
      }
   }
   return ( tempFile.size() > 0 );
}

bool ossimBatchTest::getDefaultTempFileDir( ossimFilename& tempFile ) const
{
   // Create a default tmp directory under OSSIM_BATCH_TEST_RESULTS.
   tempFile = ossimEnvironmentUtility::instance()->getEnvironmentVariable(
      ossimString("OSSIM_BATCH_TEST_RESULTS") );
   if (!tempFile.empty())
      tempFile = tempFile.dirCat("tmp");
   else
      tempFile = ossimEnvironmentUtility::instance()->getEnvironmentVariable(ossimString("TEMP"));
   return ( tempFile.size() > 0 );
}

void ossimBatchTest::usage(ossimArgumentParser& ap)
{
   ossimApplicationUsage* au = ap.getApplicationUsage();

   au->setApplicationName(ap.getApplicationName());
   au->setDescription(ap.getApplicationName()+" batch test application.");
   
   au->setCommandLineUsage(ap.getApplicationName()+" <test-configuration-file.kwl>\n\n"
      "This will run all test within the configuration file.  For individual test control edit "
      "flags of the configuration file or use individual test options below. The following status "
      "is returned on exit:"
      "\n   -1 if any error occurred during test"
      "\n    1 if any test failed"
      "\n    0 if all tests passed\n");

   au->addCommandLineOption("-a or --accept-test", 
      "<testX> Runs \"run_expected_results_commands\" portion from test configuration file for "
      "testX where X is some test number, i.e. 1, 2, 3...  This will turn off all other test in "
      "the test config file. Notes: Multiple tests can be entered by quoting string of space "
      "separated test, e.g. \"test1 test2\". To accept all tests use \"all\" for test.");
   
   au->addCommandLineOption("-c or --clean-test", "<testX> Runs \"run_clean_commands\" portion "
      "from test configuration file for testX where X is some test number, i.e. 1, 2, 3...  This "
      "will turn off all other test in the test config file. Notes: Multiple tests can be entered "
      "by quoting string of space separated test, e.g. \"test1 test2\". To clean all tests use "
      "\"all\" for test.");

   au->addCommandLineOption("-p or --preprocess-test", 
      "<testX> Runs \"run_preprocessing_commands\" portion from test configuration file for testX "
      "where X is some test number, i.e. 1, 2, 3...  This will turn off all other test in the test "
      "config file. Notes: Multiple tests can be entered by quoting string of space separated "
      "test, e.g. \"test1 test2\". To preprocess all tests use \"all\" for test.");
   
   au->addCommandLineOption("-r or --run-test", 
      "<testX> Runs \"run_test_commands\" portion from test configuration file for testX where X "
      "is some test number, i.e. 1, 2, 3...  This will turn off all other test in the test config "
      "file. Notes: Multiple tests can be entered by quoting string of space separated test, e.g. "
      "\"test1 test2\". To run all tests use \"all\" for test.");
   
   au->addCommandLineOption("-h or --help", "Display usage.");
   
   au->addCommandLineOption("-W or -w", 
      "<template_name.kwl> Writes a long-form (-W) or a short-form (-w) template test "
      "configuration file.");

   // Write usage.
   au->write(ossimNotify(ossimNotifyLevel_INFO));
}

std::string ossimBatchTest::convertToNative( const char* lookup ) const
{
   std::string s;
   
   if ( lookup )
   {
      s = lookup;
      const std::string::size_type SIZE = s.size();
      std::string::size_type i = 0;
      
#if defined(_WIN32)
      //---
      // Must not convert slashes for: "del /Q", "fc /W", and "copy /Y"
      //---
      while( i < SIZE )
      {
         if( s[i] == '/' )
         {
            bool replace = true;
            if ( (i+2) < SIZE ) // Could fit a window command and space like: "/Y "
            {
               if ( (s[i+1] == 'Q') || (s[i+1] == 'W') || (s[i+1] == 'Y') )
               {
                  if ( s[i+2] == ' ' ) // Check for space after "/Q".
                  {
                     replace = false;
                  }
               }
            }
            if ( replace )
            {
               s[i] = '\\';
            }
         }
         ++i;
      }
#else
      while( i < SIZE )
      {
         if( s[i] == '\\' )
         {
            s[i] = '/';
         }
         ++i;
      }
#endif
      
   } // Matches: if ( lookup )
   
   return s;
}

