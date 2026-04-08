# Makefile for EDR Agent tests
#
# Usage:
#   make          - build and run tests
#   make test     - run tests only
#   make coverage - run tests with coverage report
#   make clean    - clean build artifacts

CC := gcc
CFLAGS := -Wall -Wextra -g -I../src/agent -I../src/common -DEDR_TESTING
LDFLAGS :=

# Coverage flags
COV_CFLAGS := --coverage -fprofile-arcs -ftest-coverage
COV_LDFLAGS := --coverage

# Source files
AGENT_SRC := ../src/agent/edr_agent.c
TEST_SRC := test_edr_agent.c

# Targets
TEST_BIN := test_edr_agent
TEST_COV_BIN := test_edr_agent_cov

.PHONY: all test coverage clean

all: test

# Build and run tests
test: $(TEST_BIN)
	./$(TEST_BIN)

# Build test binary
$(TEST_BIN): $(TEST_SRC) $(AGENT_SRC)
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC) $(AGENT_SRC) $(LDFLAGS)

# Build with coverage and run
coverage: clean $(TEST_COV_BIN)
	./$(TEST_COV_BIN)
	@echo ""
	@echo "=== Coverage Report ==="
	@gcov edr_agent.gcda 2>&1
	@echo ""
	@echo "Detailed coverage in edr_agent.c.gcov"

$(TEST_COV_BIN): $(TEST_SRC) $(AGENT_SRC)
	$(CC) $(CFLAGS) $(COV_CFLAGS) -o $@ $(TEST_SRC) $(AGENT_SRC) $(LDFLAGS) $(COV_LDFLAGS)

clean:
	rm -f $(TEST_BIN) $(TEST_COV_BIN)
	rm -f *.gcov *.gcda *.gcno
	rm -f ../src/agent/*.gcov ../src/agent/*.gcda ../src/agent/*.gcno
