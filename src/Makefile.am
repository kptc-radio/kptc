####### kdevelop will overwrite this part!!! (begin)##########
bin_PROGRAMS = kptc

## INCLUDES were found outside kdevelop specific part

kptc_SOURCES = updatedialog.cpp update.cpp statusinfo.cpp rttyspeedwidget.cpp qttableview.cpp qtmultilineedit.cpp mytermout.cpp mylineedit.cpp modem.cpp modecommander.cpp modebuttons.cpp main.cpp kptc.cpp fixtextpicker.cpp cwspeedwidget.cpp cqdialog.cpp configmachine.cpp configdialog.cpp configdata.cpp commanddialog.cpp 
kptc_LDADD   =  $(LIB_QT) $(LIB_KDECORE) $(LIB_KDEUI)  $(LIBSOCKET)


EXTRA_DIST = kptc.desktop lo32-app-kptc.png lo16-app-kptc.png commanddialog.cpp commanddialog.h configdata.cpp configdata.h configdialog.cpp configdialog.h configmachine.cpp configmachine.h cqdialog.cpp cqdialog.h cwspeedwidget.cpp cwspeedwidget.h fixtextpicker.cpp fixtextpicker.h kptc.cpp kptc.h main.cpp mini-kptc.xpm modebuttons.cpp modebuttons.h modecommander.cpp modecommander.h modem.cpp modem.h mylineedit.cpp mylineedit.h mytermout.cpp mytermout.h qtmultilineedit.cpp qtmultilineedit.h qttableview.cpp qttableview.h rttyspeedwidget.cpp rttyspeedwidget.h statusinfo.cpp statusinfo.h update.cpp update.h updatedialog.cpp updatedialog.h 

install-data-local:
	$(mkinstalldirs) $(kde_appsdir)/Applications/
	$(INSTALL_DATA) $(srcdir)/kptc.desktop $(kde_appsdir)/Applications/kptc.desktop
	$(mkinstalldirs) $(kde_icondir)/locolor/32x32/apps/
	$(INSTALL_DATA) $(srcdir)/lo32-app-kptc.png $(kde_icondir)/locolor/32x32/apps/kptc.png
	$(mkinstalldirs) $(kde_icondir)/locolor/16x16/apps/
	$(INSTALL_DATA) $(srcdir)/lo16-app-kptc.png $(kde_icondir)/locolor/16x16/apps/kptc.png

uninstall-local:
	-rm -f $(kde_appsdir)/Applications/kptc.desktop
	-rm -f $(kde_icondir)/locolor/32x32/apps/kptc.png
	-rm -f $(kde_icondir)/locolor/16x16/apps/kptc.png

####### kdevelop will overwrite this part!!! (end)############
# These paths are KDE specific. Use them:
# kde_appsdir         Where your application's menu entry (.desktop) should go to.
# kde_icondir         Where your icon should go to - better use KDE_ICON.
# kde_sounddir        Where your sounds should go to.
# kde_htmldir         Where your docs should go to. (contains lang subdirs)
# kde_datadir         Where you install application data. (Use a subdir)
# kde_locale          Where translation files should go to. (contains lang subdirs)
# kde_cgidir          Where cgi-bin executables should go to.
# kde_confdir         Where config files should go to (system-wide ones with default values).
# kde_mimedir         Where mimetypes .desktop files should go to.
# kde_servicesdir     Where services .desktop files should go to.
# kde_servicetypesdir Where servicetypes .desktop files should go to.
# kde_toolbardir      Where general toolbar icons should go to (deprecated, use KDE_ICON).
# kde_wallpaperdir    Where general wallpapers should go to.
# kde_templatesdir    Where templates for the "New" menu (Konqueror/KDesktop) should go to.
# kde_bindir          Where executables should go to. Use bin_PROGRAMS or bin_SCRIPTS.
# kde_libdir          Where shared libraries should go to. Use lib_LTLIBRARIES.
# kde_moduledir       Where modules (e.g. parts) should go to. Use kde_module_LTLIBRARIES.
# kde_styledir        Where Qt/KDE widget styles should go to (new in KDE 3).
# kde_designerdir     Where Qt Designer plugins should go to (new in KDE 3).

# set the include path for X, qt and KDE
INCLUDES= $(all_includes)

METASOURCES = AUTO

# the library search path. 
kptc_LDFLAGS = $(all_libraries) $(KDE_RPATH)

# Uncomment the following two lines if you add a ui.rc file for your application to make use of
# KDE?s XML GUI builing
#rcdir = $(kde_datadir)/kptc
#rc_DATA = kptcui.rc

#WARNING: if you use a ui.rc file above, use:

# messages: rc.cpp

# instead of 

# messages:

messages: 
	LIST=`find . -name \*.h -o -name \*.hh -o -name \*.H -o -name \*.hxx -o -name \*.hpp -o -name \*.cpp -o -name \*.cc -o -name \*.cxx -o -name \*.ecpp -o -name \*.C`; \
	if test -n "$$LIST"; then \
	 $(XGETTEXT) $$LIST -o $(podir)/kptc.pot; \
	fi

