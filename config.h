
/* See LICENSE file for copyright and license details. */

#include "rules.h"
#include <X11/XF86keysym.h>

#define MPD_CONTROL       /* Comment this if you dont need mpd support */

#ifdef MPD_CONTROL
static const int voldelta = 5;
#include "mpd.c"
#endif

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "dina:size=11", "noto sans:size=11", "Awesome:size=11" };
static const char dmenufont[]       = "dina:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";

static const char col_bg[]          = "#1b1d1f";
static const char col_br[]          = "#000000";
static const char col_white[]       = "#ffffff";
static const char *colors[][3]      = {
  /*               fg         bg         border   */
//  [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
//  [SchemeSel] =  { col_gray4, col_cyan,  col_cyan  },
  [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
  [SchemeSel] =  { col_white, col_br, col_cyan  },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "", "", "", "", "" };

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "><>",      NULL },    /* no layout function means floating behavior */
  { "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenudesktop[]  = { "j4-dmenu-desktop", NULL };
static const char *termcmd[]  = { "termite", NULL };
static const char *scrot[]  = { "scrot '%Y-%m-%d_%T_scrot.png' -e 'mv $f ~/Картинки/Screenshots/'", NULL };

static Key keys[] = {
  /* modifier                     key        function        argument */
  { MODKEY|ShiftMask,             XK_d,      spawn,          {.v = dmenucmd } },
  { MODKEY,                       XK_d,      spawn,          {.v = dmenudesktop } },
  { MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
  { 0,                            XK_Print,  spawn,          {.v = scrot } },
  { MODKEY,                       XK_b,      togglebar,      {0} },
  { MODKEY,                       XK_Tab,    focusstack,     {.i = +1 } },
  { MODKEY|ShiftMask,             XK_Tab,    focusstack,     {.i = -1 } },
  { MODKEY,                       XK_period, incnmaster,     {.i = +1 } },
  { MODKEY,                       XK_comma,  incnmaster,     {.i = -1 } },
  { MODKEY|ShiftMask,             XK_period, setmfact,       {.f = +0.05} },
  { MODKEY|ShiftMask,             XK_comma,  setmfact,       {.f = -0.05} },
  { MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
  { MODKEY,                       XK_F4,     killclient,     {0} },
  { MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
  { MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
  { MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
  { MODKEY,                       XK_x,      togglefloating, {0} },
  { MODKEY,                       XK_0,      view,           {.ui = ~0 } },
  { MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
  TAGKEYS(                        XK_1,                      0)
  TAGKEYS(                        XK_2,                      1)
  TAGKEYS(                        XK_3,                      2)
  TAGKEYS(                        XK_4,                      3)
  TAGKEYS(                        XK_5,                      4)
  TAGKEYS(                        XK_6,                      5)
  TAGKEYS(                        XK_7,                      6)
  TAGKEYS(                        XK_8,                      7)
  TAGKEYS(                        XK_9,                      8)
  { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { 0,                 XF86XK_AudioPlay,     mpd_control,    {.i = MpdToggle} },
  { 0,                 XF86XK_AudioStop,     mpd_control,    {.i = MpdStop} },
  { 0,                 XF86XK_AudioNext,     mpd_control,    {.i = MpdNext} },
  { 0,                 XF86XK_AudioPrev,     mpd_control,    {.i = MpdPrev} },
  { MODKEY,                       XK_F11,    mpd_control,    {.i = MpdLowerVolume} },
  { MODKEY,                       XK_F12,    mpd_control,    {.i = MpdRaiseVolume} },
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        zoom,           {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

