// CWndRAN.cpp: 实现文件
//

#include "pch.h"
#include "GAMEHACKER.h"
#include "CWndRAN.h"
#include "afxdialogex.h"

CWndRAN* _wndThis;
// CWndRAN 对话框

IMPLEMENT_DYNAMIC(CWndRAN, CDialogEx)

CWndRAN::CWndRAN(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PAGE_2, pParent)
{
	_wndThis = this;
}

CWndRAN::~CWndRAN()
{
}

void CWndRAN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, mTree);
	DDX_Control(pDX, IDC_LIST1, LstData);
}

void CWndRAN::OnOk()
{
}

void CWndRAN::OnCancel()
{
}


BEGIN_MESSAGE_MAP(CWndRAN, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CWndRAN::OnBnClickedButton1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CWndRAN::OnNMRClickTree1)
	ON_COMMAND(ID_32782, &CWndRAN::OnLoadGame)
	ON_COMMAND(ID_Menu32783, &CWndRAN::OnAddClass)
	ON_COMMAND(ID_32793, &CWndRAN::OnDeleteClass)
	ON_COMMAND(ID_32794, &CWndRAN::OnSetClass)
	ON_COMMAND(ID_32792, &CWndRAN::OnAnlyClass)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST1, &CWndRAN::OnNMCustomdrawList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CWndRAN::OnNMRClickList1)
	ON_COMMAND(ID_32796, &CWndRAN::OnSetGIDUNICODE)
	ON_COMMAND(ID_32797, &CWndRAN::OnSetGIDASCII)
	ON_COMMAND(ID_32798, &CWndRAN::OnSetGID8)
	ON_COMMAND(ID_32799, &CWndRAN::OnSetGID4)
	ON_COMMAND(ID_32800, &CWndRAN::OnSetGID2)
	ON_COMMAND(ID_32801, &CWndRAN::OnSetGID1)
	ON_COMMAND(ID_32802, &CWndRAN::OnSetGIDP)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CWndRAN::OnNMDblclkList1)

	ON_COMMAND(ID_32795, &CWndRAN::OnSaveOBJ)
	ON_COMMAND(ID_32803, &CWndRAN::OnAddFunction)
END_MESSAGE_MAP()


// CWndRAN 消息处理程序

void _stdcall ReadData(HWND _hwnd, UINT uint, UINT_PTR uintPtr , DWORD tick)
{
	if (!_wndThis->hProcess) return;
	if (!_wndThis->CurOBJ)return;
	if (!_wndThis->CurOBJ->UpdateData(_wndThis->hProcess))
	{
		_wndThis->hProcess = 0;
		AfxMessageBox(L"游戏句柄打开失败!");
	}

	_wndThis->SHOWMemContext();
	//数据显示成功
	//数据=>LST
}
void CWndRAN::OnBnClickedButton1()
{

	//TIMERPROC X;
	//::SetTimer(this->m_hWnd, 10002, 1000, MyTime);
	// TODO: 在此添加控件通知处理程序代码
	//auto val = mTree.InsertItem(L"剑侠", TVI_ROOT);
	///auto val1=mTree.InsertItem(L"DATA", val);
	//auto val2 = mTree.InsertItem(L"DATA", val1);
	//mTree.DeleteItem(val);

	
}

BOOL CWndRAN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LONG_PTR lStyle;
	lStyle = GetWindowLongPtr(LstData.m_hWnd, GWL_STYLE);
	lStyle |= LVS_REPORT;
	SetWindowLongPtr(LstData.m_hWnd, GWL_STYLE, lStyle);
	DWORD dStyle = LstData.GetExtendedStyle();
	dStyle |= LVS_EX_FULLROWSELECT;
	dStyle |= LVS_EX_GRIDLINES;
	LstData.SetExtendedStyle(dStyle);

	LstData.InsertColumn(0, L"内存地址", 0, 100);
	LstData.InsertColumn(1, L"偏移", 0, 100);
	LstData.InsertColumn(2, L"类型", 0, 100);
	LstData.InsertColumn(3, L"名称", 0, 100);
	LstData.InsertColumn(4, L"值", 0, 100);
	LstData.InsertColumn(5, L"注释", 0, 100);
	LstData.InsertColumn(6, L"显示类型", 0, 100);

	InitDataTypeTable();

	::SetTimer(this->m_hWnd, 10002, 5000, ReadData);

	return TRUE;
}


