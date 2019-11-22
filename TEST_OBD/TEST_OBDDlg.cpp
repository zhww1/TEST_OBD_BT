
// TEST_OBDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TEST_OBD.h"
#include "TEST_OBDDlg.h"
#include "afxdialogex.h"
#include<string>
#include "cwpack.h"
#include "Utilities.h"
#include "CwpackUtils.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)


END_MESSAGE_MAP()


// CTEST_OBDDlg 对话框



CTEST_OBDDlg::CTEST_OBDDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTEST_OBDDlg::IDD, pParent)
	, m_pHwCOM(NULL)
	, m_hThread(NULL)
	, m_dwThreadID(0)
	, m_hEvtEndThread(NULL)
	, m_hEvtSendDataReq(NULL)
	, m_hEvtScanDeviceReq(NULL)
	, m_nBytesForSending(0)
	,m_bNoteBeCleared(FALSE)
	, m_bRecvProcess(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_LOGO);
}

void CTEST_OBDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BMP_STATUS, m_bmpStatus);
}

BEGIN_MESSAGE_MAP(CTEST_OBDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	//ListView操作
	ON_COMMAND(ID_LIST_DELETE_ITEM, OnListDeleteItem)
	ON_COMMAND(ID_SAVE_LIST_LOG, OnSaveListLog)
	ON_COMMAND(ID_LIST_EMPTY_ITEM, OnListEmptyItem)
	ON_MESSAGE(WM_CLIENTMAINRCLICK, OnMainRClick)
	
	ON_BN_CLICKED(IDC_BTN_FINDDEVICE, &CTEST_OBDDlg::OnBnClickedBtnFinddevice)
	ON_BN_CLICKED(IDC_BTN_PING, &CTEST_OBDDlg::OnBnClickedBtnPing)
	ON_BN_CLICKED(IDC_BTN_ISOTP, &CTEST_OBDDlg::OnBnClickedBtnIsotp)
	ON_BN_CLICKED(IDC_BTN_CANCONFIG, &CTEST_OBDDlg::OnBnClickedBtnCanconfig)

	ON_BN_CLICKED(IDC_BTN_L2Unpack, &CTEST_OBDDlg::OnBnClickedBtnL2unpack)

	
	ON_BN_CLICKED(IDC_BUTTON_REQUESTAUTHENTICATE, &CTEST_OBDDlg::OnBnClickedButtonRequestauthenticate)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, &CTEST_OBDDlg::OnBnClickedButtonConnect)
	
	ON_LBN_DBLCLK(IDC_LIST_DEVLIST, &CTEST_OBDDlg::OnDblclkListDevlist)
	ON_BN_CLICKED(IDC_BUTTON_ISOTP, &CTEST_OBDDlg::OnBnClickedButtonIsotp)
END_MESSAGE_MAP()

DWORD WINAPI ThreadProc_COM(
	LPVOID lpParameter   // thread data
	)
{
	CTEST_OBDDlg *pDlgBlueToothExc = reinterpret_cast<CTEST_OBDDlg*>(lpParameter);
	if (pDlgBlueToothExc)
		pDlgBlueToothExc->ThreadProc_COM();
	return TRUE;
}

LRESULT CTEST_OBDDlg::OnMainRClick(WPARAM wParam, LPARAM lParam)
{
	CPoint m_point;
	//CNewMenu m_menu;
	CMenu m_menu;
	m_menu.LoadMenu(wParam);
	GetCursorPos(&m_point);
	SetForegroundWindow();
	m_menu.GetSubMenu(0)->
		TrackPopupMenu(TPM_LEFTALIGN |
		TPM_RIGHTBUTTON,
		m_point.x, m_point.y, this);
	m_menu.DestroyMenu();
	return TRUE;

}
// CTEST_OBDDlg 消息处理程序

