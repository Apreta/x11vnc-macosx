/*
   Copyright (C) 2002-2013 Karl J. Runge <runge@karlrunge.com> 
   All rights reserved.

This file is part of x11vnc.

x11vnc is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

x11vnc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with x11vnc; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
or see <http://www.gnu.org/licenses/>.

In addition, as a special exception, Karl J. Runge
gives permission to link the code of its release of x11vnc with the
OpenSSL project's "OpenSSL" library (or with modified versions of it
that use the same license as the "OpenSSL" library), and distribute
the linked executables.  You must obey the GNU General Public License
in all respects for all of the code used other than "OpenSSL".  If you
modify this file, you may extend this exception to your version of the
file, but you are not obligated to do so.  If you do not wish to do
so, delete this exception statement from your version.
*/

/* -- macosxCG.c -- */

/*
 * We need to keep this separate from nearly everything else, e.g. rfb.h
 * and the other stuff, otherwise it does not work properly, mouse drags
 * will not work!!
 */

void macosxCG_dummy(void) {}

#if (defined(__MACH__) && defined(__APPLE__))

#include <ApplicationServices/ApplicationServices.h>
#include <Cocoa/Cocoa.h>
#include <Carbon/Carbon.h>

void macosxCG_init(void);
void macosxCG_fini(void);
void macosxCG_event_loop(void);
char *macosxCG_get_fb_addr(void);

int macosxCG_CGDisplayPixelsWide(void);
int macosxCG_CGDisplayPixelsHigh(void);
int macosxCG_CGDisplayBitsPerPixel(void);
int macosxCG_CGDisplayBitsPerSample(void);
int macosxCG_CGDisplaySamplesPerPixel(void);
int macosxCG_CGDisplayBytesPerRow(void);

void macosxCG_pointer_inject(int mask, int x, int y);
int macosxCG_get_cursor_pos(int *x, int *y);
int macosxCG_get_cursor(void);
void macosxCG_init_key_table(void);
void macosxCG_keysym_inject(int down, unsigned int keysym);
void macosxCG_keycode_inject(int down, int keycode);

CGDirectDisplayID displayID = 0;

extern int macosx_display;
extern int macosx_display_x;
extern int macosx_display_y;

extern void macosx_log(char *);
extern int collect_non_X_xdamage(int x_in, int y_in, int w_in, int h_in, int call);

static void macosxCG_callback(CGRectCount n, const CGRect *rects, void *dum) {
	int i, db = 0;
	if (db) fprintf(stderr, "macosx_callback: n=%d\n", (int) n);
	if (!dum) {}
	for (i=0; i < (int) n; i++) {
		if (db > 1) fprintf(stderr, "               : %g %g - %g %g\n", rects[i].origin.x, rects[i].origin.y, rects[i].size.width, rects[i].size.height);
        int x = (int) rects[i].origin.x - macosx_display_x;
        int y = (int) rects[i].origin.y - macosx_display_y;
        int w = rects[i].size.width;
        int h = rects[i].size.height;
        if (x < 0) {
          w += x;
          x = 0;
        }
        if (y < 0) {
          h += y;
          y = 0;
        }
		collect_non_X_xdamage(x, y, w, h, 1);
	}
}

#if 0
> 
> if gcc -DHAVE_CONFIG_H -I. -I. -I..   -I/opt/local/include   -I/opt/local/include -ObjC -g -O2 -Wall -MT x11vnc-macosxCG.o -MD -MP -MF ".deps/x11vnc-macosxCG.Tpo" -c -o x11vnc-macosxCG.o `test -f 'macosxCG.c' || echo './'`macosxCG.c; \
> 	then mv -f ".deps/x11vnc-macosxCG.Tpo" ".deps/x11vnc-macosxCG.Po"; else rm -f ".deps/x11vnc-macosxCG.Tpo"; exit 1; fi
> macosxCG.c:149: warning: CGSetLocalEventsSuppressionInterval is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGRemoteOperation.h:373)
> macosxCG.c:150: warning: CGSetLocalEventsFilterDuringSuppressionState is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGRemoteOperation.h:366)
> macosxCG.c:153: warning: CGSetLocalEventsFilterDuringSuppressionState is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGRemoteOperation.h:366)
> macosxCG.c:244: warning: CGDisplayBaseAddress is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGDirectDisplay.h:466)
> macosxCG.c:254: warning: CGDisplayBitsPerPixel is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGDirectDisplay.h:517)
> macosxCG.c:257: warning: CGDisplayBitsPerSample is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGDirectDisplay.h:522)
> macosxCG.c:260: warning: CGDisplaySamplesPerPixel is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGDirectDisplay.h:526)
> macosxCG.c:263: warning: CGDisplayBytesPerRow is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGDirectDisplay.h:476)
> macosxCG.c:419: warning: CGPostScrollWheelEvent is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGRemoteOperation.h:327)
> macosxCG.c:422: warning: CGPostScrollWheelEvent is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGRemoteOperation.h:327)
> macosxCG.c:425: warning: CGPostMouseEvent is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGRemoteOperation.h:307)
> macosxCG.c:641: warning: CGPostKeyboardEvent is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGRemoteOperation.h:333)
> macosxCG.c:661: warning: CGPostKeyboardEvent is deprecated (declared at /System/Library/Frameworks/ApplicationServices.framework/Frameworks/CoreGraphics.framework/Headers/CGRemoteOperation.h:333)
>

