# compiler settings
CXX      = g++
CXXFLAGS = -shared -Wall -s -O2 -DWIN32 -DNDEBUG -D_WINDOWS -DUSRDLL -DNppTools_EXPORTS -DUNICODE
INCLUDES = -INpp -IDialogs
LDFLAGS  = -static
MKDIR    = mkdir -p $(@D)

# libs to link with
LIBS = -lshlwapi

# source files
SRCS =  ReverseText.cpp \
		PluginDefinition.cpp

# object path Release folder
OBJS_TARGET = Release/
CXX_OBJECTS = $(patsubst %.cpp, $(OBJS_TARGET)%.o, $(SRCS)) $(OBJS_TARGET)resource.res

# target binary
MAIN = Release/mrtReverseText.dll

.PHONY: depend clean

all:    $(MAIN)
		@echo  $(MAIN) has been successfully compiled.

$(MAIN): $(CXX_OBJECTS) 
		$(MKDIR)
		$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(MAIN) $(CXX_OBJECTS) $(LDFLAGS) $(LIBS)

Release/%.o: %.cpp
		$(MKDIR)
		$(CXX) $(INCLUDES) $(CXXFLAGS) -c $<  -o $@

Release/resource.res: Dialogs/resource.rc
		windres -I$(INCLUDES) -J rc -O coff -i$^ -o $@

clean:
		$(RM) $(OBJS_TARGET)*.o $(OBJS_TARGET)resource.res $(MAIN)

depend: $(SRCS)
		makedepend $(INCLUDES) $^
