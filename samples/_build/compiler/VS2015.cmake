# Use warning level 4 & reat Warnings as Errors
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
set(COMPILER "VisualC")

# https://msdn.microsoft.com/en-us/library/w455da8a.aspx
# /Zc:rvalueCast (Enforce type conversion rules)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:rvalueCast")
# /Zc:strictStrings (Disable string literal type conversion)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:strictStrings")
# /Zc:inline (Remove unreferenced COMDAT)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:inline")

#C4100: 'pWindow' : unreferenced formal parameter
#C4127: conditional expression is constant
#C4244: '=' : conversion from 'const whiteboard::ResourceType' to 'uint8', possible loss of data
#C4512: 'whiteboard::DynamicResourceTree' : assignment operator could not be generated
#C4201: nonstandard extension used : nameless struct/union
#C4510: 'xTASK_PARAMETERS' : default constructor could not be generated
#C4610: struct 'xTASK_PARAMETERS' can never be instantiated - user defined constructor required
#C4121: 'Nea::ChronoTickDelegate<Nea::TimeProvider>' : alignment of a member was sensitive to packing
#C4245: '=' : conversion from 'int' to 'uint32', signed/unsigned mismatch
#C4310: cast truncates constant value
#C4389: '==' : signed/unsigned mismatch
#C4206: nonstandard extension used : translation unit is empty
# warning C4065: switch statement contains 'default' but no 'case' labels
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4100")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4512")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4201")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4510")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4610")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4121")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4245")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4310")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4389")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4206")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4065")

# Multiprocessor compilation if using Visual Studio compiler
if(${CMAKE_VS_PLATFORM_TOOLSET} MATCHES "v140")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
endif()

# Generate map file https://msdn.microsoft.com/en-us/library/k7xkk3e2.aspx
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MAP")

add_definitions(-D_CRT_SECURE_NO_WARNINGS) # :(
