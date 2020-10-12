We are ready to release OTB version MAJOR.MINOR.PATCH. The following steps need to be done:

## Release Candidate

### 1. Branches

* [ ] **(if major or minor release)** Feature freeze: [create the new release branch](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#create-release-branch)
* [ ] **(if patch release)** Work on the already existing branch `release-MAJOR-MINOR`
* [ ] Make sure the version number in `CMakeLists.txt` is MAJOR.MINOR.PATCH

### 2. Housekeeping
* [ ] In this story, make a list of blocking issues for the release (if any)
* [ ] Update release notes (walk the GitLab MR merged history and log all improvements)
* [ ] Update the date in RELEASE_NOTES.txt
* [ ] Check [SonarQube](https://sonar.orfeo-toolbox.org/dashboard?id=orfeotoolbox-otb)
* [ ] Run Debian [spelling](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#spelling-check) checker
* [ ] Run shellcheck script from [OTB-Devutils/Scripts/](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-devutils/blob/master/Scripts/run_shellcheck.sh)
* [ ] [Update translation](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#translation-for-monteverdi-mapla) for Monteverdi and Mapla
* [ ] [Sanity check the binary packages](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#standalone-packages-sanity-check)
    * [ ] Windows
    * [ ] Linux
    * [ ] Mac
    * [ ] Test QGIS on qgis docker image

### 3. Actual release

Once all blocking issues are closed, and the previous steps are done:
* [ ] [Tag the release candidate](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#release-tag)
* [ ] Update GIT_TAG for all official remote modules (if needed)

### 4. Publish and plan next release
* [ ]   [Prepare and upload source packages](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#prepare-and-upload-source-packages)
* [ ]   [Promote staging packages](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#promote-staging-packages)
* [ ]   [Update documentation](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#update-documentation)
    * [ ] Cookbook
    * [ ] Doxygen
* [ ] [Update the SuperBuild archive](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#superbuild-archive)
* [ ] Release Candidate announcement on the forum

## Release

### 1. Branches

* [ ] Make sure the version number in `CMakeLists.txt` is MAJOR.MINOR.PATCH

### 2. Housekeeping
* [ ] In this story, make a list of blocking issues for the release (if any)
* [ ] Fix compilation warnings on CI
* [ ] Update release notes (walk the GitLab MR merged history and log all improvements)
* [ ] Update the date in RELEASE_NOTES.txt
* [ ] Check [SonarQube](https://sonar.orfeo-toolbox.org/dashboard?id=orfeotoolbox-otb)
* [ ] Run Debian [spelling](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#spelling-check) checker
* [ ] Run shellcheck script from [OTB-Devutils/Scripts/](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-devutils/blob/master/Scripts/run_shellcheck.sh)
* [ ] [Update translation](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#translation-for-monteverdi-mapla) for Monteverdi and Mapla
* [ ] [Sanity check the binary packages](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#standalone-packages-sanity-check)
    * [ ] Windows
    * [ ] Linux
    * [ ] Mac
    * [ ] Test QGIS on qgis docker image

### 3. Actual release

Once all blocking issues are closed, and the previous steps are done:
* [ ] [Tag the release](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#release-tag)
* [ ] **(if major or minor release)**: Merge the release into develop
* [ ] **(if it's the latest release)**: Merge the release into master
* [ ] **(if patch release)**: Backport fixes
* [ ] Update GIT_TAG for all official remote modules (if needed)

### 4. Publish and plan next release
* [ ]   [Prepare and upload source packages](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#prepare-and-upload-source-packages)
* [ ]   [Promote staging packages](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#promote-staging-packages)
* [ ]   [Update documentation](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#update-documentation)
    * [ ] Cookbook
    * [ ] Doxygen
    * [ ] WordPress page "Home" and "Download" pages
* [ ] Upload OTB source archive to [Zenodo](https://zenodo.org/) to create a unique Digital Object Identifier (DOI)
* [ ] [Update the SuperBuild archive](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/Help-for-release-actions#superbuild-archive)
* [ ] Release announcement
    * [ ] On the [forum](https://forum.orfeo-toolbox.org/)
    * [ ] On the [blog](https://www.orfeo-toolbox.org/blog/)
	* [ ] On [Twitter](https://twitter.com/orfeotoolbox)
* [ ] Forward announcement to news_item@osgeo.org ([OSGeo news](https://www.osgeo.org/foundation-news/))
* [ ] Remove public branches related to MR or bugfix merged before the release

/label ~story
