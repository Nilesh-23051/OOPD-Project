# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -std=c++17

# Source Files
SRCI = Project_Shell.cpp
SRCS = Project_Q2.cpp
SRCU = Project_Multithreading.cpp

# Target Names
DEBUG_TARGET_Q1 = debug_program_Q1
DEBUG_TARGET_Q2 = debug_program_Q2
DEBUG_TARGET_Q3 = debug_program_Q3
OPTIMIZE_TARGET_Q1 = optimize_program1
OPTIMIZE_TARGET_Q2 = optimize_program2
OPTIMIZE_TARGET_Q3 = optimize_program3

all: debug_program_Q1 debug_program_Q2 debug_program_Q3 optimize_program1 optimize_program2 optimize_program3

debugQ1: $(DEBUG_TARGET_Q1)
debugQ2: $(DEBUG_TARGET_Q2)
debugQ3: $(DEBUG_TARGET_Q3)
optimizedQ1: $(OPTIMIZE_TARGET_Q1)
optimizedQ2: $(OPTIMIZE_TARGET_Q2)
optimizedQ3: $(OPTIMIZE_TARGET_Q3)
$(DEBUG_TARGET_Q1): $(SRCI)
	$(CXX) $(CXXFLAGS) -g $< -o $@

$(DEBUG_TARGET_Q2): $(SRCS)
	$(CXX) $(CXXFLAGS) -g $< -o $@

$(DEBUG_TARGET_Q3): $(SRCU)
	$(CXX) $(CXXFLAGS) -g $< -o $@

$(OPTIMIZE_TARGET_Q1): $(SRCI)
	$(CXX) $(CXXFLAGS) -O2 $< -o $@

$(OPTIMIZE_TARGET_Q2): $(SRCS)
	$(CXX) $(CXXFLAGS) -O2 $< -o $@

$(OPTIMIZE_TARGET_Q3): $(SRCU)
	$(CXX) $(CXXFLAGS) -O2 $< -o $@

run: $(DEBUG_TARGET_Q1) $(OPTIMIZE_TARGET_Q1) $(DEBUG_TARGET_Q2) $(OPTIMIZE_TARGET_Q2) $(DEBUG_TARGET_Q3) $(OPTIMIZE_TARGET_Q3)
	./$(DEBUG_TARGET_Q1)
	./$(OPTIMIZE_TARGET_Q1)
	./$(DEBUG_TARGET_Q2)
	./$(OPTIMIZE_TARGET_Q2)
	./$(DEBUG_TARGET_Q3)
	./$(OPTIMIZE_TARGET_Q3)

clean:
	rm -f $(DEBUG_TARGET_Q1) $(OPTIMIZE_TARGET_Q1) $(DEBUG_TARGET_Q2) $(OPTIMIZE_TARGET_Q2) $(DEBUG_TARGET_Q3) $(OPTIMIZE_TARGET_Q3)
