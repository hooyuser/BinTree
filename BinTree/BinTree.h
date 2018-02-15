#pragma once

#define BinNodePosi(T) BinNode<T> * //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色


//二叉树节点模板类
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


//二叉树节点模板类实现
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


//二叉树模板类
template <typename T> 
class BinTree 
{ 
protected:
	int _size; //规模
	BinNodePosi(T) _root; //根节点
	virtual int updateHeight(BinNodePosi(T) x); //更新节点x的高度
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
	void travLevel(VST& visit) { if (_root) _root->travLevel(visit); } //层次遍历
	template <typename VST> //操作器
	void travPre(VST& visit) { if (_root) _root->travPre(visit); } //先序遍历
	template <typename VST> //操作器
	void travIn(VST& visit) { if (_root) _root->travIn(visit); } //中序遍历
	template <typename VST> //操作器
	void travPost(VST& visit) { if (_root) _root->travPost(visit); } //后序遍历

	bool operator< (BinTree<T> const& t) //比较器（其余自行补充）
	{
		return _root && t._root && lt(_root, t._root);
	}
	bool operator== (BinTree<T> const& t) //判等器
	{
		return _root && t._root && (_root == t._root);
	}
}; 

//二叉树模板类实现
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