X11VNC_MACOSX_NO_DEPRECATED_LOCALEVENTS
X11VNC_MACOSX_NO_DEPRECATED_POSTEVENTS
X11VNC_MACOSX_NO_DEPRECATED_FRAMEBUFFER
X11VNC_MACOSX_NO_DEPRECATED

#endif

static int callback_set = 0;
extern int nofb;

void macosxCG_refresh_callback_on(void) {
	if (nofb) {
		return;
	}

	if (! callback_set) {
		if (1) macosx_log("macosxCG_refresh_callback: register\n");
		CGRegisterScreenRefreshCallback(macosxCG_callback, NULL);
	}
	callback_set = 1;
}

void macosxCG_refresh_callback_off(void) {
	if (callback_set) {
		if (1) macosx_log("macosxCG_refresh_callback: unregister\n");
		CGUnregisterScreenRefreshCallback(macosxCG_callback, NULL);
	}
	callback_set = 0;
}

extern int macosx_noscreensaver;
extern int macosx_read_opengl;
extern int macosx_read_cglion;
extern int macosx_read_rawfb;

extern void macosxGCS_initpb(void);
extern int  macosxCGP_init_dimming(void);
extern int  macosxCGP_undim(void);
extern int  macosxCGP_dim_shutdown(void);
extern void macosxCGP_screensaver_timer_off(void);
extern void macosxCGP_screensaver_timer_on(void);

extern void macosx_opengl_init(void);
extern void macosx_opengl_fini(void);
extern void macosx_cglion_init(void);
extern void macosx_cglion_fini(void);

int x11vnc_macosx_no_deprecated_localevents = 0;
int x11vnc_macosx_no_deprecated_postevents  = 0;
int x11vnc_macosx_no_deprecated_framebuffer = 0;

void macosxCG_init(void) {

	x11vnc_macosx_no_deprecated_localevents = 0;
	x11vnc_macosx_no_deprecated_postevents  = 0;
	x11vnc_macosx_no_deprecated_framebuffer = 0;

	if (getenv("X11VNC_MACOSX_NO_DEPRECATED_LOCALEVENTS") || getenv("X11VNC_MACOSX_NO_DEPRECATED")) {
		x11vnc_macosx_no_deprecated_localevents = 1;
	}
	if (getenv("X11VNC_MACOSX_NO_DEPRECATED_POSTEVENTS") || getenv("X11VNC_MACOSX_NO_DEPRECATED")) {
		x11vnc_macosx_no_deprecated_postevents = 1;
	}
	if (getenv("X11VNC_MACOSX_NO_DEPRECATED_FRAMEBUFFER") || getenv("X11VNC_MACOSX_NO_DEPRECATED")) {
		x11vnc_macosx_no_deprecated_framebuffer = 1;
	}

	if (displayID == 0) {
		unsigned int ndisp = 0;
		CGDirectDisplayID disp[64];
		macosx_log("macosxCG_init: initializing display.\n");

		if (CGGetActiveDisplayList(64, disp, &ndisp) == kCGErrorSuccess) {
			int i;
			fprintf(stderr, "\nAvailable Displays:\n");
			for (i=0; i < ndisp; i++) {
				CGDirectDisplayID d = disp[i];
				int w = CGDisplayPixelsWide(d);
				int h = CGDisplayPixelsHigh(d);
				int m = CGDisplayIsMain(d);
				int a = CGDisplayIsActive(d);
				int s = CGDisplayIsAsleep(d);
				int b = CGDisplayIsBuiltin(d);
				int o = CGDisplayUsesOpenGLAcceleration(d);
				unsigned int vendor = CGDisplayVendorNumber(d);
				CGSize sz = CGDisplayScreenSize(d);
				CGRect bd = CGDisplayBounds(d);
				fprintf(stderr, "%5d ID: 0x%08x width: %4d (%3d mm) height: %4d (%3d mm) at (%d, %d)\n	  Main=%d Builtin=%d Active=%d Sleep=%d OpenGL=%d Vendor: 0x%x%s\n",
				    i, d, w, (int) sz.width, h, (int) sz.height, (int) bd.origin.x, (int) bd.origin.y, m, b, a, s, o, vendor, vendor == 0x610 ? " (Apple)" : "");
			}
			fprintf(stderr, "\n");
		}

		displayID = kCGDirectMainDisplay;

#ifdef X11VNC_MACOSX_USE_GETMAINDEVICE
		/* not sure this ever did anything. */
		(void) GetMainDevice();
#endif

		if (macosx_display != 0 && ndisp > 0) {
			int n = macosx_display;
			if (n >= 0 && n < ndisp) {
				CGRect bd;
				displayID = disp[n];
				bd = CGDisplayBounds(displayID);
				macosx_display_x = (int) bd.origin.x;
				macosx_display_y = (int) bd.origin.y;
			} else {
				fprintf(stderr, "macosxCG_init: invalid display: %d.  Must be 0 <= n < %d\n", n, ndisp);
				exit(1);
			}
		}

		if (displayID == 0) {
			macosx_log("macosxCG_init: could not get kCGDirectMainDisplay / CGMainDisplayID() display.\n");
			exit(1);
		}

#if X11VNC_MACOSX_NO_DEPRECATED_LOCALEVENTS || X11VNC_MACOSX_NO_DEPRECATED
		macosx_log("NO_DEPRECATED_LOCALEVENTS: not calling CGSetLocalEventsSuppressionInterval()\n");
		macosx_log("NO_DEPRECATED_LOCALEVENTS: not calling CGSetLocalEventsFilterDuringSupressionState()\n");
#else
		if (!x11vnc_macosx_no_deprecated_localevents) {
			CGSetLocalEventsSuppressionInterval(0.0);
			CGSetLocalEventsFilterDuringSupressionState(
			    kCGEventFilterMaskPermitAllEvents,
			    kCGEventSupressionStateSupressionInterval);
			CGSetLocalEventsFilterDuringSupressionState(
			    kCGEventFilterMaskPermitAllEvents,
			    kCGEventSupressionStateRemoteMouseDrag);
		} else {
			macosx_log("NO_DEPRECATED_LOCALEVENTS: not calling CGSetLocalEventsSuppressionInterval()\n");
			macosx_log("NO_DEPRECATED_LOCALEVENTS: not calling CGSetLocalEventsFilterDuringSupressionState()\n");
		}
#endif

		macosx_cglion_init();

		if (!macosx_read_cglion) {
			macosx_opengl_init();
		}

		if (!macosx_read_opengl && !macosx_read_cglion) {
			char *addr = macosxCG_get_fb_addr();
			if (addr == NULL) {
				macosx_log("macosxCG_init: could not get raw framebuffer address / CGDisplayBaseAddress().\n");
				exit(1);
			}
			macosx_read_rawfb = 1;
			macosx_log("macosxCG_init: using raw framebuffer address for screen capture.\n");
		}

		macosxCGP_init_dimming();
		if (macosx_noscreensaver) {
			macosxCGP_screensaver_timer_on();
		}

		macosxGCS_initpb();
	}
}

