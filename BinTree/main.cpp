#include "BinTree.h"
#include <iostream>

int main()
{
	BinTree<int>* myBinTree1 = new BinTree<int>();
	myBinTree1->insertAsRoot(1);
	BinNode<int>* BinNode2 = myBinTree1->insertAsLC(myBinTree1->root(), 2);
	myBinTree1->insertAsRC(myBinTree1->root(), 3);

	BinTree<int>* myBinTree2 = new BinTree<int>();
	myBinTree2->insertAsRoot(4);
	myBinTree2->insertAsLC(myBinTree2->root(), 5);
	myBinTree2->insertAsRC(myBinTree2->root(), 6);

	myBinTree1->attachAsLC(BinNode2, myBinTree2);

	//system("PAUSE");
	return 0;
}