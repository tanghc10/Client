#pragma once


// CMyListBox

class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	CMyListBox();
	virtual ~CMyListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
public:
	int AddString(LPCTSTR lpszItem, COLORREF itemColor );
	int AddString(LPCTSTR lpszItem);
	int InsertString(int index, LPCTSTR lpszItem, COLORREF itemColor);
};


