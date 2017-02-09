SHELL			= /bin/sh

########################################

CC			= g++

SOURCE_FILES		= src/main.cpp \
			  src/BuildProbe.cpp \
			  src/Histogram.cpp \
			  src/Performance.cpp \
			  src/Pool.cpp \
			  src/RadixJoin.cpp \
			  src/Relation.cpp

HEADER_FILES		= src/BuildProbe.h \
			  src/Config.h \
			  src/Histogram.h \
			  src/Performance.h \
			  src/Pool.h \
			  src/RadixJoin.h \
			  src/Relation.h \
			  src/Tuple.h
				
########################################

PROJECT_NAME		= radix_join

########################################

CCFLAGS 		= -g
LINKFLAGS 		= -lpthread

########################################

SOURCE_FOLDER		= src
BUILD_FOLDER		= build
RELEASE_FOLDER		= release

########################################
			
OBJECT_FILES		= $(patsubst $(SOURCE_FOLDER)/%.cpp,$(BUILD_FOLDER)/%.o,$(SOURCE_FILES))
SOURCE_DIRECTORIES	= $(dir $(HEADER_FILES))
BUILD_DIRECTORIES	= $(patsubst $(SOURCE_FOLDER)/%,$(BUILD_FOLDER)/%,$(SOURCE_DIRECTORIES))

########################################

all: program

########################################

$(BUILD_FOLDER)/%.o:  $(SOURCE_FILES) $(HEADER_FILES)
	mkdir -p $(BUILD_FOLDER)
	$(CC) $(CCFLAGS) -c $(SOURCE_FOLDER)/$*.cpp -I$(SOURCE_FOLDER) -o $(BUILD_FOLDER)/$*.o

########################################

program: $(OBJECT_FILES)
	mkdir -p $(RELEASE_FOLDER)
	$(CC) $(OBJECT_FILES) $(LINKFLAGS) -o $(RELEASE_FOLDER)/$(PROJECT_NAME)
	chmod +x -R $(RELEASE_FOLDER)

########################################

clean:
	rm -rf $(BUILD_FOLDER)
	rm -rf $(RELEASE_FOLDER)
