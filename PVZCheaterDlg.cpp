
// PVZCheaterDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PVZCheater.h"
#include "PVZCheaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 监控线程的刷新时间
constexpr auto MONITOR_REFRESH_TIME_INTERVAL = 1000;

// 植物大战僵尸的进程句柄
static HANDLE g_processHandle;
// 用来监控植物大战僵尸的线程句柄
static HANDLE g_monitoringThreadHandle;
// 辅助窗口
static CPVZCheaterDlg* g_dlg;

// 将某个值写入植物大战僵尸内存（后面的可变参数是地址链，要以-1结尾）
void WriteMemory(void* value, DWORD valueSize, ...)
{
	if (value == NULL || valueSize == 0 || g_processHandle == NULL) return;

	DWORD tempValue = 0;

	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = 0;
	DWORD lastAddress = 0;
	while ((offset = va_arg(addresses, DWORD)) != -1)
	{
		lastAddress = tempValue + offset;
		::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	va_end(addresses);
	if (lastAddress != 0)
	{
		::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);

	}
}

void WriteMemory(void* value, DWORD valueSize, DWORD address) {
	WriteMemory(value, valueSize, address, -1);
}

// 用来监控植物大战僵尸的线程
DWORD WINAPI MonitoringThreadProc(LPVOID lpParam)
{
	while (1)
	{
		// 找窗口
		HWND hwnd = ::FindWindow(TEXT("MainWindow"), TEXT("植物大战僵尸中文版"));
		if (hwnd == NULL)
		{
			if (g_processHandle != 0)
			{
				::CloseHandle(g_processHandle);

			}
			g_processHandle = NULL;

			// 禁止点击
			g_dlg->MaxSun.SetCheck(FALSE);
			g_dlg->MaxSun.EnableWindow(FALSE);

			g_dlg->GodZW.SetCheck(FALSE);
			g_dlg->GodZW.EnableWindow(FALSE);
			g_dlg->OnBnClickedGodZw();

			g_dlg->BanCD.SetCheck(FALSE);
			g_dlg->BanCD.EnableWindow(FALSE);
			g_dlg->OnBnClickedBanCd();

			g_dlg->Runing.SetCheck(FALSE);
			g_dlg->Runing.EnableWindow(FALSE);
			g_dlg->OnBnClickedRuning();

			g_dlg->KillJS.SetCheck(FALSE);
			g_dlg->KillJS.EnableWindow(FALSE);
			g_dlg->OnBnClickedKillJs();

			g_dlg->GodJS.SetCheck(FALSE);
			g_dlg->GodJS.EnableWindow(FALSE);
			g_dlg->OnBnClickedGodJs();

			g_dlg->OffAll.EnableWindow(FALSE);
			g_dlg->MaxMoney.EnableWindow(FALSE);
		}
		else if (g_processHandle == NULL)
		{
			// 找进程ID
			DWORD pid = NULL;
			::GetWindowThreadProcessId(hwnd, &pid);

			// 找进程句柄
			g_processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

			// 开启点击
			g_dlg->MaxSun.EnableWindow(TRUE);
			g_dlg->GodZW.EnableWindow(TRUE);
			g_dlg->BanCD.EnableWindow(TRUE);
			g_dlg->MaxMoney.EnableWindow(TRUE);
			g_dlg->OffAll.EnableWindow(TRUE);
			g_dlg->Runing.EnableWindow(TRUE);
			g_dlg->KillJS.EnableWindow(TRUE);
			g_dlg->GodJS.EnableWindow(TRUE);
		}

		if (g_dlg->MaxSun.GetCheck())
		{ // 如果需要无限阳光
			DWORD value = 6666;
			WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x320, 0x8, 0x0, 0x8, 0x144, 0x2c, 0x5560, -1);
		}

		::Sleep(MONITOR_REFRESH_TIME_INTERVAL);
	}
	return 0;
}

