#
# Makefile to generate custom bindings using SWIG
#
# 2015 Dan Wilcox <danomatika@gmail.com>
#

# swig command
SWIG = swig

# default output language, see swig -h for more
LANG = lua

# generated bindings filename
NAME = OscBindings

# where to copy the generated bindings
DEST_DIR = ../src/bindings

# path to addons
ADDON_DIR = ../../../../addons

# C & C++ flags
CFLAGS = -I$(ADDON_DIR)
CXXFLAGS = $(CFLAGS)

.PHONY: all bindings debug clean

all: bindings move

# generates bindings
bindings:

	@echo LANG = $(LANG)
	@echo CXXFLAGS = $(CXXFLAGS)
	@echo NAME = $(NAME)
	@echo DEST_DIR = $(DEST_DIR)
	
	$(SWIG) -c++ -$(LANG) -fcompact -fvirtual $(CXXFLAGS) osc.i
	mv osc_wrap.cxx $(NAME).cpp

# move generated files to DEST_DIR
move:

	mkdir -p $(DEST_DIR)
	mv *.cpp $(DEST_DIR)

# outputs debug symbols
debug:
	$(SWIG) -c++ -$(LANG) -debug-lsymbols $(CXXFLAGS) osc.i > debug.txt
	rm -f *.cxx

# clean up
clean:
	rm -f $(DEST_DIR)/$(NAME).cpp
	rm -f debug.txt
