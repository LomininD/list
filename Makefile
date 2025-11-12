CPP = g++
CPPFLAGS = -O3 -DNDEBUG -fomit-frame-pointer \
-Wall -Wextra -Werror \
-Wcast-align -Wcast-qual -Wconversion \
-Wfloat-equal -Wformat-security -Wformat=2 \
-Wnull-dereference -Winit-self \
-Wshadow -Wstrict-overflow=2 \
-Wswitch-default -Wswitch-enum \
-Wundef -Wunused -Wvla

BUILD = build

SRCMODULES = list_functions.cpp debug.cpp list_dump.cpp image_generator.cpp
OBJMODULES = $(SRCMODULES:.cpp=.o)

$(BUILD)/list: main.cpp $(addprefix $(BUILD)/,$(OBJMODULES))
	$(CPP) $(CPPFLAGS) $^ -o $@

$(BUILD)/%.o: %.cpp
	mkdir -p $(BUILD)
	$(CPP) $(CPPFLAGS) -c $< -o $@

deps.mk: $(SRCMODULES)
	$(CPP) -MM $^ > $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif

.PHONY: clean

clean:
	rm -rf $(BUILD)