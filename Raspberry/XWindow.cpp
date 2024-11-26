#include "Common.h"
#include "XWindow.h"

#include <EGL/egl.h>
#include <EGL/eglplatform.h>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

XWindow::XWindow()
{
}

void XWindow::CreateWindow()
{
	m_display = XOpenDisplay(nullptr);
	m_root = RootWindowOfScreen(DefaultScreenOfDisplay(m_display));

	XSetWindowAttributes swa;
	XSetWindowAttributes  xattr;
	Atom wm_state;
	XWMHints hints;

	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;

	swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask;
	swa.background_pixmap = None;
	swa.background_pixel = 0;
	swa.border_pixel = 0;
	swa.override_redirect = true;

	m_window = XCreateWindow(
		m_display,
		m_root,
		0,		// puts it at the top left of the screen?
		0,
		SCRWIDTH,	//set size  
		SCRHEIGHT,
		0,
		CopyFromParent,
		InputOutput,
		CopyFromParent,
		CWEventMask,
		&swa);

	XSelectInput(m_display, m_window, KeyPressMask | KeyReleaseMask);
	xattr.override_redirect = false;
	XChangeWindowAttributes(m_display, m_window, CWOverrideRedirect, &xattr);

	hints.input = true;
	hints.flags = InputHint;
	XSetWMHints(m_display, m_window, &hints);

	// make the window visible on the screen
	XMapWindow(m_display, m_window);
	XStoreName(m_display, m_window, "Pi Project 3D");
	//XSetInputFocus(display, window, RevertToNone, CurrentTime);

	XClearWindow(m_display, m_window);
	XMapRaised(m_display, m_window);

	// get identifiers for the provided atom name strings
	wm_state = XInternAtom(m_display, "_NET_WM_STATE", false);
	Atom wm_fullscreen = XInternAtom(m_display, "_NET_WM_STATE_FULLSCREEN", false);
	XChangeProperty(m_display, m_window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char*)&wm_fullscreen, 1);

	/*XEvent xev;
	memset(&xev, 0, sizeof(xev));
	xev.type = ClientMessage;
	xev.xclient.window = window;
	xev.xclient.message_type = wm_state;
	xev.xclient.format = 32;
	xev.xclient.data.l[0] = 1;
	xev.xclient.data.l[1] = false;
	XSendEvent(
		display,
		DefaultRootWindow(display),
		false,
		SubstructureNotifyMask,
		&xev);*/

	m_state.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(m_state.display, &majorVersion, &minorVersion);
	eglGetConfigs(m_state.display, nullptr, 0, &numConfigs);
	eglChooseConfig(m_state.display, attribute_list, &m_state.config, 1, &numConfigs);

	// OpenGL3 setup.
	m_state.context = eglCreateContext(m_state.display, m_state.config, nullptr, context_attributes);

	m_state.surface = eglCreateWindowSurface(m_state.display, m_state.config, m_window, nullptr);
	eglMakeCurrent(m_state.display, m_state.surface, m_state.surface, m_state.context);

	eglSurfaceAttrib(m_display, m_state.surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
	eglSwapInterval(m_state.display, 0);


	// HIDE CURSOR.
	Cursor invisibleCursor;
	Pixmap bitmapNoData;
	XColor black;
	static char noData[] = {0, 0, 0, 0, 0, 0, 0, 0};
	black.red = black.green = black.blue = 0;
	bitmapNoData = XCreateBitmapFromData(m_display, m_window, noData, 8, 8);
	invisibleCursor = XCreatePixmapCursor(m_display, bitmapNoData, bitmapNoData, &black, &black, 0, 0);
	XDefineCursor(m_display, m_window, invisibleCursor);
	XFreeCursor(m_display, invisibleCursor);
	XFreePixmap(m_display, bitmapNoData);

	XGrabPointer(m_display, m_window, True, 0, GrabModeAsync, GrabModeAsync, m_window, None, CurrentTime);
}

const EGLState& XWindow::GetState() const
{
	return m_state;
}

Display& XWindow::GetDisplay()
{
	return *m_display;
}

Window& XWindow::GetWindow()
{
	return m_window;
}
