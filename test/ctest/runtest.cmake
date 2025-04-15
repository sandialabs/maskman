macro(EXEC_CHECK TST CMD CCSD CCBD)
  file(READ "${CCSD}/../test/ctest/params/${TST}.params" PARAMS)
  execute_process(COMMAND "${CMD}" ${PARAMS}
                  RESULT_VARIABLE CMD_RESULT
                  OUTPUT_FILE "${CCBD}/${TST}/${TST}.stdout"
                  ERROR_FILE "${CCBD}/${TST}/${TST}.stderr"
                  WORKING_DIRECTORY "${CCBD}/${TST}"
                  )
  if(CMD_RESULT)
    message(FATAL_ERROR "Error running \"${CMD}\"")
  endif()
  execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files "${CCBD}/${TST}/${TST}.stdout" "${CCSD}/../test/ctest/gold/${TST}.gold"
                  RESULT_VARIABLE DIFF_RESULT
                  OUTPUT_FILE "${CCBD}/${TST}/diff.stdout"
                  ERROR_FILE "${CCBD}/${TST}/diff.stderr"
                  WORKING_DIRECTORY "${CCBD}/${TST}"
                  )
  if(DIFF_RESULT)
    message(FATAL_ERROR "Error running compare_files")
  endif()
endmacro()
exec_check("${TST}" "${CMD}" "${CCSD}" "${CCBD}")