void macosxCG_fini(void) {
	macosxCGP_dim_shutdown();
	if (macosx_noscreensaver) {
		macosxCGP_screensaver_timer_off();
	}
	macosxCG_refresh_callback_off();
	macosx_opengl_fini();
	macosx_cglion_fini();
	displayID = 0;
}

extern int dpy_x, dpy_y, bpp, wdpy_x, wdpy_y;
extern int client_count, nofb;
extern void do_new_fb(int);
extern int macosx_wait_for_switch, macosx_resize;

extern void macosxGCS_poll_pb(void);
#if 0
extern void usleep(unsigned long usec);
#else
extern int usleep(useconds_t usec);
#endif
extern unsigned int sleep(unsigned int seconds);
extern void clean_up_exit(int ret);

void macosxCG_event_loop(void) {
	OSStatus rc;
	int nbpp, nx, ny;
	static int nbpp_save = -1;

	macosxGCS_poll_pb();
	if (nofb) {
		return;
	}

	rc = RunCurrentEventLoop(kEventDurationSecond/30);

	if (client_count) {
		macosxCG_refresh_callback_on();
	} else {
		macosxCG_refresh_callback_off();
	}

	nbpp = macosxCG_CGDisplayBitsPerPixel();
	nx = macosxCG_CGDisplayPixelsWide();
	ny = macosxCG_CGDisplayPixelsHigh();

	if (nbpp_save < 0) {
		nbpp_save = nbpp;
	}
		
	if (nbpp > 0 && nbpp != nbpp_save) {
		nbpp_save = nbpp;
		if (macosx_resize) {
			do_new_fb(1);
		}
	} else if (wdpy_x != nx || wdpy_y != ny) {
		if (nx == 0 || ny == 0) {
			if (0 && macosx_read_cglion) {
				macosx_log("nx=0 ny=0 but macosx_read_cglion\n");
				return;
			}
		}
		if (macosx_wait_for_switch) {
			int cnt = 0;
			while (1) {
				if(macosxCG_CGDisplayPixelsWide() > 0) {
					if(macosxCG_CGDisplayPixelsHigh() > 0) {
						usleep(500*1000);
						break;
					}
				}
				if ((cnt++ % 120) == 0) {
					macosx_log("waiting for user to "
					    "switch back..\n");
				}
				sleep(1);
			}
			nx = macosxCG_CGDisplayPixelsWide();
			ny = macosxCG_CGDisplayPixelsHigh();
			if (wdpy_x == nx && wdpy_y == ny) {
				macosx_log("we're back...\n");
				return;
			}
		}
		if (macosx_resize) {
			do_new_fb(1);
		}
	}
	if (nbpp > 0) {
		nbpp_save = nbpp;
	}
}

extern int macosx_no_rawfb;

extern int macosx_read_opengl;
extern int macosx_opengl_get_width();
extern int macosx_opengl_get_height();
extern int macosx_opengl_get_bpp();
extern int macosx_opengl_get_bps();
extern int macosx_opengl_get_spp();

extern int macosx_read_cglion;
extern int macosx_cglion_get_width();
extern int macosx_cglion_get_height();
extern int macosx_cglion_get_bpp();
extern int macosx_cglion_get_bps();
extern int macosx_cglion_get_spp();

int macosx_get_width(void) {
	if (macosx_read_opengl) {
		return macosx_opengl_get_width();
	}
	if (macosx_read_cglion) {
		return macosx_cglion_get_width();
	}
	return macosxCG_CGDisplayPixelsWide();
}

int macosx_get_height(void) {
	if (macosx_read_opengl) {
		return macosx_opengl_get_height();
	}
	if (macosx_read_cglion) {
		return macosx_cglion_get_height();
	}
	return macosxCG_CGDisplayPixelsHigh();
}

