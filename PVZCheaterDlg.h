
// PVZCheaterDlg.h: 头文件
//

#pragma once


// CPVZCheaterDlg 对话框
class CPVZCheaterDlg : public CDialog
{
	//友元函数
	friend DWORD WINAPI MonitoringThreadProc(LPVOID lpParam);

// 构造
public:
	CPVZCheaterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PVZCHEATER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	//无限阳光
	CButton MaxSun;
	//秒杀僵尸
	CButton KillJS;
	//禁用CD
	CButton BanCD;
	//植物无敌
	CButton GodZW;
	//后台运行
	CButton Runing;
	//僵尸无敌
	CButton GodJS;
	//无限金币
	CButton MaxMoney;
	//一键取消
	CButton OffAll;
	//联系作者
	CButton Chat;
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedMaxSun();
	afx_msg void OnBnClickedKillJs();
	afx_msg void OnBnClickedBanCd();
	afx_msg void OnBnClickedGodZw();
	afx_msg void OnBnClickedRuning();
	afx_msg void OnBnClickedGodJs();
	afx_msg void OnBnClickedMaxMoney();
	afx_msg void OnBnClickedOffAll();
	afx_msg void OnBnClickedChat();
};
