SET(work_dir "${CMAKE_CURRENT_BINARY_DIR}/font-error-roundtrip")
SET(blend_path "${work_dir}/font-text.blend")
SET(image_path "${work_dir}/font-text.png")
FILE(MAKE_DIRECTORY "${work_dir}")
FILE(REMOVE "${blend_path}" "${image_path}")
EXECUTE_PROCESS(
  COMMAND "${CMAKE_COMMAND}" -E env "FONT_ERROR_MODE=create" "FONT_ERROR_BLEND=${blend_path}" "FONT_ERROR_SOURCE=${SOURCE_ROOT}"
    "${BLENDER_EXECUTABLE}" -b "${SOURCE_ROOT}/release/datafiles/preview.blend"
    -P "${SOURCE_ROOT}/tools/macos/tests/font-error-roundtrip.py"
  RESULT_VARIABLE create_status OUTPUT_VARIABLE create_output ERROR_VARIABLE create_errors)
IF(NOT create_status EQUAL 0 OR NOT create_output MATCHES "FONT_ERROR_CREATE_PASS")
  MESSAGE(FATAL_ERROR "Text scene creation failed (${create_status}):\n${create_output}\n${create_errors}")
ENDIF()
EXECUTE_PROCESS(
  COMMAND "${CMAKE_COMMAND}" -E env "FONT_ERROR_MODE=verify" "FONT_ERROR_BLEND=${blend_path}"
    "${BLENDER_EXECUTABLE}" -b "${blend_path}" -P "${SOURCE_ROOT}/tools/macos/tests/font-error-roundtrip.py"
  RESULT_VARIABLE verify_status OUTPUT_VARIABLE verify_output ERROR_VARIABLE verify_errors)
IF(NOT verify_status EQUAL 0 OR NOT verify_output MATCHES "FONT_ERROR_REOPEN_PASS")
  MESSAGE(FATAL_ERROR "Text scene reopen failed (${verify_status}):\n${verify_output}\n${verify_errors}")
ENDIF()
EXECUTE_PROCESS(
  COMMAND "${BLENDER_EXECUTABLE}" -b "${blend_path}" -o "${work_dir}/render" -F PNG -f 1
  RESULT_VARIABLE render_status OUTPUT_VARIABLE render_output ERROR_VARIABLE render_errors)
IF(NOT render_status EQUAL 0 OR NOT EXISTS "${work_dir}/render0001.png")
  MESSAGE(FATAL_ERROR "Text render failed (${render_status}):\n${render_output}\n${render_errors}")
ENDIF()
EXECUTE_PROCESS(
  COMMAND "${CMAKE_COMMAND}" -E compare_files "${work_dir}/render0001.png"
    "${SOURCE_ROOT}/tools/macos/tests/data/font-error-baseline.png"
  RESULT_VARIABLE compare_status)
IF(NOT compare_status EQUAL 0)
  MESSAGE(FATAL_ERROR "Rendered valid-font text differs from the frozen native baseline")
ENDIF()
MESSAGE(STATUS "${create_output}${verify_output}${render_output}")
