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

Remember to check out also our [forum](https://forum.orfeo-toolbox.org/), where
we discuss some features, improvements and high level project planning.  You are
welcome to ask questions there as a beginner or future OTB contributor!

## Reporting bugs

If you have found a bug, you can first [search the existing
issues](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues?label_name%5B%5D=bug)
to see if it has already been reported.

If it's a new bug, please [open a new issue on
GitLab](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues/new).  The
'Bug' issue template will help you provide all important information and help
fixing the bug quicker. Remember to add as much information as possible!

## Feature requests and discussions

Feature requests are welcome! Generally you are welcome to simply [open an
issue](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/issues) and discuss
your idea there. For more complex requests there is an issue template for in
depth description called 'Request for Comments'.


## Documentation improvements

The main OTB documentation is the
[CookBook](https://www.orfeo-toolbox.org/CookBook/).  The source is hosted in
the main OTB repository in the `Documentation/` directory. Then, to contribute
documentation use the same workflow as for code contributions (see below).

See also the "Compiling documentation" section of the CookBook for help on
building the Sphinx source.

## Code contribution

The OTB workflow is based on GitLab [Merge
Requests](https://docs.gitlab.com/ee/gitlab-basics/add-merge-request.html).
Clone the repository, create a feature branch, commit your changes, push the
feature branch to a fork (or the main repository if you are a core developer),
then send a merge request.

Note that we also accept PRs on our [GitHub
mirror](https://github.com/orfeotoolbox/OTB) which we will manually merge.

Feature branches are tested on multiple platforms on the OTB [CI
infrastructure](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/pipelines).

Caveat: even if the CI build on develop branch is broken, it is not allowed to
push fixes directly on develop. The developer trying to fix the build should
create a merge request and submit it for review. Direct push to develop without
review must be avoided.

### Commit message

On your feature branch, write a good [commit message](https://xkcd.com/1296/):
short and descriptive. If fixing an issue or bug, put the issue number in the
commit message so that GitLab can [cross-link
it](https://docs.gitlab.com/ce/user/project/issues/crosslinking_issues.html).
You can prefix your commit message with an indicating flag (DOC, BUG, PKG, TEST,
SuperBuild, etc.).

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

* Merge requests can not be merged until all discussions have been resolved
  (this is enforced by GitLab)
* Merge requests **must receive at least 2 positives votes from core
  developers** (members of Main Repositories group in GitLab with at least
  "Developer" level; this includes PSC members) before being merged
* The merger is responsible for checking that the branch is up-to-date with
  develop
* Merge requests can be merged by anyone (not just PSC or RM) with push access
  to develop
* Merge requests can be merged once the CI pipeline passes successfully. See
  next section for details on the CI pipelines.


### Using the CI platform

There isn't much to do in order to use the CI platform. The CI pipelines are
triggered automatically when pushing commits. However, if you push to a fork,
you will first need an access to the Runners from main repository. You can
request it when doing your first Merge Request. During code review, someone from
CI admins will assign the runners to your fork.

When your pipeline ends, there are two cases:

* if all the jobs succeed, you see a green pipeline, which means no problem was
  found on your commit.
* if one job fails, you see a red pipeline, which means something is broken in
  your commit. The pipeline widget on GitLab will tell you which job failed, so
  you can check the logs. There, you may also find links to
  [CDash](https://cdash.orfeo-toolbox.org/index.php?project=OTB) submissions
  where compilation errors and failed test can be investigated more easily.

More details on the CI platform can be found
[here](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/wikis/OTB-Continuous-Integration-platform).

### Contribution license agreement

OTB requires that contributors sign out a [Contributor License
Agreement](https://en.wikipedia.org/wiki/Contributor_License_Agreement). The
purpose of this CLA is to ensure that the project has the necessary ownership or
grants of rights over all contributions to allow them to distribute under the
chosen license (Apache License Version 2.0)

To accept your contribution, we need you to complete, sign and email to *cla
[at] orfeo-toolbox [dot] org* an [Individual Contributor Licensing
Agreement](https://www.orfeo-toolbox.org/cla/icla-en.doc) (ICLA) form and a
[Corporate Contributor Licensing
Agreement](https://www.orfeo-toolbox.org/cla/ccla-en.doc) (CCLA) form if you are
contributing on behalf of your company or another entity which retains copyright
for your contribution.

The copyright owner (or owner's agent) must be mentioned in headers of all
modified source files and also added to the [NOTICE
file](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/raw/develop/NOTICE).

## Remote modules

Remote Modules are the preferred way if you wish to make your apps and filters
available to the community while keeping control and maintenance of their
sources. Those modules are just like regular modules, except they are not
distributed inside OTB source code. For more information, see [the
CookBook](https://www.orfeo-toolbox.org/CookBook-develop/RemoteModules.html)

## GitLab guidelines

In order to organize the issues in our GitLab instance, we use both labels and
milestones.

The [milestones](https://gitlab.orfeo-toolbox.org/orfeotoolbox/otb/milestones)
should be used to track in which release a feature is merged.  GitLab can then
provide a summary of all features and bugs added to a given release version.

Regarding labels, we use the following set:

| Label                                              | Description                                                                                                                                            |
|----------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------|
| ~story                                             | Significant feature to be implemented with a detailed work plan, it can correspond to a Request for Comments that has turned into a development action |
| ~bug                                               | Bug, crash or unexpected behavior, reported by a user or a developer                                                                                   |
| ~feature                                           | Feature request expressed by an OTB user/developer                                                                                                     |
| ~usage                                             | Question related to OTB's usage                                                                                                                        |
| ~patch                                             | A small patch fixing build warnings, compilation errors, typos in logs or documentation                                                                |
| ~"to investigate"                                  | This issue needs a complete analysis                                                                                                                   |
| ~"CNES backlog"                                    | The CNES could fund this issue but has not been assigned to a milestone yet                                                                            |
| ~"To Do"                                           | Action is planned                                                                                                                                      |
| ~Doing                                             | Work in progress                                                                                                                                       |
| ~"Waiting for answer"                              | Waiting for an answer or for a detailed context                                                                                                        |
| ~breaking                                          | Changes introduced by this issue will break the API                                                                                                    |
| ~refactoring                                       | Issue related to improvements like refactoring, tests, maintenance, etc                                                                                |
| ~documentation                                     | Improvemen tof the documentation                                                                                                                       |
| ~ci                                                | Issue related to the continuous integration plateform                                                                                                  |
| ~api ~app ~legal ~monteverdi <br> ~packaging ~qgis | Optional context information                                                                                                                           |
  
## Issues life cycle

When an issue is open, the core development team will review it in a
first analysis. During this first analysis, the team will evaluate is
the issue is a ~bug, a ~feature request or related to an ~usage. The
team may also ask for more information.

### Bugs

Sometimes, the first analysis shows that the issue is related to a
misuse of the OTB. In that case, the issue is redefined to ~usage.

It happens that the bug is trivial to correct or a workaround is easy
to set up. In this case, it is corrected directly, or the workaround
is documented, and the issue is closed. If the bug is not trivial, the
team will proceed to a second, more complete, analysis. During this
analysis, the issue is marked as ~"To investigate". This is a time to
iterate with the author of the issue to find the best way to address
it. At the end of this analysis, the issue's description should
contain the proposition of correction, the impact for the code and the
test, the validation criterion, and an estimation of the
implementation schedule. Sometimes, a workaround can be proposed in
place of a correction.

As the main funder of the project, the CNES may support the funding
for the correction, but it is not systematic, since its funds are
limited. In case of CNES funding, the issue is assigned to a milestone
(the next minor release or the next major release).

### Feature request

The core development team will proceed to a second, more complete,
analysis. During this analysis, the issue is marked as ~"To
investigate". This is a time to iterate with the author of the issue
to find the best way to address it. At the end of this analysis, the
issue's description should contain the conception of the new feature,
the impact for the users, the code and the test, the validation
criterion, and an estimation of the implementation schedule. In case
of CNES funding, the issue is assigned to a milestone (the next minor
release or the next major release). If the request doesn't correspond
to the roadmap of those milestone, the label ~"CNES backlog" is set to
the issue.

### Usage

When someone asks a question about the usage of the OTB, or doesn't understand
some functionalities, the community will be pleased to help them by answering
the questions and providing some support. This kind of issues are also an
opportunity to improve the documentation and make the OTB easier to use.

## Versioning

Starting from OTB 7.0.0, we use [semantic versioning](https://semver.org/). See
the website for the full spec, in summary:

> Given a version number MAJOR.MINOR.PATCH, increment the:
>
>  1. MAJOR version when you make incompatible API changes,
>  2. MINOR version when you add functionality in a backwards-compatible manner, and
>  3. PATCH version when you make backwards-compatible bug fixes.
>
> Additional labels for pre-release and build metadata are available as extensions to the MAJOR.MINOR.PATCH format.

The develop branch is always the place where the upcoming major or minor release
is worked on. Patch releases are done on release branches, for example 7.0.1 and
7.0.2 could be found on the release-7.0 branch.

For the purpose of defining backwards compatibility, the OTB API covers (not an
exhaustive list): the C++ API, the Python bindings, application names,
application parameters, output format and interpretation of input data.

When we deprecate part of our public API, we should do two things: (1) update
our documentation to let users know about the change, (2) issue a new minor or
major release with the deprecation in place.
