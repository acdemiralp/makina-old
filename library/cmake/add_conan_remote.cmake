function   (add_conan_remote NAME ADDRESS USERNAME API_KEY)
  execute_process(COMMAND conan remote add ${NAME} ${ADDRESS})
  execute_process(COMMAND conan user -p ${API_KEY} -r ${NAME} ${USERNAME})
endfunction(add_conan_remote)