BOOL CTEST_OBDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CRect logrt;
	GetClientRect(logrt);



	//设置大小
	logrt.left = logrt.left;
	logrt.top = (logrt.bottom) * 0.70;
	logrt.right = logrt.right ;
	logrt.bottom = logrt.bottom - 21;

	m_wndMyEventView = (CMyEventView*)RUNTIME_CLASS(CMyEventView)->CreateObject(); //这条语句不能少，有的教程中少了，会出错。

	m_wndMyEventView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW | WS_VSCROLL | WS_HSCROLL | WS_CHILD, logrt, this, 0, NULL);
	//使用CreateView创建的视图 不能自动调用OnInitialUpdate函数，需要人工调用OnInitialUpdate函数或者发送 WM_INITIALUPDATE消息
	m_wndMyEventView->OnInitialUpdate();
	// 使用CreateView创建的视图不会自动显示并且激活，需要人工操作 
	m_wndMyEventView->ShowWindow(SW_SHOW);
	// 初始化控件
	((CButton*)GetDlgItem(IDC_CHECK_SHOWREMEMBERED))->SetCheck(TRUE);

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_COM);
	ASSERT(pCombo);
	for (int i = 0; i < 20; i++)
	{
		CString csCom;
		csCom.Format(L"COM%d", i + 1);
		pCombo->AddString(csCom);
	}
	pCombo->SetCurSel(1 - 1);	// 默认在控件中显示的端口号
	GetDlgItem(IDC_EDIT_BAUD)->SetWindowText(L"115200");
	
	//((CButton*)GetDlgItem(IDC_CHECK_Hex))->SetCheck(TRUE);
	GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(FALSE);
	EnableBtns(FALSE);

	//-------------载入图标--------------------
	hIcons[0] = AfxGetApp()->LoadIcon(IDI_ICON_CLOSED);//串口关   
	hIcons[1] = AfxGetApp()->LoadIcon(IDI_ICON_OPENED);//串口开   

	//-------------初始化状态栏-------------------------	

	CRect Rect;
	this->GetClientRect(&Rect);                  //获取客户区域

	int strPartDim[3];
	strPartDim[0] = (int)(Rect.Width()*0.50);
	strPartDim[1] = (int)(Rect.Width()*0.85);
	strPartDim[2] = (int)(Rect.Width());

	m_StatusBar.Create(WS_CHILD | WS_VISIBLE | SBT_OWNERDRAW, CRect(0, 0, 0, 0), this, 0);
	m_StatusBar.SetParts(3, strPartDim);

	//设置状态栏文本内容
	m_StatusBar.SetText(_T("就绪"), 0, 0);
	m_StatusBar.SetText(_T("郑州越博汽车电器"), 1, 0);

	//创建内核对象
	ExitCOMThread();// 先去结束之前创建的线程
	if (!HANDLE_IS_VALID(m_hEvtEndThread))
		m_hEvtEndThread = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	if (!HANDLE_IS_VALID(m_hEvtSendDataReq))
		m_hEvtSendDataReq = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!HANDLE_IS_VALID(m_hEvtScanDeviceReq))
		m_hEvtScanDeviceReq = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!HANDLE_IS_VALID(m_hThread))
		m_hThread = ::CreateThread(NULL, 0, ::ThreadProc_COM, this, 0, &m_dwThreadID);
	if (!HANDLE_IS_VALID(m_hThread) || !HANDLE_IS_VALID(m_hEvtEndThread) || !HANDLE_IS_VALID(m_hEvtSendDataReq) || !HANDLE_IS_VALID(m_hEvtScanDeviceReq))
	{
		EnableBtns(FALSE);
		return FALSE;
	}
	m_bRecvProcess = TRUE;
	initBuff();
	//ShowMyText(_T(" 初始化完成"), FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTEST_OBDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTEST_OBDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTEST_OBDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTEST_OBDDlg::OnListDeleteItem()
{
	int nCount = m_wndMyEventView->GetListCtrl().GetSelectedCount();
	for (int i = 0; i < nCount; i++)
	{
		int nItem = m_wndMyEventView->GetListCtrl().
			GetNextItem(-1, LVNI_SELECTED);
		m_wndMyEventView->GetListCtrl().DeleteItem(nItem);
	}
	m_wndMyEventView->SendMessage(WM_RESIZE_WND, 0, 0);
}

