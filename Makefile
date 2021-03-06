
# Linux

CC = g++

RMDIR = rm -rf
RM = rm -f

DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_FLAGS)/$*.d

DIRECTIVES = -std=c++11 -Wall -Wextra -c -I $(HEADER_PATH)

LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

HEADER_PATH = include 
SRC_PATH = src
BIN_PATH = bin
DEP_PATH = dep

CPP_FILES = $(wildcard $(SRC_PATH)/*.
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))
DEP_FILES = $(wildcard $(DEP_PATH)/*.d)

EXEC = 140039082_FabioMarques

# Caso for Windows

ifeq ($(OS), Windows_NT)

RMDIR = rd /s /q
RM = del

SDL_PATH = C:\SDL2-2.0.5\x86_64-w64-mingw32

DIRECTIVES += -I $(SDL_PATH)\include

LIBS = -L $(SDL_PATH)\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

EXEC := $(EXEC).exe

else
UNAME_S "= $(shell uname -s)



# Caso for Mac

ifeq ($(UNAME_S), Darwin)

LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf

endif
endif


# Regras de compilação

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^ $(LIBS)


# Criação dos arquivos obejto caso eles não existam, um para cada arquivo "cpp"	
$(BIN_PATH)/%.o: $(SRC_PATH)/%.cpp

ifeq ($(OS), Windows_NT)
	@if not exist $(DEP_PATH) @mkdir $(DEP_PATH)
	@if not exist $(BIN_PATH) @mkdir $(BIN_PATH)
else
	@mkdir -p $(DEP_PATH) $(BIN_PATH)
endif

	$(CC) $(DEP_FLAGS) -c -o $@ $< $(DIRECTIVES)\
	
-include $(DEP_FILES)

clean:
	$(RMDIR) $(BIN_PATH) $(DEP_PATH)
	$(RM) $(EXEC)
	
.PRECIOUS: $(DEP_PATH)/%.d

.PHONY: debug clean release

print-% : @echo $* = $($*)

debug: DIRECTIVES += -ggdb -O0
debug: all

release: DIRECTIVES += -0fast -mtune=native
release: all
