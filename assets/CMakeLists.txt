# Generated an hpp file that provides the directory to the asset files
set(ASSET_INCLUDE "${GENERATED_DIR}/Asset.hpp")
configure_file("${ASSET_DIR}/Asset.hpp" ${ASSET_INCLUDE})

set(ASSET_FILES ${ASSET_INCLUDE} PARENT_SCOPE)
