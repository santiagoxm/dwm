/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 1;    /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int focusonwheel       = 0;
static const char *fonts[]          = { "JetBrainsMonoNerdFont-Regular:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_black[]       = "#000000";
static const char col_white[]       = "#ffffff";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_black, col_gray1 },
	[SchemeSel]  = { col_gray4, col_gray1,  col_gray3  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class         instance    title       tags mask     isfloating   monitor */
	{ "pavucontrol", NULL,       NULL,       0,            1,           -1 },
	{ "feh",         NULL,       NULL,       0,            1,           -1 },
//	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[D]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask // Windows key
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[]      = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucmd[]      = { "j4-dmenu-desktop", NULL };
static const char *clipboard[]     = { "clipmenu", NULL };
static const char *termcmd[]       = { "alacritty", NULL };
static const char *firefox[]       = { "firefox", NULL};
static const char *firefox_work[]  = { "firefox", "-P", "ModoPala", NULL};
static const char *files[]         = { "thunar", NULL};
static const char *killpicom[]     = { "killall", "picom", NULL};
static const char *startpicom[]    = { "picom", NULL};
static const char *pavucontrol[]   = { "pavucontrol", NULL};
static const char *lowervol[]      = { "wpctl", "set-volume", "-l", "1.0", "@DEFAULT_AUDIO_SINK@", "5%-", NULL};
static const char *raisevol[]      = { "wpctl", "set-volume", "-l", "1.0", "@DEFAULT_AUDIO_SINK@", "5%+", NULL};
static const char *togglevol[]     = { "wpctl", "set-mute", "@DEFAULT_AUDIO_SINK@", "toggle", NULL};
static const char *normalvol[]     = { "wpctl", "set-volume", "-l", "1.0", "@DEFAULT_AUDIO_SINK@", "0.75", NULL};
static const char *maxvol[]        = { "wpctl", "set-volume", "-l", "1.0", "@DEFAULT_AUDIO_SINK@", "1", NULL};
static const char *playertoggle[]  = { "playerctl", "play-pause", NULL};
static const char *playerstop[]    = { "playerctl", "stop", NULL};
static const char *playernext[]    = { "playerctl", "next", NULL};
static const char *playerprev[]    = { "playerctl", "previous", NULL};
static const char *headphones[]    = { "pactl", "set-default-sink", "alsa_output.usb-Kingston_Technology_Company_HyperX_Cloud_Flight_Wireless-00.analog-stereo", NULL};
static const char *speaker[]       = { "pactl", "set-default-sink", "alsa_output.pci-0000_09_00.1.hdmi-stereo", NULL};
static const char *redshift[]      = { "redshift", "-l", "-35:-58", NULL};
static const char *killredshift[]  = { "killall", "redshift", NULL};
static const char *shutdown[]      = { "shutdown", "now", NULL};
static const char maim_region[]    = "maim -s | xclip -selection clipboard -t image/png";
static const char maim_monitor[]   = "maim /home/sangl/screenshots/$(date +%b%d-%H%M%S).png";
static const char monitoroff[]     = "sleep 1; xset dpms force standby";

#define XF86_LVol   0x1008ff11
#define XF86_Mute   0x1008ff12
#define XF86_RVol   0x1008ff13
#define XF86_Play   0x1008ff14
#define XF86_Stop   0x1008ff15
#define XF86_Prev   0x1008ff16
#define XF86_Next   0x1008ff17
#define XF86_Pause  0x1008ff31

static const Key keys[] = {
	/* modifier           key          function        argument */
	{ MODKEY,             XK_d,        togglebar,      {0} },
	{ MODKEY,             XK_c,        killclient,     {0} },
	{ MODKEY,             XK_t,        togglefloating, {0} },
	{ MODKEY,             XK_m,        quit,           {0} },
	{ MODKEY,             XK_j,        focusstack,     {.i = +1 } },
	{ MODKEY,             XK_k,        focusstack,     {.i = -1 } },
	{ MODKEY,             XK_p,        incnmaster,     {.i = +1 } },
	{ MODKEY,             XK_o,        incnmaster,     {.i = -1 } },
	{ MODKEY,             XK_Return,   zoom,           {0} },
	{ MODKEY,             XK_q,        spawn,          {.v = termcmd } },
	{ MODKEY,             XK_r,        spawn,          {.v = dmenucmd} },
	{ MODKEY,             XK_v,        spawn,          {.v = clipboard} },
	{ MODKEY,             XK_b,        spawn,          {.v = firefox } },
	{ MODKEY|ShiftMask,   XK_b,        spawn,          {.v = firefox_work} },
	{ MODKEY|ShiftMask,   XK_i,        spawn,          {.v = killpicom} },
	{ MODKEY|ShiftMask,   XK_s,        spawn,          SHCMD(maim_region) },
	{ MODKEY,             XK_Print,    spawn,          SHCMD(maim_monitor) },
	{ MODKEY,             XK_l,        spawn,          SHCMD(monitoroff) },
	{ MODKEY,             XK_e,        spawn,          {.v = files} },
	{ MODKEY,             XK_i,        spawn,          {.v = startpicom} },
	{ MODKEY,             XK_a,        spawn,          {.v = pavucontrol} },
	{ MODKEY,             XK_h,        spawn,          {.v = redshift} },
	{ MODKEY|ShiftMask,   XK_h,        spawn,          {.v = killredshift} },
	{ MODKEY,             XF86_LVol,   spawn,          {.v = headphones} },
	{ MODKEY,             XF86_RVol,   spawn,          {.v = speaker} },
	{ MODKEY|ShiftMask,   XF86_LVol,   spawn,          {.v = normalvol} },
	{ MODKEY|ShiftMask,   XF86_RVol,   spawn,          {.v = maxvol} },
	{ 0     ,             XF86_LVol,   spawn,          {.v = lowervol} },
	{ 0     ,             XF86_RVol,   spawn,          {.v = raisevol} },
	{ 0     ,             XF86_Mute,   spawn,          {.v = togglevol} },
	{ 0     ,             XF86_Stop,   spawn,          {.v = playerstop} },
	{ 0     ,             XF86_Pause,  spawn,          {.v = playertoggle} },
	{ 0     ,             XF86_Play,   spawn,          {.v = playertoggle} },
	{ 0     ,             XF86_Next,   spawn,          {.v = playernext} },
	{ 0     ,             XF86_Prev,   spawn,          {.v = playerprev} },
	TAGKEYS(              XK_1,        0)
	TAGKEYS(              XK_2,        1)
	TAGKEYS(              XK_3,        2)
	TAGKEYS(              XK_4,        3)
	TAGKEYS(              XK_5,        4)
	TAGKEYS(              XK_6,        5)
	TAGKEYS(              XK_7,        6)
	TAGKEYS(              XK_8,        7)
	TAGKEYS(              XK_9,        8)
//	{ MODKEY,             XK_h,      setmfact,       {.f = -0.05} },
//	{ MODKEY,             XK_l,      setmfact,       {.f = +0.05} },
//	{ MODKEY,             XK_Tab,    view,           {0} },
//	{ MODKEY,             XK_t,      setlayout,      {.v = &layouts[0]} },
//	{ MODKEY,             XK_f,      setlayout,      {.v = &layouts[1]} },
//	{ MODKEY,             XK_m,      setlayout,      {.v = &layouts[2]} },
//	{ MODKEY,             XK_space,  setlayout,      {0} },
//	{ MODKEY,             XK_0,      view,           {.ui = ~0 } },
//	{ MODKEY|ShiftMask,   XK_0,      tag,            {.ui = ~0 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} }, // Layout button
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} }, // Resize window with right click
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} }, // Move window with left click
	{ ClkTagBar,            0,              Button1,        view,           {0} }, // Click workspace to switch
	{ ClkStatusText,        MODKEY,         Button1,        spawn,          {.v = shutdown } },
//	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
//	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
//	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
//	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
//	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
//	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