void CTEST_OBDDlg::OnSaveListLog()
{
	CString szFilter = _T("日志文件(*.txt)|*.txt|");
	CFileDialog m_filedialog(FALSE, NULL, _T("pslog.txt"),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (m_filedialog.DoModal() != IDOK) return;

	CFile m_File(m_filedialog.GetPathName(),
		CFile::modeCreate | CFile::modeWrite);
	int nCount = m_wndMyEventView->GetListCtrl().GetSelectedCount();
	for (int  i = 0; i < m_wndMyEventView->
		GetListCtrl().GetItemCount(); i++)
	{
		TCHAR m_Text[512] = { 0 };
		CString timer = m_wndMyEventView->GetListCtrl().GetItemText(i, 0);
		CString text = m_wndMyEventView->GetListCtrl().GetItemText(i, 1);
		_stprintf(m_Text, _T("发生时间：%s 事件内容：%s\r\n"),//换行
			timer,
			text);

		//m_File.Write(m_Text, _tcslen(m_Text));
		m_File.Write(Utilities::EncodeConvert::UnicodeToAnsi(m_Text), strlen(Utilities::EncodeConvert::UnicodeToAnsi(m_Text)));
	}
	m_File.Close();


	


}

void CTEST_OBDDlg::OnListEmptyItem()
{
	m_wndMyEventView->GetListCtrl().DeleteAllItems();
	m_wndMyEventView->SendMessage(WM_RESIZE_WND, 0, 0);
}






void CTEST_OBDDlg::OnBnClickedBtnFinddevice()
{
	// TODO:  在此添加控件通知处理程序代码
	//决定改为线程里面处理
	::SetEvent(m_hEvtScanDeviceReq);
	m_StatusBar.SetText(_T("查找设备中"), 0, 0);
}

void CTEST_OBDDlg::ShowMyText(LPCTSTR Text, BOOL Flag)
{
	m_wndMyEventView->InsertItem(Text, Flag);
}

unsigned char LRC_Calc(unsigned char *buf, int n)
{
	int i;

	unsigned char lrc = 0;
	for (i = 0; i < n; i++)
		lrc ^= buf[i];

	return lrc;
}


CString  ucToCString(unsigned char * buff, int len)
{
	CString  str;
	TCHAR sz[4] = { 0 };
	for (int i = 0; i < len; i++)
	{
		// 空格 +2 个字符 =3 字符由于后面 需要  \0需要在加1
		_stprintf(sz, TEXT("%02X"), buff[i]);
		str += sz;
	}
	return str;

}



void CTEST_OBDDlg::OnBnClickedBtnPing()
{
	// TODO:  在此添加控件通知处理程序代码
	cw_pack_context pc;
	uint8_t lrc;
	m_StatusBar.SetText(_T("PING功能测试"), 0, 0);
	
	cw_pack_context_init(&pc, (m_pL2SendBuff), 50, 0);
	cw_pack_unsigned(&pc, 0x01);//L2   PING
	if (CWP_RC_OK != pc.return_code){
		return;
	}

	int length = pc.current - pc.start;
	int totallength = GenMsgL1Data(m_pL2SendBuff, length, m_ucL1Buff);

	if (totallength == FALSE)
	{
		return;
	}
	MsgL1SendData(m_ucL1Buff, totallength,_T("Ping:"));
	
	/*
	unsigned char  mbaddr =1234;
	CString straddr;
	straddr.Format(_T("%d"), mbaddr);
	CString  BAUD = _T("11233432142314");
	m_StatusBar.SetText(_T("【测试波特率】：") + BAUD + _T("  【测试地址】") + straddr, 0, 0);
	*/
}

void CTEST_OBDDlg::EnableBtns(bool enable)
{
	
	GetDlgItem(IDC_BTN_PING)->EnableWindow(enable);
	GetDlgItem(IDC_BTN_CANCONFIG)->EnableWindow(enable);
	GetDlgItem(IDC_BTN_ISOTP)->EnableWindow(enable);
	GetDlgItem(IDC_BTN_L2Unpack)->EnableWindow(enable);
	GetDlgItem(IDC_BUTTON_ISOTP)->EnableWindow(enable);
	
	
}


void CTEST_OBDDlg::OnBnClickedBtnIsotp()
{
	// TODO:  在此添加控件通知处理程序代码

	cw_pack_context pc;
	uint8_t lrc;
	//unsigned char *pL2Buff[50];
	m_StatusBar.SetText(_T("ISOTP功能测试"), 0, 0);

	CString str = _T("ECU");
	/*L1Buff[0] = 0xA1;
	L1Buff[1] =0 ;
	L1Buff[2] = 0;*/
	//char buffer[10];
	cw_pack_context_init(&pc, (m_pL2SendBuff), 50, 0);
	cw_pack_unsigned(&pc, 0x05);//L2   类型 ISOTP 
	if (CWP_RC_OK != pc.return_code){
		return ;
	}
	cw_pack_unsigned(&pc, 1234);//L2  ID
	if (CWP_RC_OK != pc.return_code){
		return;
	}
	std::string ss = Utilities::EncodeConvert::UnicodeToAnsi(str);
	cw_pack_bin(&pc, ss.c_str(), str.GetLength());
	if (CWP_RC_OK != pc.return_code){
		return;
	}
	int length = pc.current - pc.start;

	/*uint16_t totallength;
	totallength = 1 + 2 + length + 1;
	L1Buff[0] = 0xA1;
	L1Buff[1] = (uint8_t)(totallength >> 8);
	L1Buff[2] = (uint8_t)(totallength >> 0);
	lrc = LRC_Calc(L1Buff, totallength - 1);
	L1Buff[totallength - 1] = lrc;*/
	int totallength = GenMsgL1Data(m_pL2SendBuff, length, m_ucL1Buff);

	if (totallength == FALSE)
	{
		return;
	}
	MsgL1SendData(m_ucL1Buff, totallength, _T("ISOTP:"));
	

}


void CTEST_OBDDlg::OnBnClickedBtnCanconfig()
{
	// TODO:  在此添加控件通知处理程序代码
	cw_pack_context pc;
	uint8_t lrc;
	m_StatusBar.SetText(_T("CAN CONFIG功能测试"), 0, 0);

	cw_pack_context_init(&pc, (m_pL2SendBuff), 50, 0);
	cw_pack_unsigned(&pc, 0x03);//L2   类型 CAN配置
	if (CWP_RC_OK != pc.return_code){
		return;
	}
	cw_pack_unsigned(&pc, 500);//L2  bps
	if (CWP_RC_OK != pc.return_code){
		return;
	}
	cw_pack_unsigned(&pc, 1234);//L2  ECUID
	if (CWP_RC_OK != pc.return_code){
		return;
	}
	cw_pack_unsigned(&pc, 1234);//L2  TESTERID
	if (CWP_RC_OK != pc.return_code){
		return;
	}
	cw_pack_unsigned(&pc, 1234);//L2  FUNCID
	if (CWP_RC_OK != pc.return_code){
		return;
	}

	int length = pc.current - pc.start;
	int totallength = GenMsgL1Data(m_pL2SendBuff, length, m_ucL1Buff);

	if (totallength == FALSE)
	{
		return;
	}

	MsgL1SendData(m_ucL1Buff, totallength,_T("Can Config:"));
	/*
	unsigned char  mbaddr =1234;
	CString straddr;
	straddr.Format(_T("%d"), mbaddr);
	CString  BAUD = _T("11233432142314");
	m_StatusBar.SetText(_T("【测试波特率】：") + BAUD + _T("  【测试地址】") + straddr, 0, 0);
	*/

}

void CTEST_OBDDlg::OnBnClickedBtnL2unpack()
{
	// TODO:  在此添加控件通知处理程序代码
	
	uint8_t L2buff[100] = { 0 };
	/*cw_unpack_context uc;
	uint8_t buff[8] = { 0 };
	CwpackUtils  wp;
	uint64_t type;
	int outlen;
	int ret;
	L2buff[0] = 0x06;
	L2buff[1] = 0xC4;
	L2buff[2] = 0x02;
	L2buff[3] = 0x01;
	L2buff[4] = 0x01;

	cw_unpack_context_init(&uc, L2buff, 100, NULL);
	ret = wp.ReadUnsigned(&uc, &type);
	ret = wp.ReadBin(&uc, buff, 8, &outlen);*/

	cw_pack_context pc;
	cw_pack_context_init(&pc, L2buff, 100, 0);

	cw_pack_unsigned(&pc, 50);
	cw_pack_map_size(&pc, 6);
	cw_pack_str(&pc, "NJ1", 7);
	cw_pack_unsigned(&pc, 50);
	cw_pack_str(&pc, "ZS1", 6);
	cw_pack_unsigned(&pc, 50);

	cw_pack_str(&pc, "NJ2", 7);
	cw_pack_unsigned(&pc, 50);
	cw_pack_str(&pc, "ZS2", 6);
	cw_pack_unsigned(&pc, 50);

	cw_pack_str(&pc, "NJ3", 7);
	cw_pack_unsigned(&pc, 50);
	cw_pack_str(&pc, "ZS3", 6);
	cw_pack_unsigned(&pc, 50);



}











BOOL CTEST_OBDDlg::GetCur_LocalRadioInfo(t_LocalRadioInfo& LocalRadioInfo)
{
	// 先确定本地蓝牙设备，如果还没有就查找
	
	int nSel_LocalBluetoothRadios = 0;
	if (m_BlueTooth.m_Ary_LocalRadioInfo.GetSize() < 1)
	{
		EnumerateLocal();
		if (m_BlueTooth.m_Ary_LocalRadioInfo.GetSize() < 1)
		{
			return FALSE;
		}
		
	}

	
	ASSERT(m_BlueTooth.m_Ary_LocalRadioInfo.GetSize() == m_vecLocalBluetoothRadios.size());

	// 默认选择第一个

	t_LocalRadioInfo& LocalRadioInfo_Ref = m_BlueTooth.m_Ary_LocalRadioInfo.GetAt(nSel_LocalBluetoothRadios);
	//带出去
	memcpy(&LocalRadioInfo, &LocalRadioInfo_Ref, sizeof(t_LocalRadioInfo));

	return TRUE;
}

void CTEST_OBDDlg::EnumerateLocal()
{
	
	int nCount = m_BlueTooth.EnumerateLocalRadios();
	if (nCount <= 0)
	{
		AfxMessageBox(_T("None any local bluetooth radios"));
		return;
	}
	for (int i = 0; i < nCount; i++)
	{
		t_LocalRadioInfo &LocalRadioInfo = m_BlueTooth.m_Ary_LocalRadioInfo.GetAt(i);
		CString csItemString;
		csItemString.Format(_T("%s ( %s )"), LocalRadioInfo.RadioInfo.szName, m_BlueTooth.FormatBthAddress(LocalRadioInfo.RadioInfo.address.rgBytes));
		m_vecLocalBluetoothRadios.push_back(csItemString);
		
	}
}


void CTEST_OBDDlg::OnBnClickedButtonRequestauthenticate()
{
	// TODO:  在此添加控件通知处理程序代码
	// 先确定本地蓝牙设备，如果还没有就查找
	m_StatusBar.SetText(_T("请求配对验证"), 0, 0);
	t_LocalRadioInfo LocalRadioInfo;
	if (!GetCur_LocalRadioInfo(LocalRadioInfo)) return;

	// 当前选择的远程蓝牙设备
	int nSel = GetListCurSel(IDC_LIST_DEVLIST, _T("请选择远程蓝牙设备"));
	if (nSel < 0) return;
	DWORD result = ERROR_SUCCESS;
	if (!m_BlueTooth.RequestAuthenticateDevice(nSel, GetSafeHwnd(), &result))
	{
		CString csMsg;
		csMsg.Format(_T("验证失败使用的验证码 [%s] : %s"), AUTHENTICATION_PASSKEY, hwFormatMessage(result));
		AfxMessageBox(csMsg);
	}
	else
	{
		CString csMsg;
		csMsg.Format(_T("验证成功使用验证码为 [%s] : %s"), AUTHENTICATION_PASSKEY, hwFormatMessage(result));
		AfxMessageBox(csMsg);
	}

}

int CTEST_OBDDlg::GetListCurSel(UINT nID, LPCTSTR lpszMsg)
{
	CListBox *pListBox = (CListBox*)GetDlgItem(nID);
	if (!pListBox) return -1;
	int nSel = pListBox->GetCurSel();
	if (nSel < 0 || nSel >= pListBox->GetCount())
	{
		if (lpszMsg) AfxMessageBox(lpszMsg);
		return -1;
	}

	return nSel;
}


void CTEST_OBDDlg::OnBnClickedButtonConnect()
{
	// TODO:  在此添加控件通知处理程序代码
	m_StatusBar.SetText(_T("连接/断开"), 0, 0);
	CWaitCursor WaitCurSor;
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_COM);
	ASSERT(pCombo);
	if (m_pHwCOM != NULL)
	{
		///ExitCOMThread();
		// 发送一个空循环标识
		m_bRecvProcess = FALSE;
		DeleteCOMObject();
		SendDlgItemMessage(IDC_BUTTON_CONNECT, WM_SETTEXT, 0, (LPARAM)_T("连接"));
		EnableBtns(FALSE);
		m_bmpStatus.SetIcon(hIcons[0]);//修改图标为打开串口图标
		return;
	}
	m_bRecvProcess = TRUE;
	int nSel = pCombo->GetCurSel();
	if (nSel < 0 || nSel >= pCombo->GetCount())
	{
		AfxMessageBox(L"请选择COM");
		return;
	}
	int nCOM = nSel + 1;
	int nBaudRate = (int)GetDlgItemInt(IDC_EDIT_BAUD, NULL, FALSE);
	DeleteCOMObject();

	m_pHwCOM = new CHwCOM;
	if (!m_pHwCOM)
	{
		return;
	}
	

	//连接蓝牙设备
	if (!m_pHwCOM->OpenCOM(nCOM, nBaudRate, TRUE))
	{
		DeleteCOMObject();
		CString csMsg;
		csMsg.Format(L"打开 COM%d 失败 : %s", nCOM, hwFormatMessage(GetLastError()));
		AfxMessageBox(csMsg);
		return;
	}
	SendDlgItemMessage(IDC_BUTTON_CONNECT, WM_SETTEXT, 0, (LPARAM)_T("断开"));
	m_bmpStatus.SetIcon(hIcons[1]);//修改图标为打开串口图标
	m_pHwCOM->SetCommMask(EV_RXCHAR);
	EnableBtns(TRUE);
}

