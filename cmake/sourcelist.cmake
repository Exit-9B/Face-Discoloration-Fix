set(HEADER_FILES
	src/FaceGenManager.h
	src/Hooks.h
	src/Offsets.h
	src/PCH.h
)

set(SOURCE_FILES
	src/FaceGenManager.cpp
	src/Hooks.cpp
	src/PCH.cpp
	src/main.cpp
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PREFIX "Header Files"
	FILES ${HEADER_FILES}
)

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
	PREFIX "Source Files"
	FILES ${SOURCE_FILES}
)

set(SOURCES ${SOURCES} ${HEADER_FILES} ${SOURCE_FILES})
