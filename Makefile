TARGET := WaveDash3DS
SOURCES := source
INCLUDES := include
ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

# Definir explícitamente el compilador de la 3DS
CXX := arm-none-eabi-g++
CC := arm-none-eabi-gcc

CXXFLAGS := -g -Wall -O2 -mword-relocations $(ARCH) -ffunction-sections \
            -I$(DEVKITPRO)/libctru/include -I$(DEVKITPRO)/citro3d/include \
            -I$(DEVKITPRO)/citro2d/include -std=gnu++11 -fno-rtti -fno-exceptions

LDFLAGS := -specs=3dsx.specs $(ARCH) \
           -L$(DEVKITPRO)/libctru/lib -L$(DEVKITPRO)/citro3d/lib -L$(DEVKITPRO)/citro2d/lib

LIBS := -lcitro2d -lcitro3d -lctru -lm

export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
CPPFILES := $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.cpp))
OFILES := $(notdir $(CPPFILES:.cpp=.o))

all: $(TARGET).3dsx $(TARGET).cia

$(TARGET).3dsx: $(TARGET).elf
	@3dsxtool $< $@

$(TARGET).cia: $(TARGET).elf
	@makerom -f cia -o $@ -elf $< -rsf template.rsf

$(TARGET).elf: $(OFILES)
	@$(CXX) $(LDFLAGS) $(OFILES) $(LIBS) -o $@

%.o: %.cpp
	@$(CXX) $(CXXFLAGS) -c $<

clean:
	@rm -f $(OFILES) $(TARGET).elf $(TARGET).3dsx $(TARGET).cia *.map
