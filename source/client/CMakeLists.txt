INCLUDE_DIRECTORIES (
    ${STAR_EXTERN_INCLUDES}
    ${STAR_CORE_INCLUDES}
    ${STAR_BASE_INCLUDES}
    ${STAR_GAME_INCLUDES}
    ${STAR_PLATFORM_INCLUDES}
    ${STAR_APPLICATION_INCLUDES}
    ${STAR_RENDERING_INCLUDES}
    ${STAR_WINDOWING_INCLUDES}
    ${STAR_FRONTEND_INCLUDES}
  )

SET (star_client_HEADERS
      StarClientApplication.hpp
    )

SET (star_client_SOURCES
      StarClientApplication.cpp
    )

IF (STAR_SYSTEM_WINDOWS)
  SET (star_client_RESOURCES
      xclient.rc
    )
ENDIF ()

ADD_EXECUTABLE (xclient WIN32
  $<TARGET_OBJECTS:star_extern> $<TARGET_OBJECTS:star_core> $<TARGET_OBJECTS:star_base> $<TARGET_OBJECTS:star_game>
  $<TARGET_OBJECTS:star_application> $<TARGET_OBJECTS:star_rendering> $<TARGET_OBJECTS:star_windowing> $<TARGET_OBJECTS:star_frontend>
  ${star_client_HEADERS} ${star_client_SOURCES} ${star_client_RESOURCES})
TARGET_PRECOMPILE_HEADERS (xclient REUSE_FROM star_core)
TARGET_LINK_LIBRARIES (xclient ${STAR_EXT_LIBS} ${STAR_EXT_GUI_LIBS})
IF (STAR_COMPILER_GNU AND STAR_CROSS_COMPILE)
  # `-mwindows` stops cross-compiled xClient from opening up a console window when started.
  TARGET_LINK_OPTIONS(xclient BEFORE PUBLIC "-mwindows")
ENDIF ()