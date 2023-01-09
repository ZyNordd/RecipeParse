add_library(eda SHARED eda.h eda.cpp)

#set_target_properties(eda PROPERTIES LINKER_LANGUAGE C)
#set(CMAKE_PREFIX_PATH "C:\\Qt\\5.15.2\\msvc2019_64\\")

#find_package(Qt5 REQUIRED COMPONENTS Network)
#find_package(Qt5 REQUIRED COMPONENTS Widgets)

#target_link_libraries(eda PUBLIC Qt${QT_VERSION_MAJOR}::Network)
#target_link_libraries(eda PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)