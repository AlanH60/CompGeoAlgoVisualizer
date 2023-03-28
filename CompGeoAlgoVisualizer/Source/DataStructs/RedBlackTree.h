#pragma once

template <typename T>
class RedBlackTree
{
	private:
		struct Node
		{
			T element;
			Node* parent;
			Node* left;
			Node* right;
			bool isBlack;

			~Node()
			{
				if (this->left)
					delete this->left;
				if (this->right)
					delete this->right;
			}
		};

	public:
		RedBlackTree(const std::function<bool(const T&, const T&)>& compare)
		{
			mCompare = compare;
		}
		~RedBlackTree()
		{
			if (pRoot)
				delete pRoot;
		}
		/**
		* @returns number of elements in the tree.
		*/
		size_t getSize()
		{
			return mSize;
		}
		/**
		* Inserts an element into the tree.
		* @param element - the element to be inserted.
		* @returns True if element was successfully inserted. False if the element already exists in the tree.
		*/
		bool insert(T element)
		{
			if (!pRoot)
			{
				pRoot = new Node{ element, nullptr, nullptr, nullptr, true };
				mSize++;
			}
			else
			{
				if (__insert(element, pRoot))
				{
					mSize++;
					return true;
				}
				return false;
			}
			return true;
		}
		/**
		* Remove an element from the tree.
		* @param element - the element to be removed.
		* @returns True if element was successfully removed. False if the element doesn't exist in the tree.
		*/
		bool remove(T element)
		{
			if (__remove(element, pRoot))
			{
				mSize--;
				return true;
			}
			return false;
		}
		/**
		* Checks if the tree contains a specific element.
		* @param element - the element in question.
		* @returns True if element was successfully found. False if the element doesn't exist in the tree.
		*/
		bool contains(T element)
		{
			return (__findNode(element, pRoot) != nullptr);
		}
		/**
		* Searches for the neighbors(predecessor and successor) of an element in the tree.
		* @param element the element you want the neighbors of.
		* @param pPre pointer to the T object to be filled with the predecessor element if one is found.
		* @param pSuc pointer to the T object to be filled with the successor element if one is found.
		* @returns std::pair<bool, bool> where the each flag indicates whether a predecessor or successor is found respectively.
		*/
		std::pair<bool, bool> findNeighbors(T element, T* pPre, T* pSuc)
		{
			Node* node = __findNode(element, pRoot);
			std::pair<bool, bool> res = { false, false };
			if (node)
			{
				//Find predecessor/successor in child nodes
				Node* pre = findPredecessor(node);
				Node* suc = findSuccessor(node);
				//If those don't exist, find them in the parent nodes.
				if (!pre)
					pre = __findParentLeft(node);
				if (!suc)
					suc = __findParentRight(node);
				//If any is found, fill in inputted pointers with the element.
				if (pre)
					*pPre = pre->element;
				if (suc)
					*pSuc = suc->element;
				res = { (bool)pre, (bool)suc };
			}
			else //Element isn't in the tree.
			{
				Node* parent = __findParentOfInsertion(element, pRoot);
				if (parent)
				{
					Node* pre = nullptr;
					Node* suc = nullptr;
					if (mCompare(parent->element, element))
					{
						pre = parent;
						suc = __findParentRight(parent);
					}
					else
					{
						suc = parent;
						pre = __findParentLeft(parent);
					}
					//If any is found, fill in inputted pointers with the element.
					if (pre)
						*pPre = pre->element;
					if (suc)
						*pSuc = suc->element;
					res = { (bool)pre, (bool)suc };
				}
			}
			return res;
		}
		void str()
		{
			if (!pRoot)
				return;
			std::queue<Node*> q;
			Node* node = nullptr;
			q.push(pRoot);
			do
			{
				node = q.front();
				q.pop();
				std::cout << node->element << "    ";
				if (node->left)
					q.push(node->left);
				if (node->right)
					q.push(node->right);
			} while (!q.empty());
			
		}

