#include <stdio.h>
#include "List2D.h"

CList2D::CList2D(int line, int colum)
{
	m_head = 0;
	m_curlinecnt = line;
	m_curcolumncnt = colum;

	Init();
}

void CList2D::ForEach(bool (*pFunc)(CList2DNode*))
{
	CList2DNode *plinenode = m_head;
	CList2DNode *ptmpnode;
	while (plinenode)
	{
		ptmpnode = plinenode;
		plinenode = plinenode->GetDownNode();

		while (ptmpnode)
		{
			if (!(*pFunc)(ptmpnode))
			{
				printf("for each error at %x", ptmpnode);
				return;
			}
			ptmpnode = ptmpnode->GetRightNode();
		}
		printf("\n");
	}
}

void CList2D::Init()
{
	int idx=0;
	int li,co;
	int sz = m_curcolumncnt*m_curlinecnt;
	CList2DNode *ptmp = 0;
	CList2DNode **ppnode = new CList2DNode *[sz];
	for (idx=0; idx<sz; idx++)
	{
		ppnode[idx] = new CList2DNode;
	}
	m_head = ppnode[0];
	
	if (m_curlinecnt>=2 && m_curcolumncnt>=2)
	{
		for (li=1; li<m_curlinecnt-1; li++)
		{
			for (co=1; co<m_curcolumncnt-1; co++)
			{
				ptmp = ppnode[li*m_curcolumncnt + co];
				ptmp->SetLeftNode(ppnode[li*m_curcolumncnt+co-1]);
				ptmp->SetRightNode(ppnode[li*m_curcolumncnt+co+1]);
				ptmp->SetUpNode(ppnode[(li-1)*m_curcolumncnt+co]);
				ptmp->SetDownNode(ppnode[(li+1)*m_curcolumncnt+co]);
			}
		}
		//top line 
		for (co=1; co<m_curcolumncnt-1; co++)
		{
			ptmp = ppnode[co];
			ptmp->SetLeftNode(ppnode[co-1]);
			ptmp->SetRightNode(ppnode[co+1]);
			ptmp->SetDownNode(ppnode[co+m_curcolumncnt]);
		}
		//bottom line
		for (co=sz-m_curcolumncnt+1; co<sz-1; co++)
		{
			ptmp = ppnode[co];
			ptmp->SetLeftNode(ppnode[co-1]);
			ptmp->SetRightNode(ppnode[co+1]);
			ptmp->SetUpNode(ppnode[co-m_curcolumncnt]);
		}
		//left and right column
		for (li=1; li<m_curlinecnt-1; li++)
		{
			ptmp = ppnode[li*m_curcolumncnt];
			ptmp->SetUpNode(ppnode[li*m_curcolumncnt-m_curcolumncnt]);
			ptmp->SetDownNode(ppnode[li*m_curcolumncnt+m_curcolumncnt]);
			ptmp->SetRightNode(ppnode[li*m_curcolumncnt+1]);

			ptmp = ppnode[(li+1)*m_curcolumncnt-1];
			ptmp->SetUpNode(ppnode[li*m_curcolumncnt-1]);
			ptmp->SetDownNode(ppnode[(li+2)*m_curcolumncnt-1]);
			ptmp->SetLeftNode(ppnode[(li+1)*m_curcolumncnt-2]);
		}
		//corner 4 node
		ptmp = ppnode[0];//top left
		ptmp->SetRightNode(ppnode[1]);
		ptmp->SetDownNode(ppnode[m_curcolumncnt]);
		ptmp = ppnode[m_curcolumncnt-1];//top right
		ptmp->SetLeftNode(ppnode[m_curcolumncnt-2]);
		ptmp->SetDownNode(ppnode[2*m_curcolumncnt-1]);
		ptmp = ppnode[sz-m_curcolumncnt];//bottom left
		ptmp->SetRightNode(ppnode[sz-m_curcolumncnt+1]);
		ptmp->SetUpNode(ppnode[sz-2*m_curcolumncnt]);
		ptmp = ppnode[sz-1];//bottom right
		ptmp->SetUpNode(ppnode[sz-m_curcolumncnt-1]);
		ptmp->SetLeftNode(ppnode[sz-2]);
	}else if (m_curlinecnt==1 && m_curcolumncnt>=2)//only one line
	{

	}else if (m_curlinecnt>=2 && m_curcolumncnt==1)//only one colume
	{

	}

	delete[]ppnode;
}

