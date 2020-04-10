SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.7)

# specify the cross compiler
SET(TOOLCHAIN_PATH /opt/arm/gcc-arm-none-eabi-7-2017-q4-major/bin)
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_CXX_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/arm-none-eabi-g++)
set(AS ${TOOLCHAIN_PATH}/arm-none-eabi-as)
set(AR ${TOOLCHAIN_PATH}/arm-none-eabi-ar)
set(OBJCOPY ${TOOLCHAIN_PATH}/arm-none-eabi-objcopy)
set(OBJDUMP ${TOOLCHAIN_PATH}/arm-none-eabi-objdump)
set(SIZE ${TOOLCHAIN_PATH}/arm-none-eabi-size)

project(miniblink)

add_definitions(-DSTM32F1)

set(LIBOPENCM3_DIR ${CMAKE_SOURCE_DIR}/libopencm3)
add_custom_target(libopencm3 make WORKING_DIRECTORY ${LIBOPENCM3_DIR})
link_directories(${LIBOPENCM3_DIR}/lib)

file(GLOB_RECURSE USER_SOURCES Core/*.c)

include_directories(
        ${LIBOPENCM3_DIR}/include
        inc)

set(STM32F1_FLAGS "-mcpu=cortex-m3 -mthumb -mthumb-interwork -msoft-float")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall ${STM32F1_FLAGS} -std=c99 -Os -ffunction-sections -fdata-sections ")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall --std=c++14 ${STM32F1_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/Core/libopencm3.ld -nostartfiles -lopencm3_stm32f1 -lc -lnosys --specs=rdimon.specs -Wl,--gc-sections")

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_executable(${PROJECT_NAME}.elf ${USER_SOURCES})
target_link_libraries(${PROJECT_NAME}.elf opencm3_stm32f1)

add_custom_command(TARGET ${PROJECT_NAME}.elf PRE_BUILD
        COMMAND make -j1 WORKING_DIRECTORY ${LIBOPENCM3_DIR}
        COMMENT "Building libopencm3")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMENT "Invoking: Cross ARM GNU Print Size"
        COMMAND ${SIZE} ${PROJECT_NAME}.elf)