		bool isValid()
		{
			if (pRoot && !pRoot->isBlack)
				return false;
			int bHeight = 0;
			Node* node = pRoot;
			while (node)
			{
				if (node->isBlack)
					bHeight++;
				node = node->left;
			}
			return __isValid(pRoot, bHeight, 0);
		}
	protected:
		bool __isValid(Node* node, int bHeight, int currBHeight)
		{
			if (!node)
				return bHeight == currBHeight;
			else if (node->isBlack)
				currBHeight++;
			if (__isValid(node->left, bHeight, currBHeight))
				return __isValid(node->right, bHeight, currBHeight);
			return false;
		}
		/**
		* Recursive function to insert a node.
		* @param element - the element to be inserted.
		* @param node - current node to check for availible insertion.
		* @returns True if element was successfully inserted. False if the element already exists in the tree.
		*/
		bool __insert(T element, Node* node)
		{
			if (mCompare(element, node->element))
			{
				if (!node->left)
				{
					node->left = new Node{ element, node, nullptr, nullptr, false };
					validateTreePostInsertion(node->left);
					return true;
				}
				else 
					return __insert(element, node->left);
			}
			else if (mCompare(node->element, element))
			{
				if (!node->right)
				{
					node->right = new Node{ element, node, nullptr, nullptr, false };
					validateTreePostInsertion(node->right);
					return true;
				}
				else
					return __insert(element, node->right);
			}
			else
				return false; //Ignores duplicates
		}
		/**
		* Recursive function to remove a node.
		* @param element - the element to be deleted.
		* @param node - current node to check for deletion
		* @returns True if element was successfully found and deleted. False if the element doesn't exist in the tree.
		*/ 
		bool __remove(T element, Node* node, bool dealloc=true)
		{
			if (!node)
				return false;
			if (mCompare(element, node->element))
				return __remove(element, node->left);
			if (mCompare(node->element, element))
				return __remove(element, node->right);

			//DELETE this node
			if (!node->left && !node->right) //Is leaf
			{
				if (node != pRoot) //Check if this node has parent
				{
					//Check which of the parent's branch the node is in
					bool isLeft = (node->parent->left == node);
					if (isLeft)
						node->parent->left = nullptr;
					else
						node->parent->right = nullptr;
					if (node->isBlack)
						validatePostDeletion(node->parent, isLeft);
				}
				else //If no parent then we've deleted the root
					pRoot = nullptr;
				if (dealloc)
				{
					node->left = nullptr;
					node->right = nullptr;
					delete node;
				}
			}
			else if (node->left && !node->right) //Has left child but no right
			{
				if (node != pRoot)
				{
					//Check which of the parent's branch the node is in
					bool isLeft = (node->parent->left == node);
					if (isLeft)
						node->parent->left = node->left;
					else
						node->parent->right = node->left;
					node->left->parent = node->parent;

					if (node->isBlack != node->left->isBlack) //Either is red/black
						node->left->isBlack = true;
					else
						validatePostDeletion(node->parent, isLeft);
				}
				else
				{
					pRoot = node->left;
					pRoot->isBlack = true;
				}


				if (dealloc)
				{
					node->left = nullptr;
					node->right = nullptr;
					delete node;
				}
			}
			else if (node->right && !node->left) //Has right child but no left
			{
				if (node != pRoot)
				{
					//Check which of the parent's branch the node is in
					bool isLeft = (node->parent->left == node);
					if (isLeft)
						node->parent->left = node->right;
					else
						node->parent->right = node->right;
					node->right->parent = node->parent;

					if (node->isBlack != node->right->isBlack) //Either is red/black
						node->right->isBlack = true;
					else
						validatePostDeletion(node->parent, isLeft);
				}
				else
				{
					pRoot = node->right;
					pRoot->isBlack = true;
				}

				if (dealloc)
				{
					node->left = nullptr;
					node->right = nullptr;
					delete node;
				}
			}
			else
			{
				Node* successor = findSuccessor(node); //Find successor to replace the node

				__remove(successor->element, successor, false); //Extract successor while keeping the tree balanced.

				//Inject successor
				if (node != pRoot)
				{
					//Check which of the parent's branch the node is in
					if (node->parent->right == node)
					{
						node->parent->right = successor;
						successor->parent = node->parent;
					}
					else
					{
						node->parent->left = successor;
						successor->parent = node->parent;
					}
				}
				else
					pRoot = successor;

				//Successor takes the children.
				successor->left = node->left;
				successor->right = node->right;
				//Node's left or right child could've been taken away during removal of successor
				if (node->left) 
					node->left->parent = successor;
				if (node->right)
					node->right->parent = successor;

				successor->isBlack = node->isBlack; //Successor takes the same color, since the tree is already balanced.

				if (dealloc)
				{
					node->left = nullptr;
					node->right = nullptr;
					delete node;
				}
			}
			return true;
		}
		/**
		* Finds the leftmost child in the node's right subtree.
		* @param node node in question.
		* @returns node of the successor or nullptr if inputted node has no right child.
		*/
		Node* findSuccessor(Node* node) 
		{
			if (!node->right)
				return nullptr;
			return __findSuccessor(node->right);
		}
		/**
		* Recursive function used to find the successor of a node.
		* @param node current node.
		* @returns node of the successor.
		*/
		Node* __findSuccessor(Node* node) 
		{
			if (node->left)
				return __findSuccessor(node->left);
			return node;
		}