void CTEST_OBDDlg::DeleteCOMObject()
{
	if (m_pHwCOM)
	{
		delete m_pHwCOM;
		m_pHwCOM = NULL;
	}
}

void CTEST_OBDDlg::ExitCOMThread(void)
{
	if (HANDLE_IS_VALID(m_hEvtEndThread))
		::SetEvent(m_hEvtEndThread);
	WaitForThreadEnd(&m_hThread);  //WaitForSingleObject
}

void CTEST_OBDDlg::ThreadProc_COM(void)
{
	HANDLE hAryEvt[] = { m_hEvtEndThread, m_hEvtSendDataReq, m_hEvtScanDeviceReq };//增加一个查询
	while (TRUE)
	{
		DWORD dwRet = ::WaitForMultipleObjects(LENGTH(hAryEvt), hAryEvt, FALSE, 100) - WAIT_OBJECT_0; //需要考虑多个内核对象同时被触发 返回时最小的一个内核对象，这个问题百度可以解决
		// 请求线程终止
		if (dwRet == 0)
			return;
		// 发送数据到串口
		else if (dwRet == 1)
		{
			if (m_pHwCOM)
			{
				//这里必须加锁防止多个线程写拷贝 write 实际写已经有了锁
				m_CSFor_DataForSending.Lock();
				char DataForSending[MAX_COM_BUFSIZE] = { 0 };
				int nBytesForSending = m_nBytesForSending;
				memcpy(DataForSending, m_DataForSending, nBytesForSending);
				memset(m_DataForSending, 0, sizeof(m_DataForSending));
				m_nBytesForSending = 0;
				m_CSFor_DataForSending.Unlock();
				if (m_pHwCOM->Write(DataForSending, nBytesForSending) != nBytesForSending)
				{
					AfxMessageBox(_T("Send data failed"));
					continue;
				}
			}
			
		}
		//查询系统蓝牙设备
		else if (dwRet ==2)
		{
			// 先确定本地蓝牙设备，如果还没有就查找
			GetDlgItem(IDC_BTN_FINDDEVICE)->EnableWindow(FALSE);
			t_LocalRadioInfo LocalRadioInfo;
			if (!GetCur_LocalRadioInfo(LocalRadioInfo))
			{
				AfxMessageBox(_T("未查找到本机蓝牙设备！！请确认本机是否有蓝牙设备"));
				
				continue;
				
			}
			// 搜索附近的蓝牙设备，并添加到列表中	
			CListBox *pListBox_NearbyBthDev = (CListBox*)GetDlgItem(IDC_LIST_DEVLIST);
			ASSERT(pListBox_NearbyBthDev);
			pListBox_NearbyBthDev->ResetContent();

			BOOL fReturnRemembered = ((CButton*)GetDlgItem(IDC_CHECK_SHOWREMEMBERED))->GetCheck();
			BOOL fIssueInquiry = ((CButton*)GetDlgItem(IDC_CHECK_ISSUEINQUIRY))->GetCheck();
			//BOOL fReturnRemembered = TRUE;
			//BOOL fIssueInquiry = TRUE;
			int nCount = 0;
			//选择了使用向导
			if (((CButton*)GetDlgItem(IDC_CHECK_WIZARD))->GetCheck())
			{
				nCount = m_BlueTooth.ScanNearbyBthDev_Wizard(GetSafeHwnd(), LocalRadioInfo.hRadio, _T("我现在正在搜索周边的蓝牙设备，您别打扰我！ "),
					FALSE, TRUE, fReturnRemembered, TRUE, TRUE, TRUE);
			}
			else //查找附近的蓝牙设备
			{
				nCount = m_BlueTooth.ScanNearbyBthDev(LocalRadioInfo.hRadio, TRUE, fReturnRemembered, TRUE, TRUE, fIssueInquiry);//全TURE
			}
			if (nCount <= 0)
			{
				if (nCount < 0)
				{
					AfxMessageBox(_T("查找附近蓝牙设备失败"));
					
					continue;
				}
				
			}
			for (int i = 0; i < nCount; i++)
			{
				t_RemoteBthDevInfo &RemoteBthDevInfo = m_BlueTooth.m_Ary_RemoteBthDevInfo.GetAt(i);
				CString csItemString;
				csItemString.Format(_T("%s ( %s )"), RemoteBthDevInfo.szName, m_BlueTooth.FormatBthAddress(RemoteBthDevInfo.Address.rgBytes));
				pListBox_NearbyBthDev->AddString(csItemString);
			}
			//使能连接按钮
			GetDlgItem(IDC_BUTTON_CONNECT)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_FINDDEVICE)->EnableWindow(TRUE);
			
	/*		CString str;
			str.Format(_T("查找完成，本次已找到%d个蓝牙设备"),nCount);
			ShowMyText(str, FALSE);*/
			;
			//EnableBtns(TRUE);
		}
		// 从串口读取数据
		else
		{
			if (m_bRecvProcess)
			{
				ReadDataAndDisplay();
			}
			
		}
	}
}





