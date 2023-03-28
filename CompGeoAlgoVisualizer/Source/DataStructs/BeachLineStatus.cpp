#include "PCH.h"
#include "BeachLineStatus.h"

BeachLineStatus::~BeachLineStatus()
{
	if (pRoot)
		delete pRoot;
}

size_t BeachLineStatus::getSize()
{
	return mSize;
}

double BeachLineStatus::getDirectrix()
{
	return mDirectrix;
}

void BeachLineStatus::insertBefore(Arc* arc, Arc* newArc)
{
	//New arcs are always red
	newArc->isBlack = false;
	//If the arc's left is null, insert the new arc
	if (!arc->left)
	{
		arc->left = newArc;
		newArc->parent = arc;
	}
	else //If arc has a left arc, then insert the new arc as the arc->prev's right child.  arc->prev->right is guaranteed to be nullptr
	{
		arc->prev->right = newArc;
		newArc->parent = arc->prev;
	}

	//Insert the new arc between arc->prev and arc
	newArc->prev = arc->prev;
	if (newArc->prev)
		newArc->prev->next = newArc;
	arc->prev = newArc;
	newArc->next = arc;

	//Increment size of tree and balance the tree
	mSize++;
	validateTreePostInsertion(newArc);
}

void BeachLineStatus::insertAfter(Arc* arc, Arc* newArc)
{
	//New arcs are always red
	newArc->isBlack = false;
	//If the arc's right is null, insert the new arc
	if (!arc->right)
	{
		arc->right = newArc;
		newArc->parent = arc;
	}
	else //If arc has a right arc, then insert the new arc as the arc->next's left child.  arc->next->left is guaranteed to be nullptr
	{
		arc->next->left = newArc;
		newArc->parent = arc->next;
	}

	//Insert the new arc in between arc and arc->next
	newArc->next = arc->next;
	if (newArc->next)
		newArc->next->prev = newArc;
	arc->next = newArc;
	newArc->prev = arc;

	//Increment size of tree and balance the tree
	mSize++;
	validateTreePostInsertion(newArc);
}

bool BeachLineStatus::remove(Arc* arc, std::priority_queue<Event*, std::vector<Event*>, EventCompare>& eventQueue)
{
	bool r =  __remove(arc, eventQueue, true);
	if (r)
		mSize--;
	return r;
}

void BeachLineStatus::setDirectrix(double directrix)
{
	mDirectrix = directrix;
}

