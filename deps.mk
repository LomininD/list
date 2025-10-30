list_functions.o: list_functions.cpp list_functions.h debug.h \
  font_styles.h list_properties.h
debug.o: debug.cpp debug.h font_styles.h
list_dump.o: list_dump.cpp list_dump.h debug.h font_styles.h \
  list_properties.h
image_generator.o: image_generator.cpp image_generator.h \
  list_properties.h debug.h font_styles.h
