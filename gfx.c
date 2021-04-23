/*
yeah if youre reading this i already modified it, so yeah... 
and it will end up being ultra modified if not entirely scrapped in a few commits so... 
ALSO TODO verbose thingie 
*/
#include "gfx.h"

static json_object *obs; 
static Display *gfx_display=0;
static Window  gfx_window;
static GC      gfx_gc;
static Colormap gfx_colormap;
static int      gfx_fast_color_mode = 0;
static uint32_t maxwidth=0, maxheight=0; 

static int saved_xpos = 0;
static int saved_ypos = 0;
void gfx_start(json_object *obj){
	obs=obj;

	//printf("IEBhsaghei");
	returnjson_resolution(obs, &maxwidth, &maxheight);
}
void gfx_open(uint32_t *width, uint32_t *height, const char *title){
	
	
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
	if ((uint32_t)*width>maxwidth){
		*width=maxwidth;
	}
	if((uint32_t)*height>maxheight){
		*height=maxheight;
	}
	gfx_window = XCreateSimpleWindow(gfx_display, DefaultRootWindow(gfx_display), 0, 0, (int)*width, (int)*height, 0, blackColor, blackColor);

	XSetWindowAttributes attr;
	attr.backing_store = Always;
	//attr.background_pixel = 0x80800000;
	XChangeWindowAttributes(gfx_display,gfx_window,CWBackingStore,&attr);
	
	XStoreName(gfx_display,gfx_window,title);

	XSelectInput(gfx_display, gfx_window, StructureNotifyMask|KeyPressMask|ButtonPressMask);

	XMapWindow(gfx_display,gfx_window);
	gfx_gc = XCreateGC(gfx_display, gfx_window, 0,0);

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


void gfx_point( int x, int y )
{
	XDrawPoint(gfx_display,gfx_window,gfx_gc,x,y);
}
void gfx_image(XImage *image, uint32_t wid, uint32_t hei){
	XPutImage(gfx_display,gfx_window,gfx_gc,image, 0,0,0,0,wid,hei);
}
XImage *gfxvetor_image(uint8_t *data, uint8_t bitdepth, uint32_t wid, uint32_t hei, size_t rwb){
	XImage *image;
    Visual *visual = DefaultVisual(gfx_display,0);
    int screen = DefaultScreen(gfx_display);
    int dplanes = DisplayPlanes(gfx_display, screen);
    image = XCreateImage(gfx_display, visual, dplanes, ZPixmap, 0, (char *)data, wid, hei, (int)bitdepth, (int)rwb);
    return image;
}
void gfx_line( int x1, int y1, int x2, int y2 )
{
	XDrawLine(gfx_display,gfx_window,gfx_gc,x1,y1,x2,y2);
}

void gfx_color( int r, int g, int b )
{
	XColor color;

	if(gfx_fast_color_mode) {
		color.pixel = ((b&0xff) | ((g&0xff)<<8) | ((r&0xff)<<16) );
	} else {
		color.pixel = 0;
		color.red = r<<8;
		color.green = g<<8;
		color.blue = b<<8;
		XAllocColor(gfx_display,gfx_colormap,&color);
	}

	XSetForeground(gfx_display, gfx_gc, color.pixel);
}



void gfx_clear()
{
	XClearWindow(gfx_display,gfx_window);
}

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
			
			keycodereceived=event.xkey.keycode;
			break;
		case ResizeRequest:
			*x=event.xresizerequest.width;
			*y=event.xresizerequest.height;
			#ifdef SHOW_DEBUG
				fprintf(stdout, "[Debug] Resize request on the window to %dx%d pixels\n", *x, *y);
			#endif
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


int gfx_xpos()
{
	return saved_xpos;
}

int gfx_ypos()
{
	return saved_ypos;
}

void gfx_flush()
{
	XFlush(gfx_display);
}