		/**
		* Finds the rightmost child in the node's left subtree.
		* @param node node in question.
		* @returns node of the predecessor or nullptr if inputted node has no left child.
		*/
		Node* findPredecessor(Node* node)
		{
			if (!node->left)
				return nullptr;
			return __findPredecessor(node->left);
		}
		/**
		* Recursive function used to find the predecessor of a node.
		* @param node current node.
		* @returns node of the predecessor.
		*/
		Node* __findPredecessor(Node* node)
		{
			if (node->right)
				return __findPredecessor(node->right);
			return node;
		}

		/**
		* Recursive function used to search for the node containing a specific element.
		* @param element element that is being searched for.
		* @node node current node in recursion.
		* @returns node containing the element or nullptr if no node exists.
		*/
		Node* __findNode(T element, Node* node)
		{
			if (!node)
				return nullptr;
			if (mCompare(element, node->element))
				return __findNode(element, node->left);
			if (mCompare(node->element, element))
				return __findNode(element, node->right);
			else
				return node;
		}
		/**
		* Recursive function to find the parent node of the element if it were to be inserted.
		* @param element element in question.
		* @param node current node.
		* @returns the parent of the element if it were to be inserted. Returns nullptr if tree is empty(pRoot == nullptr).
		*/
		Node* __findParentOfInsertion(T element, Node* node)
		{
			if (!node)
				return nullptr;
			if (mCompare(element, node->element))
			{
				if (!node->left)
					return node;
				else
					return __findParentOfInsertion(element, node->left);
			}
			if (mCompare(node->element, element))
			{
				if (!node->right)
					return node;
				else
					return __findParentOfInsertion(element, node->right);
			}
			else
				return node->parent;
		}
		/**
		* Recursive function to find the first parent that is less than the node.
		* @param node current node in recursion.
		*/
		Node* __findParentLeft(Node* node)
		{
			if (node == pRoot)
				return nullptr;
			if (node == node->parent->right)
				return node->parent;
			else
				return __findParentLeft(node->parent);
		}
		/**
		* Recursive function to find the first parent that is greater than the node.
		* @param node current node in recursion.
		*/
		Node* __findParentRight(Node* node)
		{
			if (node == pRoot)
				return nullptr;
			if (node == node->parent->left)
				return node->parent;
			else
				return __findParentRight(node->parent);
		}

