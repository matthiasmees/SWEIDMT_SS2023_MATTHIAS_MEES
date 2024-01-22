cmake_minimum_required(VERSION 3.5)

project(Aufgabe2_1 VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)


set(PROJECT_SOURCES
        main.cpp
        aufgabe2_1.cpp
        aufgabe2_1.h
        aufgabe2_1.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Aufgabe2_1
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

        MyLib/CMakeLists.txt
        MyLib/ctdataset.cpp
        MyLib/ctdataset.h
        MyLibUnitTests/CMakeLists.txt
        MyLibUnitTests/tst_mylibunittest.cpp
        doc/html/search/all_0.js doc/html/search/all_1.js doc/html/search/all_2.js doc/html/search/classes_0.js doc/html/search/classes_1.js doc/html/search/classes_2.js doc/html/search/close.svg doc/html/search/mag.svg doc/html/search/mag_d.svg doc/html/search/mag_sel.svg doc/html/search/mag_seld.svg doc/html/search/search.css doc/html/search/search.js doc/html/search/searchdata.js doc/html/annotated.html doc/html/aufgabe2__1_8h_source.html doc/html/bc_s.png doc/html/bc_sd.png doc/html/class_aufgabe2__1-members.html doc/html/class_aufgabe2__1.html doc/html/class_aufgabe2__1.png doc/html/class_c_t_dataset-members.html doc/html/class_c_t_dataset.html doc/html/class_my_lib_test_class-members.html doc/html/class_my_lib_test_class.html doc/html/class_my_lib_test_class.png doc/html/classes.html doc/html/closed.png doc/html/ctdataset_8h_source.html doc/html/dir_9ddf9e93c4f4d1ffed0cd5a7135a9783.html doc/html/dir_eb9f46c45142d138281eeea982d0d358.html doc/html/doc.svg doc/html/docd.svg doc/html/doxygen.css doc/html/doxygen.svg doc/html/dynsections.js doc/html/files.html doc/html/folderclosed.svg doc/html/folderclosedd.svg doc/html/folderopen.svg doc/html/folderopend.svg doc/html/hierarchy.html doc/html/index.html doc/html/jquery.js doc/html/menu.js doc/html/menudata.js doc/html/minus.svg doc/html/minusd.svg doc/html/nav_f.png doc/html/nav_fd.png doc/html/nav_g.png doc/html/nav_h.png doc/html/nav_hd.png doc/html/open.png doc/html/plus.svg doc/html/plusd.svg doc/html/splitbar.png doc/html/splitbard.png doc/html/sync_off.png doc/html/sync_on.png doc/html/tab_a.png doc/html/tab_ad.png doc/html/tab_b.png doc/html/tab_bd.png doc/html/tab_h.png doc/html/tab_hd.png doc/html/tab_s.png doc/html/tab_sd.png doc/html/tabs.css docs/Doxyfile.in
        docs/CMakeLists.txt


    )

# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Aufgabe2_1 APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Aufgabe2_1 SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Aufgabe2_1
            ${PROJECT_SOURCES}
        )
    endif()
endif()


###############################
# add Library to our project
###############################
add_subdirectory("MyLib")
include_directories(${CMAKE_PROJECT_DIR} MyLib)
add_dependencies(Aufgabe2_1 MyLib)
###############################
# add UnitTests to our project
###############################
add_subdirectory("MyLibUnitTests")
include_directories(${CMAKE_PROJECT_DIR} MyLibUnitTests)
add_dependencies(Aufgabe2_1 MyLib MyLibUnitTests)


find_package(Qt6 REQUIRED COMPONENTS Test)
target_link_libraries(Aufgabe2_1 PRIVATE Qt6::Test)
target_link_libraries(Aufgabe2_1 PRIVATE Qt6::Widgets)
target_link_libraries(Aufgabe2_1 PRIVATE Qt6::Widgets MyLib)
#target_link_libraries(Aufgabe2_1 PRIVATE Qt6::Widgets MyLibUnitTests)

add_subdirectory("docs")



# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Aufgabe2_1)
endif()
set_target_properties(Aufgabe2_1 PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Aufgabe2_1
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Aufgabe2_1)
endif()



