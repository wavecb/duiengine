#include "StdAfx.h"
#include "ImageOle.h"
using namespace DuiEngine;

CTimerHostWnd CImageOle::ms_TimerHostWnd;

BOOL CTimerHostWnd::Create()
{
	return __super::Create(NULL,WS_POPUP,0,0,0,0,0,0,0)!=0;
}

void CTimerHostWnd::OnTimer( UINT_PTR idEvent )
{
	CImageOle *pOleObj=(CImageOle*)idEvent;
	pOleObj->OnTimer(idEvent);
}

CTimerHostWnd::CTimerHostWnd()
:m_nRef(0)
{
}

void CTimerHostWnd::AddRef()
{
	if(m_nRef==0) Create();
	m_nRef++;
}

void CTimerHostWnd::Release()
{
	m_nRef--;
	if(m_nRef==0) DestroyWindow();
}


CImageOle::CImageOle()
:m_ulRef(0)
,m_pOleClientSite(NULL)
,m_pAdvSink(NULL)
,m_pSkin(NULL)
,m_iFrame(0)
{
	ms_TimerHostWnd.AddRef();
}

CImageOle::~CImageOle(void)
{
	ms_TimerHostWnd.Release();
}

HRESULT WINAPI CImageOle::QueryInterface(REFIID iid, void ** ppvObject)
{
	if (iid == IID_IUnknown || iid == IID_IOleObject)
	{
		*ppvObject = (IOleObject *)this;
		((IOleObject *)(*ppvObject))->AddRef();
		return S_OK;
	}
	else if (iid == IID_IViewObject || iid == IID_IViewObject2)
	{
		*ppvObject = (IViewObject2 *)this;
		((IViewObject2 *)(*ppvObject))->AddRef();
		return S_OK;
	}

	return E_NOINTERFACE;
}

ULONG WINAPI CImageOle::AddRef(void)
{
	m_ulRef++;
	return m_ulRef;
}

ULONG WINAPI CImageOle::Release(void)
{
	if (--m_ulRef == 0)
	{
		delete this;
		return 0;
	}

	return m_ulRef;
}

HRESULT WINAPI CImageOle::SetClientSite(IOleClientSite *pClientSite)
{
	m_pOleClientSite = pClientSite;
	return S_OK;
}

HRESULT WINAPI CImageOle::GetClientSite(IOleClientSite **ppClientSite)
{
	*ppClientSite = m_pOleClientSite;
	return S_OK;
}

HRESULT WINAPI CImageOle::SetHostNames(LPCOLESTR szContainerApp, LPCOLESTR szContainerObj)
{
	return S_OK;
}

HRESULT WINAPI CImageOle::Close(DWORD dwSaveOption)
{
	m_pOleClientSite = NULL;
	if (m_pAdvSink != NULL)
	{
		m_pAdvSink->Release();
		m_pAdvSink = NULL;
	}
	if(m_pSkin)
	{
		m_pSkin->Release();
		m_pSkin=NULL;
	}

	ms_TimerHostWnd.KillTimer((UINT_PTR)this);
	return S_OK;
}

HRESULT WINAPI CImageOle::SetMoniker(DWORD dwWhichMoniker, IMoniker *pmk)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::InitFromData(IDataObject *pDataObject, BOOL fCreation, DWORD dwReserved)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::GetClipboardData(DWORD dwReserved, IDataObject **ppDataObject)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::DoVerb(LONG iVerb, LPMSG lpmsg, IOleClientSite *pActiveSite, LONG lindex, HWND hwndParent, LPCRECT lprcPosRect)
{
	return S_OK;
}

HRESULT WINAPI CImageOle::EnumVerbs(IEnumOLEVERB **ppEnumOleVerb)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::Update(void)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::IsUpToDate(void)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::GetUserClassID(CLSID *pClsid)
{
	*pClsid = IID_NULL;
	return S_OK;
}

HRESULT WINAPI CImageOle::GetUserType(DWORD dwFormOfType, LPOLESTR *pszUserType)
{
	return E_NOTIMPL;
}

// ���ÿؼ���ʹ�õĿռ䣨�ؼ�����Ļ��ʹ�õ�����Χ��
HRESULT WINAPI CImageOle::SetExtent(DWORD dwDrawAspect, SIZEL *psizel)
{
	return E_NOTIMPL;
}

// ��ȡ�ؼ���ʹ�õĿռ�
HRESULT WINAPI CImageOle::GetExtent(DWORD dwDrawAspect, SIZEL *psizel)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::Advise(IAdviseSink *pAdvSink, DWORD *pdwConnection)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::Unadvise(DWORD dwConnection)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::EnumAdvise(IEnumSTATDATA **ppenumAdvise)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::GetMiscStatus(DWORD dwAspect, DWORD *pdwStatus)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::SetColorScheme(LOGPALETTE *pLogpal)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::Draw(DWORD dwDrawAspect, LONG lindex, void *pvAspect, 
							   DVTARGETDEVICE *ptd, HDC hdcTargetDev, HDC hdcDraw, 
							   LPCRECTL lprcBounds, LPCRECTL lprcWBounds,
							   BOOL ( WINAPI *pfnContinue )(ULONG_PTR dwContinue), 
							   ULONG_PTR dwContinue)
{
	if (lindex != -1)
		return S_FALSE;

	RECT rcItem;
	rcItem.left   = lprcBounds->left;
	rcItem.top    = lprcBounds->top;
	rcItem.right  = lprcBounds->right;
	rcItem.bottom = lprcBounds->bottom;

	if(m_pSkin)
	{
		if(m_pSkin->IsClass(CDuiSkinGif::GetClassName()))
		{
			CDuiSkinGif *pSkinGif=static_cast<CDuiSkinGif*>(m_pSkin);
			pSkinGif->Draw(hdcDraw,rcItem,m_iFrame);
		}else
		{
			m_pSkin->Draw(hdcDraw,rcItem,0);
		}
	}
	return S_OK;
}

