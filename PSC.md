# Project Steering Committee

This document describes the Project Steering Committee of the Orfeo ToolBox.

## PSC scope

The aim of the **OTB Project Steering committee (PSC)** is to provide
high level guidance and coordination for the ORFEO ToolBox.

It provides a central point of contact for the project and arbitrates
disputes. It is also a stable base of “institutional knowledge” to the
project and tries its best to involve more developers.

It should help to guarantee that OTB remains open and company neutral.

### Roadmaps

The PSC gathers and publishes high level roadmaps for OTB:

-   Feature roadmap
-   Technical roadmap (developer and coding guidelines and workflows,
    target systems, packaging ...)
-   Infrastructure roadmap (SCM, wiki, dashboard ...)

The PSC also publishes the guidelines and the acceptance policy for
feature requests. It enforces them. It monitors and approves new feature
requests.

### Communication

The PSC coordinates communication actions:

-   Ensures that the wiki is up-to-date,
-   Ensures that the website is up-to-date and proposes new content,
-   Ensures regular posting on the blog and social networks,
-   Keeps track of opportunities to communicate: Symposium and events
    where OTB should be represented,
-   Keeps track of opportunities from communities: Google Summer of Code
    project, link with other OSGeo and FOSS projects,
-   Is responsible for the organization of events around OTB (i.e. users
    meetings and hackathon).

### User support and documentation

The PSC ensures that users are given an appropriate support:

-   Ensures that support is working (unanswered questions, questions
    from other places than the user list ...)
-   Proposes addition to the documentation based on users feedback,
-   Proposes new features for the roadmap based on users feedback,
-   Proposes new ways for support and documentation

### Contribution management

The PSC publishes the guidelines and acceptance policy for
contributions. It enforces them.

It monitors and approves new proposals.

It ensures that contribution is as easy as possible, by monitoring
technical means for contribution and proposing evolutions to guidelines,
policies and means.

### Release planning

The PSC publishes release guidelines and policies, and enforces them.

The PSC puts together the next Release roadmap and proposes a planning.
It is then responsible for the release preparation.

The final approval for a release is given by the PSC (motion proposed to
the otb-developers mailing list).

### Handling of legal issues

The PSC is responsible for addressing any issue about copyright or
licensing that may occur, and most importantly, it is responsible for
taking preventive actions about those issues.

## How does the PSC work?

