FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/main/*.*)

set(EXTRA_COMPONENT_DIRS "../../components")

idf_component_register(SRCS ${app_sources}
                    INCLUDE_DIRS ""
                    PRIV_REQUIRES nvs_flash mqtt
                    REQUIRES "esp-cryptoauthlib"
                    )
 