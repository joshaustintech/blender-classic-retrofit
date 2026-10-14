IF(NOT TEST_SCRIPT)
  SET(TEST_SCRIPT "${SOURCE_ROOT}/tools/macos/tests/python-smoke.py")
ENDIF()
IF(NOT TEST_MARKER)
  SET(TEST_MARKER "NATIVE_PYTHON_PASS:")
ENDIF()
EXECUTE_PROCESS(
  COMMAND "${BLENDER_EXECUTABLE}" -b "${SOURCE_ROOT}/release/datafiles/preview.blend" -P "${TEST_SCRIPT}"
  RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE errors)
STRING(FIND "${output}" "${TEST_MARKER}" marker)
IF(NOT status EQUAL 0 OR marker EQUAL -1)
  MESSAGE(FATAL_ERROR "Legacy Python regression failed (${status}):\n${output}\n${errors}")
ENDIF()
MESSAGE(STATUS "${output}")