		/**
		* Validates the tree after an insertion has occurred.
		* @param node the newly inserted node.
		*/
		void validateTreePostInsertion(Node* node)
		{
			//Check if parent is red, if so then we need to fix the tree.
			if (!node->parent->isBlack)
			{
				bool parentIsLeft = (node->parent->parent->left == node->parent);
				Node* uncle = (parentIsLeft) ? node->parent->parent->right : node->parent->parent->left;
				if (uncle && !uncle->isBlack)
				{
					uncle->isBlack = true;
					node->parent->isBlack = true;
					if (node->parent->parent != pRoot)
					{
						node->parent->parent->isBlack = false;
						validateTreePostInsertion(node->parent->parent);
					}
				}
				else //Uncle is red
				{
					if (parentIsLeft)
					{
						if (node == node->parent->left)
						{
							node->parent->isBlack = true; //Parent to black
							node->parent->parent->isBlack = false; //Grandparent to red
							LLRotation(node->parent);
						}
						else
						{
							node->isBlack = true; //Node to black
							node->parent->parent->isBlack = false; //Grandparent to red
							LRRotation(node->parent);
							LLRotation(node);
						}
					}
					else
					{
						if (node == node->parent->right)
						{
							node->parent->isBlack = true; //Parent to black
							node->parent->parent->isBlack = false; //Grandparent to red
							RRRotation(node->parent);
						}
						else
						{
							node->isBlack = true; //Node to black
							node->parent->parent->isBlack = false; //Grandparent to red
							RLRotation(node->parent);
							RRRotation(node);
						}
					}
				}
			}
		}

		/**
		* Validates the tree after a deletion has occurred.  
		* This method is only necessary if the deleted node isn't pRoot and the deleted node and the node replacing that node is black
		* (If replacing node is red, we can change it to black.  If deleted node is red, it doesn't change the black height of the tree)
		* @param parent parent node of the deleted node
		* @param isLeft flag whether the deleted node was the left child of the parent.
		*/
		void validatePostDeletion(Node* parent, bool isLeft)
		{
			Node* sibling = ((isLeft) ? parent->right : parent->left);
			if (sibling->isBlack)
			{
				if (sibling->right && !sibling->right->isBlack) //Sibling's right child is red
				{		
					if (isLeft) //Sibling is right child
					{
						sibling->isBlack = sibling->parent->isBlack;
						parent->isBlack = true;
						sibling->right->isBlack = true;
						RRRotation(sibling);
					}
					else
					{
						sibling->right->isBlack = sibling->parent->isBlack;
						parent->isBlack = true;
						LRRotation(sibling);
						LLRotation(sibling->parent);
					}
				}
				else if (sibling->left && !sibling->left->isBlack) //Sibling's left child is red
				{
					if (isLeft) //Sibling is right child
					{
						sibling->left->isBlack = sibling->parent->isBlack;
						sibling->parent->isBlack = true;
						RLRotation(sibling);
						RRRotation(sibling->parent);
					}
					else
					{
						sibling->isBlack = sibling->parent->isBlack;
						parent->isBlack = true;
						sibling->left->isBlack = true;
						LLRotation(sibling);
					}
				}
				else //All children are black/nullptr
				{
					sibling->isBlack = false; //Sibling set to red.
					if (!parent->isBlack) //Is parent is red, then set to black
						parent->isBlack = true;
					else if (parent != pRoot) //If parent is black and not the root, recurse
						validatePostDeletion(parent->parent, parent == parent->parent->left);
				}
			}
			else //Sibling is red
			{
				if (isLeft) //Sibling is right
				{
					parent->isBlack = false;
					sibling->isBlack = true;
					RRRotation(sibling);
					validatePostDeletion(parent, isLeft);
				}
				else //Sibling is left
				{
					parent->isBlack = false;
					sibling->isBlack = true;
					LLRotation(sibling);
					validatePostDeletion(parent, isLeft);
				}
			}
		}
		/**
		* Perform an LL Rotation on the subtree centered at the inputted parent node.
		*
		*						  WHERE ( 1 ) is PARENT
		*					( 0 )						( 1 )
		*					/	\						/	\
		*				 ( 1 ) ( 2 )   ========>	 ( 3 ) ( 0 )
		*				  /		  							  \
		*			    ( 3	)	 							 ( 2 )
		*
		* @param parent the parent node that the rotation will be centered around.
		*/
		void LLRotation(Node* parent)
		{
			Node* grandparent = parent->parent;

			//Give parent's right to grandparent's left
			grandparent->left = parent->right;
			if (parent->right)
				parent->right->parent = grandparent;
			//Set parent right to grandparent
			parent->right = grandparent;
			if (grandparent == pRoot)
			{
				parent->parent = nullptr;
				pRoot = parent;
			}
			else
			{
				if (grandparent->parent->left == grandparent)
					grandparent->parent->left = parent;
				else
					grandparent->parent->right = parent;
				parent->parent = grandparent->parent;
			}
			grandparent->parent = parent;

		}

