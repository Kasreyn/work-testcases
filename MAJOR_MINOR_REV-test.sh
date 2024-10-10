#!/bin/sh

VERSION="Debian clang-format version 14.2.6-test"
#VERSION=$(clang-format --version)

MAJOR_WANT=14
MINOR_WANT=3
REV_WANT=6

MAJOR_HAS=$(echo -n "$VERSION" | sed 's/^.*version \([0-9]\+\).[0-9]\+.[0-9]\+.*$/\1/')
MINOR_HAS=$(echo -n "$VERSION" | sed 's/^.*version [0-9]\+.\([0-9]\+\).[0-9]\+.*$/\1/')
REV_HAS=$(echo -n "$VERSION" | sed 's/^.*version [0-9]\+.[0-9]\+.\([0-9]\)\+.*$/\1/')

if [ "$MAJOR_HAS" -gt $MAJOR_WANT ]; then
	echo "We're good"
	exit
fi
if [ "$MINOR_HAS" -gt $MINOR_WANT ]; then
	echo "We're good"
	exit
fi
if [ "$MAJOR_HAS" -eq $MAJOR_WANT -a "$MINOR_HAS" -eq $MINOR_WANT -a "$REV_HAS" -ge $REV_WANT ]; then
	echo "We're good"
	exit
fi

echo "It's bad!"

echo "$MAJOR_HAS.$MINOR_HAS"
