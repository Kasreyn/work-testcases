#!/bin/sh

# GOOGLE_TEST_INSTALLED=" "
# GOOGLE_TEST_INSTALLED=$(git diff HEAD --name-only -- '*.cpp' '*.hpp' | xargs)
# GOOGLE_TEST_INSTALLED=$(echo "Yes -a 1 -eq 2")
GOOGLE_TEST_INSTALLED=$(which clang-tidy)

command -v clang-format

if [ -z ${GOOGLE_TEST_INSTALLED} ]; then
	echo "Yes"
fi

if [ -z "$GOOGLE_TEST_INSTALLED" ]; then
	echo "Yes"
fi
