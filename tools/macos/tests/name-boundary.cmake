IF(NOT BLENDER_EXECUTABLE OR NOT SOURCE_ROOT)
  MESSAGE(FATAL_ERROR "BLENDER_EXECUTABLE and SOURCE_ROOT are required")
ENDIF()
SET(name_file "${CMAKE_CURRENT_BINARY_DIR}/name-boundary.blend")
FILE(REMOVE "${name_file}")
EXECUTE_PROCESS(
  COMMAND "${CMAKE_COMMAND}" -E env "BLENDER_NAME_BOUNDARY_FILE=${name_file}"
    "${BLENDER_EXECUTABLE}" -b "${SOURCE_ROOT}/release/datafiles/preview.blend"
    -P "${SOURCE_ROOT}/tools/macos/tests/name-boundary.py"
  RESULT_VARIABLE create_status OUTPUT_VARIABLE create_output ERROR_VARIABLE create_errors)
IF(NOT create_status EQUAL 0 OR NOT create_output MATCHES "NATIVE_NAME_BOUNDARY_PASS: created names")
  MESSAGE(FATAL_ERROR "Legacy name creation failed (${create_status}):\n${create_output}\n${create_errors}")
ENDIF()
EXECUTE_PROCESS(
  COMMAND "${CMAKE_COMMAND}" -E env "BLENDER_NAME_BOUNDARY_FILE=${name_file}" "BLENDER_NAME_BOUNDARY_VERIFY=1"
    "${BLENDER_EXECUTABLE}" -b "${name_file}"
    -P "${SOURCE_ROOT}/tools/macos/tests/name-boundary.py"
  RESULT_VARIABLE verify_status OUTPUT_VARIABLE verify_output ERROR_VARIABLE verify_errors)
FILE(REMOVE "${name_file}")
IF(NOT verify_status EQUAL 0 OR NOT verify_output MATCHES "NATIVE_NAME_BOUNDARY_PASS: reopened names")
  MESSAGE(FATAL_ERROR "Legacy name reopen failed (${verify_status}):\n${verify_output}\n${verify_errors}")
ENDIF()
MESSAGE(STATUS "${create_output}${verify_output}")
