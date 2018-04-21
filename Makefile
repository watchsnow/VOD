THIS := $(lastword $(MAKEFILE_LIST))
MAKEFLAGS += --no-print-directory

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

EXENAME = VOD
SRCDIR = src/
BUILDDIR = build/
BINDIR = $(BUILDDIR)bin/
OBJDIR = $(BUILDDIR)obj/
DEPDIR = $(BUILDDIR)dep/
EXE = $(BINDIR)$(EXENAME)

LIBFOLDER = NetFlux/
LIBBUILDFOLDER = $(LIBFOLDER)build/
LIBBINFOLDER = $(LIBBUILDFOLDER)bin/
LIBINCFOLDER = $(LIBBUILDFOLDER)include/
LIBFILE = $(LIBBINFOLDER)libnetflux.a
LIB = $(patsubst lib%,-l%, $(notdir $(basename $(LIBFILE))))

CLANG = @clang++
GPP = @g++

COMMONFLAGS = -std=c++11 -I$(LIBINCFOLDER)
ifneq ($(MODE),release)
COMMONFLAGS += -DDEBUG -g -O0
else
COMMONFLAGS += -Werror -g0 -O3
endif
CLANGFLAGS = $(COMMONFLAGS) -stdlib=libc++ -Weverything -Wno-padded -Wno-disabled-macro-expansion -Wno-c++98-compat -Wno-missing-prototypes -Wno-old-style-cast
GPPFLAGS = $(COMMONFLAGS) -Wall -Wextra -Weffc++ -Wno-error=effc++

LNKFLAGS = -L$(LIBBINFOLDER) $(LIB)


SRC = $(call rwildcard,$(SRCDIR),*.cpp)
OBJ = $(addprefix $(OBJDIR), $(notdir $(SRC:%.cpp=%.o)))
DEP = $(wildcard $(DEPDIR)*.d)

.PHONY: clean mrproper all
.SECONDEXPANSION:

ifneq ($(words $(OBJ)),0)
$(EXE): $(LIBFILE) $(OBJ)
	@mkdir -p $(BINDIR)
	@printf "%-13s <$@>...\n" "Linking"
ifeq ($(CC),g++)
	$(GPP) $(GPPFLAGS) -o $@ $(OBJ) $(LNKFLAGS)
else
	$(CLANG) $(CLANGFLAGS) -o $@ $(OBJ) $(LNKFLAGS)
endif
	@ln -sf $(EXE) $(EXENAME)
else
$(EXE):
	$(error No source code found)
endif

$(LIBFILE):
	@make -C $(LIBFOLDER)

$(OBJDIR)%.o: $$(call rwildcard,$(SRCDIR),%.cpp) $(THIS)
	@mkdir -p $(DEPDIR)
	@mkdir -p $(OBJDIR)
ifeq ($(CC),g++)
	@printf "%-13s <$<>...\n" "Gplusplusing"
	$(GPP) $(GPPFLAGS) -o $@ -c -MMD -MF $(addprefix $(DEPDIR), $(notdir $(<:.cpp=.d))) $<
else
	@printf "%-13s <$<>...\n" "Clanging"
	$(CLANG) $(CLANGFLAGS) -o $@ -c -MMD -MF $(addprefix $(DEPDIR), $(notdir $(<:.cpp=.d))) $<
endif

-include $(DEP)

all: mrproper $(EXE)

clean:
	@echo "Cleaning..."
	@rm -rf $(OBJDIR) $(DEPDIR)

mrproper:
	@echo "Cleaning everything..."
	@rm -rf $(BUILDDIR) $(EXENAME)
