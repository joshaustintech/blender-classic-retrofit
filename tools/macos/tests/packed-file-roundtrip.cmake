SET(work_dir "${CMAKE_CURRENT_BINARY_DIR}/packed-file-roundtrip")
SET(image_path "${work_dir}/splash.jpg")
SET(blend_path "${work_dir}/packed-image.blend")
FILE(MAKE_DIRECTORY "${work_dir}")
FILE(COPY "${SOURCE_ROOT}/release/datafiles/splash.jpg" DESTINATION "${work_dir}")

EXECUTE_PROCESS(
  COMMAND "${CMAKE_COMMAND}" -E env
    PACKED_FILE_ROUNDTRIP_MODE=create
    PACKED_FILE_ROUNDTRIP_IMAGE=${image_path}
    PACKED_FILE_ROUNDTRIP_BLEND=${blend_path}
    "${BLENDER_EXECUTABLE}" -b "${SOURCE_ROOT}/release/datafiles/preview.blend"
    -P "${SOURCE_ROOT}/tools/macos/tests/packed-file-roundtrip.py"
  RESULT_VARIABLE create_status OUTPUT_VARIABLE create_output ERROR_VARIABLE create_errors)
STRING(FIND "${create_output}" "PACKED_FILE_CREATE_PASS" create_marker)
IF(NOT create_status EQUAL 0 OR create_marker EQUAL -1)
  MESSAGE(FATAL_ERROR "Packed image creation failed (${create_status}):\n${create_output}\n${create_errors}")
ENDIF()

EXECUTE_PROCESS(
  COMMAND "${CMAKE_COMMAND}" -E env
    PACKED_FILE_ROUNDTRIP_MODE=verify
    PACKED_FILE_ROUNDTRIP_IMAGE=${image_path}
    PACKED_FILE_ROUNDTRIP_BLEND=${blend_path}
    "${BLENDER_EXECUTABLE}" -b "${blend_path}" -P "${SOURCE_ROOT}/tools/macos/tests/packed-file-roundtrip.py"
  RESULT_VARIABLE reopen_status OUTPUT_VARIABLE reopen_output ERROR_VARIABLE reopen_errors)
STRING(FIND "${reopen_output}" "PACKED_FILE_REOPEN_PASS" reopen_marker)
IF(NOT reopen_status EQUAL 0 OR reopen_marker EQUAL -1)
  MESSAGE(FATAL_ERROR "Packed image reopen failed (${reopen_status}):\n${reopen_output}\n${reopen_errors}")
ENDIF()
MESSAGE(STATUS "${create_output}${reopen_output}")
