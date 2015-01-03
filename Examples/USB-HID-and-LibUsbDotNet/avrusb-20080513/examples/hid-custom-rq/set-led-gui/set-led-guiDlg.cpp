// set-led-guiDlg.cpp : implementation file
//

#include "stdafx.h"
#include "set-led-gui.h"
#include "set-led-guiDlg.h"
#include ".\set-led-guidlg.h"
#include "libusb_func.h"
#include "usbconfig.h"
#include "requests.h"
#include "opendevice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

usb_dev_handle      *handle = NULL;
const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
char                buffer[4];
int                 cnt, vid, pid, isOn; 

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CsetledguiDlg dialog



CsetledguiDlg::CsetledguiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CsetledguiDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsetledguiDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK1, ledOn);
}

BEGIN_MESSAGE_MAP(CsetledguiDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
END_MESSAGE_MAP()


// CsetledguiDlg message handlers

BOOL CsetledguiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    CString msg;

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//Load the library:
    LoadLibusbFuncFromDLL();
    //устанавливаем галочку, если светодиод горит
    usb_init();
    /* вычисляем VID/PID из usbconfig.h, так как это центральный источник информации */
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    /* Следующая функция реализована в opendevice.c: */
    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
    {
        //fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        msg.Format("Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        MessageBox(msg, "Error", MB_ICONEXCLAMATION);
        exit(1);
    }
	cnt	= usb_control_msg(handle, USB_TYPE_VENDOR |	USB_RECIP_DEVICE | USB_ENDPOINT_IN,	CUSTOM_RQ_GET_STATUS, 0, 0,	buffer,	sizeof(buffer),	5000);
	if(cnt < 1)
    {
		if(cnt < 0)
        {
			//fprintf(stderr,	"USB error:	%s\n", usb_strerror());
            msg.Format("USB error:	%s\n", usb_strerror());
            MessageBox(msg, "Error", MB_ICONEXCLAMATION);
		}
        else
        {
			//fprintf(stderr,	"only %d bytes received.\n", cnt);
            msg.Format("only %d bytes received.\n", usb_strerror());
            MessageBox(msg, "Error", MB_ICONEXCLAMATION);
		}
	}else
    {
		//printf("LED	is %s\n", buffer[0]	? "on" : "off");
        if (buffer[0])
            ledOn.SetCheck(BST_CHECKED);
        else
            ledOn.SetCheck(BST_UNCHECKED);
	}
    usb_close(handle);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CsetledguiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CsetledguiDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CsetledguiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CsetledguiDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	UnloadLibusb();
	CDialog::OnClose();
}

void CsetledguiDlg::OnBnClickedCheck1()
{
    CString msg;

    // TODO: Add your control notification handler code here
    usb_init();
    /* вычисляем VID/PID из usbconfig.h, так как это центральный источник информации */
    vid = rawVid[1] * 256 + rawVid[0];
    pid = rawPid[1] * 256 + rawPid[0];
    /* Следующая функция реализована в opendevice.c: */
    if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0)
    {
        //fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        msg.Format("Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
        MessageBox(msg, "Error", MB_ICONEXCLAMATION);
        exit(1);
    }
    isOn = (ledOn.GetCheck() == BST_CHECKED)? 1 : 0;
    cnt = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT, CUSTOM_RQ_SET_STATUS, isOn, 0, buffer, 0, 5000);
    if(cnt < 0)
    {
        //fprintf(stderr, "USB error: %s\n", usb_strerror());
        msg.Format("USB error: %s\n", usb_strerror());
        MessageBox(msg, "Error", MB_ICONEXCLAMATION);
    }
    usb_close(handle);
}
