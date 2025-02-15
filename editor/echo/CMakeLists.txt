# configure
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
ADD_DEFINITIONS(-DQT_STATICPLUGIN)

# Policy
CMAKE_POLICY(SET CMP0020 NEW)

INCLUDE_DIRECTORIES(${ECHO_ROOT_PATH})
INCLUDE_DIRECTORIES(${ECHO_ROOT_PATH}/thirdparty)
INCLUDE_DIRECTORIES(${ECHO_ROOT_PATH}/thirdparty/nodeeditor)
INCLUDE_DIRECTORIES(${ECHO_ROOT_PATH}/editor/QLibrary/Resources/QLibrary)
INCLUDE_DIRECTORIES(${ECHO_ROOT_PATH}/editor/QLibrary/Resources/QLibrary/propertyeditor)
INCLUDE_DIRECTORIES(${ECHO_ROOT_PATH}/ThirdParty/adrenoSDK/inc)
INCLUDE_DIRECTORIES(${ECHO_ROOT_PATH}/engine)
INCLUDE_DIRECTORIES(${ECHO_ROOT_PATH}/engine/core/render)

LINK_DIRECTORIES(${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

IF(${ECHO_PLATFORM_WINDOWS})
	IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
		LINK_DIRECTORIES(${ECHO_ROOT_PATH}/thirdparty/opengles/angle/lib/win32)
		LINK_DIRECTORIES(${ECHO_ROOT_PATH}/thirdparty/live2d/Cubism31SdkNative-EAP5/Core/lib/windows/x86)
	ELSEIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
		LINK_DIRECTORIES(${ECHO_ROOT_PATH}/thirdparty/opengles/angle/lib/win64)
		LINK_DIRECTORIES(${ECHO_ROOT_PATH}/thirdparty/live2d/Cubism31SdkNative-EAP5/Core/lib/windows/x86_64)
		LINK_DIRECTORIES(${ECHO_ROOT_PATH}/thirdparty/opencl/windows/lib/x64)
	ENDIF()

    # VS  http://www.oschina.net/code/explore/mariadb-5.2.4/CMakeLists.txt
	IF(MSVC)
		SET(CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG} /wd4996 /wd4819 /MP /openmp")
		SET(CMAKE_CXX_FLAGS_RELEASE			"${CMAKE_CXX_FLAGS_RELEASE} /wd4996 /wd4819 /MP /openmp")
		SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /wd4996 /wd4819  /MP /openmp")
		SET(CMAKE_C_FLAGS_DEBUG				"${CMAKE_C_FLAGS_DEBUG} /wd4996 /wd4819  /MP /openmp")
		SET(CMAKE_C_FLAGS_RELEASE			"${CMAKE_C_FLAGS_RELEASE} /wd4996 /wd4819  /MP /openmp")
		SET(CMAKE_C_FLAGS_RELWITHDEBINFO	"${CMAKE_C_FLAGS_RELWITHDEBINFO} /wd4996 /wd4819  /MP /openmp")
	ENDIF(MSVC)
ENDIF()

# set module name
SET(MODULE_NAME echo)

# set path
SET(MODULE_HEADER_PATH   ${ECHO_ROOT_PATH}/editor/echo)
SET(MODULE_SOURCE_PATH   ${ECHO_ROOT_PATH}/editor/echo)
SET(MODULE_RESOURCE_PATH ${ECHO_ROOT_PATH}/editor/echo/Resource)
SET(QLIBRARY_PATH        ${ECHO_ROOT_PATH}/editor/QLibrary)

IF(${ECHO_PLATFORM_MAC})
	FILE(GLOB_RECURSE MAC_SOURCE_FILES	${MODULE_SOURCE_PATH}/*.mm)

    # resource files
    SET(MAC_RESOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/Resource/mac/App.icns
        ${ECHO_BIN_PATH}/qt.conf
		${ECHO_BIN_PATH}/qt
    )
    SET_SOURCE_FILES_PROPERTIES(${MAC_RESOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    SOURCE_GROUP("Resources\\MacOSX" FILES ${MAC_RESOURCE_FILES})

    # source files
    SET(MAC_ECHO_SOURCE_FILES
        ${ECHO_ROOT_PATH}/app
        ${ECHO_ROOT_PATH}/engine
        ${ECHO_ROOT_PATH}/thirdparty
        ${ECHO_ROOT_PATH}/tool
        ${ECHO_ROOT_PATH}/CMakeLists.txt
    )
    SET_SOURCE_FILES_PROPERTIES(${MAC_ECHO_SOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "echo")
    SOURCE_GROUP("Resources\\MacOSX\\echo" FILES ${MAC_ECHO_SOURCE_FILES})
    
    # editor module files
    SET(MAC_ECHO_EDITOR_MODULE_FILES
        ${ECHO_ROOT_PATH}/editor/echo
    )
    SET_SOURCE_FILES_PROPERTIES(${MAC_ECHO_EDITOR_MODULE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "echo/editor")
    SOURCE_GROUP("Resources\\MacOSX\\echo\\editor" FILES ${MAC_ECHO_EDITOR_MODULE_FILES})

	# bin files
	SET(MAC_ECHO_BIN_FILES
        ${ECHO_ROOT_PATH}/bin/app
    )
    SET_SOURCE_FILES_PROPERTIES(${MAC_ECHO_BIN_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION "echo/bin")
    SOURCE_GROUP("Resources\\MacOSX\\echo\\bin" FILES ${MAC_ECHO_BIN_FILES})
    
    SET(MAC_FILES ${MAC_SOURCE_FILES} ${MAC_RESOURCE_FILES} ${MAC_ECHO_BIN_FILES} ${MAC_ECHO_SOURCE_FILES} ${MAC_ECHO_EDITOR_MODULE_FILES})

ELSE()
	SET(MAC_SOURCE_FILES)
ENDIF()

# Find Qt Packages
TARGET_FIND_QTPACKAGES(${MODULE_NAME})

# recursive get files
FILE(GLOB_RECURSE HEADER_FILES		${MODULE_HEADER_PATH}/*.h ${MODULE_HEADER_PATH}/*.inl)
FILE(GLOB_RECURSE SOURCE_FILES		${MODULE_SOURCE_PATH}/*.c ${MODULE_SOURCE_PATH}/*.cpp ${MODULE_SOURCE_PATH}/*.cx ${MAC_SOURCE_FILES})
FILE(GLOB_RECURSE UI_FILES			${MODULE_SOURCE_PATH}/*.ui)
FILE(GLOB_RECURSE QT_HEADER_FILES	${MODULE_HEADER_PATH}/Game/UI/*.h ${MODULE_HEADER_PATH}/Editor/UI/*.h)
FILE(GLOB_RECURSE QL_HEADER_FILES	${QLIBRARY_PATH}/*.h ${QLIBRARY_PATH}/*.hpp)
FILE(GLOB_RECURSE QL_SOURCE_FILES   ${QLIBRARY_PATH}/*.cpp)

GROUP_FILES(HEADER_FILES	${MODULE_HEADER_PATH})
GROUP_FILES(SOURCE_FILES	${MODULE_SOURCE_PATH})
GROUP_FILES(QL_HEADER_FILES ${QLIBRARY_PATH})
GROUP_FILES(QL_SOURCE_FILES ${QLIBRARY_PATH})
GROUP_QT_UI_FILES(QT_GENERATE_FILES UI_FILES ${MODULE_SOURCE_PATH})

GROUP_QT_MOC_FILES(QT_GENERATE_FILES QT_HEADER_FILES)
GROUP_QT_MOC_FILES(QT_GENERATE_FILES QL_HEADER_FILES)

# recursive get qt files
FILE(GLOB_RECURSE RESOURCE_QT_FILES	${MODULE_SOURCE_PATH}/*.qrc)
FILE(GLOB_RECURSE RESOURCE_FILES	${MODULE_SOURCE_PATH}/*.rc ${MODULE_SOURCE_PATH}/*.qml ${MODULE_SOURCE_PATH}/*.qss)

QT5_ADD_RESOURCES(MODULE_QT_RCC_SRCS ${RESOURCE_QT_FILES})
SOURCE_GROUP("Resources" FILES ${MODULE_QT_RCC_SRCS} ${RESOURCE_QT_FILES} ${RESOURCE_FILES})

# add macro
IF(ECHO_GAME_SOURCE)
	ADD_DEFINITIONS(-DECHO_GAME_SOURCE)
ENDIF()


# add executable
ADD_EXECUTABLE(${MODULE_NAME} MACOSX_BUNDLE
	${HEADER_FILES}
	${SOURCE_FILES}
	${QT_HEADER_FILES}
	${QL_HEADER_FILES}
	${QL_SOURCE_FILES}
	${UI_FILES}
	${RESOURCE_FILES}
	${RESOURCE_QT_FILES}
	${MODULE_QT_RCC_SRCS}
	${QT_GENERATE_FILES}
    ${MAC_FILES}
	CMakeLists.txt)

# link libraries
TARGET_LINK_LIBRARIES(${MODULE_NAME} zlib engine lua recast physx)
TARGET_LINK_LIBRARIES(${MODULE_NAME} pugixml spine box2d)
TARGET_LINK_LIBRARIES(${MODULE_NAME} freetype)
TARGET_LINK_LIBRARIES(${MODULE_NAME} radeonrays)
TARGET_LINK_LIBRARIES(${MODULE_NAME} glslang spirv-cross)
TARGET_LINK_LIBRARIES(${MODULE_NAME} icns openfbx libpng)
TARGET_LINK_LIBRARIES(${MODULE_NAME} tensorflow-lite)
TARGET_LINK_LIBRARIES(${MODULE_NAME} nodeeditor)

# link game library
IF(ECHO_GAME_SOURCE)
	MESSAGE(STATUS "Editor link game library : " ${ECHO_GAME_NAME})
	TARGET_LINK_LIBRARIES(${MODULE_NAME} ${ECHO_GAME_NAME})
ENDIF()

# line qt
TARGET_LINK_QTLIBRARIES(${MODULE_NAME})

IF(${ECHO_PLATFORM_WINDOWS})
	TARGET_LINK_LIBRARIES(${MODULE_NAME} winmm.lib imm32.lib dxgi.lib Shlwapi.lib CfgMgr32.lib)
	TARGET_LINK_LIBRARIES(${MODULE_NAME} libEGL.lib libGLESv2.lib)
	TARGET_LINK_LIBRARIES(${MODULE_NAME} dbghelp.lib Psapi.lib)
	TARGET_LINK_LIBRARIES(${MODULE_NAME} openal-soft)
    TARGET_LINK_LIBRARIES(${MODULE_NAME} OpenCL)
	TARGET_LINK_LIBRARIES(${MODULE_NAME} vulkan-loader)
    TARGET_LINK_LIBRARIES(${MODULE_NAME} jplayer)
    TARGET_LINK_LIBRARIES(${MODULE_NAME} Live2DCubismCore)

	# Copy dlls &{CMAKE_BUILD_TYPE} is empty!!!
	ADD_CUSTOM_COMMAND(
		TARGET ${MODULE_NAME} PRE_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy 
		${ECHO_ROOT_PATH}/thirdparty/opengles/angle/lib/win64/libEGL.dll
		${ECHO_ROOT_PATH}/bin/editor//${PLATFORM}/Debug/libEGL.dll)

	ADD_CUSTOM_COMMAND(
		TARGET ${MODULE_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy 
		${ECHO_ROOT_PATH}/thirdparty/opengles/angle/lib/win64/libGLESv2.dll
		${ECHO_ROOT_PATH}/bin/editor/${PLATFORM}/Debug/libGLESv2.dll)

	ADD_CUSTOM_COMMAND(
		TARGET ${MODULE_NAME} PRE_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy 
		${ECHO_ROOT_PATH}/thirdparty/opengles/angle/lib/win64/libEGL.dll
		${ECHO_ROOT_PATH}/bin/editor//${PLATFORM}/Release/libEGL.dll)

	ADD_CUSTOM_COMMAND(
		TARGET ${MODULE_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy 
		${ECHO_ROOT_PATH}/thirdparty/opengles/angle/lib/win64/libGLESv2.dll
		${ECHO_ROOT_PATH}/bin/editor/${PLATFORM}/Release/libGLESv2.dll)

ELSE()
	# AddFrameWork Macro
	MACRO(ADD_FRAMEWORK fwname)
		SET(FRAMEWORKS "${FRAMEWORKS} -framework ${fwname}")
	ENDMACRO(ADD_FRAMEWORK)

	ADD_FRAMEWORK(AppKit)
    ADD_FRAMEWORK(QuartzCore)
    ADD_FRAMEWORK(Metal)
    ADD_FRAMEWORK(MetalKit)
    ADD_FRAMEWORK(OpenAL)
    ADD_FRAMEWORK(OpenCL)

	SET(FRAMEWORKS "${FRAMEWORKS} -ObjC")
	SET(CMAKE_EXE_LINKER_FLAGS ${FRAMEWORKS})
    
    TARGET_LINK_LIBRARIES(${MODULE_NAME} ${ECHO_ROOT_PATH}/thirdparty/live2d/Cubism31SdkNative-EAP5/Core/lib/macos/libLive2DCubismCore.a)
ENDIF()

# set folder
SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES FOLDER "editor")

# set name
SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES OUTPUT_NAME "Echo")

IF(MSVC)
	# set target properties
	SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /entry:mainCRTStartup")
	#SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES LINK_FLAGS "/level='requireAdministrator' /uiAccess='false' ")

	SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINER_FLAGS} /SAFESEH:NO")
	SET(CMAKE_EXE_LINKER_FLAGS_VERSION "${CMAKE_EXE_LINER_FLAGS} /SAFESEH:NO")
ELSE()
    # https://help.apple.com/xcode/mac/current/#/itcaec37c2a6
	SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++14")
	SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY "1,2")
    SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES XCODE_ATTRIBUTE_COPY_PHASE_STRIP No)
    SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--deep")
    SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Resource/mac/Info.plist)
    SET_TARGET_PROPERTIES(${MODULE_NAME} PROPERTIES XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path/../Resources/qt/lib")
ENDIF()

# set dependencies
ADD_DEPENDENCIES(${MODULE_NAME} engine)
