import sys
import pygtk
from services import gtk_service
if not sys.platform == 'win32':
    pygtk.require('2.0')
gtk_service.resolve_gtk()
import gtk

def main():
    window = gtk.Window(gtk.WINDOW_TOPLEVEL)
    window.set_title('Windows PyGTK')
    window.connect('destroy', quit)
    window.show_all()
    gtk.main()

def quit(window):
    gtk.main_quit()
    gtk_service.unresolve_gtk()

if __name__ == '__main__':
    main()
