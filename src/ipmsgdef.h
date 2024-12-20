﻿/*	@(#)Copyright (C) H.Shirouzu 2011-2019   ipmsgcmn.h	Ver4.99 */
/* ========================================================================
	Project  Name			: IP Messenger for Win32
	Module Name				: IP Messenger Define Header
	Create					: 2011-05-03(Tue)
	Update					: 2019-01-12(Sat)
	Copyright				: H.Shirouzu
	Reference				: 
	======================================================================== */

#ifndef IPMSGDEF_H
#define IPMSGDEF_H

#include "environ.h"

//#define _USING_V110_SDK71_

/*  IP Messenger for Windows  internal define  */
#define IPMSG_REVERSEICON			0x0100
#define IPMSG_RECVICONTICK			500
#define IPMSG_RECVICONTICK2			200
#define IPMSG_ENTRYMINSEC			5
#define IPMSG_GETLIST_FINISH		0
#define IPMSG_DELAYMSG_OFFSETTIME	2000
#define IPMSG_DELAYFOCUS_TIME		10
#define IPMSG_UPLOG_MINSPAN			(60 * 1000)
#define IPMSG_UPLOG_MAXSPAN			(3600 * 1000)
#define IPMSG_DELAYENTRY_SPAN		(20 * 1000)
#define IPMSG_MASTERACTIVE_LIMIT	600

#define IPMSG_BROADCAST_TIMER		0x0101
#define IPMSG_SEND_TIMER			0x0102
#define IPMSG_DELAYENTRY_TIMER		0x0103
#define IPMSG_LISTGET_TIMER			0x0104
#define IPMSG_LISTGETRETRY_TIMER	0x0105
#define IPMSG_ENTRY_TIMER			0x0110
#define IPMSG_DUMMY_TIMER			0x0111
#define IPMSG_RECV_TIMER			0x0112
#define IPMSG_ANS_TIMER				0x0113
#define IPMSG_CLEANUP_TIMER			0x0120
#define IPMSG_CLEANUPDIRTCP_TIMER	0x0121
#define IPMSG_BALLOON_RECV_TIMER	0x0130
#define IPMSG_BALLOON_OPEN_TIMER	0x0131
#define IPMSG_BALLOON_DELAY_TIMER	0x0132
#define IPMSG_BALLOON_RESET_TIMER	0x0133
#define IPMSG_BALLOON_INST_TIMER	0x0134
#define IPMSG_IMAGERECT_TIMER		0x0140
#define IPMSG_KEYCHECK_TIMER		0x0141
#define IPMSG_DELAYFOCUS_TIMER		0x0142
#define IPMSG_FOREDURATION_TIMER	0x0143
#define IPMSG_AUTOSAVE_TIMER		0x0150
#define IPMSG_FIRSTRUN_TIMER		0x0160
#define IPMSG_FWCHECK_TIMER			0x0162
#define IPMSG_POLL_TIMER			0x0170
#define IPMSG_DUMMY_TIMER2			0x0171
#define IPMSG_BRDIR_TIMER			0x0172
#define IPMSG_DIR_TIMER				0x0173
#define IPMSG_CMD_TIMER				0x0174

#define IPMSG_DUMMY_TIMER1			0x0180


#define IPMSG_NAMESORT			0x00000000
#define IPMSG_IPADDRSORT		0x00000001
#define IPMSG_HOSTSORT			0x00000002
#define IPMSG_NOGROUPSORTOPT	0x00000100
#define IPMSG_ICMPSORTOPT		0x00000200
#define IPMSG_NOKANJISORTOPT	0x00000400
#define IPMSG_ALLREVSORTOPT		0x00000800
#define IPMSG_GROUPREVSORTOPT	0x00001000
#define IPMSG_SUBREVSORTOPT		0x00002000