int macosx_get_bpp(void) {
	if (macosx_read_opengl) {
		return macosx_opengl_get_bpp();
	}
	if (macosx_read_cglion) {
		return macosx_cglion_get_bpp();
	}
	return macosxCG_CGDisplayBitsPerPixel();
}

int macosx_get_bps(void) {
	if (macosx_read_opengl) {
		return macosx_opengl_get_bps();
	}
	if (macosx_read_cglion) {
		return macosx_cglion_get_bps();
	}
	return macosxCG_CGDisplayBitsPerSample();
}

int macosx_get_spp(void) {
	if (macosx_read_opengl) {
		return macosx_opengl_get_spp();
	}
	if (macosx_read_cglion) {
		return macosx_cglion_get_spp();
	}
	return macosxCG_CGDisplaySamplesPerPixel();
}

int macosxCG_CGDisplayPixelsWide(void) {
	return (int) CGDisplayPixelsWide(displayID);
}
int macosxCG_CGDisplayPixelsHigh(void) {
	return (int) CGDisplayPixelsHigh(displayID);
}

#if X11VNC_MACOSX_NO_DEPRECATED_FRAMEBUFFER || X11VNC_MACOSX_NO_DEPRECATED

char *macosxCG_get_fb_addr(void) {
	return NULL;
}
int macosxCG_CGDisplayBitsPerPixel(void) {
	return 0;
}
int macosxCG_CGDisplayBitsPerSample(void) {
	return 0;
}
int macosxCG_CGDisplaySamplesPerPixel(void) {
	return 0;
}
int macosxCG_CGDisplayBytesPerRow(void) {
	return 0;
}

#else

char *macosxCG_get_fb_addr(void) {
	if (x11vnc_macosx_no_deprecated_framebuffer) {
		macosx_log("CGDisplayBaseAddress disabled by env. var\n");
		return NULL;
	}
	if (macosx_read_opengl) {
		macosx_log("CGDisplayBaseAddress disabled by OpenGL.\n");
		return NULL;
	}
	if (macosx_read_cglion) {
		macosx_log("CGDisplayBaseAddress disabled by CGLion.\n");
		return NULL;
	}
	if (macosx_no_rawfb) {
		macosx_log("CGDisplayBaseAddress disabled by user.\n");
		return NULL;
	}
	return (char *) CGDisplayBaseAddress(displayID);
}

int macosxCG_CGDisplayBitsPerPixel(void) {
	if (x11vnc_macosx_no_deprecated_framebuffer) {
		return 0;
	}
	return (int) CGDisplayBitsPerPixel(displayID);
}
int macosxCG_CGDisplayBitsPerSample(void) {
	if (x11vnc_macosx_no_deprecated_framebuffer) {
		return 0;
	}
	return (int) CGDisplayBitsPerSample(displayID);
}
int macosxCG_CGDisplaySamplesPerPixel(void) {
	if (x11vnc_macosx_no_deprecated_framebuffer) {
		return 0;
	}
	return (int) CGDisplaySamplesPerPixel(displayID);
}
int macosxCG_CGDisplayBytesPerRow(void) {
	if (x11vnc_macosx_no_deprecated_framebuffer) {
		return 0;
	}
	return (int) CGDisplayBytesPerRow(displayID);
}

#endif

typedef int CGSConnectionRef;
static CGSConnectionRef conn = 0;
extern CGError CGSNewConnection(void*, CGSConnectionRef*);
extern CGError CGSReleaseConnection(CGSConnectionRef);
extern CGError CGSGetGlobalCursorDataSize(CGSConnectionRef, int*);
extern CGError CGSGetGlobalCursorData(CGSConnectionRef, unsigned char*,
    int*, int*, CGRect*, CGPoint*, int*, int*, int*);
extern CGError CGSGetCurrentCursorLocation(CGSConnectionRef, CGPoint*);
extern int CGSCurrentCursorSeed(void);
extern int CGSHardwareCursorActive(); 

static unsigned int last_local_button_mask = 0;
static unsigned int last_local_mod_mask = 0;
static int last_local_x = 0;
static int last_local_y = 0;

extern unsigned int display_button_mask;
extern unsigned int display_mod_mask;
extern int got_local_pointer_input;
extern time_t last_local_input;

static CGPoint current_cursor_pos(void) {
	CGPoint pos;
	pos.x = 0;
	pos.y = 0;
	if (! conn) {
		if (CGSNewConnection(NULL, &conn) != kCGErrorSuccess) {
			macosx_log("CGSNewConnection error.\n");
			if (!dpy_x || !dpy_y || !wdpy_x || !wdpy_y) {
				clean_up_exit(1);
			}
		}
	}
	if (CGSGetCurrentCursorLocation(conn, &pos) != kCGErrorSuccess) {
		macosx_log("CGSGetCurrentCursorLocation error\n");
	}
    pos.x -= macosx_display_x;
    pos.y -= macosx_display_y;

	display_button_mask = GetCurrentButtonState();
#if 0
/* not used yet */
	display_mod_mask = GetCurrentKeyModifiers();
#endif

	if (last_local_button_mask != display_button_mask) {
		got_local_pointer_input++;
		last_local_input = time(NULL);
	} else if (pos.x != last_local_x || pos.y != last_local_y) {
		got_local_pointer_input++;
		last_local_input = time(NULL);
	}
	last_local_button_mask = display_button_mask;
	last_local_mod_mask = display_mod_mask;
	last_local_x = pos.x;
	last_local_y = pos.y;

    //fprintf(stderr, "Mouse: %d %d\n", last_local_x, last_local_y);

	return pos;
}

