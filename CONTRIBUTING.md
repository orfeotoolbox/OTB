# How to contribute to Orfeo ToolBox ?

Thank you for taking the time to contribute to OTB! This document will guide you
through the workflow and best practices you need to know to send your
contribution.

There are many ways to contribute to OTB:

* [Reporting a bug](#reporting-bugs)
* [Making a feature request](#feature-requests-and-discussions)
* [Improving documentation](#documentation-improvements)
* [Contributing code (C++, Python, CMake, etc.)](#code-contribution)
* [Publishing a remote module](#remote-modules)

Our main workflow uses GitLab for source control, issues and task tracking. We
use a self-hosted GitLab instance:

[`https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb`](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb)

Remember to check out also our [forum](https://forum.orfeo-toolbox.org/),
where we discuss some features, improvements and high level project planning.
You are welcome to ask questions there as a beginner or future OTB contributor!

## Reporting bugs

If you have found a bug, you can first [search the existing issues](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues?label_name%5B%5D=bug)
to see if it has already been reported.

If it's a new bug, please [open a new issue on GitLab](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/new).
The 'Bug' issue template will help you provide all important information and
help fixing the bug quicker. Remember to add as much information as possible!

## Feature requests and discussions

Feature requests are welcome! Generally you are welcome to simply [open an issue](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues)
and discuss your idea there. For more complex requests there is an issue
template for in depth description called 'Request for Comments'.


## Documentation improvements

The two main OTB documentations are the [Software Guide](https://www.orfeo-toolbox.org/SoftwareGuide/index.html)
and the [CookBook](https://www.orfeo-toolbox.org/CookBook/).  Their sources are
hosted in the main OTB repository in the `Documentation/` directory. Then, to
contribute documentation use the same workflow as for code contributions (see
below).

See also the [Compiling documentation](https://wiki.orfeo-toolbox.org/index.php/Compiling_documentation)
wiki page for help on building the Sphinx and Latex source.

## Code contribution

The OTB workflow is based on GitLab [Merge Requests](https://docs.gitlab.com/ee/gitlab-basics/add-merge-request.html).
Clone the repository, create a feature branch, commit your changes, push the
feature branch to a fork (or the main repository if you are a core developer),
then send a merge request.

Note that we also accept PRs on our [GitHub mirror](https://github.com/orfeotoolbox/OTB)
which we will manually merge.

Feature branches are tested on multiple platforms on the OTB test infrastructure (a.k.a the [Dashboard](https://cdash.orfeo-toolbox.org/)). They appear in the FeatureBranches section. 

Caveat: even if the Dashboard build on develop branch is broken, it is not
allowed to push fixes directly on develop. The developer trying to fix the
build should create a merge request and submit it for review. Direct push to
develop without review must be avoided.

### Commit message

On your feature branch, write a good [commit message](https://xkcd.com/1296/):
short and descriptive. If fixing an issue or bug, put the issue number in the
commit message so that GitLab can [cross-link it](https://docs.gitlab.com/ce/user/project/issues/crosslinking_issues.html).
You can prefix your commit message with an indicating flag (DOC, BUG, PKG,
TEST, SuperBuild, etc.).

Standard prefixes for OTB commit messages:

    BUG: Fix for runtime crash or incorrect result
    COMP: Compiler error or warning fix
    DOC: Documentation change
    ENH: New functionality
    PERF: Performance improvement
    STYLE: No logic impact (indentation, comments)
    WIP: Work In Progress not ready for merge

For example, here are some good commit messages:

    BUG: #1701 Warn users if parameter string is unset
    DOC: Fix typo in Monteverdi French translation
    COMP: Allow GeoTIFF and TIFF to be disabled when no 3rd party drags them

### Merge request

Your contribution is ready to be added to the main OTB repository? Send a Merge
Request against the `develop` branch on GitLab using the merge request
template. The merge request will then be discussed by the community and the core
OTB team.

* Merge requests can not be merged until all discussions have been resolved (this is enforced by GitLab)
* Merge requests **must receive at least 2 positives votes from core developers** (members of Main Repositories group in Gitlab with at least "Developer" level; this includes PSC members) before being merged
* The merger is responsible for checking that the branch is up-to-date with develop
* Merge requests can be merged by anyone (not just PSC or RM) with push access to develop
* Merge requests can be merged once the dashboard is proven green for this branch.
  This condition is mandatory unless reviewers and authors explicitely agree that
  it can be skipped (for instance in case of documentation merges or compilation
  fixes on develop). Branches of that sort can be identified with the ~patch label, 
  which tells the reviewer that the author would like to merge without dashboard testing.

Branches can be registered for dashboard testing by adding one line in `Config/feature_branches.txt` in [otb-devutils repository](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-devutils.git).

For branches in the main repository, the syntax is the following:

```
branch_name [otb-data_branch_name]

```
The second branch name is optional. It can be set if you need to modify [otb-data](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-data.git) according to your changes.

For branches in forks, the syntax is the following:
```
user/branch_name [user/otb-data_branch_name]
```
Again, the second branch name is optional.

For users without push access to [otb-devutils repository](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb-devutils.git), the modification can be asked through a merge requests to this repository.

Once the feature branch is registered for testing, it should appear in the *FeatureBranches* section of the [OTB dashboard](https://cdash.orfeo-toolbox.org/index.php?project=OTB) next day (remember tests are run on a nightly basis).

Do not forget to remove the feature branch for testing once it has been merged.

### Contribution license agreement

OTB requires that contributors sign out a [Contributor License
Agreement](https://en.wikipedia.org/wiki/Contributor_License_Agreement). The
purpose of this CLA is to ensure that the project has the necessary ownership or
grants of rights over all contributions to allow them to distribute under the
chosen license (Apache License Version 2.0)

To accept your contribution, we need you to complete, sign and email to *cla [at]
orfeo-toolbox [dot] org* an [Individual Contributor Licensing
Agreement](https://www.orfeo-toolbox.org/cla/icla-en.doc) (ICLA) form and a
[Corporate Contributor Licensing
Agreement](https://www.orfeo-toolbox.org/cla/ccla-en.doc) (CCLA) form if you are
contributing on behalf of your company or another entity which retains copyright
for your contribution.

The copyright owner (or owner's agent) must be mentioned in headers of all
modified source files and also added to the [NOTICE
file](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/raw/develop/NOTICE).

## Remote modules

[Remote Modules](https://wiki.orfeo-toolbox.org/index.php/Remote_Modules) are
the preferred way if you wish to make your apps and filters available to the
community while keeping control and maintenance of their sources. Remote
modules are just like regular modules, except they are not distributed inside
OTB source code. Under some conditions (dependencies, official acceptance
process, etc.), we are also able to distribute your remote module in the
official standalone binaries. See [the wiki](https://wiki.orfeo-toolbox.org/index.php/Remote_Modules)
for more information.

## GitLab guidelines

In order to organize the issues in our GitLab instance, we use both labels and
milestones.

The [milestones](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/milestones) should be used to track in which release a feature is merged.
GitLab can then provide a summary of all features and bugs added to a given release
version.

Regarding labels, we use the following set:
* ~story: significant feature to be implemented with a detailed work plan, it can
  correspond to a Request for Comments that has turned into a development action
* ~bug: Bug, crash or unexpected behavior, reported by a user or a developer
* ~feature: Feature request expressed by an OTB user/developer
* ~patch: A small patch fixing build warnings, compilation errors, typos in logs or documentation
* ~"To Do": action is planned
* ~Doing: work in progress
* ~api ~app ~documentation ~monteverdi ~packaging ~qgis: optional context information

## Versioning

Starting from OTB 7.0.0, we use [semantic versioning](https://semver.org/). See the website for the full spec, in summary:

> Given a version number MAJOR.MINOR.PATCH, increment the:
>
>  1. MAJOR version when you make incompatible API changes,
>  2. MINOR version when you add functionality in a backwards-compatible manner, and
>  3. PATCH version when you make backwards-compatible bug fixes.
>
> Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.

The develop branch is always the place where the upcoming major or minor release is worked on. Patch releases are done on release branches, for example 7.0.1 and 7.0.2 could be found on the release-7.0 branch.

For the purpose of defining backwards compatibility, the OTB API covers (not an exhaustive list): the C++ API, the Python bindings, application names, application parameters, output format and interpretation of input data.

When we deprecate part of our public API, we should do two things: (1) update our documentation to let users know about the change, (2) issue a new minor or major release with the deprecation in place.
