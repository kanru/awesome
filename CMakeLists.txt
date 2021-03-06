cmake_minimum_required(VERSION 2.6.0)
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS  TRUE)

if(COMMAND cmake_policy)
    cmake_policy(VERSION 2.6)
endif()

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})

if(NOT EXISTS ${SOURCE_DIR}/awesomeConfig.cmake)
    message(FATAL_ERROR "Please provide awesomeConfig.cmake")
endif()

include(awesomeConfig.cmake)

include_directories(
    ${SOURCE_DIR}
    ${BUILD_DIR}
    ${BUILD_DIR}/common
    ${AWESOME_COMMON_REQUIRED_INCLUDE_DIRS}
    ${AWESOME_REQUIRED_INCLUDE_DIRS}
    ${AWESOME_OPTIONAL_INCLUDE_DIRS})

file(GLOB_RECURSE AWE_LUA_FILES ${BUILD_DIR}/lib/*.lua)

set(AWE_CONF_FILE_DEFAULT ${BUILD_DIR}/awesomerc.lua)
set(AWE_CONF_FILE rc.lua)

set(AWE_ICON_DIR ${SOURCE_DIR}/icons)
set(AWE_THEMES_DIR ${SOURCE_DIR}/themes)

set(AWE_DOC_FILES
    ${SOURCE_DIR}/AUTHORS
    ${SOURCE_DIR}/BUGS
    ${SOURCE_DIR}/README
    ${SOURCE_DIR}/LICENSE)

set(AWE_SRCS
    ${SOURCE_DIR}/awesome.c
    ${SOURCE_DIR}/client.c
    ${SOURCE_DIR}/cnode.c
    ${SOURCE_DIR}/dbus.c
    ${SOURCE_DIR}/event.c
    ${SOURCE_DIR}/property.c
    ${SOURCE_DIR}/ewmh.c
    ${SOURCE_DIR}/keybinding.c
    ${SOURCE_DIR}/keygrabber.c
    ${SOURCE_DIR}/layout.c
    ${SOURCE_DIR}/luaa.c
    ${SOURCE_DIR}/hooks.c
    ${SOURCE_DIR}/mouse.c
    ${SOURCE_DIR}/screen.c
    ${SOURCE_DIR}/stack.c
    ${SOURCE_DIR}/statusbar.c
    ${SOURCE_DIR}/wibox.c
    ${SOURCE_DIR}/systray.c
    ${SOURCE_DIR}/tag.c
    ${SOURCE_DIR}/titlebar.c
    ${SOURCE_DIR}/widget.c
    ${SOURCE_DIR}/window.c
    ${SOURCE_DIR}/image.c
    ${SOURCE_DIR}/draw.c
    ${SOURCE_DIR}/swindow.c
    ${SOURCE_DIR}/common/buffer.c
    ${SOURCE_DIR}/common/atoms.c
    ${SOURCE_DIR}/common/markup.c
    ${SOURCE_DIR}/common/socket.c
    ${SOURCE_DIR}/common/util.c
    ${SOURCE_DIR}/common/version.c
    ${SOURCE_DIR}/common/xembed.c
    ${SOURCE_DIR}/common/xutil.c
    ${SOURCE_DIR}/layouts/fibonacci.c
    ${SOURCE_DIR}/layouts/floating.c
    ${SOURCE_DIR}/layouts/magnifier.c
    ${SOURCE_DIR}/layouts/fair.c
    ${SOURCE_DIR}/layouts/max.c
    ${SOURCE_DIR}/layouts/tile.c
    ${SOURCE_DIR}/widgets/graph.c
    ${SOURCE_DIR}/widgets/progressbar.c
    ${SOURCE_DIR}/widgets/textbox.c
    ${SOURCE_DIR}/widgets/systray.c
    ${SOURCE_DIR}/widgets/imagebox.c)

set(AWE_CLIENT_SRCS
    ${SOURCE_DIR}/awesome-client.c
    ${SOURCE_DIR}/common/socket.c
    ${SOURCE_DIR}/common/util.c
    ${SOURCE_DIR}/common/version.c)

set(AWE_MAN_SRCS
    ${SOURCE_DIR}/awesome.1.txt
    ${SOURCE_DIR}/awsetbg.1.txt
    ${SOURCE_DIR}/awesome-client.1.txt
    ${SOURCE_DIR}/awesomerc.5.txt)

add_executable(${PROJECT_AWE_NAME}
    ${AWE_SRCS}
    ${BUILD_DIR}/common/tokenize.c)

add_executable(${PROJECT_AWECLIENT_NAME}
    ${AWE_CLIENT_SRCS}
    ${BUILD_DIR}/common/tokenize.c)

set_target_properties(${PROJECT_AWE_NAME}
    PROPERTIES
    LINK_FLAGS -export-dynamic)

target_link_libraries(${PROJECT_AWE_NAME}
    ${AWESOME_COMMON_REQUIRED_LIBRARIES}
    ${AWESOME_REQUIRED_LIBRARIES}
    ${AWESOME_OPTIONAL_LIBRARIES})

target_link_libraries(${PROJECT_AWECLIENT_NAME}
    ${AWESOME_COMMON_REQUIRED_LIBRARIES}
    ${AWESOMECLIENT_LIBRARIES})

# {{{ Generated sources
file(GLOB LAYOUTGEN_DEPS ${SOURCE_DIR}/layouts/*.h)
add_custom_command(
    COMMAND ${SOURCE_DIR}/build-utils/layoutgen.sh
    ARGS    > ${BUILD_DIR}/layoutgen.h
    OUTPUT  ${BUILD_DIR}/layoutgen.h
    WORKING_DIRECTORY ${SOURCE_DIR}
    DEPENDS ${LAYOUTGEN_DEPS}
    COMMENT "Generating layoutgen.h"
    VERBATIM)

add_custom_command(
    COMMAND ${SOURCE_DIR}/build-utils/widgetgen.sh
    ARGS    > ${BUILD_DIR}/widgetgen.h
    OUTPUT  ${BUILD_DIR}/widgetgen.h
    WORKING_DIRECTORY ${SOURCE_DIR}
    DEPENDS ${SOURCE_DIR}/widget.h
    COMMENT "Generating widgetgen.h"
    VERBATIM)

# atoms
file(MAKE_DIRECTORY ${BUILD_DIR}/common)
add_custom_command(
    COMMAND ${SOURCE_DIR}/build-utils/atoms-ext.sh ${SOURCE_DIR}/common/atoms.list
    ARGS    > ${BUILD_DIR}/common/atoms-extern.h
    OUTPUT  ${BUILD_DIR}/common/atoms-extern.h
    WORKING_DIRECTORY ${SOURCE_DIR}
    DEPENDS ${SOURCE_DIR}/common/atoms.list
    COMMENT "Generating atoms-extern.h"
    VERBATIM)

add_custom_command(
    COMMAND ${SOURCE_DIR}/build-utils/atoms-int.sh ${SOURCE_DIR}/common/atoms.list
    ARGS    > ${BUILD_DIR}/common/atoms-intern.h
    OUTPUT  ${BUILD_DIR}/common/atoms-intern.h
    WORKING_DIRECTORY ${SOURCE_DIR}
    DEPENDS ${SOURCE_DIR}/common/atoms.list
    COMMENT "Generating atoms-intern.h"
    VERBATIM)

# gperf
add_custom_command(
    COMMAND ${SOURCE_DIR}/build-utils/gperf.sh
    ARGS    ${SOURCE_DIR}/common/tokenize.gperf
            ${BUILD_DIR}/common/tokenize.h
    OUTPUT  ${BUILD_DIR}/common/tokenize.h
    DEPENDS ${SOURCE_DIR}/common/tokenize.gperf
            ${SOURCE_DIR}/build-utils/gperf.sh
    COMMENT "Generating common/tokenize.h")

add_custom_command(
    COMMAND ${SOURCE_DIR}/build-utils/gperf.sh
    ARGS    ${SOURCE_DIR}/common/tokenize.gperf
            ${BUILD_DIR}/common/tokenize.c
    OUTPUT  ${BUILD_DIR}/common/tokenize.c
    DEPENDS ${SOURCE_DIR}/common/tokenize.gperf
            ${SOURCE_DIR}/build-utils/gperf.sh
    COMMENT "Generating common/tokenize.c")

add_custom_target(generated_sources
    DEPENDS ${BUILD_DIR}/widgetgen.h
            ${BUILD_DIR}/layoutgen.h
            ${BUILD_DIR}/common/atoms-intern.h
            ${BUILD_DIR}/common/atoms-extern.h
            ${BUILD_DIR}/common/tokenize.c
            ${BUILD_DIR}/common/tokenize.h)

# Default theme directory
file(MAKE_DIRECTORY ${BUILD_DIR}/themes/default)
file(MAKE_DIRECTORY ${BUILD_DIR}/themes/sky)
add_dependencies(${PROJECT_AWE_NAME} generated_sources)
add_dependencies(${PROJECT_AWECLIENT_NAME} generated_sources)
# }}}

# {{{ Version stamp
if(BUILD_FROM_GIT)
    add_custom_target(version_stamp ALL
        COMMAND ${SOURCE_DIR}/build-utils/git-version-stamp.sh
                ${VERSION_STAMP_FILE}
                ${BUILD_DIR}/awesome-version-internal.h
        WORKING_DIRECTORY ${SOURCE_DIR})

    add_dependencies(${PROJECT_AWE_NAME} version_stamp)
    add_dependencies(${PROJECT_AWECLIENT_NAME} version_stamp)
endif()
# }}}

# {{{ Manpages
if(GENERATE_MANPAGES)
    foreach(txtfile ${AWE_MAN_SRCS})
        string(REGEX REPLACE ".txt\$" ".xml" xmlfile ${txtfile})
        string(REPLACE ${SOURCE_DIR}
                       ${BUILD_DIR} xmlfile ${xmlfile})
        string(REGEX REPLACE ".xml\$" ".gz" gzfile ${xmlfile})
        string(REGEX REPLACE ".gz\$" "" manfile ${gzfile})

        add_custom_command(
            COMMAND ${CAT_EXECUTABLE} ${txtfile}
                    | ${ASCIIDOC_EXECUTABLE} -d manpage -b docbook -o ${xmlfile} -
            WORKING_DIRECTORY ${BUILD_DIR}
            OUTPUT  ${xmlfile}
            DEPENDS ${txtfile}
            VERBATIM)
        add_custom_command(
            COMMAND ${XMLTO_EXECUTABLE} man ${xmlfile}
            OUTPUT  ${manfile}
            DEPENDS ${xmlfile})
        add_custom_command(
            COMMAND ${CAT_EXECUTABLE} ${manfile}
                    | ${GZIP_EXECUTABLE}
                    > ${gzfile}
            OUTPUT  ${gzfile}
            DEPENDS ${manfile}
            VERBATIM)

        set(MAN_FILES ${MAN_FILES} ${gzfile})

        # figure out in what section to install to from filename
        string(REGEX REPLACE "^.*\\.([0-9])\\.gz\$" "\\1" section ${gzfile})
        set(AWE_MAN${section}_FILES ${AWE_MAN${section}_FILES} ${gzfile})
    endforeach()

    add_custom_target(man ALL DEPENDS ${MAN_FILES})
endif()
# }}}

# {{{ Doxygen
if(DOXYGEN_EXECUTABLE)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} awesome.doxygen
        WORKING_DIRECTORY ${BUILD_DIR})
endif()
# }}}

# {{{ Lua API Documentation
if(GENERATE_LUADOC)

    set(capi_lua ${BUILD_DIR}/lib/capi.lua)

    if(NOT BUILD_DIR STREQUAL SOURCE_DIR)
        file(MAKE_DIRECTORY ${BUILD_DIR}/lib)
    endif()

    # {{{ setup a command for ${capi_lua}
    macro(a_file_match infile regex result_var)
        file(STRINGS ${infile} match REGEX ${regex} LIMIT_COUNT 1)
        if(match)
            set(${result_var} TRUE)
        else()
            set(${result_var} FALSE)
        endif()
    endmacro()

    set(luadoc_c_srcs "")

    # find .c files exporting lua functions
    foreach(cfile ${AWE_SRCS})
        a_file_match(${cfile} "const struct luaL_reg" result)
        if(result)
            set(luadoc_c_srcs ${luadoc_c_srcs} ${cfile})
        else()
            a_file_match(${cfile} "luaA_.*_index" result)
            if(result)
                set(luadoc_c_srcs ${luadoc_c_srcs} ${cfile})
        endif()
        endif()
    endforeach()

    # create fake lua source file to process by luadoc
    add_custom_command(
        COMMAND ${CAT_EXECUTABLE} ${luadoc_c_srcs}
                | ${LUA_EXECUTABLE} ${SOURCE_DIR}/build-utils/fake-lua-src.lua
                > ${capi_lua}
        OUTPUT  ${capi_lua}
        DEPENDS ${luadoc_c_srcs}
        COMMENT "Generating fake lua source."
        VERBATIM)
    # }}}

    # Generate documentation of lib//0lua
    file(GLOB_RECURSE lua_lib_files ${BUILD_DIR}/lib/*.lua)
    foreach(filename ${lua_lib_files})
        file(RELATIVE_PATH filename ${BUILD_DIR}/lib ${filename})
        set(luadoc_srcs ${luadoc_srcs} ${filename})
    endforeach()

    add_custom_target(luadoc ALL
        COMMAND ${LUADOC_EXECUTABLE} ${luadoc_srcs} -d ${BUILD_DIR}/luadoc
        WORKING_DIRECTORY ${BUILD_DIR}/lib
        DEPENDS ${AWE_LUA_FILES} ${capi_lua})
endif()
# }}}

# {{{ Dist tarball
if(BUILD_FROM_GIT)
    add_custom_target(dist
        COMMAND ${SOURCE_DIR}/build-utils/dist.sh ${VERSION}
        WORKING_DIRECTORY ${SOURCE_DIR})
endif()
# }}}

# {{{ Installation
install(TARGETS ${PROJECT_AWE_NAME} ${PROJECT_AWECLIENT_NAME} RUNTIME DESTINATION bin)
install(FILES "utils/awsetbg" DESTINATION bin PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(DIRECTORY ${BUILD_DIR}/lib DESTINATION ${AWESOME_DATA_PATH})
install(FILES ${AWE_CONF_FILE_DEFAULT} DESTINATION ${AWESOME_SYSCONFDIR}
        RENAME ${AWE_CONF_FILE})
if(GENERATE_MANPAGES)
    install(FILES ${AWE_MAN1_FILES} DESTINATION ${AWESOME_MAN_PATH}/man1)
    install(FILES ${AWE_MAN5_FILES} DESTINATION ${AWESOME_MAN_PATH}/man5)
endif()
install(DIRECTORY ${AWE_ICON_DIR} DESTINATION ${AWESOME_DATA_PATH})
install(DIRECTORY ${BUILD_DIR}/themes DESTINATION ${AWESOME_DATA_PATH})
install(DIRECTORY ${SOURCE_DIR}/themes DESTINATION ${AWESOME_DATA_PATH}
    PATTERN "*.in" EXCLUDE)
install(FILES ${AWE_DOC_FILES} DESTINATION ${AWESOME_DOC_PATH})
install(FILES "awesome.desktop" DESTINATION ${AWESOME_XSESSION_PATH})
if(GENERATE_LUADOC)
    install(DIRECTORY ${BUILD_DIR}/luadoc DESTINATION ${AWESOME_DOC_PATH})
endif()
# }}}

# vim: filetype=cmake:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:encoding=utf-8:textwidth=80