CList2D::~CList2D()
{
	DeleteAll();
}

bool CList2D::AddLine2Top()
{
	//from m_head
	CList2DNode* pnode = m_head;
	CList2DNode* plastnode, *plastnodeleft;
	CList2DNode* pnewnode;
	while (pnode)
	{
		plastnode = pnode;
		pnode = pnode->GetRightNode();
		pnewnode = new CList2DNode;
		if (pnewnode == 0)//messed up TODO:....
			return false;//////////////////////////////////////////////////////////////////////////
		pnewnode->SetUpNode(0);
		pnewnode->SetDownNode(plastnode);
		pnewnode->SetRightNode(0);
		plastnodeleft = plastnode->GetLeftNode();
		if (plastnodeleft)
		{
			pnewnode->SetLeftNode(plastnodeleft->GetUpNode());
			plastnodeleft->GetUpNode()->SetRightNode(pnewnode);
		}
		plastnode->SetUpNode(pnewnode);
	}

	m_curlinecnt++;
	m_head = m_head->GetUpNode();//new head
	return true;
}

bool CList2D::AddLine2Bottom()
{
	//from bottom
	CList2DNode* pnode = GetBottomLineLeftMost();
	CList2DNode* plastnode, *plastnodeleft;
	CList2DNode* pnewnode;
	while (pnode)
	{
		plastnode = pnode;
		pnode = pnode->GetRightNode();
		pnewnode = new CList2DNode;
		if (pnewnode == 0)//messed up TODO:....
			return false;///////////////////////////////////////////////////////////////////////////
		pnewnode->SetDownNode(0);
		pnewnode->SetUpNode(plastnode);
		pnewnode->SetRightNode(0);
		plastnodeleft = plastnode->GetLeftNode();
		if (plastnodeleft)
		{
			pnewnode->SetLeftNode(plastnodeleft->GetDownNode());
			plastnodeleft->GetDownNode()->SetRightNode(pnewnode);
		}
		plastnode->SetDownNode(pnewnode);
	}
	
	m_curlinecnt++;
	return true;
}

bool CList2D::AddColumn2Left()
{
	//from m_head
	CList2DNode* pnode = m_head;
	CList2DNode* plastnode, *plastnodeup;
	CList2DNode* pnewnode;
	while (pnode)
	{
		plastnode = pnode;
		pnode = pnode->GetDownNode();
		pnewnode = new CList2DNode;
		if (pnewnode == 0)//messed up TODO:....
			return false;/////////////////////////////////////////////////////////////////////
		pnewnode->SetLeftNode(0);
		pnewnode->SetRightNode(plastnode);
		pnewnode->SetDownNode(0);
		plastnodeup = plastnode->GetUpNode();
		if (plastnodeup)
		{
			pnewnode->SetUpNode(plastnodeup->GetLeftNode());
			plastnodeup->GetLeftNode()->SetDownNode(pnewnode);
		}
		plastnode->SetLeftNode(pnewnode);
	}

	m_curcolumncnt++;
	m_head = m_head->GetLeftNode();//new head
	return true;
}

bool CList2D::AddColumn2Right()
{
	CList2DNode *pnode = GetRightColumnTopMost();
	CList2DNode* plastnode, *plastnodeup;
	CList2DNode* pnewnode;
	while (pnode)
	{
		plastnode = pnode;
		pnode = pnode->GetDownNode();
		pnewnode = new CList2DNode;
		if (pnewnode == 0)//messed up TODO:....
			return false;////////////////////////////////////////////////////////////////////////
		pnewnode->SetRightNode(0);
		pnewnode->SetLeftNode(plastnode);
		pnewnode->SetDownNode(0);
		plastnodeup = plastnode->GetUpNode();
		if (plastnodeup)
		{
			pnewnode->SetUpNode(plastnodeup->GetRightNode());
			plastnodeup->GetRightNode()->SetDownNode(pnewnode);
		}
		plastnode->SetRightNode(pnewnode);
	}
	m_curcolumncnt++;
	return true;
}

