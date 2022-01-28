
//从PE文件加载，注意从文件加载路径位置
#define RES_TYPE 0
//#define SYSRES_TYPE 0
// #define RES_TYPE 0   //PE
// #define RES_TYPE 1   //ZIP
// #define RES_TYPE 2   //7z
// #define RES_TYPE 2   //文件
//去掉多项支持，以免代码显得混乱
#if (RES_TYPE==1)
#include "resprovider-zip\zipresprovider-param.h"
#else 
#if (RES_TYPE==2)
#include "resprovider-7zip\zip7resprovider-param.h"
#endif
#endif
#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif

#include <interface/SFactory-i.h>

#define INIT_R_DATA
#include "res/resource.h"

#include "ComLoader.h"

//debug时方便调试设置当前目录以便从文件加载资源
void SetDefaultDir()
{
	TCHAR szCurrentDir[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));

	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
#ifdef _DEBUG
	_tcscpy(lpInsertPos + 1, _T("..\\demo2"));
#else
	_tcscpy(lpInsertPos + 1, _T("\0"));
#endif
	SetCurrentDirectory(szCurrentDir);
}

BOOL OnBtnClose(IEvtArgs *evt,void *Ctx)
{
	IHostWnd *pHostWnd = (IHostWnd*)Ctx;
	pHostWnd->DestroyWindow();
	return TRUE;
}

BOOL OnBtnEndDialog(IEvtArgs *evt,void *Ctx)
{
	IHostDialog *hostWnd = (IHostDialog *)Ctx;
	hostWnd->EndDialog(IDOK);
	return TRUE;
}

BOOL OnHostMsg(const LPMSG pMsg,LRESULT *pRes,void *Ctx)
{
	if(pMsg->message != WM_INITDIALOG)
	{
		return FALSE;
	}
	IHostDialog *hostWnd = (IHostDialog *)Ctx;
	IWindow* root = hostWnd->GetIRoot();
	IWindow* btnClose = root->FindIChildByID(R.id.btn_close,-1);
	btnClose->SubscribeEvent(EventCmd::EventID,&Subscriber(OnBtnEndDialog,hostWnd));

	return TRUE;
}


BOOL LoadRenderFac(IRenderFactory ** ppRenderFac){
	BOOL bLoaded = TRUE;
	IImgDecoderFactory *pImgDecoderFactory;
	do{
		//使用GDI渲染界面
		bLoaded = LoadComObj(Render_Gdi,(IObjRef * *)ppRenderFac);
		SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
		if(!bLoaded) break;
		//设置图像解码引擎。默认为GDIP。基本主流图片都能解码。系统自带，无需其它库
		bLoaded = LoadComObj(Decoder_Gdip,&pImgDecoderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));
		if(!bLoaded) break;

		(*ppRenderFac)->SetImgDecoderFactory(pImgDecoderFactory);
		pImgDecoderFactory->Release();
	}while(false);
	return bLoaded;
}

//加载系统资源
BOOL LoadSystemRes(IApplication *pApp)
{
	HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
	if (hModSysResource)
	{
		IResProvider * sysResProvider;
		CreateResProvider(RES_PE, (IObjRef * *)& sysResProvider);
		sysResProvider->Init((WPARAM)hModSysResource, 0);
		pApp->LoadSystemNamedResource(sysResProvider);
		sysResProvider->Release();
		FreeLibrary(hModSysResource);
		return TRUE;
	}
	else
	{
		SASSERT(FALSE);
		return FALSE;
	}
}

//加载用户资源
BOOL LoadUserRes(IApplication *pApp)
{
	IResProvider*   pResProvider;
	BOOL bLoaded = FALSE;
	do{
#ifdef _DEBUG		
		//选择了仅在Release版本打包资源则在DEBUG下始终使用文件加载
		{
			CreateResProvider(RES_FILE, (IObjRef * *)& pResProvider);
			bLoaded = pResProvider->Init((LPARAM)_T("uires"), 0);
			SASSERT(bLoaded);
			if(!bLoaded) break;
		}
#else
		{
			CreateResProvider(RES_PE, (IObjRef * *)& pResProvider);
			bLoaded = pResProvider->Init((WPARAM)pApp->GetModule(), 0);
			SASSERT(bLoaded);
			if(!bLoaded) break;
		}
#endif
		pApp->InitXmlNamedID((const LPCWSTR*)&R.name, (const int*)&R.id,sizeof(R.id)/sizeof(int));
		pApp->GetResProviderMgr()->AddResProvider(pResProvider);
	}while(false);
	return bLoaded;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	HRESULT hRes = OleInitialize(NULL);
	SASSERT(SUCCEEDED(hRes));
	int nRet = 0;
	{
		ISouiFactory * m_factory =NULL;
		IApplication * theApp = NULL;
		IRenderFactory * pRenderFac = NULL;
		CreateSouiFactory(&m_factory);
		SetDefaultDir();

		if(!LoadRenderFac(&pRenderFac))
			return -1;
		m_factory->CreateApp(&theApp,pRenderFac,hInstance,L"test",FALSE);
		if(!LoadSystemRes(theApp))
			return -2;
		if(!LoadUserRes(theApp))
			return -3;

		{
			//使用接口方式创建HostWnd。
			IHostWnd * hostWnd;
			IHostDialog* hostDialog;
			m_factory->CreateHostWnd(&hostWnd,UIRES.LAYOUT.XML_MAINWND);
			m_factory->CreateHostDialog(&hostDialog,UIRES.LAYOUT.DLG_TEST);

			hostWnd->Create(NULL,0,0,0,0);
			hostWnd->ShowWindow(SW_SHOWNORMAL);

			//使用事件订阅方式响应界面控件事件。
			{
				IWindow* root = hostWnd->GetIRoot();
				IWindow* btnClose = root->FindIChildByID(R.id.btn_close,-1);
				btnClose->SubscribeEvent(EventCmd::EventID,&Subscriber(OnBtnClose,hostWnd));
			}

			hostDialog->SetMsgHandler(OnHostMsg,hostDialog);
			hostDialog->DoModal(NULL);

			nRet = theApp->Run(hostWnd->GetHwnd());

			hostWnd->Release();
			hostDialog->Release();
		}
		else
		{
			MessageBox(NULL, _T("无法正常初使化SOUI"), _T("错误"), MB_OK | MB_ICONERROR);
		}
	}
	OleUninitialize();
	return nRet;
}
