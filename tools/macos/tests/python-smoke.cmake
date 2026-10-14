EXECUTE_PROCESS(
  COMMAND "${BLENDER_EXECUTABLE}" -b "${SOURCE_ROOT}/release/datafiles/preview.blend" -P "${SOURCE_ROOT}/tools/macos/tests/python-smoke.py"
  RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE errors)
STRING(FIND "${output}" "NATIVE_PYTHON_PASS:" marker)
IF(NOT status EQUAL 0 OR marker EQUAL -1)
  MESSAGE(FATAL_ERROR "Legacy Python regression failed (${status}):\n${output}\n${errors}")
ENDIF()
MESSAGE(STATUS "${output}")
