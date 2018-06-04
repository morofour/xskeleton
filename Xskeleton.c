/* ++
	Cool program for X Window System
-- */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>

#define X 0
#define Y 0
#define WIDTH 200
#define HEIGHT 200
#define WIDTH_MIN 50
#define HEIGHT_MIN 50
#define BORDER_WIDTH 5
#define TITLE "Example"
#define ICON_TITLE "Example"
#define PRG_CLASS "Example"

static int SetWindowManagerHints(
	Display *display,
	char *PClass,
	char *argv[],
	int argc,
	Window window,
	int x,
	int y,
	int win_wdt,
	int win_hgt,
	int win_wdt_min,
	int win_hgt_min,
	char *ptrTitle,
	char *ptrITitle,
	Pixmap pixmap
)
{
	XSizeHints size_hints;
	XWMHints wm_hints;
	XClassHint class_hint;
	XTextProperty windowname, iconname;

	if(!XStringListToTextProperty(&ptrTitle, 1, &windowname) ||
		!XStringListToTextProperty(&ptrITitle, 1, &iconname))
	{
		puts("No memory!\n");
		return 1;
	}

	size_hints.flags = PPosition | PSize | PMinSize;
	size_hints.min_width = win_wdt_min;
	size_hints.min_height = win_hgt_min;
	wm_hints.flags = StateHint | IconPixmapHint |InputHint;
	wm_hints.initial_state = NormalState;
	wm_hints.input = True;
	wm_hints.icon_pixmap = pixmap;
	class_hint.res_name = argv[0];
	class_hint.res_class = PClass;

	XSetWMProperties(display, window, &windowname, &iconname,
	argv, argc, &size_hints, &wm_hints, &class_hint);

	return 0;
}

int main(int argc, char *argv[])
{
	Display *display;
	int ScreenNumber;
	GC gc;
	XEvent report;
	Window window;

	if((display = XOpenDisplay(NULL)) == NULL)
	{
		puts("Can not connect to the X server!\n");
		return 1;
	}

	ScreenNumber = DefaultScreen(display);

	window = XCreateSimpleWindow(display,
		RootWindow(display, ScreenNumber),
		X, Y, WIDTH, HEIGHT, BORDER_WIDTH,
		BlackPixel(display, ScreenNumber),
		WhitePixel(display, ScreenNumber));

	SetWindowManagerHints(display, PRG_CLASS, argv, argc, window, X, Y,
		WIDTH, HEIGHT, WIDTH_MIN, HEIGHT_MIN, TITLE, ICON_TITLE, 0);

	XSelectInput(display, window, ExposureMask | KeyPressMask);

	XMapWindow(display, window);

	while(1)
	{
		XNextEvent(display, &report);

		switch(report.type)
		{
			case Expose :
				if(report.xexpose.count != 0)
					break;

				gc = XCreateGC(display, window, 0, NULL);
				XSetForeground(display, gc, BlackPixel(display, 0));
				XDrawString(display, window, gc, 20, 50, "First example", strlen("First Example"));
				XFreeGC(display, gc);
				XFlush(display);
				break;

			case KeyPress :
				XCloseDisplay(display);
				return 0;
		} // switch
	} // while
} // main