void CList2D::DeleteOneLine(CList2DNode* pleftnode)//delete from left to right
{
	if ( pleftnode == m_head)
		m_head = m_head->GetDownNode();

	CList2DNode *pnode = pleftnode;
	CList2DNode *plastnode, *plastnodeup, *plastnodedown;
	while (pnode)
	{
		plastnode = pnode;
		pnode = pnode->GetRightNode();
		plastnodeup = plastnode->GetUpNode();
		plastnodedown = plastnode->GetDownNode();
		if (plastnodeup)
			plastnodeup->SetDownNode(plastnodedown);
		if (plastnodedown)
			plastnodedown->SetUpNode(plastnodeup);

		delete plastnode;
	}
	m_curlinecnt--;
}

void CList2D::DeleteOneColumn(CList2DNode* ptopnode)//delete from top to bottom
{
	if (ptopnode == m_head)
		m_head = m_head->GetRightNode();

	CList2DNode *pnode = ptopnode;
	CList2DNode *plastnode, *plastnodeleft, *plastnoderight;
	while (pnode)
	{
		plastnode = pnode;
		pnode = pnode->GetDownNode();
		plastnodeleft = plastnode->GetLeftNode();
		plastnoderight = plastnode->GetRightNode();
		if (plastnodeleft)
			plastnodeleft->SetRightNode(plastnoderight);
		if (plastnoderight)
			plastnoderight->SetLeftNode(plastnodeleft);

		delete plastnode;
	}
	m_curcolumncnt--;
}

void CList2D::DeleteAll()
{
	CList2DNode *pline = m_head;
	CList2DNode *pnode, *pleftmost, *ptmpnode;
	while (pline)
	{
		pleftmost = pline;
		pnode = pleftmost->GetRightNode();
		//for every other node in line
		while (pnode)
		{
			ptmpnode = pnode;
			pnode = pnode->GetRightNode();
			delete ptmpnode;
		}

		pline = pline->GetDownNode();//next line
		//delete leftmost node
		delete pleftmost;
	}
	m_head = 0;
	m_curlinecnt = 0;
	m_curcolumncnt = 0;
}

void CList2D::MoveBottomLine2Top()
{	
	if (!m_head)
		return;
	CList2DNode *pori_first_line_leftmost = m_head;
	CList2DNode *pori_first_line_node = pori_first_line_leftmost;
	CList2DNode *pori_last_line_leftmost = GetBottomLineLeftMost();
	CList2DNode *pori_last_line_node = pori_last_line_leftmost;
	
	CList2DNode *pori_last_but_one_line_leftmost = pori_last_line_leftmost->GetUpNode();//mind head been changed
	CList2DNode *pori_last_but_one_line_node = pori_last_but_one_line_leftmost;
	while (pori_first_line_node && pori_last_line_node)
	{
		pori_last_line_node->SetDirty(true);
		pori_last_line_node->SetUpNode(0);
		pori_last_line_node->SetDownNode(pori_first_line_node);
		pori_first_line_node->SetUpNode(pori_last_line_node);

		pori_last_line_node = pori_last_line_node->GetRightNode();
		pori_first_line_node = pori_first_line_node->GetRightNode();
	}
	while (pori_last_but_one_line_node)
	{
		pori_last_but_one_line_node->SetDownNode(0);
		pori_last_but_one_line_node = pori_last_but_one_line_node->GetRightNode();
	}

	m_head = pori_last_line_leftmost;//new head
}

