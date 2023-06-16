#pragma once
#include <interface/obj-ref-i.h>
#include <interface/SEvtArgs-i.h>
SNSBEGIN

typedef BOOL (*FunMsgHandler)(const LPMSG pMsg, LRESULT *pRes, void *ctx);

typedef struct _MsgHandlerInfo{
	FunMsgHandler fun;
	void * ctx;
}MsgHandlerInfo;

#undef INTERFACE
#define INTERFACE INativeWnd
DECLARE_INTERFACE_(INativeWnd, IObjRef)
{
#include "SNativeWndApi.h"
};

SNSEND