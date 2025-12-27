add_executable(CLASH main.cpp
        util/model/word.h
        parser/parseCommand.cpp
        parser/parseCommand.h)


file(GLOB_RECURSE util CONFIGURE_DEPENDS ./util/*.cpp ./util/*.h)
file(GLOB_RECURSE process CONFIGURE_DEPENDS ./process/*.cpp ./process/*.h)
file(GLOB_RECURSE expand CONFIGURE_DEPENDS ./expand/*.cpp ./expand/*.h)
file(GLOB_RECURSE lexer CONFIGURE_DEPENDS ./lexer/*.cpp ./lexer/*.h)


#libraries
add_library(utilLib STATIC ${util})
add_library(processLib STATIC ${process})
add_library(expandLib STATIC ${expand})
add_library(lexerLib STATIC ${lexer})

##process
target_link_libraries(processLib PUBLIC utilLib)
#expand
target_link_libraries(expandLib PUBLIC utilLib)
#lexer
target_link_libraries(lexerLib PUBLIC utilLib)

#link libraries with executable
target_link_libraries(CLASH PUBLIC utilLib)
target_link_libraries(CLASH PUBLIC processLib)
target_link_libraries(CLASH PUBLIC expandLib)
target_link_libraries(CLASH PUBLIC lexerLib)



set_target_properties(CLASH
        lexerLib
        expandLib
        processLib
        utilLib
        PROPERTIES LINKER_LANGUAGE CXX)