/*
A simple graphics library for CSE 20211 by Douglas Thain

This work is licensed under a Creative Commons Attribution 4.0 International License.  https://creativecommons.org/licenses/by/4.0/

For complete documentation, see:
http://www.nd.edu/~dthain/courses/cse20211/fall2013/gfx
Version 3, 11/07/2012 - Now much faster at changing colors rapidly.
Version 2, 9/23/2011 - Fixes a bug that could result in jerky animation.
*/

#include "gfx.h"

/*
gfx_open creates several X11 objects, and stores them in globals
for use by the other functions in the library.
*/

static Display *gfx_display=0;
static Window  gfx_window;
static GC      gfx_gc;
static Colormap gfx_colormap;
static int      gfx_fast_color_mode = 0;

/* These values are saved by gfx_wait then retrieved later by gfx_xpos and gfx_ypos. */

static int saved_xpos = 0;
static int saved_ypos = 0;

/* Open a new graphics window. */

void gfx_open( int width, int height, const char *title )
{
	gfx_display = XOpenDisplay(0);
	if(!gfx_display) {
		fprintf(stderr,"gfx_open: unable to open the graphics window.\n");
		exit(1);
	}

	Visual *visual = DefaultVisual(gfx_display,0);
	if(visual && visual->class==TrueColor) {
		gfx_fast_color_mode = 1;
	} else {
		gfx_fast_color_mode = 0;
	}

	int blackColor = BlackPixel(gfx_display, DefaultScreen(gfx_display));
	int whiteColor = WhitePixel(gfx_display, DefaultScreen(gfx_display));

	gfx_window = XCreateSimpleWindow(gfx_display, DefaultRootWindow(gfx_display), 0, 0, width, height, 0, blackColor, blackColor);

	XSetWindowAttributes attr;
	attr.backing_store = Always;

	XChangeWindowAttributes(gfx_display,gfx_window,CWBackingStore,&attr);

	XStoreName(gfx_display,gfx_window,title);

	XSelectInput(gfx_display, gfx_window, StructureNotifyMask|KeyPressMask|ButtonPressMask);

	XMapWindow(gfx_display,gfx_window);

	gfx_gc = XCreateGC(gfx_display, gfx_window, 0, 0);

	gfx_colormap = DefaultColormap(gfx_display,0);

	XSetForeground(gfx_display, gfx_gc, whiteColor);

	// Wait for the MapNotify event

	for(;;) {
		XEvent e;
		XNextEvent(gfx_display, &e);
		if (e.type == MapNotify)
			break;
	}
}
void gfx_resize(uint32_t width, uint32_t height){
	XResizeWindow(gfx_display, gfx_window, width, height);
}
int32_t gfx_close(void){
	return (int32_t)XDestroyWindow(gfx_display, gfx_window);
}
/* Draw a single point at (x,y) */

void gfx_point( int x, int y )
{
	XDrawPoint(gfx_display,gfx_window,gfx_gc,x,y);
}

/* Draw a line from (x1,y1) to (x2,y2) */

void gfx_line( int x1, int y1, int x2, int y2 )
{
	XDrawLine(gfx_display,gfx_window,gfx_gc,x1,y1,x2,y2);
}

/* Change the current drawing color. */

void gfx_color( int r, int g, int b )
{
	XColor color;

	if(gfx_fast_color_mode) {
		/* If this is a truecolor display, we can just pick the color directly. */
		color.pixel = ((b&0xff) | ((g&0xff)<<8) | ((r&0xff)<<16) );
	} else {
		/* Otherwise, we have to allocate it from the colormap of the display. */
		color.pixel = 0;
		color.red = r<<8;
		color.green = g<<8;
		color.blue = b<<8;
		XAllocColor(gfx_display,gfx_colormap,&color);
	}

	XSetForeground(gfx_display, gfx_gc, color.pixel);
}

/* Clear the graphics window to the background color. */

void gfx_clear()
{
	XClearWindow(gfx_display,gfx_window);
}

/* Change the current background color. */

void gfx_clear_color( int r, int g, int b )
{
	XColor color;
	color.pixel = 0;
	color.red = r<<8;
	color.green = g<<8;
	color.blue = b<<8;
	XAllocColor(gfx_display,gfx_colormap,&color);

	XSetWindowAttributes attr;
	attr.background_pixel = color.pixel;
	XChangeWindowAttributes(gfx_display,gfx_window,CWBackPixel,&attr);
}
int gfx_event_waiting()
{
       XEvent event;

       gfx_flush();

       while (1) {
               if(XCheckMaskEvent(gfx_display,-1,&event)) {
                       if(event.type==KeyPress) {
                               XPutBackEvent(gfx_display,&event);
                               return 1;
                       } else if (event.type==ButtonPress) {
                               XPutBackEvent(gfx_display,&event);
                               return 1;
                       } else {
                               return 0;
                       }
               } else {
                       return 0;
               }
       }
}


uint8_t waitForKey(uint8_t keycode, uint32_t *x, uint32_t *y){
	uint8_t keycodereceived; 
	XEvent event;
	gfx_flush();
	while(keycodereceived!=keycode) {
		XNextEvent(gfx_display,&event);
		switch (event.type){
		case KeyPress:
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			*x=saved_xpos;
			*y=saved_ypos;
			keycodereceived=XLookupKeysym(&event.xkey, 0);
			break;
		case ButtonPress:
		    saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			*x=saved_xpos;
			*y=saved_ypos;
			//printf("AHHHHHH%u", keycode);
			keycodereceived=event.xkey.keycode;
			break;
		case ResizeRequest:
			*x=event.xresizerequest.width;
			*y=event.xresizerequest.height;
			return 1; 
			break;
		case DestroyNotify:
			return 0; 
			break;
		default:
		 	for(int i=0; i<2; i++){
				 fprintf(stdout, "[INFO]DEBUG EVENTO TIPO %d\n", event.type);
			 }
			break;
		}
	}
	return 0; 
}
/* Wait for the user to press a key or mouse button. */
uint8_t gfx_wait(){
	XEvent event;
	gfx_flush();
	while(1) {
		XNextEvent(gfx_display,&event);
		if(event.type==KeyPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return XLookupKeysym(&event.xkey,0);
		} else if(event.type==ButtonPress) {
			saved_xpos = event.xkey.x;
			saved_ypos = event.xkey.y;
			return event.xbutton.button;
		}
	}
}

/* Return the X and Y coordinates of the last event. */

int gfx_xpos()
{
	return saved_xpos;
}

int gfx_ypos()
{
	return saved_ypos;
}

/* Flush all previous output to the window. */

void gfx_flush()
{
	XFlush(gfx_display);
}