void CWndRAN::OnNMRClickTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	DWORD dMenuId=1;
	if (hProcess)
	{
		auto hSel  = mTree.GetSelectedItem();
		if (hSel)
		{
			PTREE_DATA val = (PTREE_DATA)mTree.GetItemData(hSel);
			if (val)dMenuId = val->MenuId;
			else dMenuId = 0;
		}
		else dMenuId = 2;
	}

	if (dMenuId == 0) return;
	
		DWORD   dwPos = GetMessagePos();
		CPoint  point(LOWORD(dwPos), HIWORD(dwPos));   //0X ffff ffff
		CMenu   menu;
		menu.LoadMenuW(IDR_MENU1);

		CMenu* pop = menu.GetSubMenu(dMenuId);
		
		pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	
}


void CWndRAN::OnLoadGame()
{
	//mTree.InsertItem(L"Sword2.exe", TVI_ROOT);
	if (wndSelProcess.DoModal() == IDOK)
	{
		//wndSelProcess.wExe;
		//wndSelProcess.dPid; 
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, wndSelProcess.dPid);
		if (hProcess)
		{
			auto troot = InsertItemEx(wndSelProcess.wExe, 2, NULL);
			InsertItemEx(L"公共数据", 3, NULL, troot);
			InsertItemEx(L"公共函数", 4, NULL, troot);
			InsertItemEx(L"HOOK分析", 5, NULL, troot);



			wAnlyData = wAnlyPath + wndSelProcess.wExe;
			if (!PathIsDirectory(wAnlyData))
			{
				if (!CreateDirectory(wAnlyData, NULL))
				{
					AfxMessageBox(L"创建文件夹失败!");
					return;
				}
			}

			//ini//----
			WIN32_FIND_DATA fileData;
			HANDLE hFind = FindFirstFile(wAnlyData + L"\\*.ini", &fileData);
			while (hFind != INVALID_HANDLE_VALUE)
			{
				CString _Name = fileData.cFileName;
				_Name.Replace(L".ini", L"");
				COBJContext* obj = new COBJContext(wAnlyData,_Name);
				AddOBJTree(obj);
				/*
				auto hRoot = InsertItemEx(_Name,6,obj,TVI_ROOT);
				auto hBase = InsertItemEx(L"基本信息", 6, NULL, hRoot);
				CString txtVal;
					txtVal = L"内存地址:" + obj->GetAddress();
					InsertItemEx(txtVal, 6,NULL,hBase);
					txtVal.Format(L"对象大小:[%d]", obj->GetSize());
					InsertItemEx(txtVal, 6,NULL,hBase);
					txtVal = L"注释:" + obj->GetNote();
					InsertItemEx(txtVal,6,NULL, hBase);
				InsertItemEx(L"成员变量",6,NULL, hRoot);
				InsertItemEx(L"成员函数",6,NULL, hRoot);
				//OBJ=>HTREE
				*/
				if (!FindNextFile(hFind, &fileData)) break;
			}
			FindClose(hFind);
		}
		else
		{
			AfxMessageBox(L"游戏加载失败!");
		}
	}

	
}

void CWndRAN::SetAppPath(const wchar_t* path)
{
	wAppPath = path;
	wAnlyPath = wAppPath + L"\\RAN\\";

	//DWORD_PTR
}

HTREEITEM CWndRAN::InsertItemEx(const wchar_t* txt, DWORD _menuId, LPVOID _data, HTREEITEM h)
{
	auto _rt = mTree.InsertItem(txt, h);
	PTREE_DATA DATA_PTR = new TREE_DATA{ _menuId, _data };
	mTree.SetItemData(_rt,(DWORD_PTR)DATA_PTR);
	return _rt;
}

void CWndRAN::DeleteItemEx(HTREEITEM h)
{
	PTREE_DATA  val = (PTREE_DATA)mTree.GetItemData(h);
	if (val) delete val;
	mTree.DeleteItem(h);
}