		/**
		* Perform an RR Rotation on the subtree centered at the inputted parent node.
		*
		*						  WHERE ( 2 ) is PARENT
		*					( 0 )						( 2 )
		*					/	\						/	\
		*				 ( 1 ) ( 2 )   ========>	 ( 0 ) ( 3 )
		*						  \					  /
		*						 ( 3 )			   ( 1 )
		*
		* @param parent the parent node that the rotation will be centered around.
		*/
		void RRRotation(Node* parent)
		{
			Node* grandparent = parent->parent;

			//Give parent's left to grandparent's right
			grandparent->right = parent->left;
			if (parent->left)
				parent->left->parent = grandparent;
			//Set parent right to grandparent
			parent->left = grandparent;
			if (grandparent == pRoot)
			{
				parent->parent = nullptr;
				pRoot = parent;
			}
			else
			{
				if (grandparent->parent->left == grandparent)
					grandparent->parent->left = parent;
				else
					grandparent->parent->right = parent;
				parent->parent = grandparent->parent;
			}
			grandparent->parent = parent;
		}
		/**
		* Perform an LR Rotation on the subtree centered at the inputted parent node.
		*
		*						  WHERE ( 1 ) is PARENT
		*					( 0 )						( 0 )
		*					/	\						/	\
		*				 ( 1 ) ( 2 )   ========>	 ( 3 ) ( 2 )
		*				    \		  				  /
		*				   ( 3 )	 			   ( 1 )		 
		*
		* @param parent the parent node that the rotation will be centered around.
		*/
		void LRRotation(Node* parent)
		{
			Node* childNode = parent->right;

			//Give childNode's left to the parent's right
			parent->right = childNode->left;
			if (childNode->left)
				childNode->left->parent = parent;
			//Child node replaces parent
			parent->parent->left = childNode;
			childNode->parent = parent->parent;

			//Parent is now the left child of the childNode
			childNode->left = parent;
			parent->parent = childNode;
		}
		/**
		* Perform an RL Rotation on the subtree centered at the inputted parent node.
		*
		*						  WHERE ( 2 ) is PARENT
		*					( 0 )						( 0 )
		*					/	\						/	\
		*				 ( 1 ) ( 2 )   ========>	 ( 1 ) ( 3 )
		*						/		  					  \
		*					 ( 3 )	 						 ( 2 )
		*
		* @param parent the parent node that the rotation will be centered around.
		*/
		void RLRotation(Node* parent)
		{
			Node* childNode = parent->left;

			//Give childNode's right to the parent's left
			parent->left = childNode->right;
			if (childNode->right)
				childNode->right->parent = parent;
			//Child node replaces parent
			parent->parent->right = childNode;
			childNode->parent = parent->parent;

			//Parent is now the right child of the childNode
			childNode->right = parent;
			parent->parent = childNode;
		}

	private:
		Node* pRoot = nullptr;
		size_t mSize = 0;
		std::function<bool(const T&, const T&)> mCompare;
};

