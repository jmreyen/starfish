#############################################################################
# Makefile for building: pic
# Generated by qmake (2.01a) (Qt 4.8.1) on: Mo 9. Jul 19:31:44 2012
# Project:  pic.pro
# Template: app
# Command: c:\qtsdk\desktop\qt\4.8.1\mingw\bin\qmake.exe -spec ..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\win32-g++ CONFIG+=declarative_debug -o Makefile pic.pro
#############################################################################

first: debug
install: debug-install
uninstall: debug-uninstall
MAKEFILE      = Makefile
QMAKE         = c:\qtsdk\desktop\qt\4.8.1\mingw\bin\qmake.exe
DEL_FILE      = del
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
COPY          = copy /y
COPY_FILE     = $(COPY)
COPY_DIR      = xcopy /s /q /y /i
INSTALL_FILE  = $(COPY_FILE)
INSTALL_PROGRAM = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
DEL_FILE      = del
SYMLINK       = 
DEL_DIR       = rmdir
MOVE          = move
CHK_DIR_EXISTS= if not exist
MKDIR         = mkdir
SUBTARGETS    =  \
		debug \
		release

debug: $(MAKEFILE).Debug FORCE
	$(MAKE) -f $(MAKEFILE).Debug
debug-make_default: $(MAKEFILE).Debug FORCE
	$(MAKE) -f $(MAKEFILE).Debug 
debug-make_first: $(MAKEFILE).Debug FORCE
	$(MAKE) -f $(MAKEFILE).Debug first
debug-all: $(MAKEFILE).Debug FORCE
	$(MAKE) -f $(MAKEFILE).Debug all
debug-clean: $(MAKEFILE).Debug FORCE
	$(MAKE) -f $(MAKEFILE).Debug clean
debug-distclean: $(MAKEFILE).Debug FORCE
	$(MAKE) -f $(MAKEFILE).Debug distclean
debug-install: $(MAKEFILE).Debug FORCE
	$(MAKE) -f $(MAKEFILE).Debug install
debug-uninstall: $(MAKEFILE).Debug FORCE
	$(MAKE) -f $(MAKEFILE).Debug uninstall
release: $(MAKEFILE).Release FORCE
	$(MAKE) -f $(MAKEFILE).Release
release-make_default: $(MAKEFILE).Release FORCE
	$(MAKE) -f $(MAKEFILE).Release 
release-make_first: $(MAKEFILE).Release FORCE
	$(MAKE) -f $(MAKEFILE).Release first
release-all: $(MAKEFILE).Release FORCE
	$(MAKE) -f $(MAKEFILE).Release all
release-clean: $(MAKEFILE).Release FORCE
	$(MAKE) -f $(MAKEFILE).Release clean
release-distclean: $(MAKEFILE).Release FORCE
	$(MAKE) -f $(MAKEFILE).Release distclean
release-install: $(MAKEFILE).Release FORCE
	$(MAKE) -f $(MAKEFILE).Release install
release-uninstall: $(MAKEFILE).Release FORCE
	$(MAKE) -f $(MAKEFILE).Release uninstall

Makefile: pic.pro  ../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/win32-g++/qmake.conf ../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/qconfig.pri \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/modules/qt_webkit_version.pri \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/qt_functions.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/qt_config.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/exclusive_builds.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/default_pre.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/win32/default_pre.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/debug.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/debug_and_release.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/default_post.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/win32/default_post.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/declarative_debug.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/win32/rtti.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/win32/exceptions.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/win32/stl.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/shared.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/warn_on.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/qt.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/win32/thread.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/moc.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/win32/windows.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/resources.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/uic.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/yacc.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/lex.prf \
		../../QtSDK/Desktop/Qt/4.8.1/mingw/mkspecs/features/include_source_dir.prf \
		c:/QtSDK/Desktop/Qt/4.8.1/mingw/lib/qtmaind.prl
	$(QMAKE) -spec ..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\win32-g++ CONFIG+=declarative_debug -o Makefile pic.pro
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\qconfig.pri:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\modules\qt_webkit_version.pri:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\qt_functions.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\qt_config.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\exclusive_builds.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\default_pre.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\win32\default_pre.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\debug.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\debug_and_release.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\default_post.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\win32\default_post.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\declarative_debug.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\win32\rtti.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\win32\exceptions.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\win32\stl.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\shared.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\warn_on.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\qt.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\win32\thread.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\moc.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\win32\windows.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\resources.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\uic.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\yacc.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\lex.prf:
..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\features\include_source_dir.prf:
c:\QtSDK\Desktop\Qt\4.8.1\mingw\lib\qtmaind.prl:
qmake: qmake_all FORCE
	@$(QMAKE) -spec ..\..\QtSDK\Desktop\Qt\4.8.1\mingw\mkspecs\win32-g++ CONFIG+=declarative_debug -o Makefile pic.pro

qmake_all: FORCE

make_default: debug-make_default release-make_default FORCE
make_first: debug-make_first release-make_first FORCE
all: debug-all release-all FORCE
clean: debug-clean release-clean FORCE
distclean: debug-distclean release-distclean FORCE
	-$(DEL_FILE) Makefile

check: first

debug-mocclean: $(MAKEFILE).Debug
	$(MAKE) -f $(MAKEFILE).Debug mocclean
release-mocclean: $(MAKEFILE).Release
	$(MAKE) -f $(MAKEFILE).Release mocclean
mocclean: debug-mocclean release-mocclean

debug-mocables: $(MAKEFILE).Debug
	$(MAKE) -f $(MAKEFILE).Debug mocables
release-mocables: $(MAKEFILE).Release
	$(MAKE) -f $(MAKEFILE).Release mocables
mocables: debug-mocables release-mocables
FORCE:

$(MAKEFILE).Debug: Makefile
$(MAKEFILE).Release: Makefile
