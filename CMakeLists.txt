cmake_minimum_required(VERSION 3.0)

include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

project(flatbuffers_example VERSION 1.0.0)

include("${config_file_path}")

# Executable
add_executable(flatbuffers_example
  ./src/main.cpp
  )

# Install Include
install(
  DIRECTORY "./include"
  DESTINATION ${config_relative_output_dir}/../..
  FILES_MATCHING PATTERN "*"
  )

# Install Targets (Binaries)
install(TARGETS
  flatbuffers_example
  EXPORT flatbuffers_example_targets
  LIBRARY DESTINATION ${config_relative_output_dir}/bin
  RUNTIME DESTINATION ${config_relative_output_dir}/bin
  ARCHIVE DESTINATION ${config_relative_output_dir}/lib
  INCLUDES DESTINATION ${config_relative_output_dir}/../../include
  )

# Install CMAKE Configuration Files
install(
  EXPORT flatbuffers_example_targets
  NAMESPACE example
  DESTINATION "${config_relative_output_dir}/cmake"
  )
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-configversion.cmake" COMPATIBILITY SameMajorVersion
  )
configure_package_config_file(
  "./build/config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake"
  INSTALL_DESTINATION "build/cmake"
  )
install(
  DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cmake"
  DESTINATION "build"
  FILES_MATCHING PATTERN "*"
  )
