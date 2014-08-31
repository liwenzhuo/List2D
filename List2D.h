#ifndef LIST2D_H_
#define LIST2D_H_

#pragma once

class CList2D;

class CList2DNode
{
	friend class CList2D;
public:
	CList2DNode(){
		m_pupnode=m_pdownnode=m_pleftnode=m_prightnode=0;
		m_pdata=0;
		m_bisdirty=true;
	};
    ~CList2DNode(){};//no virtual

	//getters
	CList2DNode* GetUpNode(){return m_pupnode;};
	CList2DNode* GetDownNode(){return m_pdownnode;};
	CList2DNode* GetLeftNode(){return m_pleftnode;};
	CList2DNode* GetRightNode(){return m_prightnode;};

	void*        GetDatePointer() {return m_pdata;};
	bool         IsDirty(){return m_bisdirty;}

	//setters
	void         SetDataPointer(void* pdata){m_pdata=pdata;};
	void         SetDirty(bool bdirty){m_bisdirty=bdirty;};
	void         SetUpNode(CList2DNode* pnode){m_pupnode=pnode;};
	void         SetDownNode(CList2DNode* pnode){m_pdownnode=pnode;};
	void         SetLeftNode(CList2DNode* pnode){m_pleftnode=pnode;};
	void         SetRightNode(CList2DNode* pnode){m_prightnode=pnode;};

	//special
	static int   Reserve1Offset_int(){return (int)&(((CList2DNode*)0)->m_lpReserve1);};
	static int   Reserve2Offset_int64(){return (int)&(((CList2DNode*)0)->m_lpReserve2);};

protected:
	CList2DNode* m_pupnode;
	CList2DNode* m_pdownnode;
	CList2DNode* m_pleftnode;
	CList2DNode* m_prightnode;
	bool         m_bisdirty;
	void*        m_pdata;     //life circle managed by others
	int          m_lpReserve1;
	int          m_lpReserve2;
};

class CList2D
{
public:
	CList2D(int line, int colum);
	virtual ~CList2D();

	void         ForEach(bool (*)(CList2DNode*));

	//getter
	CList2DNode* GetHead(){return m_head;};
	int          GetLineCnt(){return m_curlinecnt;};
	int          GetColumnCnt(){return m_curcolumncnt;};

	//modify list
	bool         AddLine2Top();
	bool         AddLine2Bottom();
	bool         AddColumn2Left();
	bool         AddColumn2Right();
	
	void         DeleteOneLine(CList2DNode* pleftnode);//pleftnode must be one line's leftmost node
	void         DeleteOneColumn(CList2DNode* ptopnode);//ptopnode must be one column's topmost node
	void         DeleteAll();

	//move around
	void         MoveBottomLine2Top();
	void         MoveTopLine2Bottom();
	void         MoveLeftMostColumn2Right();
	void         MoveRightMostColumn2Left();

protected:
	void         Init();
	CList2DNode* GetTopLineLeftMost(){return m_head;};
	CList2DNode* GetBottomLineLeftMost();
	CList2DNode* GetLeftColumnTopMost(){return m_head;}
	CList2DNode* GetRightColumnTopMost();
protected:
	CList2DNode* m_head;//always pointer to the up-left node, change when possible
// 	CList2DNode* m_curline;//pointer to the current line leftmost node, reset to head when modified or moved
// 	CList2DNode* m_curcolumn;//pointer to the current column topmost node, reset to head when modified or moved
	int          m_curlinecnt;
	int          m_curcolumncnt;
};

#endif//LIST2D_H_