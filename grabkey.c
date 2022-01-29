/* grabkey: Simple tool for grabbing X keys and reporting keypresses */

#include <X11/Xlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>

typedef enum { false, true } bool;

void
usage(char *argv0)
{
    printf("usage: %s [-x] [-h] keybind_names...\n"
           "    -x: stop program silently when any of the\n"
           "        specified keybinds are pressed.\n"
           "    -p: when keys are pressed, only print key\n"
           "        name to stdout.\n"
           "    -h: display this message.\n\n"
           "Keybind names should consist of 'Control', 'Alt',\n"
           "'Shift', or 'Win', separated with +, followed by a\n"
           "valid key name.\n"
           "Examples: Control+Shift+w, Alt+e, F12\n", argv0);
}

/* grab_key: grab a key with XGrabKey using a key name.
 * Accepts key strings that consist of the following:
 *     Any of 'Control', 'Shift', 'Alt', 'Win', delimited by '+'s
 *     A valid key name
 * Examples: Control+Shift+w, Alt+e, F12
 */
bool
grab_key(char *keystr, Display *d)
{
    unsigned int modmask = 0;
    KeySym keysym;

    if (strstr(keystr, "Control"))
        modmask |= ControlMask;
    if (strstr(keystr, "Shift"))
        modmask |= ShiftMask;
    if (strstr(keystr, "Alt"))
        modmask |= Mod1Mask;
    if (strstr(keystr, "Win"))
        modmask |= Mod4Mask;

    if (strrchr(keystr, '+'))
        keystr = strrchr(keystr, '+') + 1;

    /* AFAIK, XStringToKeysym returns 0 on failure */
    if (!(keysym = XStringToKeysym(keystr)))
        return false;

    return XGrabKey(d, XKeysymToKeycode(d, keysym), modmask,
            DefaultRootWindow(d), 1, GrabModeAsync, GrabModeAsync);
}

int
main(int argc, char **argv)
{
    Display *dpy;
    bool keypress_break = false;
    bool keyname_only = false;
    XEvent e;
    char *keyname;
    int opt;

    assert(dpy = XOpenDisplay(0));
    XSelectInput(dpy, DefaultRootWindow(dpy), KeyPressMask);

    while ((opt = getopt(argc, argv, "xph")) != -1) {
        switch (opt) {
            case 'x':
                keypress_break = true;
                break;
            case 'p':
                keyname_only = true;
                break;
            case 'h':
            default:
                usage(argv[0]);
                return 0;
                break;
        }
    }

    /* Treat leftover arguments as key strings */
    if (optind < argc) {
        do {
            if (!grab_key(argv[optind], dpy)) {
                fprintf(stderr, "Could not grab key '%s'\n", argv[optind]);
                return 1;
            }
        } while (++optind < argc);
    } else {
        usage(argv[0]);
        return 0;
    }

    /* Handle keypresses */
    for (;;) {
        XNextEvent(dpy, &e);

        if (e.type == KeyPress) {
            if (keypress_break)
                break;

            keyname = XKeysymToString(XLookupKeysym((XKeyEvent*) &e, 0));
            printf((keyname_only) ? "%s\n" : "Key '%s' pressed\n", keyname);
        }
    }


    XCloseDisplay(dpy);
    return 0;
}
