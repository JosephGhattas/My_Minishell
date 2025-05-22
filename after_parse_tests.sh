#!/bin/bash

MINISHELL=./minishell
TMP_OUT=out.txt

function run_test() {
	echo "Running: $1"
	echo -e "$1\nexit" | $MINISHELL > "$TMP_OUT"
	cat "$TMP_OUT"
	echo "---------------------------"
}

# Builtins
run_test "echo Hello World"
run_test "echo -n Hello"
run_test "pwd"
run_test "cd .. && pwd"
run_test "export FOO=bar && env | grep FOO"
run_test "unset FOO && env | grep FOO"
run_test "exit 42"

# External command
run_test "ls -1"
run_test "echo Hello > test.txt && cat < test.txt"
run_test "echo again >> test.txt && cat test.txt"

# Pipeline
run_test "ls | grep minishell"
run_test "echo -e \"a\nb\nc\" | grep b | wc -l"

# Heredoc
run_test "cat << EOF\nhello\nworld\nEOF"

# Error & edge cases
run_test "cd nonexistentdir"
run_test "exit too many args"
run_test "echo \$?"