int macosxCG_get_cursor_pos(int *x, int *y) {
	CGPoint pos = current_cursor_pos();
	*x = pos.x;
	*y = pos.y;
	return 1;
}

extern int get_cursor_serial(int);
extern int store_cursor(int serial, unsigned long *data, int w, int h, int cbpp, int xhot, int yhot);

int macosxCG_get_cursor(void) {
	int last_idx = (int) get_cursor_serial(1);
	int which = 1;
	CGError err;
	int datasize, row_bytes, cdepth, comps, bpcomp;
	CGRect rect;
	CGPoint hot;
	unsigned char *data;
	int cursor_seed;
	static int last_cursor_seed = -1;
	static time_t last_fetch = 0;
	time_t now = time(NULL);

	if (last_idx) {
		which = last_idx;
	}

	if (! conn) {
		if (CGSNewConnection(NULL, &conn) != kCGErrorSuccess) {
			macosx_log("CGSNewConnection error.\n");
			if (!dpy_x || !dpy_y || !wdpy_x || !wdpy_y) {
				clean_up_exit(1);
			}
			return which;
		}
	}

	/* XXX all of these interfaces are undocumented. */

	cursor_seed = CGSCurrentCursorSeed();
	if (last_idx && cursor_seed == last_cursor_seed) {
		if (now < last_fetch + 2) {
			return which;
		}
	}
	last_cursor_seed = cursor_seed;
	last_fetch = now;

	if (CGSGetGlobalCursorDataSize(conn, &datasize) != kCGErrorSuccess) {
		macosx_log("CGSGetGlobalCursorDataSize error\n");
		return which;
	}

	data = (unsigned char*) malloc(datasize);

	err = CGSGetGlobalCursorData(conn, data, &datasize, &row_bytes,
	    &rect, &hot, &cdepth, &comps, &bpcomp);
#if 0
	fprintf(stderr, "datasize: %d row_bytes: %d cdepth: %d comps: %d bpcomp: %d w: %d h: %d\n",
	  datasize, row_bytes, cdepth, comps, bpcomp, (int) rect.size.width, (int) rect.size.height);
#endif
	if (err != kCGErrorSuccess) {
		macosx_log("CGSGetGlobalCursorData error\n");
		return which;
	}

	if (cdepth == 24) {
		cdepth = 32;
	}

	if (sizeof(long) == 8 && comps * bpcomp <= 32) {
		/* pad it out to unsigned long array size (like xfixes) macosx */
		int i;
		unsigned char *dsave;
		unsigned char *data64 = (unsigned char*) malloc(2 *datasize);
		unsigned int  *uI = (unsigned int  *) data;
		unsigned long *uL = (unsigned long *) data64;
		for (i=0; i < datasize/4; i++) {
			uL[i] = uI[i];
		}
		dsave = data;
		data = data64;
		free(dsave);
	}

	which = store_cursor(cursor_seed, (unsigned long*) data,
	    (int) rect.size.width, (int) rect.size.height, cdepth, (int) hot.x, (int) hot.y);

	free(data);
	return(which);
}

extern int macosx_mouse_wheel_speed;
extern int macosx_swap23;
extern int off_x, coff_x, off_y, coff_y;

extern int debug_pointer;

static void CGPostScrollWheelEvent_wr(CGWheelCount wheel_count, int wheel_distance) {
	static int post_mode = -1, mcnt = 0, mmax = 16;

#if !X11VNC_MACOSX_NO_DEPRECATED_POSTEVENTS && !X11VNC_MACOSX_NO_DEPRECATED
	if (post_mode < 0) {
		post_mode = 1;
		if (getenv("X11VNC_MACOSX_NO_DEPRECATED_POSTEVENTS") || getenv("X11VNC_MACOSX_NO_DEPRECATED")) {
			post_mode = 0;
		}
	}
	
	if (post_mode) {
		if (mcnt++ < mmax || debug_pointer) fprintf(stderr, "CGPostScrollWheelEvent(count=%d, distance=%d)\n", (int) wheel_count, wheel_distance);
		CGPostScrollWheelEvent(wheel_count, wheel_distance);
	} else
#endif
	{
		/* XXX 10.5 and later */
#ifndef X11VNC_MACOSX_NO_CGEVENTCREATESCROLLWHEELEVENT
		CGEventRef event;
		event = CGEventCreateScrollWheelEvent(NULL, kCGScrollEventUnitLine, wheel_count, wheel_distance);
		if (event != NULL) {
			CGEventPost(kCGHIDEventTap, event);
			CFRelease(event);
		}
#endif
		if (mcnt++ < mmax || debug_pointer) fprintf(stderr, "CGEventCreateScrollWheelEvent(count=%d, distance=%d)\n", (int) wheel_count, wheel_distance);
	}
}