BeachLineStatus::Arc* BeachLineStatus::addArc(const Vector2D& site, std::priority_queue<Event*, std::vector<Event*>, EventCompare>& eventQueue)
{
	Arc* newArc = new Arc();
	newArc->face = mVoronoi.getFace(site);
	if (mSize == 0)
	{
		pRoot = newArc;
		newArc->isBlack = true;
		mSize++;
		return newArc;
	}
	Arc* currArc = pRoot;

	Vector2D bounds = {};
	while (currArc)
	{
		bounds = { -10000, 10000 };
		Vector2D intersection;
		//Left edge
		bounds.x = getLeftX(currArc);
		bounds.y = getRightX(currArc);
		if (site.x < bounds.x) //Site is to the left of the arc
			currArc = currArc->left;
		else if (site.x > bounds.y) //Site is to the right of the arc
			currArc = currArc->right;
		else //Site is within the bounds of the current arc.  Divide the current arc into two.
		{
			if (currArc->face->site.y == site.y) //Sites have the same y(Only possible with the first points with the same y coordinate.
			{
				VoronoiDiagram::Vertex* v = mVoronoi.getVertex({ (currArc->face->site.x + site.x) / 2, 10000 });
				VoronoiDiagram::HalfEdge* currEdge = new VoronoiDiagram::HalfEdge(EdgeD());
				VoronoiDiagram::HalfEdge* newEdge = new VoronoiDiagram::HalfEdge(EdgeD());
				mHalfEdgePtrs.push_back(currEdge);
				mHalfEdgePtrs.push_back(newEdge);

				currEdge->v2 = v->v;
				currEdge->head = v;
				newEdge->v1 = v->v;
				newEdge->tail = v;
				v->rep = newEdge;

				newArc->leftEdge = newEdge;
				currArc->rightEdge = currEdge;

				currEdge->left = currArc->face;
				newEdge->left = newArc->face;

				currEdge->twin = newEdge;
				newEdge->twin = currEdge;

				insertAfter(currArc, newArc);
				break;
			}
			else 
			{
				//Split arc will be the right half of current arc.
				Arc* splitArc = new Arc();
				splitArc->face = currArc->face;
				splitArc->rightEdge = currArc->rightEdge;

				VoronoiDiagram::HalfEdge* topEdge = new VoronoiDiagram::HalfEdge(EdgeD());
				VoronoiDiagram::HalfEdge* bottomEdge = new VoronoiDiagram::HalfEdge(EdgeD());
				mHalfEdgePtrs.push_back(topEdge);
				mHalfEdgePtrs.push_back(bottomEdge);

				newArc->leftEdge = bottomEdge;
				newArc->rightEdge = bottomEdge;

				splitArc->leftEdge = topEdge;
				currArc->rightEdge = topEdge;

				bottomEdge->twin = topEdge;
				topEdge->twin = bottomEdge;

				newArc->face->rep = bottomEdge;
				bottomEdge->left = newArc->face;
				topEdge->left = currArc->face;

				//Insert new arcs
				insertAfter(currArc, newArc);
				insertAfter(newArc, splitArc);

				//Update circle events
				if (currArc->prev)
				{
					if (currArc->circleEvent)
						currArc->circleEvent->isValid = false;
					currArc->circleEvent = getCircleEvent(currArc);
					if (currArc->circleEvent)
						eventQueue.push(currArc->circleEvent);
				}
				if (splitArc->next)
				{
					splitArc->circleEvent = getCircleEvent(splitArc);
					if (splitArc->circleEvent)
						eventQueue.push(splitArc->circleEvent);
				}
				break;
			}
		}
	}
	return newArc;
}

void BeachLineStatus::resolveLastArcs()
{
	mDirectrix = -1000;

	Arc* arc = getLeftmostArc();
	if (!arc)
		return;
	while (arc->next)
	{
		double rightX = getRightX(arc);
		Vector2D intersection = { rightX, getY(arc, rightX) };
		VoronoiDiagram::Vertex* vertex = mVoronoi.getVertex(intersection);
		arc->rightEdge->tail = vertex;
		arc->rightEdge->v1 = intersection;
		arc->next->leftEdge->head = vertex;
		arc->next->leftEdge->v2 = intersection;
		vertex->rep = arc->rightEdge;

		if (arc->rightEdge->head)
			mVoronoi.insertHalfEdge(arc->rightEdge);
		if (arc->next->leftEdge->tail)
			mVoronoi.insertHalfEdge(arc->next->leftEdge);
		arc = arc->next;
	}
}

std::vector<EdgeD> BeachLineStatus::getEdges()
{
	std::vector<EdgeD> edges;
	for (auto& pair : mVoronoi.getHalfEdges())
		edges.push_back(pair.first);
	return edges;
}
std::vector<VoronoiDiagram::HalfEdge*>& BeachLineStatus::getHalfEdgePtrs()
{
	return mHalfEdgePtrs;
}


BeachLineStatus::Arc* BeachLineStatus::getLeftmostArc()
{
	Arc* arc = pRoot;
	if (pRoot)
	{
		while (arc->left)
			arc = arc->left;
	}
	return arc;
}

void BeachLineStatus::updateEdges()
{
	Arc* arc = getLeftmostArc();
	if (arc)
	{
		Vector2D rightPoint = {};
		while (arc->next)
		{
			if (arc->prev)
				arc->leftEdge->v2 = rightPoint;
			double right = getRightX(arc);
			rightPoint = { right, getY(arc, right) };
			arc->rightEdge->v1 = rightPoint;
			arc = arc->next;
		}
	}
}

