/*
 * Copyright 2008 Tungsten Graphics, Inc., Cedar Park, Texas.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL TUNGSTEN GRAPHICS AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * Author: Alan Hourihane <alanh@tungstengraphics.com>
 *
 */

#include <errno.h>
#include <drm.h>
#include <xf86drm.h>
#include <damage.h>

#include "drmmode_display.h"
#include "exa.h"

#ifdef LONG64
#  define FMT_CARD32 "x"
#else
#  define FMT_CARD32 "lx"
#endif

#define TEGRA_ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

typedef struct
{
    int lastInstance;
    int refCount;
    ScrnInfoPtr pScrn_1;
    ScrnInfoPtr pScrn_2;
} EntRec, *EntPtr;

typedef struct _TegraRec
{
    char *path;
    int fd;

    EntPtr entityPrivate;
    EntityInfoPtr pEnt;

    CloseScreenProcPtr CloseScreen;

    /* Broken-out options. */
    OptionInfoPtr Options;

    CreateScreenResourcesProcPtr createScreenResources;
    ScreenBlockHandlerProcPtr BlockHandler;
    void *driver;

    drmmode_rec drmmode;

    drmEventContext event_context;

    DamagePtr damage;
    Bool dirty_enabled;

    uint32_t cursor_width, cursor_height;

    TegraEXAPtr exa;

    Bool dri2_enabled;

    struct drm_tegra *drm;
} TegraRec, *TegraPtr;

#define TegraPTR(p) ((TegraPtr)((p)->driverPrivate))

void TegraXvScreenInit(ScreenPtr pScreen);

void TegraEXAScreenInit(ScreenPtr pScreen);
void TegraEXAScreenExit(ScreenPtr pScreen);

void TegraDRI2ScreenInit(ScreenPtr pScreen);
void TegraDRI2ScreenExit(ScreenPtr pScreen);

Bool TegraVBlankScreenInit(ScreenPtr screen);
void TegraVBlankScreenExit(ScreenPtr screen);

/* vim: set et sts=4 sw=4 ts=4: */
