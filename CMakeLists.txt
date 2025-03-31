cmake_minimum_required(VERSION 3.16)

project(XMLFeedReader VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Multimedia)

set(PROJECT_SOURCES
        main.cpp
        xfeed.cpp
        xfeed.h
        xfeed.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(XMLFeedReader
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        xfeedreader.h xfeedreader.cpp
        xfeedchannel.h xfeedchannel.cpp
        xfeedch.h xfeedch.cpp
        xfeedxmlreader.h xfeedxmlreader.cpp
        channelinfo.h channelinfo.cpp
        addchannel.ui
        addchannel.cpp
        addchannel.h

        xfeedfolder.h
        xfeedfolder.cpp
        addfolder.ui
        addfolder.h
        addfolder.cpp
        xjsonadapter.h xjsonadapter.cpp
        xfeedmodel.h xfeedmodel.cpp
        XCommon.h
        Icons.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET XMLFeedReader APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(XMLFeedReader SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(XMLFeedReader
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(XMLFeedReader PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(XMLFeedReader PRIVATE Qt6::Xml)
target_link_libraries(XMLFeedReader PRIVATE Qt6::Test)
target_link_libraries(XMLFeedReader PRIVATE Qt6::Network)
target_link_libraries(XMLFeedReader PRIVATE Qt6::Multimedia)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.XMLFeedReader)
endif()
set_target_properties(XMLFeedReader PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS XMLFeedReader
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(XMLFeedReader)
endif()

qt_add_executable(XFeed_Test
    channelinfo.h channelinfo.cpp
    xfeedchannel.h  xfeedchannel.cpp
    xfeedch.h   xfeedch.cpp
    xfeedfolder.cpp xfeedfolder.h
    xfeedreader.cpp xfeedreader.h
    xfeedxmlreader.cpp xfeedxmlreader.h
    xjsonadapter.h xjsonadapter.cpp

    tests/xfeedtest.cpp
    tests/xfeedtest.h



)

target_link_libraries(XFeed_Test PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Test
    Qt6::Widgets
)
install(TARGETS XFeed_Test
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET XFeed_Test
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
