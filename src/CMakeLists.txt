add_executable(CLASH main.cpp)


file(GLOB_RECURSE util CONFIGURE_DEPENDS ./util/*.cpp ./util/*.h)
#file(GLOB_RECURSE process CONFIGURE_DEPENDS ./process/*.cpp ./process/*.h)
#
add_library(utilLib STATIC ${util})
#add_library(processLib STATIC ${process})
#
#
##process
#target_link_libraries(processLib PUBLIC utilLib)

#link libraries with executable
target_link_libraries(CLASH PUBLIC utilLib)


set_target_properties(CLASH utilLib PROPERTIES LINKER_LANGUAGE CXX)