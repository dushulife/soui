#pragma once
#include <sobject/Sobject.hpp>
#include <interface/SNcPainter-i.h>
#include <core/SItemPanel.h>

SNSBEGIN
class SHostWnd;
class SNcPainter : public TObjRefImpl<SObjectImpl<INcPainter>>, IItemContainer, IHostProxy 
{
	DEF_SOBJECT_EX(TObjRefImpl<SObjectImpl<INcPainter>>,L"ncpainter",Undef)
	friend class SHostWnd;
	enum{
		IDC_SYS_ICON=100,
		IDC_SYS_TITLE=101,
		IDC_SYS_CLOSE=102,
		IDC_SYS_MIN=103,
		IDC_SYS_MAX = 104,
		IDC_SYS_RESTORE = 105,
	};
public:
	SNcPainter(SHostWnd * pHost);
	~SNcPainter(void);

	static int toNcBuiltinID(const SStringW & str);
public:
	STDMETHOD_(BOOL,InitFromXml)(THIS_ IXmlNode *pXmlNode) OVERRIDE;
	STDMETHOD_(IWindow*,GetRoot)(THIS) OVERRIDE;

protected:
	virtual void OnItemSetCapture(SOsrPanel *pItem, BOOL bCapture);

	virtual BOOL OnItemGetRect(const SOsrPanel *pItem, CRect &rcItem) const;

	virtual BOOL IsItemRedrawDelay() const;

	virtual IObject * GetHost();

protected:
	virtual BOOL OnFireEvent(IEvtArgs *e);

	virtual BOOL IsHostUpdateLocked() const;

	virtual BOOL IsHostVisible() const;

	virtual CRect GetHostRect() const;

	virtual void InvalidateHostRect(LPCRECT pRc);

	virtual ISwndContainer * GetHostContainer();

	virtual IRenderTarget * OnGetHostRenderTarget(LPCRECT rc, GrtFlag gdcFlags);

	virtual void OnReleaseHostRenderTarget(IRenderTarget *pRT,LPCRECT rc,GrtFlag gdcFlags);

	int GetScale() const;
public:
	SOUI_ATTRS_BEGIN()
		ATTR_LAYOUTSIZE(L"titleHeight",m_titleHeight,FALSE)
		ATTR_LAYOUTSIZE(L"borderWidth",m_borderWidth,FALSE)
		ATTR_SKIN(L"skinBorder",m_skinBorder,FALSE)
	SOUI_ATTRS_END()

protected:
	LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
	BOOL OnNcActivate(BOOL bActive);
	UINT OnNcHitTest(CPoint point);
	void OnNcPaint(HRGN hRgn);
	LRESULT OnSetText(LPCTSTR pszText);
	LRESULT OnRepaint(UINT msg,WPARAM wp,LPARAM lp);
	void OnNcDestroy();
	void OnNcLButtonDown(UINT flag,CPoint pt);
	LRESULT OnNcMouseEvent(UINT msg,WPARAM wp,LPARAM lp);
	LRESULT OnNcMouseLeave(UINT msg,WPARAM wp,LPARAM lp);
	void OnSize(UINT nType, CSize size);

	BEGIN_MSG_MAP_EX(SNcPainter)
		MSG_WM_NCCALCSIZE(OnNcCalcSize)
		MSG_WM_NCACTIVATE(OnNcActivate)
		MSG_WM_NCHITTEST(OnNcHitTest)
		MSG_WM_NCPAINT(OnNcPaint)
		MSG_WM_NCDESTROY(OnNcDestroy)
		MSG_WM_SETTEXT(OnSetText)
		MSG_WM_SIZE(OnSize)
		MSG_WM_NCLBUTTONDOWN(OnNcLButtonDown)
		MESSAGE_HANDLER_EX(WM_NCMOUSELEAVE,OnNcMouseLeave)
		MESSAGE_RANGE_HANDLER_EX(WM_NCMOUSEMOVE,WM_NCMBUTTONDBLCLK,OnNcMouseEvent)
		MESSAGE_HANDLER_EX(WM_SETTINGCHANGE,OnRepaint)
		MESSAGE_HANDLER_EX(WM_SYSCOLORCHANGE,OnRepaint)
	END_MSG_MAP()

protected:
	void Reset();
	BOOL IsDrawNc() const;
	void PaintCaption();
	SWindow* GetSRoot();

private:
	SHostWnd * m_pHost;

	SLayoutSize m_titleHeight;
	SLayoutSize m_borderWidth;
	SAutoRefPtr<ISkinObj> m_skinBorder;

	SOsrPanel * m_root;

	SAutoRefPtr<IRenderTarget> m_memRT;
	SAutoRefPtr<IRenderTarget> m_memLeft;
	SAutoRefPtr<IRenderTarget> m_memRight;
	SAutoRefPtr<IRenderTarget> m_memTop;
	SAutoRefPtr<IRenderTarget> m_memBottom;
	UINT		  m_htPart;
	BOOL		  m_bInPaint;
};

SNSEND
