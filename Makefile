NAME = RT84
ICON = icon.png
DESCRIPTION = "Raytracer in a weekend on ti 84"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -O3
CXXFLAGS = -Wall -Wextra -O3

include $(shell cedev-config --makefile)
