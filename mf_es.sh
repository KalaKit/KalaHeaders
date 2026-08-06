#!/bin/sh

# Move file for use with mf, read more at https://github.com/greeenlaser/personal-stash/tree/main/mf

set -e

#
# References
#

README=README.md
LICENSE=LICENSE.md
INCLUDE=include

TARGET_NAME=KalaHeaders
LIB_DEST=../external-shared/${TARGET_NAME}

#
# Core stuff
#

mf --o --f README.md --t "${LIB_DEST}/README.md"
mf --o --f LICENSE.md --t "${LIB_DEST}/LICENSE.md"
mf --o --f include --t "${LIB_DEST}"