void CList2D::MoveTopLine2Bottom()
{
	if (!m_head)
		return;
	CList2DNode *pori_first_line_leftmost = m_head;
	CList2DNode *pori_first_line_node = pori_first_line_leftmost;
	CList2DNode *pori_last_line_leftmost = GetBottomLineLeftMost();
	CList2DNode *pori_last_line_node = pori_last_line_leftmost;
	CList2DNode *pori_second_line_leftmost = m_head->GetDownNode();//mind head been changed
	CList2DNode *pori_second_line_node = pori_second_line_leftmost;
	while (pori_first_line_node && pori_last_line_node)
	{
		pori_first_line_node->SetDirty(true);
		pori_first_line_node->SetDownNode(0);
		pori_first_line_node->SetUpNode(pori_last_line_node);
		pori_last_line_node->SetDownNode(pori_first_line_node);

		pori_first_line_node = pori_first_line_node->GetRightNode();
		pori_last_line_node = pori_last_line_node->GetRightNode();
	}
	while (pori_second_line_node)
	{
		pori_second_line_node->SetUpNode(0);
		pori_second_line_node = pori_second_line_node->GetRightNode();
	}

	m_head = pori_second_line_leftmost;//new head
}

void CList2D::MoveLeftMostColumn2Right()
{
	if (!m_head)
		return;
	CList2DNode *pori_first_column_topmost = m_head;
	CList2DNode *pori_first_column_node = pori_first_column_topmost;
	CList2DNode *pori_last_column_topmost = GetRightColumnTopMost();
	CList2DNode *pori_last_column_node = pori_last_column_topmost;
	CList2DNode *pori_second_column_topmost = m_head->GetRightNode();
	CList2DNode *pori_second_column_node = pori_second_column_topmost;
	while (pori_first_column_node && pori_last_column_node)
	{
		pori_first_column_node->SetRightNode(0);
		pori_first_column_node->SetLeftNode(pori_last_column_node);
		pori_last_column_node->SetRightNode(pori_first_column_node);

		pori_last_column_node = pori_last_column_node->GetDownNode();
		pori_first_column_node = pori_first_column_node->GetDownNode();
	}
	while (pori_second_column_node)
	{
		pori_second_column_node->SetLeftNode(0);
		pori_second_column_node = pori_second_column_node->GetDownNode();
	}

	m_head = pori_second_column_topmost;//new head
}

void CList2D::MoveRightMostColumn2Left()
{
	if (!m_head)
		return;
	CList2DNode *pori_first_column_topmost = m_head;
	CList2DNode *pori_first_column_node = pori_first_column_topmost;
	CList2DNode *pori_last_column_topmost = GetRightColumnTopMost();
	CList2DNode *pori_last_column_node = pori_last_column_topmost;
	CList2DNode *pori_last_but_one_column_topomost = pori_last_column_topmost->GetLeftNode();
	CList2DNode *pori_last_but_one_column_node = pori_last_but_one_column_topomost;

	while (pori_first_column_node && pori_last_column_node)
	{
		pori_last_column_node->SetLeftNode(0);
		pori_last_column_node->SetRightNode(pori_first_column_node);
		pori_first_column_node->SetLeftNode(pori_last_column_node);

		pori_first_column_node = pori_first_column_node->GetDownNode();
		pori_last_column_node = pori_last_column_node->GetDownNode();
	}
	while (pori_last_but_one_column_node)
	{
		pori_last_but_one_column_node->SetRightNode(0);
		pori_last_but_one_column_node = pori_last_but_one_column_node->GetDownNode();
	}

	m_head = pori_last_column_topmost;
}

CList2DNode* CList2D::GetBottomLineLeftMost()
{
	CList2DNode* pleftmost = m_head;
	CList2DNode* ptmpnode;
	while (pleftmost)
	{
		ptmpnode = pleftmost;
		pleftmost = pleftmost->GetDownNode();
	}
	return ptmpnode;
}

CList2DNode* CList2D::GetRightColumnTopMost()
{
	CList2DNode* ptopmost = m_head;
	CList2DNode* ptmpnode;
	while (ptopmost)
	{
		ptmpnode = ptopmost;
		ptopmost = ptopmost->GetRightNode();
	}
	return ptmpnode;
}