void CTEST_OBDDlg::OnDblclkListDevlist()
{
	// TODO:  在此添加控件通知处理程序代码

	int nSel = GetListCurSel(IDC_LIST_DEVLIST, _T("请选择一个远程蓝牙设备"));
	if (nSel < 0) return;
	m_BlueTooth.ShowPropertySheet(nSel, GetSafeHwnd());
}

BOOL CTEST_OBDDlg::SendData(BYTE* data, int size)
{

	if (!data || size < 1)
	{
		return FALSE;

	}	
	//对缓冲区枷锁 
	m_CSFor_DataForSending.Lock();
	m_nBytesForSending = size;
	memcpy(m_DataForSending, data, size);
	m_CSFor_DataForSending.Unlock();
	::SetEvent(m_hEvtSendDataReq);
	return TRUE;
}

void CTEST_OBDDlg::CalcAndShowDataLength(UINT nEditID, UINT nStaticID, bool bHex)
{
	CString csData;
	GetDlgItemText(nEditID, csData);
	csData.Trim();
	csData.Replace(_T("\r\n"), _T(""));
	csData.Replace(_T("\r"), _T(""));
	csData.Replace(_T("\n"), _T(""));

	int nLength = csData.GetLength();

	if (bHex)
	{
		nLength = (nLength + 1) / 3;
	}
	CString csLength;
	csLength.Format(_T("%d byte(s)"), nLength);
	SetDlgItemText(nStaticID, csLength);
}

