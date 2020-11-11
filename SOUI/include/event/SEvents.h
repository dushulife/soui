﻿/**
* Copyright (C) 2014-2050 
* All rights reserved.
* 
* @file       Events.h
* @brief      
* @version    v1.0      
* @author     SOUI group   
* @date       2014/08/02
* 
* Describe    SOUI系统中使用的事件系统
*/

#pragma once
#include <core/sobjType.h>
#include <sobject/Sobject.hpp>
#include <helper/obj-ref-impl.hpp>

struct EvtEmptyData{};
//定义一组事件定义的宏，简化事件的定义。
#define DEF_EVT(evt,id,evt_name,api,evtData) \
class api evt : public TObjRefImpl< SObject >, public SEvtBase, public evtData \
{ \
	SOUI_CLASS_NAME(evt,L#evt_name ) \
public:\
	STDMETHOD_(int,GetID)(THIS) const { return evt::EventID; }\
	STDMETHOD_(LPCWSTR,GetName)(THIS) const { return evt::GetClassName(); }\
	STDMETHOD_(LPCVOID,GetData)(THIS) {return (evtData*)this;}\
	enum { EventID = id};\
	evt(SOUI::IObject *pSender) :SEvtBase(pSender){}\
};


//定义一组事件定义的宏，简化事件的定义。
#define SEVENT_BEGIN_EX(evt,id,evt_name, api) \
class evt : public SOUI::EventArgs \
{ \
public:\
	virtual int WINAPI GetID() const { return evt::EventID; }\
	virtual LPCWSTR WINAPI GetName() const { return evt::GetClassName(); }\
	SOUI_CLASS_NAME(evt,L#evt_name ) \
	enum {\
		EventID = id\
	};\
	evt(SOUI::IObject *pSender) :EventArgs(pSender){}\

#define EVT_EXP
#define SEVENT_BEGIN(evt,id) \
SEVENT_BEGIN_EX(evt,id,L"on_"#evt, EVT_EXP)

#define SEVENT_END()\
};

#define EVENTID(x) x::EventID,x::GetClassName()

namespace SOUI
{
    class SWindow;
    enum SOUI_EVENTS
    {
        EVT_INIT=8000,
        EVT_EXIT,
        EVT_TIMER,

        //基本窗口事件
        EVT_SETFOCUS=8100,
        EVT_KILLFOCUS,
        EVT_CREATE,
		EVT_INIT_FINISH,
        EVT_DESTROY,
        EVT_SIZE,
        EVT_VISIBLECHANGED,
        EVT_STATECHANGED,
		EVT_LBUTTONDOWN,
		EVT_LBUTTONUP,
		EVT_UPDATE_TOOLTIP,

		EVT_KEYDOWN=8200,

        //两个窗口鼠标状态事件
        EVT_MOUSE_HOVER=9000,
        EVT_MOUSE_LEAVE,
        
        EVT_CMD=10000,
        EVT_CTXMENU,
        
        //增加两个滚动VIEW的事件
        EVT_SCROLLVIEW_ORIGINCHANGED,
        EVT_SCROLLVIEW_SIZECHANGED,

        EVT_SCROLL,
        EVT_OFEVENT,    //消息二次包装
        EVT_OFPANEL,    //一个itemPanel中的消息的二次包装

        //增加4个itempanel相关的事件,经过EVT_OFPANEL到达应用层
        EVT_ITEMPANEL_CLICK,
        EVT_ITEMPANEL_DBCLICK,
        EVT_ITEMPANEL_RCLICK,
        EVT_ITEMPANEL_HOVER,
        EVT_ITEMPANEL_LEAVE,
        
		EVT_RADIOGROUP_CHECK_CHANGED = 10100,

        EVT_TAB_SELCHANGING=11000,
        EVT_TAB_SELCHANGED,
        EVT_TAB_ITEMHOVER,
        EVT_TAB_ITEMLEAVE,

        EVT_LB_SELCHANGING=12000,
        EVT_LB_SELCHANGED,
		EVT_LB_DBCLICK,

        EVT_LC_SELCHANGING=13000,
        EVT_LC_SELCHANGED,
        EVT_LC_ITEMDELETED,
		EVT_LC_DBCLICK,

		EVT_TV_SELCHANGING = 14000,
        EVT_TV_SELCHANGED,
        
        EVT_TC_SELCHANGING=15000,
        EVT_TC_SELCHANGED,
        EVT_TC_EXPAND,
        EVT_TC_CHECKSTATE,
        EVT_TC_DBCLICK,    //treectrl的叶子节点双击事件, add by zhaosheng
        
        EVT_LV_SELCHANGING = 15100,
        EVT_LV_SELCHANGED,
        EVT_LV_ITEMCLICK,
        
        EVT_RE_NOTIFY=16000,
        EVT_RE_MENU,
        
        EVT_SLIDER_POS=17000,

        EVT_HEADER_CLICK=18000,
        EVT_HEADER_ITEMCHANGING,
        EVT_HEADER_ITEMCHANGED,
        EVT_HEADER_ITEMSWAP,
		EVT_HEADER_RELAYOUT,

        EVT_CB_SELCHANGE=19000,
		EVT_CB_DROPDOWN,

        EVT_CALENDAR_SELDAY=20000,
        EVT_CALENDAR_SETDATE,
		EVT_CALENDAREX_CHANGED,
		EVT_DATETIME_CHANGED,
        
        EVT_SPIN_VALUE2STRING = 21000,
        
        EVT_SPLIT_PANE_MOVED  = 22000,

		EVT_HOT_KEY_SET = 23000,
        
		//图片动画开始，结束事件
		EVT_ANI_START	= 22100,
		EVT_ANI_STOP,

		EVT_SELECTMENU = 22150,
		EVT_POPMENU,


        EVT_EXTERNAL_BEGIN=10000000,
    };


    /*!
    \brief
    Base class used as the argument to all subscribers Event object.

    The base EventArgs class does not contain any useful information, it is intended
    to be specialised for each type of event that can be generated by objects within
    the system.  The use of this base class allows all event subscribers to have the
    same function signature.

    The \a handled field is used to signal whether an event was actually handled or not.  While
    the event system does not look at this value, code at a higher level can use it to determine
    how far to propagate an event.
    */
    class SOUI_EXP EventArgs : public TObjRefImpl< SObject >
    {
        SOUI_CLASS_NAME(EventArgs,L"eventargs")
    public:
        /*************************************************************************
        Construction
        *************************************************************************/
        EventArgs( IObject *pSender ) 
            : handled(0)
            , sender(pSender)
            , bubbleUp(true)
        {
            if(NULL!=pSender) {
                idFrom = pSender->GetID();
                nameFrom = pSender->GetName();
            } else {
               idFrom = 0;
               nameFrom = NULL;
            }
        }

        virtual ~EventArgs(void) {}

        /*************************************************************************
        Data members
        *************************************************************************/
        bool bubbleUp; /**< indicating whether the event should bubble up along the event handle chain, event handler can set the value to true to cancel the procedure  */
        UINT handled; /**< handlers should increment this if they handled the event. */

        int  idFrom; /**< 事件发送者ID */
        LPCWSTR nameFrom; /**< 事件发送者name */
        
        IObject *sender;  /**< 产生事件的原始SOUI对象，可能和idFrom and nameFrom指定的对象不同 */
    };
    

#undef INTERFACE
#define INTERFACE IEvtBase
	DECLARE_INTERFACE(IEvtBase)
	{
		STDMETHOD_(IObject*,GetSender)(THIS) PURE;
		STDMETHOD_(int,GetIdFrom)(THIS) SCONST PURE;
		STDMETHOD_(LPCWSTR,GetNameFrom) (THIS) SCONST PURE;
		STDMETHOD_(BOOL,IsBubbleUp) (THIS) SCONST PURE;
		STDMETHOD_(void,SetBubbleUp) (THIS_ BOOL bBubbleUp) PURE;
		STDMETHOD_(LPCVOID,GetData)(THIS) PURE;
	};

	class SEvtBase : public IEvtBase
	{
	public:
		UINT handled; 
		BOOL bubbleUp; 
		int  idFrom; 
		LPCWSTR nameFrom;
		SAutoRefPtr<IObject> sender; 

		STDMETHOD_(IObject*,GetSender)(THIS){return sender;}
		STDMETHOD_(int,GetIdFrom) (THIS) SCONST{ return idFrom;}
		STDMETHOD_(LPCWSTR,GetNameFrom) (THIS) SCONST{return nameFrom;}
		STDMETHOD_(BOOL,IsBubbleUp) (THIS) SCONST{return bubbleUp;}
		STDMETHOD_(void,SetBubbleUp) (THIS_ BOOL bBubbleUp_) {bubbleUp = bBubbleUp_;}
		STDMETHOD_(LPCVOID,GetData)(THIS) {return NULL;}
		
	public:
		SEvtBase(IObject *pSender)
			: handled(0)
			, sender(pSender)
			, bubbleUp(true)
		{
			if(NULL!=pSender) {
				idFrom = pSender->GetID();
				nameFrom = pSender->GetName();
			} else {
				idFrom = 0;
				nameFrom = NULL;
			}
		}
	};

	


    /*!
    \brief 一个事件对象的模板类，用来实现EventArgs两个虚函数
    */
    template<class T>
    class TplEventArgs : public EventArgs
    {
    public:
        TplEventArgs(IObject *pSender):EventArgs(pSender)
        {
        }

        virtual int WINAPI GetID() const {return T::EventID;}
        virtual LPCWSTR WINAPI GetName() const {return T::GetClassName();}
    };

    class SOUI_EXP EventCmnArgs : public EventArgs
    {
    public:
        EventCmnArgs(IObject *pSender,int _nID):EventArgs(pSender),nID(_nID)
        {

        }
        virtual int WINAPI GetID() const{return nID;}

    protected:
        int nID;
    };


	SEVENT_BEGIN_EX(EventInit, EVT_INIT, on_init, SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventExit, EVT_EXIT, on_exit, SOUI_EXP)
	SEVENT_END()


	SEVENT_BEGIN_EX(EventTimer, EVT_TIMER, on_timer, SOUI_EXP)
		    UINT uID;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventSwndCreate, EVT_CREATE, on_create, SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventSwndInitFinish, EVT_INIT_FINISH, on_init_finish, SOUI_EXP)
	SEVENT_END()
	
	SEVENT_BEGIN_EX(EventSwndDestroy, EVT_DESTROY, on_destroy, SOUI_EXP)
	SEVENT_END()


	SEVENT_BEGIN_EX(EventSwndSize, EVT_SIZE, on_size, SOUI_EXP)
		    CSize szWnd;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventSwndStateChanged, EVT_STATECHANGED, on_state_changed, SOUI_EXP)
		bool CheckState(DWORD dwState)
		{
			return (dwOldState & dwState) != (dwNewState & dwState);
		}

