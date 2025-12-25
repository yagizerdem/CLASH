add_executable(CLASH main.cpp
        util/model/command.h
        util/model/pipe.h)


file(GLOB_RECURSE util CONFIGURE_DEPENDS ./util/*.cpp ./util/*.h)
file(GLOB_RECURSE process CONFIGURE_DEPENDS ./process/*.cpp ./process/*.h)
file(GLOB_RECURSE expand CONFIGURE_DEPENDS ./expand/*.cpp ./expand/*.h)


#libraries
add_library(utilLib STATIC ${util})
add_library(processLib STATIC ${process})
add_library(expandLib STATIC ${expand})

##process
target_link_libraries(processLib PUBLIC utilLib)
target_link_libraries(expandLib PUBLIC utilLib)

#link libraries with executable
target_link_libraries(CLASH PUBLIC utilLib)
target_link_libraries(CLASH PUBLIC processLib)


set_target_properties(CLASH expandLib processLib utilLib PROPERTIES LINKER_LANGUAGE CXX)