#include <gtk/gtk.h>
#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <stdio.h>

void getMousePosition(int* x1,int* y1,int* x2,int* y2)
{
    Display* dpy;
    XEvent e;
    dpy=XOpenDisplay(NULL);
    XGrabPointer(dpy, DefaultRootWindow(dpy),1,
                 ButtonPressMask|ButtonReleaseMask,
                 GrabModeAsync,GrabModeAsync,
                 None,None,
                 CurrentTime);

    while(1){
        XNextEvent(dpy,&e);
        if(e.type==ButtonPress){
            *x1=e.xmotion.x_root;
            *y1=e.xmotion.y_root;
            break;
        }
    }
    while(1){
      XNextEvent(dpy,&e);
        if(e.type==ButtonPress){
          *x2=e.xmotion.x_root;
          *y2=e.xmotion.y_root;
          break;
        }
    }
    XCloseDisplay(dpy);
}

void destroy(void) {
  gtk_main_quit();
}

GdkPixbuf * get_screenshot(int x,int y,int width,int height){
    return gdk_pixbuf_get_from_drawable (NULL, gdk_get_default_root_window (), NULL,x,y,0,0,width,height);
}

int main (int argc, char** argv) {
  GtkWidget* window;
  int x1,y1,x2,y2;
  int x,y,width,height;
  getMousePosition(&x1,&y1,&x2,&y2);
  x=MIN(x1,x2);
  y=MIN(y1,y2);
  width=ABS(x1-x2);
  height=ABS(y1-y2);
  gtk_init (&argc,&argv);
  window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect(GTK_OBJECT (window),"destroy",GTK_SIGNAL_FUNC (destroy),NULL);
  gtk_container_add(GTK_CONTAINER (window),gtk_image_new_from_pixbuf(get_screenshot(x,y,width,height)));
  gtk_window_set_resizable(window,TRUE);
  gtk_widget_set_size_request(window,width,height);
  gtk_widget_show_all(window);
  gtk_window_set_keep_above(GTK_WINDOW(window),TRUE);
  gtk_window_move(GTK_WINDOW (window),x,y);
  gtk_main();
  return 0;
}

//cc -Wall sstop.c -o sstop `pkg-config --cflags --libs gtk+-2.0` `pkg-config --cflags --libs x11 xtst`