void CTEST_OBDDlg::ReadDataAndDisplay(void)
{
	BYTE data[MAX_COM_BUFSIZE] = { 0 };
	if (!m_pHwCOM)
	{
		return;
	}
	int nReadBytes = m_pHwCOM->Read((char*)data, sizeof(data)-1);//实际想读4K 比较大。因为我们数据包不一定长，除非改成动态读一定长度
	if (nReadBytes > 0)//读出大于1个字节
	{
		// 将收到的数据保存到临时文件
		//WriteDataToFile(_T("c:\\bthdebug.dat"), (char*)data, nReadBytes, _T("ab+")); //会创建文件，ab+ 向二进制文件末添加数据，允许读  但是我们这里不需要保存临时文件，保存接受到数据没用

		CEdit *pEdit_Hex = (CEdit*)GetDlgItem(IDC_EDIT_RECEIVEDATA_HEX);
		ASSERT(pEdit_Hex);
		if (!m_bNoteBeCleared)//第一次是FALSE 
		{
			pEdit_Hex->SetWindowText(_T(""));
		}

		pEdit_Hex->SetSel(-1, -1);
		pEdit_Hex->ReplaceSel(Data2HexString((char*)data, nReadBytes));
		pEdit_Hex->SetSel(-1, -1);
		pEdit_Hex->ReplaceSel(L"\r\n");
		CalcAndShowDataLength(IDC_EDIT_RECEIVEDATA_HEX, IDC_STATIC_RECEIVEDDATA_HAX_LENGHT, TRUE);//
		m_bNoteBeCleared = TRUE;
	}


}