bool BeachLineStatus::__remove(Arc* arc, std::priority_queue<Event*, std::vector<Event*>, EventCompare>& eventQueue, bool dealloc)
{
	//If we're not deallocating the arc, it is still in the beachline.  It is just getting moved around in the tree bc of replacing a deleted arc.
	if (dealloc) 
	{
		double x1 = getRightX(arc);
		double x2 = getLeftX(arc);
		if (!EQUALD(x1, arc->circleEvent->center.x) || !EQUALD(x2, arc->circleEvent->center.x) || EQUALD(arc->face->site.y, arc->circleEvent->point.y)) //If the arc's right boundary(or left) isn't at the center of the circle event, then this circle event is invalid.
			return false;

		VoronoiDiagram::Vertex* vertex = mVoronoi.getVertex(arc->circleEvent->center);

		//Resolve edges
		vertex->rep = arc->rightEdge;
		arc->leftEdge->head = vertex;
		arc->leftEdge->v2 = arc->circleEvent->center;
		arc->rightEdge->tail = vertex;
		arc->rightEdge->v1 = arc->circleEvent->center;

		arc->leftEdge->next = arc->rightEdge;
		arc->rightEdge->prev = arc->leftEdge;

		arc->prev->rightEdge->tail = vertex;
		arc->prev->rightEdge->v1 = arc->circleEvent->center;

		arc->next->leftEdge->head = vertex;
		arc->next->leftEdge->v2 = arc->circleEvent->center;

		//Make sure these edges are complete before inserting into diagram.
		if (arc->leftEdge->tail)
			mVoronoi.insertHalfEdge(arc->leftEdge);
		if (arc->rightEdge->head)
			mVoronoi.insertHalfEdge(arc->rightEdge);
		if (arc->prev->rightEdge->head)
			mVoronoi.insertHalfEdge(arc->prev->rightEdge);
		if (arc->next->leftEdge->tail)
			mVoronoi.insertHalfEdge(arc->next->leftEdge);

		//Create new edges between arc->prev and arc->next
		VoronoiDiagram::HalfEdge* prevEdge = new VoronoiDiagram::HalfEdge(EdgeD());
		VoronoiDiagram::HalfEdge* nextEdge = new VoronoiDiagram::HalfEdge(EdgeD());
		mHalfEdgePtrs.push_back(prevEdge);
		mHalfEdgePtrs.push_back(nextEdge);

		prevEdge->head = vertex;
		prevEdge->v2 = arc->circleEvent->center;
		prevEdge->left = arc->prev->face;

		nextEdge->tail = vertex;
		nextEdge->v1 = arc->circleEvent->center;
		nextEdge->left = arc->next->face;

		prevEdge->twin = nextEdge;
		nextEdge->twin = prevEdge;

		prevEdge->next = arc->prev->rightEdge;
		arc->prev->rightEdge->prev = prevEdge;
		arc->prev->rightEdge = prevEdge;

		nextEdge->prev = arc->next->leftEdge;
		arc->next->leftEdge->next = nextEdge;
		arc->next->leftEdge = nextEdge;

		arc->prev->next = arc->next;
		arc->next->prev = arc->prev;
		//Circle Events
		if (arc->prev->prev && arc->prev->prev->face != arc->face)
		{
			if (arc->prev->circleEvent)
				arc->prev->circleEvent->isValid = false;
			arc->prev->circleEvent = getCircleEvent(arc->prev);
			if (arc->prev->circleEvent)
				eventQueue.push(arc->prev->circleEvent);
		}
		if (arc->next->next && arc->next->next->face != arc->face)
		{
			if (arc->next->circleEvent)
				arc->next->circleEvent->isValid = false;
			arc->next->circleEvent = getCircleEvent(arc->next);
			if (arc->next->circleEvent)
				eventQueue.push(arc->next->circleEvent);
		}
	}


	//DELETE this arc
	if (!arc->left && !arc->right) //Is leaf
	{
		if (arc != pRoot) //Check if this arc has parent
		{
			//Check which of the parent's branch the arc is in
			bool isLeft = (arc->parent->left == arc);
			if (isLeft)
				arc->parent->left = nullptr;
			else
				arc->parent->right = nullptr;
			if (arc->isBlack)
				validatePostDeletion(arc->parent, isLeft);
		}
		else //If no parent then we've deleted the root
			pRoot = nullptr;
		if (dealloc)
		{
			arc->left = nullptr;
			arc->right = nullptr;
			delete arc;
		}
	}
	else if (arc->left && !arc->right) //Has left child but no right
	{
		if (arc != pRoot)
		{
			//Check which of the parent's branch the arc is in
			bool isLeft = (arc->parent->left == arc);
			if (isLeft)
				arc->parent->left = arc->left;
			else
				arc->parent->right = arc->left;
			arc->left->parent = arc->parent;

			if (arc->isBlack != arc->left->isBlack) //Either is red/black
				arc->left->isBlack = true;
			else
				validatePostDeletion(arc->parent, isLeft);
		}
		else
		{
			pRoot = arc->left;
			pRoot->isBlack = true;
		}

		if (dealloc)
		{
			arc->left = nullptr;
			arc->right = nullptr;
			delete arc;
		}
	}
	else if (arc->right && !arc->left) //Has right child but no left
	{
		if (arc != pRoot)
		{
			//Check which of the parent's branch the arc is in
			bool isLeft = (arc->parent->left == arc);
			if (isLeft)
				arc->parent->left = arc->right;
			else
				arc->parent->right = arc->right;
			arc->right->parent = arc->parent;

			if (arc->isBlack != arc->right->isBlack) //Either is red/black
				arc->right->isBlack = true;
			else
				validatePostDeletion(arc->parent, isLeft);
		}
		else
		{
			pRoot = arc->right;
			pRoot->isBlack = true;
		}

		if (dealloc)
		{
			arc->left = nullptr;
			arc->right = nullptr;
			delete arc;
		}
	}
	else
	{
		Arc* successor = findSuccessor(arc);// findSuccessor(arc); //Find successor to replace the arc

		__remove(successor, eventQueue, false); //Extract successor while keeping the tree balanced.

		//Inject successor
		if (arc != pRoot)
		{
			//Check which of the parent's branch the arc is in
			if (arc->parent->right == arc)
			{
				arc->parent->right = successor;
				successor->parent = arc->parent;
			}
			else
			{
				arc->parent->left = successor;
				successor->parent = arc->parent;
			}
		}
		else
			pRoot = successor;

		//Successor takes the children.
		successor->left = arc->left;
		successor->right = arc->right;
		//Arc's left or right child could've been taken away during removal of successor
		if (arc->left)
			arc->left->parent = successor;
		if (arc->right)
			arc->right->parent = successor;

		successor->isBlack = arc->isBlack; //Successor takes the same color, since the tree is already balanced.

		if (dealloc)
		{
			arc->left = nullptr;
			arc->right = nullptr;
			delete arc;
		}
	}
	return true;
}

