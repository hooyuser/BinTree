#include "BinTree.h"
#include <iostream>
#include <iomanip>
#include <time.h> 

void myPrint(int n)
{
	cout << n << " ";
}


int main()
{
	clock_t  start, end;
	start = clock();


	BinTree<int> myTree;
	myTree.insertAsRoot(1);

	Queue<BinNodePosi(int)> posi;
	posi.enqueue(myTree.root());
	for (int i = 2; i < 10000; )
	{
		BinNodePosi(int) x = posi.dequeue();
		posi.enqueue(myTree.insertAsLC(x, i));
		i++;
		posi.enqueue(myTree.insertAsRC(x, i));
		i++;
	}


	end = clock();
	cout << "The time was:" << setprecision(6) << 1.0 * (end - start) / CLK_TCK << "s" << endl;

	void(*pPrint)(int) = myPrint;
	myTree.travLevel(pPrint);

	/*BinTree<int>* myBinTree1 = new BinTree<int>();
	myBinTree1->insertAsRoot(1);
	BinNode<int>* BinNode2 = myBinTree1->insertAsLC(myBinTree1->root(), 2);
	myBinTree1->insertAsRC(myBinTree1->root(), 3);

	BinTree<int>* myBinTree2 = new BinTree<int>();
	myBinTree2->insertAsRoot(4);
	myBinTree2->insertAsLC(myBinTree2->root(), 5);
	myBinTree2->insertAsRC(myBinTree2->root(), 6);

	myBinTree1->attachAsLC(BinNode2, myBinTree2);*/




	end = clock();
	cout << "The time was:" << setprecision(6) << 1.0 * (end - start) / CLK_TCK << "s" << endl;
	//CLK_TCK, CLOCKS_PER_SEC是为了将ms转化成s

	system("PAUSE");
	return 0;
}