We are ready to release OTB version MAJOR.MINOR.PATCH. The following steps need to be done:

If major or minor release:
* [ ] [Create the new release branch](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Create_release_branch)

If patch release:
* [ ] Checkout the `release-MAJOR-MINOR` branch

* [ ] Make sure the version number is MAJOR.MINOR.PATCH, if not update it it `CMakeLists.txt`.

* [ ] [Update dashboard scripts](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Dashboard) to support new version numbers
* [ ] [Update the SuperBuild archive](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#SuperBuild_archive) (if needed)
* [ ] Gather feedback on bugs that need to be fixed, make a public list and track the fixes to be able to get progress info during the release
* [ ] Update release notes (walk the GitLab MR merged history and log all improvements) for OTB, Monteverdi and Ice.
* [ ] Update the date in RELEASE_NOTES.txt
* [ ] Run Debian [spelling](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Spelling_check) checker
* [ ] Run shellcheck script from [OTB-Devutils/Scripts/](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-devutils/blob/master/Scripts/run_shellcheck.sh)
* [ ] [Update translation](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Translation_for_Monteverdi_.26_Mapla) for Monteverdi and Mapla
* [ ] Update GIT_TAG for all official remote modules

* [ ] [Sanity check the binary packages](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Standalone_packages_sanity_check)
  * [ ] Windows
  * [ ] Linux
  * [ ] Mac
* [ ] [Tag the release](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Release_tag)

If major or minor release:
    * [ ] Merge the release into develop
    * [ ] Merge the release into master

If patch release:
   * [ ] Backport fixes

* [ ]   [Prepare and upload source packages](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Prepare_and_upload_source_packages)
* [ ]   [Promote nightly packages](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Promote_nightly_packages)
* [ ]   [Update documentation](https://wiki.orfeo-toolbox.org/index.php/Help_for_release_actions#Update_documentation)
    * [ ] Software Guide
    * [ ] Cookbook
    * [ ] Doxygen
    * [ ] Application online documentation
    * [ ] WordPress page "Home" and "Download" pages
* [ ] Upload OTB source archive to [Zenodo](https://zenodo.org/) to create a unique Digital Object Identifier (DOI)
* [ ] Update OTB-Data-Examples.tgz on orfeo-toolbox (packages)
* [ ] Send email to mailing list to announce the release
* [ ] Release announcement on the blog
* [ ] Announcement on social networks (twitter, google+)
* [ ] Forward announcement to news_item@osgeo.org ([OSGeo news](https://www.osgeo.org/foundation-news/))
* [ ] Plan the next release (nominate new release manager, setup PSC meeting on IRC)
* [ ] Contact QGis processing plugin maintainer to update XML description for new OTB-Applications (or [supply it](https://wiki.orfeo-toolbox.org/index.php/QGIS_access_to_OTB_applications#Updating_the_XML_descriptors))
* [ ] Remove public branches related to MR or bugfix merged before the release

/label ~story
