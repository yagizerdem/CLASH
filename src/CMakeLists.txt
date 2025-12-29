add_executable(CLASH main.cpp
        util/model/engineResponse.h)


file(GLOB_RECURSE util CONFIGURE_DEPENDS ./util/*.cpp ./util/*.h)
file(GLOB_RECURSE process CONFIGURE_DEPENDS ./process/*.cpp ./process/*.h)
file(GLOB_RECURSE expand CONFIGURE_DEPENDS ./expand/*.cpp ./expand/*.h)
file(GLOB_RECURSE lexer CONFIGURE_DEPENDS ./lexer/*.cpp ./lexer/*.h)
file(GLOB_RECURSE parser CONFIGURE_DEPENDS ./parser/*.cpp ./parser/*.h)
file(GLOB_RECURSE shell CONFIGURE_DEPENDS ./shell/*.cpp ./shell/*.h)


#libraries
add_library(utilLib STATIC ${util})
add_library(processLib STATIC ${process})
add_library(expandLib STATIC ${expand})
add_library(lexerLib STATIC ${lexer})
add_library(parserLib STATIC ${parser})
add_library(shellLib STATIC ${shell})

##process
target_link_libraries(processLib PUBLIC utilLib)
#expand
target_link_libraries(expandLib PUBLIC utilLib)
target_link_libraries(expandLib PUBLIC lexerLib)
target_link_libraries(expandLib PUBLIC parserLib)
target_link_libraries(expandLib PUBLIC processLib)
#lexer
target_link_libraries(lexerLib PUBLIC utilLib)
#parser
target_link_libraries(parserLib PUBLIC utilLib)
#shell
target_link_libraries(shellLib PUBLIC utilLib)
target_link_libraries(shellLib PUBLIC lexerLib)
target_link_libraries(shellLib PUBLIC parserLib)
target_link_libraries(shellLib PUBLIC processLib)
target_link_libraries(shellLib PUBLIC expandLib)

#link libraries with executable
target_link_libraries(CLASH PUBLIC utilLib)
target_link_libraries(CLASH PUBLIC processLib)
target_link_libraries(CLASH PUBLIC expandLib)
target_link_libraries(CLASH PUBLIC lexerLib)
target_link_libraries(CLASH PUBLIC parserLib)
target_link_libraries(CLASH PUBLIC shellLib)



set_target_properties(CLASH
        shellLib
        parserLib
        lexerLib
        expandLib
        processLib
        utilLib
        PROPERTIES LINKER_LANGUAGE CXX)