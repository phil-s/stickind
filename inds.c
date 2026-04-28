// XkbGetIndicatorState  -*- c-basic-offset: 4; c-file-style: "k&r"; -*-
#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

void output(unsigned char v) {
    if (v == 0) {
        printf("-");
        return;
    }
    if (v & 128) {
        printf("AltGr-");
    }
    if (v & 64) {
        printf("Super-");
    }
    if (v & 8) {
        printf("Meta-");
    }
    if (v & 4) {
        printf("Ctrl-");
    }
    if (v & 1) {
        printf("Shift-");
    }
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "-h") == 0) {
        printf("Returns status of latched (firstly) and locked (secondly) modifiers.\n");
        printf("\t'-'      - No modifer.\n");
        printf("\t'Shift-' - 'Shift' active.\n");
        printf("\t'Ctrl-'  - 'Control' active.\n");
        printf("\t'Meta-'  - 'Meta' active.\n");
        printf("\t'AltGr-' - 'AltGr' active.\n");
        printf("\t'Super-' - 'Super' active.\n");
        printf("\n");
        return (0);
    }
    int screen;
    Display *dpy;

    dpy = XOpenDisplay(NULL);
    if (!dpy) {
        fprintf(stderr, "Unable to connect to display.\n");
        return 7;
    }

    screen = DefaultScreen(dpy);
    // unsigned int *mask;
    // int status = XkbGetIndicatorState(dpy, XkbUseCoreKbd, mask);
    XkbStateRec state;

    int status = XkbGetState(dpy, XkbUseCoreKbd, &state);
    if (status > 0) {
        printf("!\n");
        return (status);
    }
    if (argc > 1 && strcmp(argv[1], "-r") == 0) {
        printf("%d %d\n", state.latched_mods, state.locked_mods);
        // printf("%d %d %d %d\n",
        //        state.latched_mods,
        //        state.locked_mods,
        //        state.base_mods,
        //        state.mods
        //        );
        // // unsigned int mods;           /* modifier state */
        // // unsigned int base_mods;      /* base modifier state */
        // // unsigned int latched_mods;   /* latched modifiers */
        // // unsigned int locked_mods;    /* locked modifiers */
    }
    else {
        output(state.latched_mods);
        if (state.locked_mods != 16) {
            printf(" ++ ");
            // printf("%d", state.locked_mods);
            output(state.locked_mods);
        }
        printf("\n");
    }

    // Cleanup
    XCloseDisplay(dpy);
    return 0;
}
