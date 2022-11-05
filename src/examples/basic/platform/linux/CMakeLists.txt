cmake_minimum_required (VERSION 3.20)
project (qts_x86_linux_test VERSION 1.0)

#
# Main
#
SET (MAIN_SRC_DIR ".")

#
# QuarkTS 
#
SET (QUARKTS_SRC_DIR "../../../../../src/os")
SET (QUARKTS_INC_DIR "../../../../../src/os/include")
SET (QUARKTS_CONFIG_DIR "../../../../../src/config")
FILE (GLOB_RECURSE QUARKTS_SOURCES ${QUARKTS_SRC_DIR}/*.c)

#
# HSMs
#
SET (HSMS_SRC_DIR "../../hsms/src")
SET (HSMS_INC_DIR "../../hsms/inc")
SET (HSMS_SYSTEM_SIGNALS_INC_DIR "../../hsms")
FILE (GLOB_RECURSE HSMS_SOURCES ${HSMS_SRC_DIR}/*.c)

# Main
add_executable (main ${MAIN_SRC_DIR}/main.c)

# QuarkTS
add_library (quarkts STATIC ${QUARKTS_SOURCES})
include_directories (${QUARKTS_INC_DIR} ${QUARKTS_CONFIG_DIR})

# HSMs
add_library (hsms STATIC ${HSMS_SOURCES})
include_directories (${HSMS_INC_DIR} ${HSMS_SYSTEM_SIGNALS_INC_DIR})

target_link_libraries (main quarkts hsms)