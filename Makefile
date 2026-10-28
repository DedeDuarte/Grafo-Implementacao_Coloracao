CXX = g++
CPPFLAGS ?=
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
LDFLAGS ?=
LDLIBS ?=

COMMON_SRC = src/TGrafo.cpp src/utils.cpp
HEADERS = include/TGrafo.hpp include/utils.hpp
TARGET = a.out
TEST_TARGET = build/test_coloracao
SANITIZE_TARGET = build/test_coloracao_sanitize
VALGRIND_TARGET = build/test_coloracao_valgrind
SANITIZE_FLAGS = -g -fno-omit-frame-pointer -fsanitize=address,undefined

.PHONY: all run test sanitize valgrind clean

all: $(TARGET)

$(TARGET): src/main.cpp $(COMMON_SRC) $(HEADERS) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Iinclude src/main.cpp $(COMMON_SRC) $(LDFLAGS) $(LDLIBS) -o $@

$(TEST_TARGET): tests/test_coloracao.cpp $(COMMON_SRC) $(HEADERS) Makefile | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -Iinclude tests/test_coloracao.cpp $(COMMON_SRC) $(LDFLAGS) $(LDLIBS) -o $@

$(SANITIZE_TARGET): tests/test_coloracao.cpp $(COMMON_SRC) $(HEADERS) Makefile | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SANITIZE_FLAGS) -Iinclude tests/test_coloracao.cpp $(COMMON_SRC) $(LDFLAGS) $(LDLIBS) -o $@

$(VALGRIND_TARGET): tests/test_coloracao.cpp $(COMMON_SRC) $(HEADERS) Makefile | build
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -g -Iinclude tests/test_coloracao.cpp $(COMMON_SRC) $(LDFLAGS) $(LDLIBS) -o $@

build:
	mkdir -p $@

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

sanitize: $(SANITIZE_TARGET)
	ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 UBSAN_OPTIONS=halt_on_error=1:print_stacktrace=1 ./$(SANITIZE_TARGET)

valgrind: $(VALGRIND_TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./$(VALGRIND_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(SANITIZE_TARGET) $(VALGRIND_TARGET)
	rm -f build/*.o build/*.d
	@if [ -d build ]; then rmdir --ignore-fail-on-non-empty build; fi
