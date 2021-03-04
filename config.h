/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
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
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

static char *const autostart[] = {
  "light-locker", "--lock-on-suspend", "--lock-on-lid",   NULL,
  "pipewire",                                             NULL,
  "nitrogen", "--restore",                                NULL,
  "remap",                                                NULL,
  NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "爵", "爵", "", "", "ﲾ", "", "", "", "" };
static const char *tagsalt[] = { "", "", "", "", "", "", "", "", "" };

#define TERMINAL "alacritty"
#define TERMCLASS "Alacritty"
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
#define MODKEY Mod4Mask
#define MODKEY2 Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

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
    { "dwm.fonts",              STRING,  &fonts },
    { "dwm.startup",            STRING,  &autostart },
		{ "dwm.borderpx",          	INTEGER, &borderpx },
		{ "dwm.snap",          		  INTEGER, &snap },
		{ "dwm.showbar",          	INTEGER, &showbar },
		{ "dwm.topbar",          	  INTEGER, &topbar },
		{ "dwm.nmaster",          	INTEGER, &nmaster },
		{ "dwm.resizehints",       	INTEGER, &resizehints },
    { "dwm.gappiv",             INTEGER, &gappiv },
    { "dwm.gappih",             INTEGER, &gappih },
    { "dwm.gappov",             INTEGER, &gappov },
    { "dwm.gappoh",             INTEGER, &gappoh },
    { "dwm.smartgaps",          INTEGER, &smartgaps },
		{ "dwm.mfact",      	 	    FLOAT,   &mfact },
};

/* Include Shiftview and cyclelayout */
#include "shiftview.c"
#include "cyclelayout.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY2,                      XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
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
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_space,  togglefullscr,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    togglealttag,   {0} },
  { MODKEY|ShiftMask,             XK_n,      shiftview,      {.i = +1} },
  { MODKEY|ShiftMask,             XK_p,      shiftview,      {.i = -1} },
  /* Application shortcuts */
  { MODKEY,                       XK_b,      spawn,          SHCMD("librewolf") },
  { MODKEY,                       XK_q,      spawn,          SHCMD("firefox") },
  { MODKEY|ShiftMask,             XK_t,      spawn,          SHCMD(TERMINAL " -e ranger") },
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
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
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
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