uint16_t CTEST_OBDDlg::GenMsgL1Data(uint8_t *L2payload, uint16_t len, uint8_t * outL1Buff)
{
	uint8_t *pL1Data = outL1Buff;
	uint8_t lrc;
	uint16_t totallength;
	totallength = MSG_L1_START_LEN + MSG_L1_LENGTH_LEN + len + MSG_L1_LRC_LEN;
	if (totallength > sizeof(m_ucL1Buff))
	{
		return FALSE;
	}
	pL1Data[0] = MSG_L1_START;
	pL1Data[1] = (uint8_t)(totallength >> 8);
	pL1Data[2] = (uint8_t)(totallength >> 0);


	lrc = LRC_Calc(pL1Data, totallength - MSG_L1_LRC_LEN);
	pL1Data[totallength - 1] = lrc;

	return totallength;
}

void CTEST_OBDDlg::initBuff()
{
	//初始化 共享内容
	m_pL2SendBuff = m_ucL1Buff + MSG_L1_START_LEN + MSG_L1_LENGTH_LEN;
}

BOOL CTEST_OBDDlg::MsgL1SendData(unsigned char *MsgL1Data, int size,CString showtext)
{
	CString csData;
	BOOL ret;
	csData += ucToCString(MsgL1Data, size);

	if (csData.IsEmpty())
	{
		return FALSE;
	}
	m_nBytesForSending = ConvStringToHexData(csData, (BYTE*)m_DataForSending, (int)sizeof(m_DataForSending));//讲A1 字符串转成HEXDATA

#ifdef UNICODE //如果定义了Unicode编码就得再转换成2个字节表示一个字转换回来了，这里我们不需要。
	//m_nBytesForSending = WideCharToMultiByte(CP_ACP, 0, csData, csData.GetLength(), (LPSTR)m_DataForSending, sizeof(m_DataForSending), NULL, NULL);
#else
	m_nBytesForSending = _sntprintf(m_DataForSending, sizeof(m_DataForSending), "%s", csData);
#endif
	ret =SendData(m_DataForSending, m_nBytesForSending); //发送数据 触发事件
	if (ret ==FALSE)
	{
		return ret;
	}
	
	 ShowMyText(showtext + csData, FALSE);
}