void CWndRAN::OnAddClass()
{
	//创建对象
	//if (!hProcess) return;
	
	if (wndAddClass.DoModal() == IDOK)
	{
		
		auto h = mTree.GetRootItem();
		while (h)
		{
			h = mTree.GetNextSiblingItem(h);
			auto txt = mTree.GetItemText(h);
			if (txt == wndAddClass.txtName)
			{
				AfxMessageBox(L"该对象已经存在!请勿添加重名对象!");
				return;
			}
		}

		COBJContext* obj = new COBJContext(wAnlyData, wndAddClass.txtName, 
			wndAddClass.txtAddress, wndAddClass.Size, wndAddClass.txtNote);
		obj->Save();
		AddOBJTree(obj);
		//obj->htree
		/*
		auto hRoot = InsertItemEx(wndAddClass.txtName, 6, obj, TVI_ROOT);
		auto hBase = InsertItemEx(L"基本信息", 6, NULL, hRoot);
		CString txtVal;
		txtVal = L"内存地址:" + obj->GetAddress();
		InsertItemEx(txtVal, 6, NULL, hBase);
		txtVal.Format(L"对象大小:[%d]", obj->GetSize());
		InsertItemEx(txtVal, 6, NULL, hBase);
		txtVal = L"注释:" + obj->GetNote();
		InsertItemEx(txtVal, 6, NULL, hBase);
		InsertItemEx(L"成员变量", 6, NULL, hRoot);
		InsertItemEx(L"成员函数", 6, NULL, hRoot);
		*/
	}
	
}


void CWndRAN::OnDeleteClass()
{
	//删除对象
	auto hSel  = mTree.GetSelectedItem();
	auto hRoot = mTree.GetRootItem();
	if (hSel == hRoot) return;
	auto h = GetFirstItem(hSel);

	

	PTREE_DATA VAL = (PTREE_DATA)mTree.GetItemData(h);



	if (VAL)
	{
		COBJContext* p = (COBJContext*)VAL->DATA_PTR;
		if (p)
		{
			p->Delete();
			delete p;
		}
	}

	DeleteItemEx(h);

}

HTREEITEM CWndRAN::GetFirstItem(HTREEITEM h)
{
	HTREEITEM newH=h;
	HTREEITEM oldH;
	do
	{
		oldH = newH;
		newH = mTree.GetParentItem(oldH);
	} while (newH);
	return oldH;
}

HTREEITEM CWndRAN::GetSonItem(HTREEITEM h, int index)
{
	auto t = mTree.GetChildItem(h);
	for (int i = 0; i < index; i++)
	{
		t = mTree.GetNextSiblingItem(t);
		if (!t) return nullptr;
	}
	return t;
}


void CWndRAN::OnSetClass()
{
	auto hSel = mTree.GetSelectedItem();
	auto hRoot = mTree.GetRootItem();
	if (hSel == hRoot) return;
	auto h = GetFirstItem(hSel);
	if (h)
	{
		auto obj = GetOBJPtr(h);
		//wndSelProcess.wExe = obj.g
		wndAddClass.txtAddress = obj->GetAddress();
		wndAddClass.txtNote = obj->GetNote();
		wndAddClass.Size = obj->GetSize();
		wndAddClass.txtName = mTree.GetItemText(h);

		if (wndAddClass.DoModal() == IDOK)
		{
			obj->Set(wndAddClass.txtName, wndAddClass.txtAddress, wndAddClass.Size, wndAddClass.txtNote);
			SetOBJTree(h, obj);
		}

	}


}

PCOBJContext CWndRAN::GetOBJPtr(HTREEITEM h)
{

	PTREE_DATA val = (PTREE_DATA)mTree.GetItemData(h);
	if (val)
	{
		PCOBJContext obj = (PCOBJContext)val->DATA_PTR;
		return obj;
	}
	
	return nullptr;
}

HTREEITEM CWndRAN::AddOBJTree(PCOBJContext obj)
{
	auto hRoot = InsertItemEx(obj->GetName(), 6, obj, TVI_ROOT);
	auto hBase = InsertItemEx(L"基本信息", 6, NULL, hRoot);
	CString txtVal;
	txtVal = L"内存地址:" + obj->GetAddress();
	InsertItemEx(txtVal, 6, NULL, hBase);
	txtVal.Format(L"对象大小:[%d]", obj->GetSize());
	InsertItemEx(txtVal, 6, NULL, hBase);
	txtVal = L"注释:" + obj->GetNote();
	InsertItemEx(txtVal, 6, NULL, hBase);
	InsertItemEx(L"成员变量", 6, NULL, hRoot);
	InsertItemEx(L"成员函数", 8, NULL, hRoot);
	return hRoot;
}

void CWndRAN::SetOBJTree(HTREEITEM h, PCOBJContext obj)
{
	mTree.SetItemText(h, obj->GetName());//

	HTREEITEM hBase    = mTree.GetChildItem(h);
	HTREEITEM hBaseSon = mTree.GetChildItem(hBase);
	CString txtVal;
	txtVal = L"内存地址:" + obj->GetAddress();
	mTree.SetItemText(hBaseSon, txtVal);
	
	hBaseSon = mTree.GetNextSiblingItem(hBaseSon);
	txtVal.Format(L"对象大小:[%d]", obj->GetSize());
	mTree.SetItemText(hBaseSon, txtVal);

	hBaseSon = mTree.GetNextSiblingItem(hBaseSon);
	txtVal = L"注释:" + obj->GetNote();
	mTree.SetItemText(hBaseSon, txtVal);
	
}