		DWORD dwOldState;
		DWORD dwNewState;
	SEVENT_END()


	SEVENT_BEGIN_EX(EventSwndVisibleChanged, EVT_VISIBLECHANGED, on_visible_changed, SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventKeyDown, EVT_KEYDOWN, on_key_down, SOUI_EXP)
			UINT nChar;
			UINT nFlags;
			bool bCancel;
	SEVENT_END()


	SEVENT_BEGIN_EX(EventSwndMouseLeave, EVT_MOUSE_LEAVE, on_mouse_leave, SOUI_EXP)
	SEVENT_END()


	SEVENT_BEGIN_EX(EventSwndMouseHover, EVT_MOUSE_HOVER, on_mouse_hover, SOUI_EXP)
	SEVENT_END()


	SEVENT_BEGIN_EX(EventSwndUpdateTooltip, EVT_UPDATE_TOOLTIP, on_update_tooltip, SOUI_EXP)
		SStringT strToolTip;
		BOOL bUpdated;
	SEVENT_END()

	class SRadioBox;
	SEVENT_BEGIN_EX(EventRadioGroupCheckChanged, EVT_RADIOGROUP_CHECK_CHANGED, on_radiogroup_check_changed, SOUI_EXP)
		SRadioBox * pChecked;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventItemPanelRclick, EVT_ITEMPANEL_RCLICK, on_itemparem_rclick, SOUI_EXP)
		WPARAM wParam;
		LPARAM lParam;
	SEVENT_END()


