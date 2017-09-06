CWD := $(shell echo %cd%)

all: gtk python

python: winpython.exe
	@winpython.exe
	@del winpython.exe

gtk: gtk.zip
	@7z x gtk.zip gtk
	@del gtk.zip

winpython.exe:
	@curl -L --insecure -o winpython.exe https://github.com/winpython/winpython/releases/download/1.7.20170401/WinPython-64bit-2.7.13.1Zero.exe

gtk.zip:
	@curl -L -o gtk.zip http://www.tarnyko.net/repo/gtk3_build_system/gtk+-bundle_3.6.4-20130513_win32.zip

wheels:
	@curl -LO http://www.lfd.uci.edu/~gohlke/pythonlibs/hkfw9m5o/pycairo_gtk-1.10.0-cp27-none-win_amd64.whl
	@curl -LO http://www.lfd.uci.edu/~gohlke/pythonlibs/hkfw9m5o/pygobject-2.28.6-cp27-none-win_amd64.whl
	@curl -LO http://www.lfd.uci.edu/~gohlke/pythonlibs/hkfw9m5o/pygtk-2.22.0-cp27-none-win_amd64.whl
