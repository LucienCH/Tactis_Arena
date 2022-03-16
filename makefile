# project name (generate executable with this name)
TARGET   = TacticsArena

CC       = gcc
# compiling flags here
CFLAGS   = -std=c99 -Wall -I.
SOCKLIBFLAGS = lib/libws2_32.a
THREADFLAGS = -lpthread -D_REENTRANT

LINKER   = gcc
# linking flags here
LFLAGS   = -Wall -I. -lm

# change these to proper directories where each file should be
SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
LIBDIR	 = lib

SDL_DIR			= SDL2
SDLLIB_DIR		= $(SDL_DIR)/lib
SDLINC_DIR		= $(SDL_DIR)/include

SDL_LIBS		=-L${SDLLIB_DIR} -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer
SDL_INC			=-I${SDLINC_DIR}

PLATFORM := $(shell uname)



DIRS	 = $(OBJDIR) $(BINDIR)

.PHONY: DIRS
all: $(DIRS) $(BINDIR)/$(TARGET)

$(OBJDIR):
	mkdir $(OBJDIR)
$(BINDIR):
	mkdir $(BINDIR)

SOURCES  := $(wildcard $(SRCDIR)/*.c)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
rm       = rm -f


$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(OBJECTS) $(LFLAGS) -o $@ $(SDL_LIBS) $(SDL_INC) $(SOCKLIBFLAGS) $(THREADFLAGS)
	@echo "Linking complete!"


$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@$(rm) $(OBJECTS)
	@echo "Cleanup complete!"
	@echo $(PLATFORM)


.PHONY: remove
remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"
