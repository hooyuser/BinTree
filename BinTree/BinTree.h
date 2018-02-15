#pragma once

#define BinNodePosi(T) BinNode<T> * //节点位置
#define stature(p) ((p) ? (p)->height : -1) //节点高度（与“空树高度为-1”的约定相统一）
typedef enum { RB_RED, RB_BLACK } RBColor; //节点颜色


//二叉树节点模板类
template <typename T> class BinNode
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
		int h = 0, int l = 1, RBColor c = RB_RED) :s
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