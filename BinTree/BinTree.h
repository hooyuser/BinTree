#ifndef BINTREE_H_
#define BINTREE_H_

#include "stdio.h"
#include <algorithm>
#include <typeinfo.h>
#include "stack.h"
//#include "vector.h"
#include "queue.h"


#define BinNodePosi(T) BinNode<T> * //节点位置，即指向BinNode对象的指针
#define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色


/******************************************************************************************
* BinNode状态与性质的判断
******************************************************************************************/
#define IsRoot(x) (!((x).parent)) //判断节点x是否为根节点
#define IsLChild(x) (!IsRoot(x) && (&(x) == (x).parent->lc)) //判断节点x是否为某一节点的左孩子
#define IsRChild(x) (!IsRoot(x) && (&(x) == (x).parent->rc)) //判断节点x是否为某一节点的右孩子
#define HasParent(x) (!IsRoot(x))
#define HasLChild(x) ((x).lc) //判断节点x是否拥有左孩子
#define HasRChild(x) ((x).rc) //判断节点x是否拥有右孩子
#define HasChild(x) (HasLChild(x) || HasRChild(x)) //判断节点x是否至少拥有一个孩子
#define HasBothChild(x) (HasLChild(x) && HasRChild(x)) //判断节点x是否同时拥有两个孩子
#define IsLeaf(x) (!HasChild(x)) //判断节点x是否为叶节点

/******************************************************************************************
* 与BinNode具有特定关系的节点及指针
******************************************************************************************/
#define sibling(p) (IsLChild(*(p)) ? (p)->parent->rc : (p)->parent->lc ) //返回指针p所指节点的兄弟的指针
#define uncle(p) sibling((p)->parent) //返回指针p所指节点的叔叔的指针
#define FromParentTo(x) (IsRoot(x) ? _root : ( IsLChild(x) ? (x).parent->lc : (x).parent->rc ) ) //返回节点x的父亲拥有的一个指针，该指针指向x自身
//#define uncle(x) (IsLChild(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc ) //叔叔


using namespace std;


/******************************************************************************************
* 列表、向量等结构内的节点中，可以存放基本类型或构造类型
* 按照本书约定，出于效率的考虑，对于后一情况通常只保存对象的指针
* 因此，在主体结构析构之前，需要首先释放这些成员对象所占的空间
* 此处，借助C++中偏特化技术区分上述两种情况，并做对应处理
*******************************************************************************************/

template <typename T>
struct Cleaner
{
	static void clean(T x)
	{ //相当于递归基
#ifdef _DEBUG
		static int n = 0;
		if (7 > strlen(typeid (T).name()))
		{ //复杂类型一概忽略，只输出基本类型
			printf("\t<%s>[%d]=", typeid (T).name(), ++n);
			//print(x);
			printf(" purged\n");
		}
#endif
	}
};

template <typename T>
struct Cleaner<T*>
{
	static void clean(T* x)
	{
		if (x) { delete x; } //如果其中包含指针，递归释放
#ifdef _DEBUG                
		static int n = 0;
		printf("\t<%s>[%d] released\n", typeid (T*).name(), ++n);
#endif
	}

};

template <typename T>
void release(T x)
{
	Cleaner<T>::clean(x);
}


/*二叉树节点模板类*/
template <typename T>
class BinNode
{
public:
	// 成员
	T data; //数值
	BinNodePosi(T) parent; //父节点
	BinNodePosi(T) lc; //左孩子
	BinNodePosi(T) rc; //右孩子
	int height; //高度（通用）
	int npl; //Null Path Length（左式堆，也可直接用height代替）
	RBColor color; //颜色（红黑树）

 // 构造函数
	BinNode() :
		parent(NULL), lc(NULL), rc(NULL), height(0), npl(1), color(RB_RED) { }