#define WM_NOTIFY_TRAY			(WM_APP + 101)
#define WM_NOTIFY_RECV			(WM_APP + 102)
#define WM_NOTIFY_OPENCHECK		(WM_APP + 103)
#define WM_IPMSG_INITICON		(WM_APP + 104)
#define WM_IPMSG_CHANGE_MCAST	(WM_APP + 105)
#define WM_IPMSG_DELAY_FWDLG	(WM_APP + 106)
#define WM_IPMSG_SETFWRES		(WM_APP + 107)
#define WM_IPMSG_DIRMODE_SHEET	(WM_APP + 108)
#define WM_RECVDLG_OPEN			(WM_APP + 110)
#define WM_RECVDLG_EXIT			(WM_APP + 111)
#define WM_RECVDLG_FILEBUTTON	(WM_APP + 112)
#define WM_RECVDLG_READCHECK	(WM_APP + 113)
#define WM_RECVDLG_BYVIEWER		(WM_APP + 114)
#define WM_SENDDLG_OPEN			(WM_APP + 121)
#define WM_SENDDLG_CREATE		(WM_APP + 122)
#define WM_SENDDLG_EXIT			(WM_APP + 123)
#define WM_SENDDLG_EXITEX		(WM_APP + 124)
#define WM_SENDDLG_RESIZE		(WM_APP + 125)
#define WM_SENDDLG_FONTCHANGED	(WM_APP + 126)
#define WM_UDPEVENT				(WM_APP + 130)
#define WM_TCPEVENT				(WM_APP + 131)
#define WM_TCPDIREVENT			(WM_APP + 132)
#define WM_STOPTRANS			(WM_APP + 133)
#define WM_REFRESH_HOST			(WM_APP + 140)
#define WM_MSGDLG_EXIT			(WM_APP + 150)
#define WM_DELMISCDLG			(WM_APP + 151)
#define WM_HIDE_CHILDWIN		(WM_APP + 160)
#define WM_EDIT_DBLCLK			(WM_APP + 170)
#define WM_DELAYSETTEXT			(WM_APP + 180)
#define WM_DELAY_BITMAP			(WM_APP + 182)
#define WM_DELAY_PASTE			(WM_APP + 183)
#define WM_PASTE_REV			(WM_APP + 184)
#define WM_PASTE_IMAGE			(WM_APP + 185)
#define WM_PASTE_TEXT			(WM_APP + 186)
#define WM_SAVE_IMAGE			(WM_APP + 187)
#define WM_EDIT_IMAGE			(WM_APP + 188)
#define WM_INSERT_IMAGE			(WM_APP + 189)
#define WM_HISTDLG_OPEN			(WM_APP + 190)
#define WM_HISTDLG_HIDE			(WM_APP + 191)
#define WM_HISTDLG_NOTIFY		(WM_APP + 192)
#define WM_FORCE_TERMINATE		(WM_APP + 193)
#define WM_FINDDLG_DELAY		(WM_APP + 194)
#define WM_IPMSG_IMECTRL		(WM_APP + 200)
#define WM_IPMSG_BRNOTIFY		(WM_APP + 201)
#define WM_IPMSG_REMOTE			(WM_APP + 202)
#define WM_LOGVIEW_UPDATE		(WM_APP + 220)
#define WM_LOGVIEW_RESETCACHE	(WM_APP + 221)
#define WM_LOGVIEW_RESETFIND	(WM_APP + 222)
#define WM_LOGVIEW_OPEN			(WM_APP + 223)
//#define WM_LOGVIEW_PRECLOSE		(WM_APP + 224)
#define WM_LOGVIEW_CLOSE		(WM_APP + 225)
#define WM_LOGVIEW_MOUSE_EMU	(WM_APP + 226)
#define WM_LOGFETCH_DONE		(WM_APP + 230)
#define WM_IPMSG_SETUPDLG		(WM_APP + 240)
//#define WM_IPMSG_SVRDETECT		(WM_APP + 241)

#define WM_IPMSG_SVRDETECTED	(WM_APP + 242)
#define WM_IPMSG_HELP			(WM_APP + 243)

#define WM_IPMSG_FOCUS			(WM_APP + 250)
#define WM_IPMSG_PLAYFIN		(WM_APP + 251)

#define WM_IPMSG_SLACKRES		(WM_APP + 260)

#define WM_IPMSG_UPDATERES		(WM_APP + 270)
#define WM_IPMSG_UPDATEDLRES	(WM_APP + 271)
#define WM_IPMSG_UPDINFORESULT	(WM_APP + 272)
#define WM_IPMSG_UPDDATARESULT	(WM_APP + 273)
#define WM_IPMSG_UPDATEDLG		(WM_APP + 274)

#define WM_DELAYMSG_BALLOON		(WM_APP + 280)
#define WM_DELAYSEND_DEL		(WM_APP + 281)

#define WM_CROPMENU				(WM_APP + 300)
#define WM_CROPMENU_MAX			(WM_APP + 399)
#define WM_FILEMENU				(WM_APP + 400)
#define WM_FILEMENU_MAX			(WM_APP + 499)

#define WM_DUMMY_MSG1			(WM_APP + 500)
#define WM_DUMMY_MSG2			(WM_APP + 600)
#define WM_DUMMY_MSG3			(WM_APP + 601)
#define WM_DUMMY_MSG			(WM_APP + 699)