void CTEST_OBDDlg::OnBnClickedButtonIsotp()
{
	// TODO:  在此添加控件通知处理程序代码
	m_StatusBar.SetText(_T("ISOTP发送"), 0, 0);
	CString str;
	GetDlgItemText(IDC_EDIT_ISOTP,str);
	//排除中文
	bool isChi = Utilities::IsChinese(str);
	if (isChi)
	{
		AfxMessageBox(_T("请不要输入ISO数据不合法"));
	}//检查是否为0--9  A-Z

	 

	
	 //数据发送
	cw_pack_context pc;
	uint8_t lrc;

	cw_pack_context_init(&pc, (m_pL2SendBuff), 50, 0);
	cw_pack_unsigned(&pc, 0x05);//L2   类型 ISOTP 
	if (CWP_RC_OK != pc.return_code){
		return;
	}
	cw_pack_unsigned(&pc, 1234);//L2  ID
	if (CWP_RC_OK != pc.return_code){
		return;
	}
	int len =str.GetLength();

	//讲unicode  转成 ANSI
	std::string ss =Utilities::EncodeConvert::UnicodeToAnsi(str);
	cw_pack_bin(&pc, ss.c_str(), ss.length());
	if (CWP_RC_OK != pc.return_code){
		return;
	}

	int length = pc.current - pc.start;
	int totallength = GenMsgL1Data(m_pL2SendBuff, length, m_ucL1Buff);

	if (totallength == FALSE)
	{
		return;
	}
	MsgL1SendData(m_ucL1Buff, totallength, _T("ISOTP:"));


}
