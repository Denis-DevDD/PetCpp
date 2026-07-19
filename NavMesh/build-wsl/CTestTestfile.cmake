# CMake generated Testfile for 
# Source directory: /mnt/c/Users/denis/Desktop/PetCpp/NavMesh
# Build directory: /mnt/c/Users/denis/Desktop/PetCpp/NavMesh/build-wsl
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[tests]=] "/mnt/c/Users/denis/Desktop/PetCpp/NavMesh/build-wsl/tests")
set_tests_properties([=[tests]=] PROPERTIES  WORKING_DIRECTORY "/mnt/c/Users/denis/Desktop/PetCpp/NavMesh" _BACKTRACE_TRIPLES "/mnt/c/Users/denis/Desktop/PetCpp/NavMesh/CMakeLists.txt;100;add_test;/mnt/c/Users/denis/Desktop/PetCpp/NavMesh/CMakeLists.txt;0;")
subdirs("_deps/json-build")
subdirs("_deps/doctest-build")
subdirs("_deps/raylib-build")
subdirs("_deps/clipper2-build")