BeachLineStatus::Arc* BeachLineStatus::findSuccessor(Arc* arc)
{
	if (!arc->right)
		return nullptr;
	return __findSuccessor(arc->right);
}

BeachLineStatus::Arc* BeachLineStatus::__findSuccessor(Arc* arc)
{
	if (arc->left)
		return __findSuccessor(arc->left);
	return arc;
}

BeachLineStatus::Arc* BeachLineStatus::findPredecessor(Arc* arc)
{
	if (!arc->left)
		return nullptr;
	return __findPredecessor(arc->left);
}

BeachLineStatus::Arc* BeachLineStatus::__findPredecessor(Arc* arc)
{
	if (arc->right)
		return __findPredecessor(arc->right);
	return arc;
}

void BeachLineStatus::validateTreePostInsertion(Arc* arc)
{
	//Check if parent is red, if so then we need to fix the tree.
	if (!arc->parent->isBlack)
	{
		bool parentIsLeft = (arc->parent->parent->left == arc->parent);
		Arc* uncle = (parentIsLeft) ? arc->parent->parent->right : arc->parent->parent->left;
		if (uncle && !uncle->isBlack)
		{
			uncle->isBlack = true;
			arc->parent->isBlack = true;
			if (arc->parent->parent != pRoot)
			{
				arc->parent->parent->isBlack = false;
				validateTreePostInsertion(arc->parent->parent);
			}
		}
		else //Uncle is red
		{
			if (parentIsLeft)
			{
				if (arc == arc->parent->left)
				{
					arc->parent->isBlack = true; //Parent to black
					arc->parent->parent->isBlack = false; //Grandparent to red
					LLRotation(arc->parent);
				}
				else
				{
					arc->isBlack = true; //Arc to black
					arc->parent->parent->isBlack = false; //Grandparent to red
					LRRotation(arc->parent);
					LLRotation(arc);
				}
			}
			else
			{
				if (arc == arc->parent->right)
				{
					arc->parent->isBlack = true; //Parent to black
					arc->parent->parent->isBlack = false; //Grandparent to red
					RRRotation(arc->parent);
				}
				else
				{
					arc->isBlack = true; //Arc to black
					arc->parent->parent->isBlack = false; //Grandparent to red
					RLRotation(arc->parent);
					RRRotation(arc);
				}
			}
		}
	}
}

