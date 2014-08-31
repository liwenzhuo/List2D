#include <stdio.h>
#include "List2D.h"

int gidx = 1;
bool FreeNodeData(CList2DNode* pnode)
{
	delete pnode->GetDatePointer();
	return true;
}
bool SetNodeData(CList2DNode* pnode)
{
	int* pint = new int;
	*pint = gidx++;
	pnode->SetDataPointer(pint);
	return true;
}
bool PrintNodeData(CList2DNode* pnode)
{
	void *data = pnode->GetDatePointer();
	if (data)
		printf("%10d", *((int*)data));
	else
		printf("%10.6f", 0.f);
	return true;
}

int main(int argc, char*argv[])
{
	CList2D lst2d(5, 7);
	lst2d.ForEach(SetNodeData);
	lst2d.ForEach(PrintNodeData);
	printf("\n\n");

	lst2d.AddLine2Top();
	lst2d.AddLine2Bottom();
	lst2d.AddColumn2Left();
	lst2d.AddColumn2Right();
	lst2d.ForEach(PrintNodeData);
	printf("\n\n");

	lst2d.ForEach(FreeNodeData);
	printf("\n\n");
	printf ("%d\t\t%d\n", CList2DNode::Reserve1Offset_int(), CList2DNode::Reserve2Offset_int64());
	printf ("%d\n", sizeof(CList2DNode));
	return 0;
}