This section describes how the PSC works. It is inspired by existing
governance statuses in other open source community projects related to
OTB like
[GDAL](https://trac.osgeo.org/gdal/wiki/GovernanceAndCommunity),
[Quantum
GIS](http://www2.qgis.org/en/site/getinvolved/governance/index.html) or
[GRASS](http://trac.osgeo.org/grass/wiki/PSC).

### PSC members

All members have equal standing and voice in the PSC. The PSC seats are
non-expiring. PSC members may resign their position, or be asked to
vacate their seat after a unanimous vote of no confidence from the
remaining PSC members.

The expectations on PSC members are:

-   Be willing to commit to the OTB development effort
-   Be responsive to requests for information from fellow members
-   Be able and willing to attend on-line meetings
-   Act in the best interests of the project

It is important to note that the PSC is not a legal entity!

### Roles

Members can be assigned roles corresponding to each category of the PSC
scope described above.

Being assigned a role does not mean undertaking all necessary actions,
but rather ensuring that actions will be undertaken.

In addition to their specific roles, members of the PSC commit to
participate actively in discussions and votes.

One member of the PSC is designated as the Chair and is the ultimate
adjudicator in case of deadlock or irretrievable break down of
decision-making, or in case of disputes over voting.

### When is a PSC vote required?

A vote of the PSC is required in the following cases:

1.  Some Merge Request (see below)
2.  Addition or removal of PSC members (including the selection of a new
    Chair)
3.  Release process

In addition, a vote can be summoned for:

1.  Changing PSC rules and processes
2.  Anything else that might be controversial

#### Merge Request

All changes in Orfeo ToolBox (code, API, infrastructure or processes) must be 
handle with a Merge Request :

-   Anything that could cause backward compatibility issues,
-   Adding substantial amounts of new code,
-   Changing inter-subsystem APIs, or objects,
-   Any change in the code or in the documentation.

Merge Request can implement an issue in GitLab. 

Merge Requests must be provided to a git hosted platform (GitLab, GitHub, etc.). 
Merge request can be discussed on the developer list or directly on GitLab.

Votes are necessary to accept Merge Request : 
-   Core developers ('Master' members in Gitlab ; it includes PSC members) can vote
-   At least two +1 are necessary
-   PSC members have veto

#### Add or remove PSC members

To be eligible for membership in the PSC, a person should demonstrate
**a substantial and ongoing involvement in OTB**. The PSC is not only
composed of OTB developers as there are many ways to join and contribute
to the project. Anyone is eligible to be nominated to the OTB PSC.
Ideally, nominees would be OTB users or developers who have a deep
understanding of the project. In addition, nominees should meet the
qualifications set forth in this document. Anyone can submit a
nomination.

#### Release phases

The release manager (the PSC member in charge of release planning)
submits to vote the following release decisions:

1.  Date of the next release
2.  Codename of the next release
3.  Date and revision of Release Candidate
4.  Date and revision of Final Release
5.  Date and revision of bug-fixes Release

### Process

-   Proposals are written up and submitted as GitLab merge requests or on the
    otb-developers mailing list for discussion and voting, by any interested
    party, not just committee members. Proposals are available for review for at
    least three days before a vote can be closed. It is acknowledged that some
    more complex issues may require more time for discussion and deliberation.
-   Respondents may vote “+1” to indicate support for the proposal and a
    willingness to support implementation.
-   Respondents may vote “-1” to veto a proposal, but must provide
    argumented reasoning and alternate approaches to resolve the problem
    within the two days.
-   A vote of -0 indicates mild disagreement, but has no effect. A 0
    indicates no opinion. A +0 indicates mild support, but has no
    effect.
-   Anyone may comment and vote on proposals on the list or on the merge request
    thread, but only members of the PSC's votes (including the Chair) will be
    counted (“eligible voters”).
-   A proposal will be accepted if it receives at least +2 (including
    the proponent) and no vetos (-1)
-   If a proposal is vetoed, and it cannot be revised to satisfy all
    parties, then it can be resubmitted for an override vote in which a
    majority of all eligible voters indicating +1 is sufficient to pass
    it. Note that this is a majority of all committee members, not just
    those who actively vote.
-   The Chair adjudicates in cases of disputes about voting.

A summary of discussions is published in a dedicated section of the wiki
[Requests for Changes](https://wiki.orfeo-toolbox.org/index.php/Requests_for_Changes).

## Current members and roles

In March 2015, CNES nominated 3 persons deeply involved in OTB as
initial PSC members. They are responsible for defining PSC rules and
establishing a fully functioning PSC. PSC has now 4 members.

**Name**                    | **Affiliation**  | **Email**                        | **Role**                                   |
----------------------------|------------------|----------------------------------|--------------------------------------------|
Agustin Lobo                | ICTJA-CSIC       | Agustin DOT Lobo AT ictja.csic.es| User perspective, documentation            |
Julien Radoux               | UCLouvain        | jradoux AT hotmail DOT com       | User perpsective, documentation            |
Rémi Cresson                | IRSTEA           | cresson.r AT gmail DOT com       | Release Manager for release 5.2            |
Guillaume Pasero            | CS-SI            | guillaume.pasero AT c-s DOT fr   | release planner                            |
Julien Michel               | CNES             | julien.michel AT cnes DOT fr     | Communication, contributions               |
Victor Poughon              | CNES             | victor.poughon AT cnes DOT fr    | User support and documentation, roadmaps   |
Jordi Inglada (resigned)    | CNES/CESBIO      | jordi.inglada AT cesbio DOT eu   |                                            |
Manuel Grizonnet (resigned) | CNES             | manuel.grizonnet AT cnes DOT fr  | Infrastructure, legal issues               |

## Release manager

A **release manager** is nominated for each release. Nomination is made
shortly after previous release announcement. A **backup release
manager** is also nominated, to assist and possibly step in if the
official release manager is not available. The **release manager** and
his/her backup can be a member of the PSC, but also another member of
otb-developers list willing to take the spot.

The **release manager** is in charge of :

1.  Listing and tracking all major features proposed for next release
    (from call for contribution in the early phase and then approved
    RFCs)
2.  Planing release date and ensures sync with RFCs execution,
3.  Approving feature branch merges (if possible, leave at least 3 days
    between RFC submission and merge, so that people have time to do a
    review)
4.  Actually merging feature branches when authors do not have commit
    rights (github pull request for instance)
5.  Tracking remote module that can be candidate for official inclusion
    in next release, and ensuring they will be included (see
    [Contributors guidelines](CONTRIBUTING.md))
6.  Submitting the start of the [release
    process](https://wiki.orfeo-toolbox.org/index.php/How_to_Release) to PSC vote
7.  Ensuring proper execution of [release
    process](https://wiki.orfeo-toolbox.org/index.php/How_to_Release)

**Feature freeze:** Starting the release process is also called *feature
freeze* which describes the period between the creation of the release
branch and the announcement of the release. It's an important time for
the RM who should ensures the proper execution of the release process
and facilitate the communication between developers. The RM is among
other things responsible of gathering feedback on bugs that need to be
fixed before the release, making a list that is public to be able to
follow progression of the release process.

**Remember:** all new features must be submitted by Merge Requests and
approved by the PSC. The release manager only approves the feature
branches merge.

### Feature branch merge acceptance checklist

1.  The feature branch corresponds to an [approved
    Merge Request](#process)
2.  The feature branch is synched with develop
3.  The feature branch is tested on the dashboard
    and has no major failure
    (compilation errors, tremendous amount of warning or failing tests,
    segfault or not executed tests)
4.  Feature branch author are available during days following the merge
    to analyse dashboard and fix things in case of unexpected issues
    after the merge

It is important to note that a feature branch should be kept relatively
small and does not necessarily correspond to the full implementation of
a RFC. Small consistent branches implementing parts of RFC can be merged
early. Further evolutions and implementations of the RFC can be done by
continuing the feature branch or opening new branches, and approval of
Release Manager should be requested again before merging.