void BeachLineStatus::validatePostDeletion(Arc* parent, bool isLeft)
{
	Arc* sibling = ((isLeft) ? parent->right : parent->left);
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

void BeachLineStatus::LLRotation(Arc* parent)
{
	Arc* grandparent = parent->parent;

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

void BeachLineStatus::RRRotation(Arc* parent)
{
	Arc* grandparent = parent->parent;

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

void BeachLineStatus::LRRotation(Arc* parent)
{
	Arc* childArc = parent->right;

	//Give childArc's left to the parent's right
	parent->right = childArc->left;
	if (childArc->left)
		childArc->left->parent = parent;
	//Child arc replaces parent
	parent->parent->left = childArc;
	childArc->parent = parent->parent;

	//Parent is now the left child of the childArc
	childArc->left = parent;
	parent->parent = childArc;
}

void BeachLineStatus::RLRotation(Arc* parent)
{
	Arc* childArc = parent->left;

	//Give childArc's right to the parent's left
	parent->left = childArc->right;
	if (childArc->right)
		childArc->right->parent = parent;
	//Child arc replaces parent
	parent->parent->right = childArc;
	childArc->parent = parent->parent;

	//Parent is now the right child of the childArc
	childArc->right = parent;
	parent->parent = childArc;
}

Vector2D BeachLineStatus::getArcIntersection(Arc* arc1, Arc* arc2)
{
	Vector2D& site1 = arc1->face->site;
	Vector2D& site2 = arc2->face->site;
	if (site1.y == site2.y) //If the two sites have the same y, their intersection is the middle of their xs'.
		return { (site1.x + site2.x) / 2, std::numeric_limits<double>::quiet_NaN() };

	if (site1.y == mDirectrix) //If the sites have different y but one site is at the directrix.
		return { site1.x, std::numeric_limits<double>::quiet_NaN() };
	if (site2.y == mDirectrix)
		return { site2.x, std::numeric_limits<double>::quiet_NaN() };


	double a1 = 1 / (2 * (site1.y - mDirectrix));
	double a2 = 1 / (2 * (site2.y - mDirectrix));

	double a = (a1 - a2);
	double b = -2 * (a1 * site1.x - a2 * site2.x);
	double c = (site1.x * site1.x * a1 - site2.x * site2.x * a2 + (site1.y - site2.y) / 2);

	if (a == 0) //Linear(1 intersection)
		return { -c / b, std::numeric_limits<double>::quiet_NaN() };

	double sqrtTerm = b * b - 4 * a * c;
	if (sqrtTerm < 0)
		return Vector2D();
	double pmTerm = sqrt(sqrtTerm);
	double x1 = (-b + pmTerm) / (2 * a);
	double x2 = (-b - pmTerm) / (2 * a);

	return { x1, x2 };
}

double BeachLineStatus::getY(Arc* arc, double x)
{
	double p = (arc->face->site.y - mDirectrix) / 2;
	double xDiff = x - arc->face->site.x;
	return (xDiff * xDiff) / (4 * p) + arc->face->site.y - p;
}

BeachLineStatus::Event* BeachLineStatus::getCircleEvent(Arc* arc)
{
	Vector2D v1 = arc->prev->face->site;
	Vector2D v2 = arc->face->site;
	Vector2D v3 = arc->next->face->site;

	//Vector2D dv2 = { v2.x - v1.x, v2.y - v1.y };
	//Vector2D dv3 = { v3.x - v1.x, v3.y - v1.y };

	//Vector2D dv22 = { dv2.x * dv2.x, dv2.y * dv2.y };
	//Vector2D dv32 = { dv3.x * dv3.x, dv3.y * dv3.y };

	//double deno = 2 * (dv2.x * dv3.y - dv3.x * dv2.y);

	//double x = v1.x + (dv22.x * dv3.y - dv32.x * dv2.y + dv22.y * dv3.y - dv32.y * dv2.y) / deno;
	//double y = v1.y + (dv22.x * dv3.x - dv32.x * dv2.x + dv22.y * dv3.x - dv32.y * dv2.x) / -deno;


	double s1 = -(v1.x - v2.x) / (v1.y - v2.y);
	double s2 = -(v2.x - v3.x) / (v2.y - v3.y);

	if (EQUALD(s1,s2)) //Arc sites are collinear.
		return nullptr;

	double x = 0;
	double y = 0;

	if (!isinf(s1) && isinf(s2)) //Arcs 1 and 2 sites don't have the same y-value but arcs 2 and 3 do.
	{
		x = (v2.x + v3.x) / 2;
		y = s1 * (x - (v1.x + v2.x) / 2) + (v1.y + v2.y) / 2;
	}
	else if (!isinf(s2) && isinf(s1)) //Arcs 2 and 3 sites don't have the same y-value but arcs 1 and 2 do.
	{
		x = (v1.x + v2.x) / 2;
		y = s2 * (x - (v2.x + v3.x) / 2) + (v2.y + v3.y) / 2;
	}
	else if (isinf(s1) && isinf(s2))//Arc sites all have the same y-value.
		return nullptr;
	else //normal
	{
		x = ((s1 * v1.x - s2 * v3.x + v3.y - v1.y) / (s1 - s2) + v2.x) / 2;
		y = s1 * (x - (v1.x + v2.x) / 2) + (v1.y + v2.y) / 2;
	}

	double xDiff = v1.x - x;
	double yDiff = v1.y - y;
	double r = sqrt(xDiff * xDiff + yDiff * yDiff);

	if (GREATERD(y - r, mDirectrix)) //Invalid circle event.
		return nullptr;

	Event* e = new Event(arc, { x, y - r }, { x, y });
	return e;
}

double BeachLineStatus::getLeftX(Arc* arc)
{
	if (arc->prev)
	{
		Vector2D intersection = getArcIntersection(arc, arc->prev);

		if (isnan(intersection.y))
			return intersection.x;
		else
		{
			if (arc->face->site.y < arc->prev->face->site.y)
				return std::min(intersection.x, intersection.y);
			else
				return std::max(intersection.x, intersection.y);
		}
	}
	return -10000;
}

double BeachLineStatus::getRightX(Arc* arc)
{
	if (arc->next)
	{
		Vector2D intersection = getArcIntersection(arc, arc->next);
		if (isnan(intersection.y))
			return intersection.x;
		else
		{
			if (arc->face->site.y < arc->next->face->site.y)
				return std::max(intersection.x, intersection.y);
			else
				return std::min(intersection.x, intersection.y);
		}
	}
	return 10000;
}


BeachLineStatus::Arc::~Arc()
{
	if (this->left)
		delete this->left;
	if (this->right)
		delete this->right;
}
