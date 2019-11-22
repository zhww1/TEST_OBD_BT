
// TEST_OBDDlg.h : 头文件
//

#pragma once
#include "MyEventView.h"

#include <vector>
#include "cwpack.h"



#include "BlueTooth.h" ////蓝牙  封装类
#include "HwCOM.h"	//串口

/* 消息头标识字符*/
#define MSG_L1_START  0xA1

/* 起始字符长度 */
#define MSG_L1_START_LEN  1
/* 长度字段长度 */
#define MSG_L1_LENGTH_LEN 2
/* LRC字段长度 */
#define MSG_L1_LRC_LEN    1





// CTEST_OBDDlg 对话框
class CTEST_OBDDlg : public CDialogEx
{
// 构造
public:
	CTEST_OBDDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_TEST_OBD_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
public:
	/*显示提示信息*/
	void ShowMyText(LPCTSTR Text, BOOL Flag);
	// 使能按钮
	void EnableBtns(bool enable);
	//计算显示输出
	void CalcAndShowDataLength(UINT nEditID, UINT nStaticID, bool bHex);
	//读出数据并且显示
	void ReadDataAndDisplay(void);
public:
	CStatic				m_bmpStatus;
	HICON				hIcons[2];							//串口关 串口开     
	CBlueTooth			m_BlueTooth;
	CHwCOM				*m_pHwCOM;
	HANDLE				m_hEvtEndThread;					//事件内核对象
	HANDLE				m_hEvtSendDataReq;					//事件内核对象
	HANDLE				m_hEvtScanDeviceReq;				//事件内核对象
	DWORD				m_dwThreadID;
	HANDLE				m_hThread;

	unsigned char		m_ucL1Buff[1024];
	unsigned char		*m_pL2SendBuff;
	

	BYTE				m_DataForSending[MAX_COM_BUFSIZE];  //4K 最大缓冲区
	int					m_nBytesForSending;
	CCriticalSection	m_CSFor_DataForSending;				//临界区锁  ,用于同步
	BOOL				m_bNoteBeCleared;					//接受记录清除
	BOOL				m_bRecvProcess;
private:
	void initBuff();

	BOOL GetCur_LocalRadioInfo(t_LocalRadioInfo& LocalRadioInfo);
	void EnumerateLocal();
	int  GetListCurSel(UINT nID, LPCTSTR lpszMsg);
	void DeleteCOMObject();
	uint16_t  GenMsgL1Data(uint8_t *L2payload, uint16_t len, uint8_t * outL1Buff);


	BOOL SendData(BYTE* data, int size);					//触发事件线程运行将要把缓冲区数据发送出去
	BOOL MsgL1SendData(unsigned char *MsgL1Data, int size, CString showtext = _T(""));


	
public:
	void ThreadProc_COM(void);	//类对象回调掉
	void ExitCOMThread(void);
private:

	CMyEventView						*m_wndMyEventView;

	bool                                _connected;			// 连接标识
	std::vector<CString>				m_vecLocalBluetoothRadios;
	CStatusBarCtrl						m_StatusBar;//状态栏
public:
	afx_msg void OnBnClickedButtonRequestauthenticate();
	afx_msg void OnBnClickedButtonConnect();
	afx_msg LRESULT OnMainRClick(WPARAM wParam, LPARAM lParam);
	afx_msg void OnListDeleteItem();
	afx_msg void OnSaveListLog();
	afx_msg void OnListEmptyItem();

	afx_msg void OnBnClickedBtnFinddevice();
	afx_msg void OnBnClickedBtnPing();
	afx_msg void OnBnClickedBtnIsotp();
	afx_msg void OnBnClickedBtnCanconfig();
	afx_msg void OnBnClickedBtnL2unpack();
	

	
	
	afx_msg void OnDblclkListDevlist();
	afx_msg void OnBnClickedButtonIsotp();
};
