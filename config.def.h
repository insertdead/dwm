/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static unsigned int systrayspacing = 2;   /* systray spacing */
static int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static int showsystray        = 1;     /* 0 means no systray */
static unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps    = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "Ubuntu Mono Nerd Font:size=12", "JoyPixels:pixelsize=10:antialias=true:autohint=true" };
static char dmenufont[]       = "Ubuntu Mono Nerd Font:size=10";
static char normbgcolor[]           = "#2e3440";
static char normbordercolor[]       = "#2e3440";
static char normfgcolor[]           = "#d8dee9";
static char selfgcolor[]            = "#e5e9f0";
static char selbordercolor[]        = "#e5e9f0";
static char selbgcolor[]            = "#4c566a";
static char col1[]            = "#ebcb8b";
static char col2[]            = "#bf616a";
static char col3[]            = "#a3be8c";
static char col4[]            = "#5e81ac";
static char col5[]            = "#d08770";
static char col6[]            = "#b48ead";

enum { SchemeNorm, SchemeCol1, SchemeCol2, SchemeCol3, SchemeCol4,
       SchemeCol5, SchemeCol6, SchemeSel }; /* color schemes */

static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm]  = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeCol1]  = { col1,        normbgcolor, normbordercolor },
       [SchemeCol2]  = { col2,        normbgcolor, normbordercolor },
       [SchemeCol3]  = { col3,        normbgcolor, normbordercolor },
       [SchemeCol4]  = { col4,        normbgcolor, normbordercolor },
       [SchemeCol5]  = { col5,        normbgcolor, normbordercolor },
       [SchemeCol6]  = { col6,        normbgcolor, normbordercolor },
       [SchemeSel]   = { selfgcolor,  selbgcolor,  selbordercolor  },
};

static char *const autostart[] = {
  "light-locker", "--lock-on-suspend", "--lock-on-lid",   NULL,
  "dwmblocks",                                            NULL,
  NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "爵", "爵", "", "", "ﲾ", "", "", "", "" };
static const char *tagsalt[] = { "", "", "", "", "", "", "", "", "" };

#define TERMINAL "alacritty"
#define TERMCLASS "Alacritty"
#define BROWSER "brave"
#define BROWSEREXTRA "firefox"
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class          instance    title       tags mask     isfloating   isterminal noswallow monitor */
	{ "Gimp",         NULL,       NULL,       0,            1,           0,         0,        -1 },
	{ "Firefox",      NULL,       NULL,       1 << 1,       0,           0,         0,        -1 },
	{ TERMCLASS,      NULL,       NULL,       0,            0,           1,         1,        -1 },
  { "discord",      NULL,       NULL,       1 << 5,       0,           0,         0,        -1 },
  { "Pulseeffects", NULL,       NULL,       1 << 6,       0,           0,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "H[]",      deck },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	// { NULL,       NULL },
};

/* key definitions */
#include <X11/XF86keysym.h> /* to add bindings for media keys */
#define MODKEY Mod4Mask
#define MODKEY2 Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

