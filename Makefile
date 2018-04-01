###########################################################################
#                                                                         #
#  Copyright 2010 Ben Selfridge                                           #
#                                                                         #
#  This file is part of mazecut.                                          #
#                                                                         #
#  mazecut is free software: you can redistribute it and/or modify        #
#  it under the terms of the GNU General Public License as published by   #
#  the Free Software Foundation, either version 3 of the License, or      #
#  (at your option) any later version.                                    #
#                                                                         #
#  mazecut is distributed in the hope that it will be useful,             #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of         #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          #
#  GNU General Public License for more details.                           #
#                                                                         #
#  You should have received a copy of the GNU General Public License      #
#  along with mazecut.  If not, see <http://www.gnu.org/licenses/>.       #
#                                                                         #
###########################################################################

INSTALLDIR=$(HOME)/bin
INSTALLCPFLAGS=-i

VERSION=0.1

CC=g++
MACFLAGS=-arch i386
CYGWINFLAGS=-mno-cygwin -mwindows
LINUXFLAGS=

CFLAGS=-g -Wall

# Use this for a cygwin build
# CFLAGS=-g -Wall $(CYGWINFLAGS)

# This for a mac build
# CFLAGS = -g -Wall $(MACFLAGS)

# This for a linux build
# CFLAGS=-g -Wall $(LINUXFLAGS)

# Use this for a cygwin build
WXFLAGS=`$(WXPREFIX)wx-config --cxxflags --libs` $(CYGWINFLAGS)
WXPREFIX=
WXLIBS=`$(WXPREFIX)wx-config --libs` $(CYGWINFLAGS)
WXCPPFLAGS=`$(WXPREFIX)wx-config --cppflags` $(CYGWINFLAGS)
WXCXXFLAGS=`$(WXPREFIX)wx-config --cxxflags` $(CYGWINFLAGS)


# This for a mac build
# WXFLAGS=`$(WXPREFIX)wx-config --cxxflags --libs` $(MACFLAGS)

# This for a linux build
# WXFLAGS=`$(WXPREFIX)wx-config --cxxflags --libs` $(LINUXFLAGS)