// 针对XP提高程序的访问权限
BOOL ImproveAccessPrivilege()
{
	HANDLE tokenHandle;
	LUID privilegeValue;

	if (!::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenHandle)) return FALSE;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &privilegeValue))
	{
		::CloseHandle(tokenHandle);
		return FALSE;
	}

	TOKEN_PRIVILEGES privileges;
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Luid = privilegeValue;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!::AdjustTokenPrivileges(tokenHandle, FALSE, &privileges, sizeof(privileges), NULL, NULL))
	{
		::CloseHandle(tokenHandle);
		return FALSE;
	}

	return TRUE;
}

// CPVZCheaterDlg 对话框

CPVZCheaterDlg::CPVZCheaterDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PVZCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPVZCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MAX_SUN, MaxSun);
	DDX_Control(pDX, IDC_KILL_JS, KillJS);
	DDX_Control(pDX, IDC_BAN_CD, BanCD);
	DDX_Control(pDX, IDC_GOD_ZW, GodZW);
	DDX_Control(pDX, IDC_RUNING, Runing);
	DDX_Control(pDX, IDC_GOD_JS, GodJS);
	DDX_Control(pDX, IDC_MAX_MONEY, MaxMoney);
	DDX_Control(pDX, IDC_OFF_ALL, OffAll);
	DDX_Control(pDX, IDC_CHAT, Chat);
}

BEGIN_MESSAGE_MAP(CPVZCheaterDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_MAX_SUN, &CPVZCheaterDlg::OnBnClickedMaxSun)
	ON_BN_CLICKED(IDC_KILL_JS, &CPVZCheaterDlg::OnBnClickedKillJs)
	ON_BN_CLICKED(IDC_BAN_CD, &CPVZCheaterDlg::OnBnClickedBanCd)
	ON_BN_CLICKED(IDC_GOD_ZW, &CPVZCheaterDlg::OnBnClickedGodZw)
	ON_BN_CLICKED(IDC_RUNING, &CPVZCheaterDlg::OnBnClickedRuning)
	ON_BN_CLICKED(IDC_GOD_JS, &CPVZCheaterDlg::OnBnClickedGodJs)
	ON_BN_CLICKED(IDC_MAX_MONEY, &CPVZCheaterDlg::OnBnClickedMaxMoney)
	ON_BN_CLICKED(IDC_OFF_ALL, &CPVZCheaterDlg::OnBnClickedOffAll)
	ON_BN_CLICKED(IDC_CHAT, &CPVZCheaterDlg::OnBnClickedChat)
END_MESSAGE_MAP()


// CPVZCheaterDlg 消息处理程序

BOOL CPVZCheaterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 针对XP系统提高程序的权限
	ImproveAccessPrivilege();

	// 辅助窗口
	g_dlg = this;

	// 启动监控线程
	g_monitoringThreadHandle = ::CreateThread(NULL, 0, MonitoringThreadProc, NULL, 0, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPVZCheaterDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPVZCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//关闭窗口
void CPVZCheaterDlg::OnClose()
{

	// 去掉外挂效果
	this->OnBnClickedOffAll();

	// 杀死线程
	::TerminateThread(g_monitoringThreadHandle, 0);
	::CloseHandle(g_monitoringThreadHandle);

	// 关闭句柄
	::CloseHandle(g_processHandle);

	CDialog::OnClose();
}

//无限阳光
void CPVZCheaterDlg::OnBnClickedMaxSun()
{
	if (g_dlg->MaxSun.GetCheck())
	{ // 如果需要无限阳光
		DWORD value = 6666;
		WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x320, 0x8, 0x0, 0x8, 0x144, 0x2c, 0x5560, -1);
	}
}

//秒杀僵尸
void CPVZCheaterDlg::OnBnClickedKillJs()
{
	DWORD address1 = 0x53130F;
	DWORD address2 = 0x531066;
	DWORD address3 = 0x530CB1;
	if (this->KillJS.GetCheck())
	{ // 如果需要秒杀僵尸
	  // 去除僵尸不死
		g_dlg->GodJS.SetCheck(FALSE);
		g_dlg->OnBnClickedGodJs();

		BYTE data1[] = { 0x2B, 0xFF, 0x90, 0x90 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3);
	}
	else
	{ // 如果不需要秒杀僵尸
		BYTE data1[] = { 0x2B, 0x7C, 0x24, 0x20 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x75, 0x11 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x75, 0x17 };
		WriteMemory(data3, sizeof(data3), address3);
	}
}

