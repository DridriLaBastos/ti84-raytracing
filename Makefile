NAME = RT84
ICON = icon.png
DESCRIPTION = "Raytracer in a weekend on ti 84"
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz --std=c++17
STATIC = 1

include $(shell cedev-config --makefile)