#define WM_IPMSG_CMDVER			(WM_APP + 700)
#define WM_IPMSG_CMDVERRES		(WM_APP + 701)
#define WM_IPMSG_CMD			(WM_APP + 702)
#define WM_IPMSG_CMDRES			(WM_APP + 703)
#define WM_IPMSG_POSTCMD		(WM_APP + 704) // ipcmd internal

#define WM_CMDWIN_MAIN			(WM_APP + 800)

#define IPMSG_CMD_VER1			100
#define IPMSG_CMD_GETABSENCE	1
#define IPMSG_CMD_SETABSENCE	2
#define IPMSG_CMD_GETHOSTLIST	3
#define IPMSG_CMD_REFRESH		4
#define IPMSG_CMD_SENDMSG		5
#define IPMSG_CMD_RECVMSG		6
#define IPMSG_CMD_GETSTATE		7
#define IPMSG_CMD_TERMINATE		999

// for IPMSG_CMD_REFRESH
#define IPMSG_REMOVE_FLAG		0x000000001

// for IPMSG_CMD_SENDMSG
#define IPMSG_SEAL_FLAG			0x000000001

// for IPMSG_CMD_RECVMSG
#define IPMSG_RECVPROC_FLAG		0x000000001

#define DIRMODE_NONE	0
#define DIRMODE_USER	1
#define DIRMODE_MASTER	2

//typedef long	time_t;		// size of time_t is 64bit in VS2005 or later

#define SKEY_HEADER_SIZE	12

// General define
#define MAX_SOCKBUF		(256 * 1024)
#define MAX_UDPBUF		(32 * 1024)
#define MAX_ULISTBUF	3072
#define MAX_ULIST		30
#define MAX_VERBUF		40
#define MAX_BUF			1024
#define MAX_BUF_EX		(MAX_BUF * 3)
#define MAX_MULTI_PATH	(MAX_BUF * 32)
#define MAX_NAMEBUF		80
#define MAX_LISTBUF		(MAX_NAMEBUF * 4)
#define MAX_ANSLIST		100
#define MAX_DOSHOST		5
#define MAX_FILENAME_U8	(255 * 3)
#define MAX_MSGBODY		65536
#define MAX_FQDN		256

#define ADAY_MSEC		(24 * 60 * 60 * 1000)
#define ERR_MSEC		(20 * 1000)
#define MONTH_SEC		(30 * 24 * 60 * 60)

#define HS_TOOLS		"HSTools"
#define IP_MSG			"IPMsg"
#define IP_MSG_W		L"IPMsg"
#define NO_NAME			"no_name"
#define URL_STR			"://"
#define MAILTO_STR		"mailto:"
#define MSG_STR			"msg"

#define IPMSG_CLASS		"ipmsg_class"
#define IPMSG_FULLNAME	"IP Messenger for Win"
#define IPMSG_APPID		"IPMSG for Win"
#define IPMSG_APPID_W	L"IPMSG for Win"
#define IPMSG_SITE		"ipmsg.org"

#define IPMSG_UPDATEINFO		"ipmsg-update.dat"
#ifdef _WIN64
#define UPDATE_FILENAME			UPDATE64_FILENAME
#else
#define UPDATE_FILENAME			UPDATE32_FILENAME
#endif
#define UPDATE32_FILENAME		"ipmsgupd32.exe"
#define UPDATE64_FILENAME		"ipmsgupd64.exe"



#define IPMSG_LOGNAME	"ipmsg.log"
#define IPMSG_LOGDBNAME	"ipmsg.db"

#define REMOTE_CMD			"ipmsg-cmd:"
#define REMOTE_FMT			REMOTE_CMD "%s"
#define REMOTE_HEADERLEN	10
#define REMOTE_KEYLEN		32
#define REMOTE_KEYSTRLEN	43

#define DEFAULT_PRIORITY	10
#define PRIORITY_OFFSET		10
#define DEFAULT_PRIORITYMAX	6

// inter module flags
#define RDLG_FREE_MI		1
#define RDLG_FILESAVE		2

// cfg.cpp & install.cpp
#define FWCHECKMODE_STR		"FwCheckMode"

#ifdef IPMSG_PRO
#define IPMSGDEF_HEAD
#include "ipmsgext.dat"
#undef IPMSGDEF_HEAD
#endif


extern volatile int64 gEnableHook;	// for global
#define ENABLE_HOOK	(0x0d762de84d9f01a4LL) // a4 01 9f 4d e8 2d 76 0d

#endif