void CWndRAN::AddMEMList(PCMEMContext context, DWORD index,DWORD lstIndex, DWORD_PTR exId)
{
	PLIST_DATA  lstDATA = new LIST_DATA{ exId,context,index };
	LstData.InsertItem(lstIndex,L"");
	LstData.SetItemData(lstIndex,(DWORD_PTR)lstDATA);
}


void CWndRAN::OnAnlyClass()
{
	auto hSel = mTree.GetSelectedItem();
	auto hRoot = mTree.GetRootItem();
	if (hSel == hRoot) return;
	auto h = GetFirstItem(hSel);
	if (h)
	{
		auto obj = GetOBJPtr(h);
		if (obj) 
		{
			if (CurOBJ != obj)
			{
				LstData.DeleteAllItems();
			}
			CurOBJ = obj;
			ReadData(0, 0, 10002, 0);
		}
	}
}

//显示内存分析结果
void CWndRAN::SHOWMemContext()
{
	if (!CurOBJ)return;

	LstData.SetRedraw(FALSE);

	
	int LstCount = LstData.GetItemCount();
	int icount = 0;
	/*
	LstData.InsertColumn(0, L"内存地址", 0, 100);
	LstData.InsertColumn(1, L"偏移", 0, 200);
	LstData.InsertColumn(2, L"类型", 0, 200);
	LstData.InsertColumn(3, L"名称", 0, 200);
	LstData.InsertColumn(4, L"值", 0, 200);
	LstData.InsertColumn(5, L"注释", 0, 200);
	*/

	LPVOID _address = CurOBJ->GetMemAddress();
	PCMEMContext _mcontext = CurOBJ->MEMContext;
	//CString txt;
	//txt.Format(L"%x", CurOBJ->MEMContext);
	//AfxMessageBox(txt);

	while (_mcontext)
	{

		int lenth = _mcontext->lenth;
		for (int i = 0; i < lenth; i++)
		{
			int ilenth;
			ilenth = lenth;
			if (_mcontext->Extend)ilenth = i;


			DWORD_PTR adr = (DWORD_PTR)_address + _mcontext->offset;
			CString txt, offset,name,val,oldval;
			txt.Format(L"0x%X", adr+ i*_mcontext->type->Size);
			offset.Format(L"+0x%X", _mcontext->offset + i * _mcontext->type->Size);
			if (lenth > 1)
				name.Format(L"%s[%d]", _mcontext->txtName.GetBuffer(), ilenth);
			else
				name.Format(L"%s", _mcontext->txtName.GetBuffer());
			
			if (!_mcontext->Extend)lenth = 1;

			val = CurOBJ->ReadValue(_mcontext, i);
			oldval = LstData.GetItemText(icount, 4);
			DWORD exId = (val != oldval);

			//CString _txt;
			//_txt.Format(L"%d", exId);
			//AfxMessageBox(_txt);

			if (!LstCount)AddMEMList(_mcontext, i, icount, 0);  //
			
			
			LstData.SetItemText(icount,0, txt);
			LstData.SetItemText(icount, 1, offset);
			
			LstData.SetItemText(icount, 2, _mcontext->type->UName);
			
			LstData.SetItemText(icount, 3, name);
			LstData.SetItemText(icount, 4, val);
			//
			
			PLIST_DATA p = (PLIST_DATA)LstData.GetItemData(icount);
			if (p)p->ExId = exId;

			
			LstData.SetItemText(icount, 5, _mcontext->txtNote);
			val = CurOBJ->ReadGValue(_mcontext, i);
			LstData.SetItemText(icount++, 6, val);



		}
		_mcontext = _mcontext->Next;
	}
	LstData.SetRedraw(TRUE);

}


void CWndRAN::OnNMCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLVCUSTOMDRAW* lp = (NMLVCUSTOMDRAW*)pNMHDR;
	auto& cmd = lp->nmcd;
	switch (cmd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
	{
		PLIST_DATA p = (PLIST_DATA)LstData.GetItemData(cmd.dwItemSpec);
		if (p)
		{
			COLORREF color[2]{ RGB(0,0,0) ,RGB(255,0,0) };
			lp->clrText = color[p->ExId];
		}

	}
	*pResult = CDRF_DODEFAULT;
	break;
	}
}