//禁用CD
void CPVZCheaterDlg::OnBnClickedBanCd()
{
	DWORD address = 0x487296;
	if (this->BanCD.GetCheck())
	{ // 如果需要无CD
		BYTE data[] = { 0x90, 0x90 };
		WriteMemory(data, sizeof(data), address);
	}
	else
	{ // 如果不需要无CD
		BYTE data[] = { 0x7E, 0x14 };
		WriteMemory(data, sizeof(data), address);
	}
}

//植物无敌
void CPVZCheaterDlg::OnBnClickedGodZw()
{
	DWORD address1 = 0x52FCF0;
	DWORD address2 = 0x46D7A6;
	DWORD address3 = 0x45EC63;
	DWORD address4 = 0x46CFEB;
	if (this->GodZW.GetCheck())
	{ // 如果需要植物不死
		BYTE data1[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3);

		BYTE data4[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data4, sizeof(data4), address4);
	}
	else
	{ // 如果不需要植物不死
		BYTE data1[] = { 0x83, 0x46, 0x40, 0xFC };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x29, 0x4E, 0X40 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x83, 0x46, 0x40, 0xCE };
		WriteMemory(data3, sizeof(data3), address3);

		BYTE data4[] = { 0x29, 0x50, 0x40 };
		WriteMemory(data4, sizeof(data4), address4);
	}
}

//后台运行
void CPVZCheaterDlg::OnBnClickedRuning()
{
	DWORD address = 0x54E1C2;
	if (this->Runing.GetCheck())
	{ // 如果需要后台运行
		BYTE data[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data, sizeof(data), address);
	}
	else
	{ // 如果不需要后台运行
		BYTE data[] = { 0x0F, 0x95, 0xC0 };
		WriteMemory(data, sizeof(data), address);
	}
}

//僵尸无敌
void CPVZCheaterDlg::OnBnClickedGodJs()
{
	DWORD address1 = 0x53130F;
	DWORD address2 = 0x531042;
	DWORD address3 = 0x530C9F;
	if (this->GodJS.GetCheck())
	{ // 如果需要僵尸不死
	  // 去除秒杀僵尸
		g_dlg->KillJS.SetCheck(FALSE);
		g_dlg->OnBnClickedKillJs();

		BYTE data1[] = { 0x83, 0xEF, 0x00, 0x90 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3, -1);
	}
	else
	{ // 如果不需要僵尸不死
		BYTE data1[] = { 0x2B, 0x7C, 0x24, 0x20 };
		WriteMemory(data1, sizeof(data1), address1);

		BYTE data2[] = { 0x2B, 0xF0, 0x2B, 0xC8 };
		WriteMemory(data2, sizeof(data2), address2);

		BYTE data3[] = { 0x2B, 0xD0, 0x29, 0x86, 0xDC, 0x00, 0x00, 0x00 };
		WriteMemory(data3, sizeof(data3), address3);
	}
}

//无限金币
void CPVZCheaterDlg::OnBnClickedMaxMoney()
{
	DWORD value = 66666;
	WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x82C, 0x28, -1);
}

//一键取消
void CPVZCheaterDlg::OnBnClickedOffAll()
{
	this->MaxSun.SetCheck(FALSE);

	this->GodZW.SetCheck(FALSE);
	this->OnBnClickedGodZw();

	this->BanCD.SetCheck(FALSE);
	this->OnBnClickedBanCd();

	this->Runing.SetCheck(FALSE);
	this->OnBnClickedRuning();

	this->KillJS.SetCheck(FALSE);
	this->OnBnClickedKillJs();

	this->GodJS.SetCheck(FALSE);
	this->OnBnClickedGodJs();
}

//联系作者
void CPVZCheaterDlg::OnBnClickedChat()
{
	ShellExecute(
		NULL,
		CString("open"),
		CString("http://wpa.qq.com/msgrd?v=3&uin=3606264624&site=qq&menu=yes"),
		NULL, NULL, SW_SHOWNORMAL);
}
