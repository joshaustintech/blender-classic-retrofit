SET(work_dir "${CMAKE_CURRENT_BINARY_DIR}/python-traceback-lifetime")
FILE(MAKE_DIRECTORY "${work_dir}")
FOREACH(project IN ITEMS first second third)
  CONFIGURE_FILE("${SOURCE_ROOT}/release/datafiles/preview.blend"
    "${work_dir}/${project}.blend" COPYONLY)
ENDFOREACH()
EXECUTE_PROCESS(
  COMMAND "${BLENDER_EXECUTABLE}" -b "${work_dir}/first.blend"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-error.py"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-check.py"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-valid.py"
    "${work_dir}/second.blend"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-error.py"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-check.py"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-valid.py"
    "${work_dir}/third.blend"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-error.py"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-check.py"
    -P "${SOURCE_ROOT}/tools/macos/tests/python-traceback-valid.py"
  RESULT_VARIABLE status OUTPUT_VARIABLE output ERROR_VARIABLE errors)
FOREACH(marker IN ITEMS TICKET04_WRAPPED_OBJECT TICKET04_TRACEBACK_CLEARED
    TICKET04_EXCEPTION_STATE_PRESERVED TICKET04_CURRENT_OBJECT_VALID
    TICKET04_VALID_SCRIPT TICKET04_EXPECTED_SCRIPT_ERROR
    "Traceback \\(most recent call last\\)"
    "${SOURCE_ROOT}/tools/macos/tests/python-traceback-error.py"
    "line 10, in <module>")
  STRING(REGEX MATCHALL "${marker}" matches "${output}${errors}")
  LIST(LENGTH matches count)
  IF(NOT count EQUAL 3)
    MESSAGE(FATAL_ERROR "Expected ${marker} 3 times; found ${count}.\n${output}\n${errors}")
  ENDIF()
ENDFOREACH()
IF(NOT status EQUAL 0)
  MESSAGE(FATAL_ERROR "Python traceback reload regression failed (${status}):\n${output}\n${errors}")
ENDIF()
MESSAGE(STATUS "Repeated Python error/reload/error cycles preserved valid scripts and exception diagnostics")
