function(repository_status root_repo_dir result_var1)
  if(NOT EXISTS "${root_repo_dir}/.git")
    return()
  endif()

  find_package(Git)
  if(NOT GIT_FOUND)
    message(STATUS "git not found. Make sure git can be found in your PATH. (source status will not be reported)")
    set(${result_var1})
    return()
  endif()

  execute_process(COMMAND ${GIT_EXECUTABLE} config --get remote.origin.url
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE git_remote_origin_url
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
  if(git_remote_origin_url)
    get_filename_component(dir_name ${git_remote_origin_url} NAME)
  else()
    get_filename_component(dir_name ${root_repo_dir} NAME)
  endif()

  execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H --oneline
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE last_commit_id
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

  #git symbolic-ref --short -q HEAD does not work on older git version
  #rather than checking for git version. it is better to not use that
  #option and employ cmake stuff to do the work
  execute_process(COMMAND ${GIT_EXECUTABLE} symbolic-ref -q HEAD
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE git_symbolic_ref_output
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)

  set(branch_name)
  if(git_symbolic_ref_output)
    get_filename_component(branch_name ${git_symbolic_ref_output} NAME)
  endif()

  set(return_msg "${dir_name} repository info: \n")
  set(return_msg "${return_msg} git log -1 --oneline     = [${last_commit_id}] \n")
  set(return_msg "${return_msg} git symbolic-ref -q HEAD = ${branch_name}")
  set(${result_var1} ${return_msg} PARENT_SCOPE)

  #print other information inside function call
  execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%H
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE OTB_WC_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
  execute_process(COMMAND ${GIT_EXECUTABLE} status -s
    WORKING_DIRECTORY ${root_repo_dir}
    OUTPUT_VARIABLE OTB_WC_STATUS
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
  message(STATUS "Repository status (${dir_name}) :")
  message(STATUS "  Repository revision is ${OTB_WC_REVISION}")
  if(OTB_WC_STATUS)
    message(STATUS "  Local file modifications (${dir_name}) :")
    string(REPLACE "\n" "\n--     " OTB_WC_STATUS_PRETTYPRINT "    ${OTB_WC_STATUS}")
    message(STATUS "${OTB_WC_STATUS_PRETTYPRINT}")
  else()
    message(STATUS "  No files modified locally (${dir_name})")
  endif()

endfunction()