#define STACKKEYS(MOD,ACTION) \
	{ MOD, XK_j,     ACTION##stack, {.i = INC(+1) } }, \
	{ MOD, XK_k,     ACTION##stack, {.i = INC(-1) } },
	// { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \
	// { MOD, XK_q,     ACTION##stack, {.i = 0 } }, \
	// { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \
	// { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \
	// { MOD, XK_x,     ACTION##stack, {.i = -1 } },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
#define ROFI_MODE "drun"
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static char *dmenucmd[] = { "rofi", "-show", ROFI_MODE, NULL};
static char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
    { "dwm.normbgcolor",        STRING,  &normbgcolor },
    { "dwm.normbordercolor",    STRING,  &normbordercolor },
    { "dwm.normfgcolor",        STRING,  &normfgcolor },
    { "dwm.selbgcolor",         STRING,  &selbgcolor },
    { "dwm.selbordercolor",     STRING,  &selbordercolor },
    { "dwm.selfgcolor",         STRING,  &selfgcolor },
    { "dwm.term.cmd",           STRING,  &TERMINAL },
    { "dwm.term.class",         STRING,  &TERMCLASS },
    { "dwm.browser",            STRING,  &BROWSER },
    { "dwm.browserextra",       STRING,  &BROWSEREXTRA },
    { "dwm.fonts",              STRING,  &fonts },
    { "dwm.startup",            STRING,  &autostart },
    { "dwm.borderpx",           INTEGER, &borderpx },
    { "dwm.snap",               INTEGER, &snap },
    { "dwm.showbar",            INTEGER, &showbar },
    { "dwm.topbar",             INTEGER, &topbar },
    { "dwm.nmaster",            INTEGER, &nmaster },
    { "dwm.resizehints",        INTEGER, &resizehints },
    { "dwm.gaps.iv",            INTEGER, &gappiv },
    { "dwm.gaps.ih",            INTEGER, &gappih },
    { "dwm.gaps.ov",            INTEGER, &gappov },
    { "dwm.gaps.oh",            INTEGER, &gappoh },
    { "dwm.gaps.smart",         INTEGER, &smartgaps },
    { "dwm.systray",            INTEGER, &showsystray },
    { "dwm.systray.pinning",    INTEGER, &systraypinning },
    { "dwm.systray.pinff",      INTEGER, &systraypinningfailfirst },
    { "dwm.systray.spacing",    INTEGER, &systrayspacing },
    { "dwm.mfact",              FLOAT,   &mfact },
};

/* Include Shiftview and cyclelayout */
#include "shiftview.c"
#include "cyclelayout.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY2,                      XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY|MODKEY2,               XK_Return, zoom,           {0} },
	{ MODKEY|MODKEY2,               XK_u,      incrgaps,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_u,      incrgaps,       {.i = -1 } },
	{ MODKEY|MODKEY2,               XK_i,      incrigaps,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_i,      incrigaps,      {.i = -1 } },
	{ MODKEY|MODKEY2,               XK_o,      incrogaps,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_o,      incrogaps,      {.i = -1 } },
	{ MODKEY|MODKEY2,               XK_0,      togglegaps,     {0} },
	{ MODKEY2|ShiftMask,            XK_0,      defaultgaps,    {0} },
	{ MODKEY|MODKEY2,               XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY,                       XK_Tab,    setnextlayout,  {0} }, /* Cycle through layouts */
	{ MODKEY,             XK_f,  togglefloating, {0} },
	{ MODKEY,                       XK_space,  togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    togglealttag,   {0} },
	{ MODKEY,                       XK_s,      togglesticky,   {0} },
	{ MODKEY|ShiftMask,             XK_n,      shiftview,      {.i = +1} },
	{ MODKEY|ShiftMask,             XK_p,      shiftview,      {.i = -1} },
	/* Application shortcuts */
	{ MODKEY,                       XK_b,      spawn,          SHCMD(BROWSER) },
	{ MODKEY,                       XK_q,      spawn,          SHCMD("librewolf -P noprivacy") },
	{ MODKEY|ShiftMask,             XK_t,      spawn,          SHCMD(TERMINAL " -e ranger") },
  { MODKEY2,                      XK_Print,  spawn,          SHCMD("maim -s ~/Images/screenshots/pic-sel-$(date '+%y%m%d-%H%M-%S').png ") },
  { MODKEY,                       XK_p,      spawn,          SHCMD("dmenufm") },
  { MODKEY|ControlMask,          XK_p,      spawn,          SHCMD("rofi-pass") },
  /* Media keys */
  { 0, XF86XK_AudioLowerVolume,              spawn,          SHCMD("pamixer -d 5 " "&&" " sigdwmblocks 3") },
  { 0, XF86XK_AudioRaiseVolume,              spawn,          SHCMD("pamixer -i 5 " "&&" " sigdwmblocks 3") },
  { 0, XF86XK_AudioMute,                     spawn,          SHCMD("pamixer -t " "&&" " sigdwmblocks 3") },
  { 0, XF86XK_MonBrightnessDown,             spawn,          SHCMD("brightnessctl s 5%-") },
  { 0, XF86XK_MonBrightnessUp,               spawn,          SHCMD("brightnessctl s 5%+") },
  { 0, XK_Print,                             spawn,          SHCMD("maim ~/Images/screenshots/pic-full-$(date '+%y%m%d-%H%M-%S').png ") },
  /* Function keys */
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	STACKKEYS(MODKEY,                                          focus)
	STACKKEYS(MODKEY|ShiftMask,                                push)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
  { MODKEY|ShiftMask,             XK_r,      quit,           {1} },
  /* Unused keybindings */
	// { MODKEY|MODKEY2,               XK_6,      incrihgaps,     {.i = +1 } },
	// { MODKEY|MODKEY2|ShiftMask,     XK_6,      incrihgaps,     {.i = -1 } },
	// { MODKEY|MODKEY2,               XK_7,      incrivgaps,     {.i = +1 } },
	// { MODKEY|MODKEY2|ShiftMask,     XK_7,      incrivgaps,     {.i = -1 } },
	// { MODKEY|MODKEY2,               XK_8,      incrohgaps,     {.i = +1 } },
	// { MODKEY|MODKEY2|ShiftMask,     XK_8,      incrohgaps,     {.i = -1 } },
	// { MODKEY|MODKEY2,               XK_9,      incrovgaps,     {.i = +1 } },
	// { MODKEY|MODKEY2|ShiftMask,     XK_9,      incrovgaps,     {.i = -1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

