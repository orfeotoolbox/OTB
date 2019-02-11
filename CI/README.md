# CI
This folder contains files for the continuous integration.

## Docker files
The docker files are: 

 * Dockerfile.nat: native environment to test OTB. Shark is not on this one as the version OTB is using is not packaged (since 6.7)
 * Dockerfile.shark.1.3.4: native environment and Shark version 3.1.4.

## CMake files
The docker files are:

 * main_ci.cmake: main script that will launch configure, build and soon test. It aims at configuring Ctest before running the different jobs (site name, build command...)
 * configure_option.cmake: script that gathers all options needed by OTB.

## Yaml file
The last file needed for the CI is the *.gitlab-ci.yml* it aims at interfacing with GitLab. Here is the basic structure: 
```
Job-name:
  stage: job-type
  image: docker_image_that_will_be_used
  script:
    - script that will be launch inside the docker
```

# CDash interaction
Results of build can be seen in CDash on the track "Experimental". The site is the image docker that run the build, and the build name should be the branch name that is tested.