static void CGPostMouseEvent_wr(CGPoint loc, int update, int count, int d1, int d2, int d3, int p1, int p2, int p3) {
	static int post_mode = -1, mcnt = 0, mmax = 16;

#if !X11VNC_MACOSX_NO_DEPRECATED_POSTEVENTS && !X11VNC_MACOSX_NO_DEPRECATED
	if (post_mode < 0) {
		post_mode = 1;
		if (getenv("X11VNC_MACOSX_NO_DEPRECATED_POSTEVENTS") || getenv("X11VNC_MACOSX_NO_DEPRECATED")) {
			post_mode = 0;
		}
	}

	loc.x += macosx_display_x;
	loc.y += macosx_display_y;
	
	if (post_mode) {
		if (mcnt++ < mmax || debug_pointer) fprintf(stderr, "CGPostMouseEvent(%d, %d)\n", (int) loc.x, (int) loc.y);
		CGPostMouseEvent(loc, update, count, d1, d2, d3);
	} else
#endif
	{
		/* XXX 10.4 and later */
#ifndef X11VNC_MACOSX_NO_CGEVENTCREATEMOUSEEVENT
		CGEventRef event;
		static int xp = -1, yp;

		if (xp == -1) {
			xp = loc.x;
			yp = loc.y;
		}
		if (xp != loc.x || yp != loc.y) {
			int moved = 0;
			if (p1 && p1 == d1) {
				event = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDragged, loc, 0);
				if (event != NULL) {
					moved = 1;
					CGEventPost(kCGHIDEventTap, event);
					CFRelease(event);
				}
			}
			if (p3 && p3 == d3) {
				event = CGEventCreateMouseEvent(NULL, kCGEventOtherMouseDragged, loc, 0);
				if (event != NULL) {
					moved = 1;
					CGEventPost(kCGHIDEventTap, event);
					CFRelease(event);
				}
			}
			if (p2 && p2 == d2) {
				event = CGEventCreateMouseEvent(NULL, kCGEventRightMouseDragged, loc, 0);
				if (event != NULL) {
					moved = 1;
					CGEventPost(kCGHIDEventTap, event);
					CFRelease(event);
				}
			}
			if (!moved) {
				event = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, loc, 0);
				if (event != NULL) {
					CGEventPost(kCGHIDEventTap, event);
					CFRelease(event);
				}
			}
		}
		xp = loc.x;
		yp = loc.y;
		if (p1 != d1) {
			CGEventType type = (!p1 && d1) ? kCGEventLeftMouseDown : kCGEventLeftMouseUp;
			event = CGEventCreateMouseEvent(NULL, type, loc, 0);
			if (event != NULL) {
				CGEventPost(kCGHIDEventTap, event);
				CFRelease(event);
			}
		}
		if (p3 != d3) {
			CGEventType type = (!p3 && d3) ? kCGEventOtherMouseDown : kCGEventOtherMouseUp;
			event = CGEventCreateMouseEvent(NULL, type, loc, kCGMouseButtonCenter);
			if (event != NULL) {
				CGEventPost(kCGHIDEventTap, event);
				CFRelease(event);
			}
		}
		if (p2 != d2) {
			CGEventType type = (!p2 && d2) ? kCGEventRightMouseDown : kCGEventRightMouseUp;
			event = CGEventCreateMouseEvent(NULL, type, loc, 0);
			if (event != NULL) {
				CGEventPost(kCGHIDEventTap, event);
				CFRelease(event);
			}
		}
#endif
		if (mcnt++ < mmax || debug_pointer) fprintf(stderr, "CGEventCreateMouseEvent(%d, %d)\n", (int) loc.x, (int) loc.y);
	}
}

void macosxCG_pointer_inject(int mask, int x, int y) {
	int swap23 = macosx_swap23;
	int s1 = 0, s2 = 1, s3 = 2, s4 = 3, s5 = 4;
	CGPoint loc;
	int wheel_distance = macosx_mouse_wheel_speed;
	static int cnt = 0;
	static int first = 1, prev1 = 0, prev2 = 0, prev3 = 0; 
	int curr1, curr2, curr3;

	if (swap23) {
		s2 = 2;
		s3 = 1;
	}

	loc.x = x + off_x + coff_x;
	loc.y = y + off_y + coff_y;

	if ((cnt++ % 10) == 0) {
		macosxCGP_undim();
	}

	if ((mask & (1 << s4))) {
		CGPostScrollWheelEvent_wr(1,  wheel_distance);
	}
	if ((mask & (1 << s5))) {
		CGPostScrollWheelEvent_wr(1, -wheel_distance);
	}

	curr1 = (mask & (1 << s1)) ? TRUE : FALSE;
	curr2 = (mask & (1 << s2)) ? TRUE : FALSE;
	curr3 = (mask & (1 << s3)) ? TRUE : FALSE;

	if (first) {
		prev1 = curr1;
		prev2 = curr2;
		prev3 = curr3;
		first = 0;
	}

	CGPostMouseEvent_wr(loc, TRUE, 3, curr1, curr2, curr3, prev1, prev2, prev3);

	prev1 = curr1;
	prev2 = curr2;
	prev3 = curr3;
}

#define keyTableSize 0xFFFF

#include <rfb/keysym.h>

