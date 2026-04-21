# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file LICENSE.rst or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/src/dep_glad")
  file(MAKE_DIRECTORY "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/src/dep_glad")
endif()
file(MAKE_DIRECTORY
  "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/src/dep_glad-build"
  "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix"
  "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/tmp"
  "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/src/dep_glad-stamp"
  "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/src"
  "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/src/dep_glad-stamp"
)

set(configSubDirs Debug;Release;MinSizeRel;RelWithDebInfo)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/src/dep_glad-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/OpenGL_Study/learn_openGL/build/dep_glad-prefix/src/dep_glad-stamp${cfgdir}") # cfgdir has leading slash
endif()