	BinNode(T e, BinNodePosi(T) p = NULL, BinNodePosi(T) lc = NULL, BinNodePosi(T) rc = NULL,
		int h = 0, int l = 1, RBColor c = RB_RED) :
		data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) { }

	// 操作接口
	int size(); //统计当前节点后代总数，亦即以其为根的子树的规模
	BinNodePosi(T) insertAsLC(T const&); //作为当前节点的左孩子插入新节点
	BinNodePosi(T) insertAsRC(T const&); //作为当前节点的右孩子插入新节点
	BinNodePosi(T) succ(); //取当前节点的直接后继
	template <typename VST> void travLevel(VST&); //子树层次遍历
	template <typename VST> void travPre(VST&); //子树先序遍历
	template <typename VST> void travIn(VST&); //子树中序遍历
	template <typename VST> void travPost(VST&); //子树后序遍历

 // 比较器、判等器
	bool operator< (BinNode const& bn) //小于
	{
		return data < bn.data;
	}
	bool operator== (BinNode const& bn) //等于
	{
		return data == bn.data;
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////
/*二叉树节点模板类实现*/
///////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
int BinNode<T>::size() //统计当前节点后代总数，即以其为根的子树规模
{
	int s = 1; //计入本身
	if (lc) s += lc->size(); //递归计入左子树规模
	if (rc) s += rc->size(); //递归计入右子树规模
	return s;
}


template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(T const& e)
{
	return lc = new BinNode(e, this); //将e作为当前节点的左孩子插入二叉树 
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(T const& e) //将e作为当前节点的右孩子插入二叉树
{
	return rc = new BinNode(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::succ()  //定位节点v的直接后继
{
	BinNodePosi(T) s = this; //记录后继的临时变量
	if (rc)
	{ //若有右孩子，则直接后继必在右子树中，具体地就是
		s = rc; //右子树中
		while (HasLChild(*s))
			s = s->lc; //最靠左（最小）的节点
	}
	else
	{ //否则，直接后继应是“将当前节点包含于其左子树中的最低祖先”，具体地就是
		while (IsRChild(*s))
			s = s->parent; //逆向地沿右向分支，不断朝左上方移动
		s = s->parent; //最后再朝右上方移动一步，即抵达直接后继（如果存在）
	}
	return s;
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travLevel(VST& visit)  //二叉树层次遍历算法
{
	Queue<BinNodePosi(T)> Q; //辅助队列
	Q.enqueue(this); //根节点入队
	while (!Q.empty()) //在队列再次变空之前，反复迭代
	{
		BinNodePosi(T) x = Q.dequeue();
		visit(x->data); //取出队首节点并访问之
		if (HasLChild(*x)) Q.enqueue(x->lc); //左孩子入队
		if (HasRChild(*x)) Q.enqueue(x->rc); //右孩子入队
	}
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travPre(VST& visit)
{ //二叉树先序遍历算法统一入口
	switch (rand() % 3)  //此处暂随机选择以做测试，共三种选择
	{
	case 1: travPre_I1(this, visit); break; //迭代版#1
	case 2: travPre_I2(this, visit); break; //迭代版#2
	default: travPre_R(this, visit); break; //递归版
	}
}

template <typename T, typename VST> //元素类型、操作器
void travPre_I1(BinNodePosi(T) x, VST& visit) //二叉树先序遍历算法（迭代版#1）
{
	Stack<BinNodePosi(T)> S; //辅助栈
	if (x)
		S.push(x); //根节点入栈
	while (!S.empty())
	{ //在栈变空之前反复循环
		x = S.pop();
		visit(x->data); //弹出并访问当前节点，其非空孩子的入栈次序为先右后左
		if (HasRChild(*x))
			S.push(x->rc);
		if (HasLChild(*x))
			S.push(x->lc);
	}
}

//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template <typename T, typename VST> //元素类型T、操作器VST
static void visitAlongVine(BinNodePosi(T) x, VST& visit, Stack<BinNodePosi(T)>& S)
{
	while (x)
	{
		visit(x->data); //访问当前节点
		S.push(x->rc); //右孩子入栈暂存（可优化：通过判断，避免空的右孩子入栈）
		x = x->lc;  //沿左分支深入一层
	}
}

template <typename T, typename VST> //元素类型、操作器
void travPre_I2(BinNodePosi(T) x, VST& visit)  //二叉树先序遍历算法（迭代版#2）
{
	Stack<BinNodePosi(T)> S; //辅助栈
	while (true)
	{
		visitAlongVine(x, visit, S); //从当前节点出发，逐批访问
		if (S.empty()) break; //若栈空，则退出循环
		x = S.pop(); //弹出下一批的起点
	}
}

template <typename T, typename VST> //元素类型、操作器
void travPre_R(BinNodePosi(T) x, VST& visit) //二叉树先序遍历算法（递归版）
{
	if (!x) return;
	visit(x->data);
	travPre_R(x->lc, visit);
	travPre_R(x->rc, visit);
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travIn(VST& visit)  //二叉树中序遍历算法统一入口
{
	switch (rand() % 5)  //此处暂随机选择以做测试，共五种选择
	{
	case 1: travIn_I1(this, visit); break; //迭代版#1
	case 2: travIn_I2(this, visit); break; //迭代版#2
	case 3: travIn_I3(this, visit); break; //迭代版#3
	case 4: travIn_I4(this, visit); break; //迭代版#4
	default: travIn_R(this, visit); break; //递归版
	}
}

template <typename T> //从当前节点出发，沿左分支不断深入，直至没有左分支的节点
static void goAlongVine(BinNodePosi(T) x, Stack<BinNodePosi(T)>& S) 
{
	while (x) 
	{ 
		S.push(x); 
		x = x->lc; 
	} //当前节点入栈后随即向左侧分支深入，迭代直到无左孩子
}

template <typename T, typename VST> //元素类型、操作器
void travIn_I1(BinNodePosi(T) x, VST& visit) { //二叉树中序遍历算法（迭代版#1）
	Stack<BinNodePosi(T)> S; //辅助栈
	while (true) {
		goAlongVine(x, S); //从当前节点出发，逐批入栈
		if (S.empty()) break; //直至所有节点处理完毕
		x = S.pop(); 
		visit(x->data); //弹出栈顶节点并访问之
		x = x->rc; //转向右子树
	}
}

template <typename T, typename VST> //元素类型、操作器
void travIn_R(BinNodePosi(T) x, VST& visit)
{ //二叉树中序遍历算法（递归版）
	if (!x) return;
	travIn_R(x->lc, visit);
	visit(x->data);
	travIn_R(x->rc, visit);
}

template <typename T> template <typename VST> //元素类型、操作器
void BinNode<T>::travPost(VST& visit)  //二叉树后序遍历算法统一入口
{
	switch (rand() % 2)  //此处暂随机选择以做测试，共两种选择
	{
	case 1: travPost_I(this, visit); break; //迭代版
	default: travPost_R(this, visit); break; //递归版
	}

}

///////////////////////////////////////////////////////////////////////////////////////////////
/*二叉树模板类*/
///////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
class BinTree
{
protected:
	int _size; //规模
	BinNodePosi(T) _root; //根节点指针
	virtual int updateHeight(BinNodePosi(T) x); //根据子树高度更新节点x的高度
	void updateHeightAbove(BinNodePosi(T) x); //更新节点x及其祖先的高度
public:
	BinTree() : _size(0), _root(NULL) { } //构造函数

	~BinTree() //析构函数
	{
		if (0 < _size)
			remove(_root);
	}

	int size() const  //规模
	{
		return _size;
	}

	bool empty() const  //判空
	{
		return !_root;
	}

	BinNodePosi(T) root() const //树根
	{
		return _root;
	}

	BinNodePosi(T) insertAsRoot(T const& e); //插入根节点

	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, T const& e); //e作为x的左孩子（原无）插入

	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, T const& e); //e作为x的右孩子（原无）插入

	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T); //T作为x左子树接入

	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T); //T作为x右子树接入

	int remove(BinNodePosi(T) x); //删除以位置x处节点为根的子树，返回该子树原先的规模

	BinTree<T>* secede(BinNodePosi(T) x); //将子树x从当前树中摘除，并将其转换为一棵独立子树

	template <typename VST> //操作器
	void travLevel(VST& visit)
	{
		if (_root)
			_root->travLevel(visit);
	} //层次遍历

	template <typename VST> //操作器
	void travPre(VST& visit)
	{
		if (_root)
			_root->travPre(visit);
	} //先序遍历

	template <typename VST> //操作器
	void travIn(VST& visit)
	{
		if (_root)
			_root->travIn(visit);
	} //中序遍历

	template <typename VST> //操作器
	void travPost(VST& visit)
	{
		if (_root)
			_root->travPost(visit);
	} //后序遍历

	bool operator< (BinTree<T> const& t) //比较器（其余自行补充）
	{
		return _root && t._root && lt(_root, t._root);
	}
	bool operator== (BinTree<T> const& t) //判等器
	{
		return _root && t._root && (_root == t._root);
	}
};


///////////////////////////////////////////////////////////////////////////////////////////////
/*二叉树模板类实现*/
///////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) //更新节点x高度
{
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
} //具体规则，因树而异

template <typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) //更新高度
{
	while (x)
	{
		updateHeight(x);
		x = x->parent;
	}
} //从x出发，覆盖历代祖先。可优化

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(T const& e) //将e当作根节点插入空的二叉树
{
	_size = 1;
	return _root = new BinNode<T>(e);
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, T const& e)  //e插入为x的左孩子
{
	_size++;
	x->insertAsLC(e);
	updateHeightAbove(x);
	return x->lc;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, T const& e)  //e插入为x的右孩子
{
	_size++;
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
}

template <typename T> //二叉树子树接入算法：将S当作节点x的左子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &S)
{ //x->lc == NULL
	if (x->lc = S->_root) //将树S根节点设置为x的左孩子。如果S->_root不为NULL
		x->lc->parent = x;  //则将x设置为树S根节点的父亲
	_size += S->_size;
	updateHeightAbove(x); //更新全树规模与x所有祖先的高度
	S->_root = NULL;
	S->_size = 0;
	release(S);
	S = NULL;
	return x; //释放原树，返回接入位置
}

template <typename T> //二叉树子树接入算法：将S当作节点x的右子树接入，S本身置空
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &S)
{ //假设x->rc == NULL
	if (x->rc = S->_root) //将树S根节点设置为x的右孩子。如果S->_root不为NULL
		x->rc->parent = x; //则将x设置为树S根节点的父亲
	_size += S->_size; //更新全树规模
	updateHeightAbove(x); //更新x所有祖先的高度
	S->_root = NULL;
	S->_size = 0;
	release(S);
	S = NULL;
	return x; //释放原树，返回接入位置
}

template <typename T> //二叉树子树分离算法：将子树x从当前树中摘除，将其封装为一棵独立子树返回
BinTree<T>* BinTree<T>::secede(BinNodePosi(T) x)
{ //assert: x为二叉树中的合法位置·····························
	FromParentTo(*x) = NULL; //切断来自父节点的指针
	updateHeightAbove(x->parent); //更新原树中所有祖先的高度
	BinTree<T>* S = new BinTree<T>;
	S->_root = x;
	x->parent = NULL; //新树以x为根
	S->_size = x->size();
	_size -= S->_size;
	return S; //更新规模，返回分离出来的子树
}

template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
int BinTree<T>::remove(BinNodePosi(T) x)
{ //assert: x为二叉树中的合法位置
	FromParentTo(*x) = NULL; //切断来自父节点的指针
	updateHeightAbove(x->parent); //更新祖先高度
	int n = removeAt(x);
	_size -= n;
	return n; //删除子树x，更新规模，返回删除节点总数
}

template <typename T> //删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
static int removeAt(BinNodePosi(T) x)
{ //assert: x为二叉树中的合法位置
	if (!x)
		return 0; //递归基：空树
	int n = 1 + removeAt(x->lc) + removeAt(x->rc); //递归释放左、右子树
	release(x->data);
	release(x);
	return n; //释放被摘除节点，并返回删除节点总数
} //release()负责释放复杂结构，与算法无直接关系，具体实现详见代码包



#endif 