static int USKeyCodes[] = {
    /* The alphabet */
    XK_A,                  0,      /* A */
    XK_B,                 11,      /* B */
    XK_C,                  8,      /* C */
    XK_D,                  2,      /* D */
    XK_E,                 14,      /* E */
    XK_F,                  3,      /* F */
    XK_G,                  5,      /* G */
    XK_H,                  4,      /* H */
    XK_I,                 34,      /* I */
    XK_J,                 38,      /* J */
    XK_K,                 40,      /* K */
    XK_L,                 37,      /* L */
    XK_M,                 46,      /* M */
    XK_N,                 45,      /* N */
    XK_O,                 31,      /* O */
    XK_P,                 35,      /* P */
    XK_Q,                 12,      /* Q */
    XK_R,                 15,      /* R */
    XK_S,                  1,      /* S */
    XK_T,                 17,      /* T */
    XK_U,                 32,      /* U */
    XK_V,                  9,      /* V */
    XK_W,                 13,      /* W */
    XK_X,                  7,      /* X */
    XK_Y,                 16,      /* Y */
    XK_Z,                  6,      /* Z */
    XK_a,                  0,      /* a */
    XK_b,                 11,      /* b */
    XK_c,                  8,      /* c */
    XK_d,                  2,      /* d */
    XK_e,                 14,      /* e */
    XK_f,                  3,      /* f */
    XK_g,                  5,      /* g */
    XK_h,                  4,      /* h */
    XK_i,                 34,      /* i */
    XK_j,                 38,      /* j */
    XK_k,                 40,      /* k */
    XK_l,                 37,      /* l */
    XK_m,                 46,      /* m */
    XK_n,                 45,      /* n */
    XK_o,                 31,      /* o */
    XK_p,                 35,      /* p */
    XK_q,                 12,      /* q */
    XK_r,                 15,      /* r */
    XK_s,                  1,      /* s */
    XK_t,                 17,      /* t */
    XK_u,                 32,      /* u */
    XK_v,                  9,      /* v */
    XK_w,                 13,      /* w */
    XK_x,                  7,      /* x */
    XK_y,                 16,      /* y */
    XK_z,                  6,      /* z */

    /* Numbers */
    XK_0,                 29,      /* 0 */
    XK_1,                 18,      /* 1 */
    XK_2,                 19,      /* 2 */
    XK_3,                 20,      /* 3 */
    XK_4,                 21,      /* 4 */
    XK_5,                 23,      /* 5 */
    XK_6,                 22,      /* 6 */
    XK_7,                 26,      /* 7 */
    XK_8,                 28,      /* 8 */
    XK_9,                 25,      /* 9 */

    /* Symbols */
    XK_exclam,            18,      /* ! */
    XK_at,                19,      /* @ */
    XK_numbersign,        20,      /* # */
    XK_dollar,            21,      /* $ */
    XK_percent,           23,      /* % */
    XK_asciicircum,       22,      /* ^ */
    XK_ampersand,         26,      /* & */
    XK_asterisk,          28,      /* * */
    XK_parenleft,         25,      /* ( */
    XK_parenright,        29,      /* ) */
    XK_minus,             27,      /* - */
    XK_underscore,        27,      /* _ */
    XK_equal,             24,      /* = */
    XK_plus,              24,      /* + */
    XK_grave,             50,      /* ` */  /* XXX ? */
    XK_asciitilde,        50,      /* ~ */
    XK_bracketleft,       33,      /* [ */
    XK_braceleft,         33,      /* { */
    XK_bracketright,      30,      /* ] */
    XK_braceright,        30,      /* } */
    XK_semicolon,         41,      /* ; */
    XK_colon,             41,      /* : */
    XK_apostrophe,        39,      /* ' */
    XK_quotedbl,          39,      /* " */
    XK_comma,             43,      /* , */
    XK_less,              43,      /* < */
    XK_period,            47,      /* . */
    XK_greater,           47,      /* > */
    XK_slash,             44,      /* / */
    XK_question,          44,      /* ? */
    XK_backslash,         42,      /* \ */
    XK_bar,               42,      /* | */
    /* OS X Sends this (END OF MEDIUM) for Shift-Tab (with US Keyboard) */
    0x0019,               48,      /* Tab */
    XK_space,             49,      /* Space */
};

static int SpecialKeyCodes[] = {
    /* "Special" keys */
    XK_Return,            36,      /* Return */
    XK_Delete,           117,      /* Delete */
    XK_Tab,               48,      /* Tab */
    XK_Escape,            53,      /* Esc */
    XK_Caps_Lock,         57,      /* Caps Lock */
    XK_Num_Lock,          71,      /* Num Lock */
    XK_Scroll_Lock,      107,      /* Scroll Lock */
    XK_Pause,            113,      /* Pause */
    XK_BackSpace,         51,      /* Backspace */
    XK_Insert,           114,      /* Insert */

    /* Cursor movement */
    XK_Up,               126,      /* Cursor Up */
    XK_Down,             125,      /* Cursor Down */
    XK_Left,             123,      /* Cursor Left */
    XK_Right,            124,      /* Cursor Right */
    XK_Page_Up,          116,      /* Page Up */
    XK_Page_Down,        121,      /* Page Down */
    XK_Home,             115,      /* Home */
    XK_End,              119,      /* End */

    /* Numeric keypad */
    XK_KP_0,              82,      /* KP 0 */
    XK_KP_1,              83,      /* KP 1 */
    XK_KP_2,              84,      /* KP 2 */
    XK_KP_3,              85,      /* KP 3 */
    XK_KP_4,              86,      /* KP 4 */
    XK_KP_5,              87,      /* KP 5 */
    XK_KP_6,              88,      /* KP 6 */
    XK_KP_7,              89,      /* KP 7 */
    XK_KP_8,              91,      /* KP 8 */
    XK_KP_9,              92,      /* KP 9 */
    XK_KP_Enter,          76,      /* KP Enter */
    XK_KP_Decimal,        65,      /* KP . */
    XK_KP_Add,            69,      /* KP + */
    XK_KP_Subtract,       78,      /* KP - */
    XK_KP_Multiply,       67,      /* KP * */
    XK_KP_Divide,         75,      /* KP / */

    /* Function keys */
    XK_F1,               122,      /* F1 */
    XK_F2,               120,      /* F2 */
    XK_F3,                99,      /* F3 */
    XK_F4,               118,      /* F4 */
    XK_F5,                96,      /* F5 */
    XK_F6,                97,      /* F6 */
    XK_F7,                98,      /* F7 */
    XK_F8,               100,      /* F8 */
    XK_F9,               101,      /* F9 */
    XK_F10,              109,      /* F10 */
    XK_F11,              103,      /* F11 */
    XK_F12,              111,      /* F12 */

    /* Modifier keys */
    XK_Alt_L,             55,      /* Alt Left (-> Command) */
    XK_Alt_R,             55,      /* Alt Right (-> Command) */
    XK_Shift_L,           56,      /* Shift Left */
    XK_Shift_R,           56,      /* Shift Right */
    XK_Meta_L,            58,      /* Option Left (-> Option) */
    XK_Meta_R,            58,      /* Option Right (-> Option) */
    XK_Super_L,           58,      /* Option Left (-> Option) */
    XK_Super_R,           58,      /* Option Right (-> Option) */
    XK_Control_L,         59,      /* Ctrl Left */
    XK_Control_R,         59,      /* Ctrl Right */
};

