#include<iostream>

using namespace std;


template<class K, class V>
struct AVLTreeNode //节点结构
{
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent; //父节点
	K _key;
	V _value;
	int _bf; // 平衡因子：右子树与左子树的高度差

	AVLTreeNode(const K& key, const V& value)
		:_left(NULL)
		, _right(NULL)
		, _parent(NULL)
		, _key(key)
		, _value(value)
		, _bf(0)
	{}
};

template<class K, class V>
class AVLTree
{
	typedef AVLTreeNode<K, V> Node;
public:
	//构造函数
	AVLTree()
		:_root(NULL)
	{}
	
	//插入

	bool Insert(const K& key, const V& value)
	{
		if (_root == NULL)
		{
			_root = new Node(key, value);
			return true;
		}
		//先把节点插入对应的位置
		//调整树的结构，使树平衡
		//更新平衡因子
		Node* parent = NULL;
		Node* cur = _root;

		while (cur)
		{
			if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (key > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}

		cur = new Node(key, value);

		if (key < parent->_key)
		{
			parent->_left = cur;
			cur->_parent = parent;
		}
		else
		{
			parent->_right = cur;
			cur->_parent = parent;
		}

		
		// 如果平衡因子在-1~1 之间，继续向上更新
		while (parent)
		{
			
			if (cur == parent->_left)
				parent->_bf--;
			else
				parent->_bf++;

			if (parent->_bf == 0)
			{
				break;
			}
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				
				cur = parent;
				parent = parent->_parent;
			}

			// -2 或 +2 的情况 不满足平衡条件 进行旋转
			else 
			{
				if (parent->_bf == 2)
				{
					if (cur->_bf == 1) // 左单旋
					{
						_RotateL(parent);
					}
					else // -1   右左双旋
					{
						_RotateRL(parent);
					}
				}

				// parent == -2
				else 
				{
					if (cur->_bf == -1) // 右单旋
					{
						_RotateR(parent);
					}
					else // cur = 1  左右双旋
					{
						_RotateLR(parent);
					}
				}

				break;
			}
		}

		return true;

	}

	//查找
	Node* Find(const K& key)
	{
		return _Find(_root, key);
	}

	//树的高度
	int Height()
	{
		return _Height(_root);
	}

	// 检查是否是 平衡二叉树
	bool IsBalance() 
	{
		return _IsBalance(_root);
	}

	//输出
	void PrintAVLTree()
	{
		_PrintAVLTree(_root);
		cout << endl;
	}

protected:
	Node* _Find(Node* root, const K& key)
	{
		if (root == NULL)
		{
			return NULL;
		}

		if (key < root->_key)
		{
			return _Find(root->_left, key);
		}
		else if (key > root->_key)
		{
			return _Find(root->_right, key);
		}
		else
		{
			return root;
		}
	}

	void _RotateL(Node* parent) // 左单旋转
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		parent->_right = subRL;
		if (subRL)
			subRL->_parent = parent;
		Node* pp = parent->_parent;
		subR->_parent = pp;
		subR->_left = parent;
		parent->_parent = subR;

		if (pp) // 不是根节点
		{
			if (pp->_left == parent)
			{
				pp->_left = subR;
			}
			else
			{
				pp->_right = subR;
			}
		}
		else
		{
			_root = subR;
		}

		// 更新平衡因子 
		parent->_bf = subR->_bf = 0;
	}

	void _RotateR(Node* parent) // 右单旋转
	{
		Node* SubL = parent->_left;
		Node* SubLR = SubL->_right;
		Node* pp = parent->_parent;

		parent->_left = SubLR;
		if (SubLR)
		{
			SubLR->_parent = parent;
		}
		SubL->_right = parent;
		parent->_parent = SubL;
		SubL->_parent = pp;

		if (pp) // 判断是否为根节点
		{
			if (parent == pp->_left)
			{
				pp->_left = SubL;
			}
			else
			{
				pp->_right = SubL;
			}
		}
		else
		{
			_root = SubL;
		}


		// 更新平衡因子 
		parent->_bf = SubL->_bf = 0;

	}

	void _RotateLR(Node* parent) // 左右旋转
	{
		// 记录结点指针 后面修正 平衡因子
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;

		_RotateL(parent->_left);
		_RotateR(parent);

		// 根据subLR的平衡因子修正其他结点的平衡因子
		if (bf == -1)
		{
			//subL->_bf = 0;
			parent->_bf = 1;
		}
		else if (bf == 1)
		{
			subL->_bf = -1;
			//parent->_bf = 0;
		}
	}

	void _RotateRL(Node* parent) // 右左旋转
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;
		
		_RotateR(parent->_right);
		_RotateL(parent);

		// 修正平衡因子
		if (bf == 1)
		{
			parent->_bf = -1;
		}
		else if (bf == -1)
		{
			subR->_bf = 1;
		}
	}

	int _Height(Node* root)
	{
		if (root == NULL)
		{
			return 0;
		}

		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);

		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}

	bool _IsBalance(Node* root)
	{
		if (root == NULL)
		{
			return true;
		}

		int left = _Height(root->_left);
		int right = _Height(root->_right);

		if (right - left != root->_bf || abs(right - left) > 1)
		{
			cout << "结点的平衡因子有问题：" << root->_key << endl;
			return false;
		}

		return _IsBalance(root->_left) && _IsBalance(root->_right);
	}

	void _PrintAVLTree(Node* root)
	{
		if (root == NULL)
		{
			return;
		}

		_PrintAVLTree(root->_left);
		cout << root->_key << "  ";
		_PrintAVLTree(root->_right);
	}

protected:
	Node* _root;
};
