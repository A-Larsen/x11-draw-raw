#include <stdio.h>
#include <X11/Xlib.h>
#include <math.h>

int main(){
	Display *dpy;
	Window win;
	XEvent event;
	XGCValues gcv_square_color;
	GC gc_square_color;

	dpy = XOpenDisplay(":0");
	Screen *scr = DefaultScreenOfDisplay(dpy);


	int win_width = 1000;
	int win_height = 1000;
	int dpy_width = WidthOfScreen(scr);
	int dpy_height = HeightOfScreen(scr);


	gc_square_color = XCreateGC(dpy, RootWindow(dpy, 0),
			GCForeground | GCBackground, &gcv_square_color);

	win = XCreateSimpleWindow(dpy, RootWindow(dpy, 0),
			dpy_width/2-(win_width/2), 
			dpy_height/2-(win_height/2), 
			win_width, win_height, 1, 
			BlackPixel(dpy, 0), BlackPixel(dpy, 0));



	XSelectInput(dpy, win, ExposureMask | KeyPressMask);
	XMapWindow(dpy, win);


	XNextEvent(dpy, &event);

	FILE *fp = fopen("./small2.data", "r");

	int ch;
	int count = 0;
	int hc[3];
	int *hcp = hc;

	int pos = -1;

	while((ch = fgetc(fp))!= EOF){
		
		if(count <= 2){
			*hcp++ = ch;
		}

		if(count == 2){
			if(pos < 15*15){
				pos++;
			}

			int y = floor(pos/15);
			int x = pos%15;
			gcv_square_color.foreground =
				hc[0] * pow(16, 4) + 
				hc[1] * pow(16, 2) +
				hc[2];

			int w = 60;
			int h = 60;

			gc_square_color = XCreateGC(dpy, RootWindow(dpy, 0),
					GCForeground, &gcv_square_color);
			XFillRectangle(dpy, win, gc_square_color,
					x*w, y*h, w, h);

			hcp = hc;
			count = 0;

		}else{
			count++;
		}

		/* XFlushGC(dpy, gc_square_color); */
		XFlush(dpy);

	}

	while(1){
		XNextEvent(dpy, &event);
		switch(event.type){
			case Expose:
				/* XFlushGC(dpy, gc_square_color); */
				break;
		}
	}
	return 0;

}