HRESULT WINAPI CImageOle::GetColorSet(DWORD dwDrawAspect, LONG lindex, void *pvAspect, 
									  DVTARGETDEVICE *ptd, HDC hicTargetDev, LOGPALETTE **ppColorSet)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::Freeze(DWORD dwDrawAspect, LONG lindex, void *pvAspect, DWORD *pdwFreeze)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::Unfreeze(DWORD dwFreeze)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::SetAdvise(DWORD aspects, DWORD advf, IAdviseSink *pAdvSink)
{
	if (pAdvSink != NULL)
	{
		m_pAdvSink = pAdvSink;
		m_pAdvSink->AddRef();
	}

	if (m_pSkin != NULL && m_pSkin->IsClass(CDuiSkinGif::GetClassName()))
	{
		CDuiSkinGif *pSkinGif=static_cast<CDuiSkinGif*>(m_pSkin);
		ms_TimerHostWnd.SetTimer((UINT_PTR)this, 
			pSkinGif->GetFrameDelay(), CImageOle::TimerProc);
	}

	return S_OK;
}

HRESULT WINAPI CImageOle::GetAdvise(DWORD *pAspects, DWORD *pAdvf, IAdviseSink **ppAdvSink)
{
	return E_NOTIMPL;
}

HRESULT WINAPI CImageOle::GetExtent(DWORD dwDrawAspect, LONG lindex, DVTARGETDEVICE *ptd, LPSIZEL lpsizel)
{
	if (m_pSkin != NULL)
	{
		SIZE sz=m_pSkin->GetSkinSize();

		HDC hDC = ::GetDC(NULL);
		lpsizel->cx = ::MulDiv(sz.cx, 2540, GetDeviceCaps(hDC, LOGPIXELSX));
		lpsizel->cy = ::MulDiv(sz.cx, 2540, GetDeviceCaps(hDC, LOGPIXELSY));
		::ReleaseDC(NULL, hDC);
	}

	return S_OK;
}


void CImageOle::OnTimer(UINT_PTR idEvent)
{
	ms_TimerHostWnd.KillTimer(idEvent);

	if (m_pAdvSink != NULL)
		m_pAdvSink->OnViewChange(DVASPECT_CONTENT, -1);

	CDuiSkinGif *pSkinGif=static_cast<CDuiSkinGif*>(m_pSkin);
	DUIASSERT(pSkinGif);
	m_iFrame++;
	if(m_iFrame==pSkinGif->GetStates())
		m_iFrame=0;
	long nDelay=pSkinGif->GetFrameDelay();
	ms_TimerHostWnd.SetTimer((UINT_PTR)this, nDelay, CImageOle::TimerProc);
}

VOID CALLBACK CImageOle::TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CImageOle *pImageOle = (CImageOle *)idEvent;
	if (pImageOle != NULL)
		pImageOle->OnTimer(idEvent);
}

void CImageOle::SetDuiSkinObj( CDuiSkinBase *pSkin )
{
	if(m_pSkin)
	{
		m_pSkin->Release();
	}
	m_pSkin=pSkin;
	if(m_pSkin)
	{
		m_pSkin->AddRef();
	}
}

BOOL RichEdit_InsertSkin(CDuiRichEdit *pRicheditCtrl, CDuiSkinBase *pSkin)
{
	IRichEditOle *pRichEditOle=NULL;
	LRESULT lRes=pRicheditCtrl->DuiSendMessage(EM_GETOLEINTERFACE,0,(LPARAM)&pRichEditOle);
	if(!pRichEditOle) return FALSE;

	SCODE sc;
	IOleClientSite *pOleClientSite = NULL;
	pRichEditOle->GetClientSite(&pOleClientSite);
	if (NULL == pOleClientSite)
		return FALSE;

	IStorage *pStorage = NULL;

	LPLOCKBYTES lpLockBytes = NULL;
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		return FALSE;

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStorage);
	if (sc != S_OK)
	{
		lpLockBytes->Release();
		lpLockBytes = NULL;
		return FALSE;
	}


	CImageOle *pImageOle = new CImageOle;
	if (NULL == pImageOle)
		return FALSE;

	pImageOle->SetDuiSkinObj(pSkin);

	IOleObject *pOleObject = NULL;
	pImageOle->QueryInterface(IID_IOleObject, (void **)&pOleObject);
	if (NULL == pOleObject)
	{
		delete pImageOle;
		return FALSE;
	}

	HRESULT hr = ::OleSetContainedObject(pOleObject, TRUE);

	REOBJECT reobject = {0};
	reobject.cbStruct = sizeof(REOBJECT);
	reobject.clsid    = CLSID_NULL;
	reobject.cp       = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.poleobj  = pOleObject;
	reobject.polesite = pOleClientSite;
	reobject.pstg     = pStorage;
	reobject.dwUser   = 0;

	pRichEditOle->InsertObject(&reobject);

	pOleObject->Release();
	pOleClientSite->Release();
	pStorage->Release();
	pRichEditOle->Release();

	return TRUE;
}

BOOL RichEdit_InsertImage(CDuiRichEdit *pRicheditCtrl, LPCTSTR lpszFileName)
{
	CDuiImgX *pImg=new CDuiImgX;
	if(!pImg->LoadImg(lpszFileName))
	{
		delete pImg;
		return FALSE;
	}
	CDuiSkinGif *pSkinGif=new CDuiSkinGif;
	pSkinGif->SetImage(pImg);
	BOOL bRet = RichEdit_InsertSkin(pRicheditCtrl,pSkinGif);
	pSkinGif->Release();
	return bRet;
}