void CWndRAN::OnNMRClickList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	
	if (CurOBJ)
	{

		DWORD   dwPos = GetMessagePos();
		CPoint  point(LOWORD(dwPos), HIWORD(dwPos));   //0X ffff ffff
		CMenu   menu;
		menu.LoadMenuW(IDR_MENU1);

		CMenu* pop = menu.GetSubMenu(7);

		pop->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}


void CWndRAN::OnSetGIDUNICODE()
{
	if (CurOBJ)CurOBJ->SetGID(0);
	ReadData(0, 0, 10002, 0);
}


void CWndRAN::OnSetGIDASCII()
{
	if (CurOBJ)CurOBJ->SetGID(1);
	ReadData(0, 0, 10002, 0);
}


void CWndRAN::OnSetGID8()
{
	if (CurOBJ)CurOBJ->SetGID(2);
	ReadData(0, 0, 10002, 0);
}


void CWndRAN::OnSetGID4()
{
	if (CurOBJ)CurOBJ->SetGID(3);
	ReadData(0, 0, 10002, 0);
}


void CWndRAN::OnSetGID2()
{
	if (CurOBJ)CurOBJ->SetGID(4);
	ReadData(0, 0, 10002, 0);
}


void CWndRAN::OnSetGID1()
{
	if (CurOBJ)CurOBJ->SetGID(5);
	ReadData(0, 0, 10002, 0);
}


void CWndRAN::OnSetGIDP()
{
	if (CurOBJ)CurOBJ->SetGID(6);
	ReadData(0, 0, 10002, 0);
}


void CWndRAN::OnNMDblclkList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	//
	if (pNMItemActivate->iItem != -1)
	{
		PLIST_DATA p = (PLIST_DATA)LstData.GetItemData(pNMItemActivate->iItem);
		if (p)
		{

			wndEditMC.txtName = p->MContext->txtName;
			wndEditMC.txtNote = p->MContext->txtNote;
			wndEditMC.txtAddress = LstData.GetItemText(pNMItemActivate->iItem,0);
			wndEditMC.txtOffSet = LstData.GetItemText(pNMItemActivate->iItem,1);
			if(p->index)
				wndEditMC.Size = 1;
			else
				wndEditMC.Size = p->MContext->lenth;
			wndEditMC.TypeId = p->MContext->type_id;
			wndEditMC.txtData = CurOBJ->ReadGAllValue(p->MContext, p->index);
			//val = CurOBJ->ReadValue(p->MContext, p->index);

			if (wndEditMC.DoModal() == IDOK)
			{
				auto ret=CurOBJ->SetMemContext(p->MContext, wndEditMC.txtName, wndEditMC.txtNote, wndEditMC.Size, wndEditMC.TypeId,p->index);
				if (ret == -1) AfxMessageBox(L"内存空间不足!");
				else
				{
					LstData.DeleteAllItems();
					ReadData(0, 0, 10002, 0);
				}
			}

		}
		
	}
}





void CWndRAN::OnSaveOBJ()
{
	auto hSel = mTree.GetSelectedItem();
	auto hRoot = mTree.GetRootItem();
	if (hSel == hRoot) return;
	auto h = GetFirstItem(hSel);
	if (h)
	{
		auto obj = GetOBJPtr(h);
		obj->CreateCPP();
	}
}


void CWndRAN::OnAddFunction()
{
	auto hSel = mTree.GetSelectedItem();
	auto hRoot = mTree.GetRootItem();
	if (hSel == hRoot) return;
	auto h = GetFirstItem(hSel);
	if (h)
	{
		auto obj = GetOBJPtr(h);
		if (obj)
		{
			wndAddFunction.Init(nullptr,obj);
			if (wndAddFunction.DoModal() == IDOK)
			{
				auto p = wndAddFunction._FunReturn;
				auto pp = p->Params;
				CString val, txtP{};
				bool    bfirst{true};
				CString txtFix[]{ L",",L"" };
				while (pp)
				{
					
					txtP = txtP+txtFix[bfirst]+ pp->txtType + L" " + pp->txtName;
					pp = pp->Next;
					bfirst = false;
				}
				val.Format(L"%s %s(%s);",p->typeName.GetBuffer(),p->txtName.GetBuffer(),txtP.GetBuffer());
				auto t = GetSonItem(h, 2);
				//mTree.InsertItem(L"", t);
				InsertItemEx(val, 8, NULL, t);
			}
		}
	}
	
}