	SEVENT_BEGIN_EX(EventItemPanelDbclick, EVT_ITEMPANEL_DBCLICK, on_itempanel_dbclick, SOUI_EXP)
		WPARAM wParam;
		LPARAM lParam;
	SEVENT_END()


	SEVENT_BEGIN_EX(EventItemPanelClick, EVT_ITEMPANEL_CLICK, on_itempanel_click, SOUI_EXP)
		WPARAM wParam;
		LPARAM lParam;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventItemPanelHover, EVT_ITEMPANEL_HOVER, on_itempanel_hover, SOUI_EXP)
		WPARAM wParam;
		LPARAM lParam;
	SEVENT_END()

    
    //注：在EventItemPanelLeave中从IItemPanel中通过GetItemIndex获取表项索引时需要检查索引有效性。
	SEVENT_BEGIN_EX(EventItemPanelLeave, EVT_ITEMPANEL_LEAVE, on_itempanel_leave, SOUI_EXP)
	SEVENT_END()
    
	SEVENT_BEGIN_EX(EventLButtonDown, EVT_LBUTTONDOWN, on_mouse_lbutton_down, SOUI_EXP)
		POINT           pt;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLButtonUp, EVT_LBUTTONUP, on_mouse_lbutton_up, SOUI_EXP)
		POINT           pt;
	SEVENT_END()


	SEVENT_BEGIN_EX(EventCmd, EVT_CMD, on_command, SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventCtxMenu, EVT_CTXMENU, on_conext_menu, SOUI_EXP)
		    POINT           pt;
		    BOOL            bCancel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventSetFocus, EVT_SETFOCUS, on_set_focus, SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventKillFocus, EVT_KILLFOCUS, on_kill_focus, SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventScrollViewOriginChanged, EVT_SCROLLVIEW_ORIGINCHANGED, on_scrollview_origin_changed, SOUI_EXP)
			CPoint ptOldOrigin;
			CPoint ptNewOrigin;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventScrollViewSizeChanged, EVT_SCROLLVIEW_SIZECHANGED, on_scrollview_size_changed, SOUI_EXP)
		CSize szOldViewSize;
		CSize szNewViewSize;
	SEVENT_END()


	SEVENT_BEGIN_EX(EventScroll, EVT_SCROLL, on_scroll, SOUI_EXP)
		int         nSbCode;
		int         nPos;
		BOOL        bVertical;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventOfEvent, EVT_OFEVENT, on_event_of_event, SOUI_EXP)
		EventArgs * pOrgEvt;
	SEVENT_END()

    class SItemPanel;
	SEVENT_BEGIN_EX(EventOfPanel, EVT_OFPANEL, on_event_of_panel, SOUI_EXP)
		SItemPanel *pPanel;
		EventArgs * pOrgEvt;
	SEVENT_END()
    
	SEVENT_BEGIN_EX(EventTabSelChanging, EVT_TAB_SELCHANGING, on_tab_sel_changing, SOUI_EXP)
		UINT        uOldSel;
		UINT        uNewSel;
		BOOL        bCancel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventTabSelChanged, EVT_TAB_SELCHANGED, on_tab_sel_changed, SOUI_EXP)
		UINT        uOldSel;
		UINT        uNewSel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventTabItemHover, EVT_TAB_ITEMHOVER, on_tab_item_hover, SOUI_EXP)
		int iHover;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventTabItemLeave, EVT_TAB_ITEMLEAVE, on_tab_item_leave, SOUI_EXP)
		int iLeave;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLBSelChanging, EVT_LB_SELCHANGING, on_listbox_sel_changing, SOUI_EXP)
		int nNewSel;
		int nOldSel;
		BOOL bCancel;
	SEVENT_END()


	SEVENT_BEGIN_EX(EventLBSelChanged, EVT_LB_SELCHANGED, on_listbox_sel_changed, SOUI_EXP)
		int nNewSel;
		int nOldSel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLBDbClick, EVT_LB_DBCLICK, on_listbox_item_dbclick, SOUI_EXP)
		int nCurSel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLVSelChanged, EVT_LV_SELCHANGED, on_listview_select_changed, SOUI_EXP)
		int iOldSel;    //原选中项
		int iNewSel;    //新选中项
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLVSelChanging, EVT_LV_SELCHANGING, on_listview_select_changing, SOUI_EXP)
		int iOldSel;    //原选中项
		int iNewSel;    //新选中项
		BOOL bCancel;
	SEVENT_END()
 
	SEVENT_BEGIN_EX(EventTVSelChanged, EVT_TV_SELCHANGED, on_treeview_select_changed, SOUI_EXP)
		ULONG_PTR hOldSel;    //原选中项
		ULONG_PTR hNewSel;    //新选中项
	SEVENT_END()

	SEVENT_BEGIN_EX(EventTVSelChanging, EVT_TV_SELCHANGING, on_treeview_select_changing, SOUI_EXP)
		ULONG_PTR hOldSel;    //原选中项
		ULONG_PTR hNewSel;    //新选中项
		BOOL bCancel;
	SEVENT_END()


	SEVENT_BEGIN_EX(EventRENotify, EVT_RE_NOTIFY, on_richedit_notify, SOUI_EXP)
        DWORD iNotify;
        LPVOID pv;
        HRESULT hr;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventREMenu, EVT_RE_MENU, on_richedit_menu, SOUI_EXP)
		UINT uCmd;
	SEVENT_END()
    
	SEVENT_BEGIN_EX(EventSliderPos, EVT_SLIDER_POS, on_slider_pos, SOUI_EXP)
		int     nPos;
	SEVENT_END()

    //点击表头
	SEVENT_BEGIN_EX(EventHeaderClick, EVT_HEADER_CLICK, on_header_click, SOUI_EXP)
		int   iItem;
	SEVENT_END()

    //表头宽度改变中
	SEVENT_BEGIN_EX(EventHeaderItemChanging, EVT_HEADER_ITEMCHANGING, on_header_item_changing, SOUI_EXP)
        int   iItem;
        int   nWidth;
	SEVENT_END()

    //表头宽度改变
	SEVENT_BEGIN_EX(EventHeaderItemChanged, EVT_HEADER_ITEMCHANGED, on_header_item_changed, SOUI_EXP)
        int   iItem;
        int   nWidth;
	SEVENT_END()

    //拖动表项调整位置
	SEVENT_BEGIN_EX(EventHeaderItemSwap, EVT_HEADER_ITEMSWAP, on_header_item_swap, SOUI_EXP)
		int   iOldIndex;
		int   iNewIndex;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventHeaderRelayout, EVT_HEADER_RELAYOUT,on_header_relayout,SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventCBSelChange, EVT_CB_SELCHANGE, on_combobox_sel_change, SOUI_EXP)
		int nCurSel;
	SEVENT_END()

	class SDropDownWnd;
	SEVENT_BEGIN_EX(EventCBDropdown, EVT_CB_DROPDOWN, on_combobox_dropdown, SOUI_EXP)
		SDropDownWnd *pDropDown;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLCSelChanging, EVT_LC_SELCHANGING, on_listctrl_sel_changing, SOUI_EXP)
        int nNewSel;
        int nOldSel;
        BOOL bCancel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLCSelChanged, EVT_LC_SELCHANGED, on_listctrl_sel_changed, SOUI_EXP)
        int nNewSel;
        int nOldSel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLCDbClick, EVT_LC_DBCLICK, on_listctrl_item_dbclick, SOUI_EXP)
		int nCurSel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventLCItemDeleted, EVT_LC_ITEMDELETED, on_listctrl_del_item, SOUI_EXP)
		int		nItem;
		LPARAM   dwData;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventCalendarSelDay, EVT_CALENDAR_SELDAY, on_calendar_sel_day, SOUI_EXP)
		WORD   wOldDay;
		WORD   wNewDay;
	SEVENT_END()
    
	SEVENT_BEGIN_EX(EventCalendarSetDate, EVT_CALENDAR_SETDATE, on_calendar_set_date, SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventCalendarExChanged, EVT_CALENDAREX_CHANGED, on_calendarex_changed, SOUI_EXP)
		WORD iNewDay;
		WORD iNewMonth;
		WORD iNewYear;
		int nBtnType;			// 按钮 类型
	SEVENT_END()

	SEVENT_BEGIN_EX(EventDateTimeChanged, EVT_DATETIME_CHANGED, on_datetime_changed, SOUI_EXP)
		SYSTEMTIME newTime;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventTCSelChanging, EVT_TC_SELCHANGING, on_treectrl_sel_changing, SOUI_EXP)
        HSTREEITEM hOldSel;
        HSTREEITEM hNewSel;
        BOOL bCancel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventTCSelChanged, EVT_TC_SELCHANGED, on_treectrl_sel_changed, SOUI_EXP)
        HSTREEITEM hOldSel;
        HSTREEITEM hNewSel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventTCCheckState, EVT_TC_CHECKSTATE, on_treectrl_item_check, SOUI_EXP)
        HSTREEITEM  hItem;
        UINT        uCheckState;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventTCExpand, EVT_TC_EXPAND, on_treectrl_item_expand, SOUI_EXP)
        HSTREEITEM  hItem;
        BOOL bCollapsed;
	SEVENT_END()

    //双击treectrl的叶子节点 add by zhaosheng
	SEVENT_BEGIN_EX(EventTCDbClick, EVT_TC_DBCLICK, on_treectrl_item_dbclick, SOUI_EXP)
        HSTREEITEM hItem;  //双击选中的节点
        BOOL bCancel;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventSpinValue2String, EVT_SPIN_VALUE2STRING, on_spin_value2string, SOUI_EXP)
		bool	 bInit;
        int      nValue;
        SStringT strValue;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventSplitPaneMoved, EVT_SPLIT_PANE_MOVED, on_split_pane_moved, SOUI_EXP)
		CRect rcPane;
	SEVENT_END()
    
	SEVENT_BEGIN_EX(EventAnimateStart, EVT_ANI_START, on_animate_start, SOUI_EXP)
	SEVENT_END()

	SEVENT_BEGIN_EX(EventAnimateStop, EVT_ANI_STOP, EVT_ANI_STOP, SOUI_EXP)
	SEVENT_END()

	class SMenu;
	SEVENT_BEGIN_EX(EventSelectMenu, EVT_SELECTMENU, on_select_menu, SOUI_EXP)
		UINT m_id;
		SMenu *m_pMenu;
	SEVENT_END()
	SEVENT_BEGIN_EX(EventPopMenu, EVT_POPMENU, on_pop_menu, SOUI_EXP)
		UINT m_index;
		SMenu* m_pMenu;
	SEVENT_END()

	SEVENT_BEGIN_EX(EventSetHotKey, EVT_HOT_KEY_SET, on_hot_key_set_event, SOUI_EXP)
		WORD vKey;
		WORD wModifiers;
	SEVENT_END()

} // End of  SOUI namespace section