SRCDIR=src
SRCFILES=$(CLASSDIR)/* $(SRCDIR)/main/*
CLASSDIR=$(SRCDIR)/classes
OBJDIR=obj
BINDIR=bin
DOCDIR=doc
DISTDIR=dist
EXDIR=examples

OBJTARGETS=$(OBJDIR)/constants.o $(OBJDIR)/MazeNode.o $(OBJDIR)/Maze2d.o
mazecutTARGETS=$(OBJTARGETS) $(OBJDIR)/mazecut.o
mazecutGUITARGETS=$(OBJTARGETS) $(OBJDIR)/MazeCutter.o

CLASSHEADERS=-I$(CLASSDIR)
MAINHEADERS=-I$(SRCDIR)/main

APPRESOURCES=$(SRCDIR)/resources/Info.plist $(SRCDIR)/resources/maze.icns $(SRCDIR)/resources/version.plist

EXSMALLSIZE=20 20
EXBIGSIZE=100 100
EXFLAGS=-d

.PHONY: dirs installcl installgui gui guimac cl clean uninstall tarsrc tarbin

all: cl gui

installcl: $(INSTALLDIR)/mazecut

installgui: $(INSTALLDIR)/MazeCutter

gui: dirs $(BINDIR)/MazeCutter

guimac: dirs $(BINDIR)/MazeCutter.app

cl: dirs $(BINDIR)/mazecut

dirs: $(BINDIR) $(OBJDIR)

clean:
	@echo -n Removing $(BINDIR), $(OBJDIR), $(DOCDIR), $(DISTDIR), $(EXDIR)...
	@rm -rf $(BINDIR)/
	@rm -rf $(OBJDIR)/
	@rm -rf $(DOCDIR)/
	@rm -rf $(DISTDIR)/
	@rm -rf $(EXDIR)/
	@echo done
	@echo -n Removing log files...
	@rm -f *.log
	@echo done
	@echo Directory is clean.

uninstall:
	@echo -n Removing $(INSTALLDIR)/mazecut and $(INSTALLDIR)/MazeCutter...
	@rm -rf $(INSTALLDIR)/mazecut $(INSTALLDIR)/MazeCutter $(INSTALLDIR)/MazeCutterIcons
	@echo done

tarsrc: $(DISTDIR) $(SRCFILES)
	@echo -n Creating source distribution $(DISTDIR)/mazecut$(VERSION)_src.tgz...
	@tar -czf $(DISTDIR)/mazecut$(VERSION)_src.tgz $(SRCDIR) Makefile mazecut.doxyfile README icons/ COPYING
	@echo done

tarbin: $(DISTDIR) $(BINDIR)/mazecut $(BINDIR)/MazeCutter
	@echo -n Creating binary distribution $(DISTDIR)/mazecut$(VERSION)_bin.tgz...
	@tar -czf $(DISTDIR)/mazecut$(VERSION)_bin.tgz $(BINDIR)/mazecut $(BINDIR)/MazeCutter
	@echo done

tarbinwindows: $(DISTDIR) $(BINDIR)/mazecut $(BINDIR)/MazeCutter
	@echo -n Creating Windows binary distribution $(DISTDIR)/mazecut$(VERSION)_win_bin.tgz...
	@tar -czf $(DISTDIR)/mazecut$(VERSION)_win_bin.tgz $(BINDIR)/mazecut.exe $(BINDIR)/MazeCutter.exe
	@echo done

zipbinwindows: $(DISTDIR) $(BINDIR)/mazecut $(BINDIR)/MazeCutter
	@echo -n Creating Windows binary distribution $(DISTDIR)/mazecut$(VERSION)_win_bin.zip...
	@zip -rq $(DISTDIR)/mazecut$(VERSION)_win_bin.zip $(BINDIR)/mazecut.exe $(BINDIR)/MazeCutter.exe
	@echo done

$(INSTALLDIR)/mazecut: $(BINDIR)/mazecut
	@echo -n Copying mazecut to $(INSTALLDIR)...
	@cp $(INSTALLCPFLAGS) $(BINDIR)/mazecut $(INSTALLDIR)
	@echo done

$(INSTALLDIR)/MazeCutter: $(BINDIR)/MazeCutter
	@echo -n Copying MazeCutter to $(INSTALLDIR)...
	@cp $(INSTALLCPFLAGS) $(BINDIR)/MazeCutter $(INSTALLDIR)
	@echo done

$(BINDIR)/MazeCutter.app: $(BINDIR)/MazeCutter $(APPRESOURCES)
	mkdir -p $(BINDIR)/MazeCutter.app    
	mkdir -p $(BINDIR)/MazeCutter.app/Contents
	mkdir -p $(BINDIR)/MazeCutter.app/Contents/MacOS
	mkdir -p $(BINDIR)/MazeCutter.app/Contents/Resources
	@#mkdir $(BINDIR)/MazeCutter.app/Contents/Resources/English.lproj
	cp src/resources/Info.plist $(BINDIR)/MazeCutter.app/Contents/
	@#cp src/resources/version.plist $(BINDIR)/MazeCutter.app/Contents/
	@#cp InfoPlist.strings YourApp.app/Contents/Resources/English.lproj/
	echo -n 'APPL????' > $(BINDIR)/MazeCutter.app/Contents/PkgInfo
	cp $(BINDIR)/MazeCutter $(BINDIR)/MazeCutter.app/Contents/MacOS/MazeCutter
	cp src/resources/maze.icns $(BINDIR)/MazeCutter.app/Contents/Resources/

$(BINDIR)/MazeCutter: $(mazecutGUITARGETS)
	@echo -n Linking object files for MazeCutter program...
	@$(CC) $(CFLAGS) $(CLASSHEADERS) $(MAINHEADERS) $(mazecutGUITARGETS) -o $(BINDIR)/MazeCutter $(WXFLAGS)
	@echo done
	@echo Executable location: ./$(BINDIR)/MazeCutter
	@echo done
	@echo "***********************"
	@echo "* Done compiling GUI! *"
	@echo "***********************"

$(BINDIR)/mazecut: $(mazecutTARGETS) 
	@echo -n Linking object files for mazecut program...
	@$(CC) $(CFLAGS) $(mazecutTARGETS) -o $(BINDIR)/mazecut
	@echo done
	@echo Executable location: ./$(BINDIR)/mazecut
	@echo "****************************************"
	@echo "* Done compiling command-line utility! *"
	@echo "****************************************"

$(OBJDIR)/constants.o: $(CLASSDIR)/constants.h $(CLASSDIR)/MazeNode.h $(CLASSDIR)/constants.cpp
	@echo -n Compiling constants.cpp...
	@$(CC) $(CFLAGS) -c $(CLASSDIR)/constants.cpp -o $(OBJDIR)/constants.o
	@echo done

$(OBJDIR)/MazeNode.o: $(CLASSDIR)/constants.h $(CLASSDIR)/MazeNode.h $(CLASSDIR)/MazeNode.cpp
	@echo -n Compiling MazeNode.cpp...
	@$(CC) $(CFLAGS) -c $(CLASSDIR)/MazeNode.cpp -o $(OBJDIR)/MazeNode.o
	@echo done

$(OBJDIR)/Maze2d.o: $(CLASSDIR)/constants.h $(CLASSDIR)/MazeNode.h $(CLASSDIR)/Maze2d.h $(CLASSDIR)/Maze2d.cpp
	@echo -n Compiling Maze2d.cpp...
	@$(CC) $(CFLAGS) -c $(CLASSDIR)/Maze2d.cpp -o $(OBJDIR)/Maze2d.o
	@echo done

$(OBJDIR)/MazeProfiler.o: $(CLASSDIR)/constants.h $(CLASSDIR)/Maze2d.h $(CLASSDIR)/MazeProfiler.h $(CLASSDIR)/MazeProfiler.cpp
	@echo -n Compiling MazeProfiler.cpp...
	@$(CC) $(CFLAGS) -c $(CLASSDIR)/MazeProfiler.cpp -o $(OBJDIR)/MazeProfiler.o
	@echo done

$(OBJDIR)/MazeCutter.o: $(SRCDIR)/main/MazeCutter.cpp $(SRCDIR)/main/MazeCutter.h $(CLASSDIR)/Maze2d.h
	@echo -n Compiling MazeCutter.cpp... 
	@$(CC) $(CFLAGS) $(CLASSHEADERS) -c $(WXCXXFLAGS) $(SRCDIR)/main/MazeCutter.cpp -o $(OBJDIR)/MazeCutter.o
	@echo done

$(OBJDIR)/mazecut.o: $(CLASSDIR)/constants.h $(CLASSDIR)/Maze2d.h $(SRCDIR)/main/mazecut.cpp
	@echo -n Compiling mazecut.cpp...
	@$(CC) $(CFLAGS) $(CLASSHEADERS) $(GRAPHICSHEADERS) -c $(SRCDIR)/main/mazecut.cpp -o $(OBJDIR)/mazecut.o
	@echo done

$(BINDIR):
	@echo -n Creating directory $(BINDIR)...
	@mkdir -p $(BINDIR)
	@echo done

$(OBJDIR):
	@echo -n Creating directory $(OBJDIR)...
	@mkdir -p $(OBJDIR)
	@echo done

$(DOCDIR): $(SRCFILES)
	@echo -n Generating documentation...
	@mkdir -p $(DOCDIR)
	@date >> doxygen.log
	@echo -n ' ' >> doxygen.log
	@doxygen mazecut.doxyfile >> doxygen.log 2>> doxygen.log
	@echo done

$(DISTDIR):
	@echo -n Creating $(DISTDIR) directory...
	@mkdir -p $(DISTDIR)
	@echo done

$(EXDIR): cl
	@echo Creating examples...
	@mkdir -p $(EXDIR)
	@mkdir -p $(EXDIR)/small
	@mkdir -p $(EXDIR)/big
	@echo -n Creating $(EXDIR)/small/rb.txt...
	@./$(BINDIR)/mazecut $(EXSMALLSIZE) $(EXFLAGS) -a rb > $(EXDIR)/small/rb.txt
	@echo done
	@echo -n Creating $(EXDIR)/small/prims.txt...
	@./$(BINDIR)/mazecut $(EXSMALLSIZE) $(EXFLAGS) -a prims > $(EXDIR)/small/prims.txt
	@echo done
	@echo -n Creating $(EXDIR)/small/ellers.txt...
	@./$(BINDIR)/mazecut $(EXSMALLSIZE) $(EXFLAGS) -a ellers > $(EXDIR)/small/ellers.txt
	@echo done
	@echo -n Creating $(EXDIR)/small/aldousBroder.txt...
	@./$(BINDIR)/mazecut $(EXSMALLSIZE) $(EXFLAGS) -a ab > $(EXDIR)/small/aldousBroder.txt
	@echo done
	@echo -n Creating $(EXDIR)/small/wilsons.txt...
	@./$(BINDIR)/mazecut $(EXSMALLSIZE) $(EXFLAGS) -a w > $(EXDIR)/small/wilsons.txt
	@echo done
	@echo -n Creating $(EXDIR)/small/binaryTree.txt...
	@./$(BINDIR)/mazecut $(EXSMALLSIZE) $(EXFLAGS) -a bt > $(EXDIR)/small/binaryTree.txt
	@echo done
	@echo -n Creating $(EXDIR)/big/rb.txt...
	@./$(BINDIR)/mazecut $(EXBIGSIZE) $(EXFLAGS) -a rb > $(EXDIR)/big/rb.txt
	@echo done
	@echo -n Creating $(EXDIR)/big/prims.txt...
	@./$(BINDIR)/mazecut $(EXBIGSIZE) $(EXFLAGS) -a prims > $(EXDIR)/big/prims.txt
	@echo done
	@echo -n Creating $(EXDIR)/big/ellers.txt...
	@./$(BINDIR)/mazecut $(EXBIGSIZE) $(EXFLAGS) -a ellers > $(EXDIR)/big/ellers.txt
	@echo done
	@echo -n Creating $(EXDIR)/big/aldousBroder.txt...
	@./$(BINDIR)/mazecut $(EXBIGSIZE) $(EXFLAGS) -a ab > $(EXDIR)/big/aldousBroder.txt
	@echo done
	@echo -n Creating $(EXDIR)/big/wilsons.txt...
	@./$(BINDIR)/mazecut $(EXBIGSIZE) $(EXFLAGS) -a w > $(EXDIR)/big/wilsons.txt
	@echo done
	@echo -n Creating $(EXDIR)/big/binaryTree.txt...
	@./$(BINDIR)/mazecut $(EXBIGSIZE) $(EXFLAGS) -a bt > $(EXDIR)/big/binaryTree.txt
	@echo done
	@echo Done creating examples.