CGKeyCode keyTable[keyTableSize];
unsigned char keyTableMods[keyTableSize];

void macosxCG_init_key_table(void) {
	static int init = 0;
	int i;
	if (init) {
		return;
	}
	init = 1;

	for (i=0; i < keyTableSize; i++) {
		keyTable[i] = 0xFFFF;
		keyTableMods[i] = 0;
	}
	for (i=0; i< (int) (sizeof(USKeyCodes) / sizeof(int)); i += 2) {
		int j = USKeyCodes[i];
		keyTable[(unsigned short) j] = (CGKeyCode) USKeyCodes[i+1];
	}
	for (i=0; i< (int) (sizeof(SpecialKeyCodes) / sizeof(int)); i += 2) {
		int j = SpecialKeyCodes[i];
		keyTable[(unsigned short) j] = (CGKeyCode) SpecialKeyCodes[i+1];
	}
}

extern void init_key_table(void);
extern int macosx_us_kbd;

extern int debug_keyboard;

void CGPostKeyboardEvent_wr(CGCharCode keyChar, CGKeyCode keyCode, int down) {
	static int post_mode = -1, mcnt = 0;

#if !X11VNC_MACOSX_NO_DEPRECATED_POSTEVENTS && !X11VNC_MACOSX_NO_DEPRECATED
	if (post_mode < 0) {
		post_mode = 1;
		if (getenv("X11VNC_MACOSX_NO_DEPRECATED_POSTEVENTS") || getenv("X11VNC_MACOSX_NO_DEPRECATED")) {
			post_mode = 0;
		}
	}
	
	if (post_mode) {
		if (mcnt++ < 10 || debug_keyboard) fprintf(stderr, "CGPostKeyboardEvent(keyChar=%d, keyCode=%d, down=%d)\n", keyChar, keyCode, down);
		CGPostKeyboardEvent(keyChar, keyCode, down);
	} else
#endif
	{
		/* XXX 10.4 and later */
#ifndef X11VNC_MACOSX_NO_CGEVENTCREATEKEYBOARDEVENT
		CGEventRef event;
		event = CGEventCreateKeyboardEvent(NULL, keyCode, down);
		if (event != NULL) {
			CGEventPost(kCGHIDEventTap, event);
			CFRelease(event);
		}
#endif
		if (mcnt++ < 10 || debug_keyboard) fprintf(stderr, "CGEventCreateKeyboardEvent(NULL, keyCode=%d, down=%d)\n", keyCode, down);
	}
}

void macosxCG_keycode_inject(int down, int keycode) {
	CGKeyCode keyCode = (CGKeyCode) keycode;
	CGCharCode keyChar = 0;

	if (debug_keyboard) fprintf(stderr, "macosxCG_keycode_inject(down=%d, keycode=%d)\n", down, keycode);

	CGPostKeyboardEvent_wr(keyChar, keyCode, down);
}

void macosxCG_keysym_inject(int down, unsigned int keysym) {
	CGKeyCode keyCode = keyTable[(unsigned short)keysym];
	CGCharCode keyChar = 0;
#if 0
	int pressModsForKeys = FALSE;
	UInt32 modsForKey = keyTableMods[keysym] << 8;
#endif

	init_key_table();

	if (debug_keyboard) fprintf(stderr, "macosxCG_keysym_inject(down=%d, keysym=%d)\n", down, (int) keysym);

	if (keysym < 0xFF && macosx_us_kbd) {
		keyChar = (CGCharCode) keysym;
		if (debug_keyboard) fprintf(stderr, "macosxCG_keysym_inject keyChar=>%d\n", (int) keyChar);
	}
	if (keyCode == 0xFFFF) {
		return;
	}
	macosxCGP_undim();

	CGPostKeyboardEvent_wr(keyChar, keyCode, down);
}

#endif	/* __APPLE__ */


