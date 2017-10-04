# Generated an hpp file that provides the directory to the shader files
set(SHADER_INCLUDE "${GENERATED_DIR}/Shader.hpp")
configure_file("${SHADER_DIR}/Shader.hpp" ${SHADER_INCLUDE})

set(SHADER_FILES ${SHADER_INCLUDE} PARENT_SCOPE)
