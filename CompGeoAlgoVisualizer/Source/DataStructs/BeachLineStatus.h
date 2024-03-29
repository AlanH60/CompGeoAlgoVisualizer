#pragma once
#include "VoronoiDiagram.h"

class FortuneEventQueue;

class BeachLineStatus
{
	public:
		struct Arc;
		//An event struct used to represent an event that the algorithm encounters.
		struct Event
		{
			Vector2D point; //Point where the directrix/sweepline should go to.
			Arc* arc; //If it is a circle event, pointer to the arc that should disappear.
			Vector2D center; //If it is a circle event, the center of the circle.
			bool isCircle; //Flag whether this event is a circle event.
			bool isValid = true; //Flag whether this is still a valid circle event.

			Event(const Vector2D& point) : point(point), isCircle(false), arc(nullptr) {}
			Event(Arc* arc, const Vector2D& point, const Vector2D& center) : arc(arc), point(point), center(center), isCircle(true) {}
		};
		//Comparison class used to sort the events in the queue.
		class EventCompare
		{
			public:
				//Return true if v1 has greater y value than v2. X value used as tie breaker.
				bool operator()(const Event* e1, const Event* e2)
				{
					return LESSF(e1->point.y, e2->point.y) || (EQUALF(e1->point.y, e2->point.y) && GREATERF(e1->point.x, e2->point.x));
				}
		};
		//Structure representing both a node in the tree and an arc in the beachline.  
		//arc->left, arc->right, arc->parent are relative to the tree.
		//arc->prev and arc->next are relative to the beachline(left to right, like a linked list)
		struct Arc
		{
			Arc* parent = nullptr; //Parent arc in the tree
			Arc* left = nullptr; //Left child arc in the tree
			Arc* right = nullptr; //Right child arc in the tree
			bool isBlack = false; //Flag whether this arc is black or red- used for balancing

			Arc* prev = nullptr; //Arc to the left of this arc in the beachline
			Arc* next = nullptr; //Arc to the right of this arc in the beachline

			VoronoiDiagram::Face* face = nullptr; //The face that this arc is focused at.
			VoronoiDiagram::HalfEdge* leftEdge = nullptr; //The left edge generated by this arc and its prev arc.
			VoronoiDiagram::HalfEdge* rightEdge = nullptr; //The right edge genereated by this arc and its next arc.
			Event* circleEvent = nullptr; //Its associated circle event.  Its current circle event is always valid.  When this arc encounters a new circle event, the old one is invalidated.

			Arc() = default;
			~Arc();
		};

	public:
		/*
		* @param directrixBegin initial value of the directrix.
		*/
		BeachLineStatus(double directrixBegin) : mDirectrix(directrixBegin) {}
		~BeachLineStatus();
		/**
		* @returns number of elements in the tree.
		*/
		size_t getSize();
		/**
		* @returns directrix of the beachline.
		*/
		double getDirectrix();
		/**
		* Inserts a new arc before a specific arc. The new arc will either be the arc's left child or its prev's right child.
		* @param arc the specific arc that the new arc is supposed to be before.
		* @param newArc the new arc to be inserted.
		*/
		void insertBefore(Arc* arc, Arc* newArc);
		/**
		* Inserts a new arc after a specific arc. The new arc will either be the arc's right child or its next's left child.
		* @param arc the specific arc that the new arc is supposed to be after.
		* @param newArc the new arc to be inserted.
		*/
		void insertAfter(Arc* arc, Arc* newArc);
		/**
		* Remove an arc from the tree. ONLY called if its circle event is triggered.
		* @param arc - arc to be removed.
		* @param eventQueue - reference to the event queue of the algorithm so that new circle events can be pushed.
		* @returns True if element was successfully removed. False if the element doesn't exist in the tree.
		*/
		bool remove(Arc* arc, FortuneEventQueue& eventQueue);

		/**
		* Sets the y coordinate of the directrix/sweepline.
		*/
		void setDirectrix(double directrix);

		/**
		* Adds the arc to the beachline and returns the pointer to the arc.
		* @param site the coordinate of the site.
		* @param eventQueue reference to the event queue used for pushing new circle events.
		* @returns pointer to the new arc.
		*/
		Arc* addArc(const Vector2D& site, FortuneEventQueue& eventQueue);

		/**
		* Resolves the arc intersections that go to infinity at the end of the algorithm.
		*/
		void resolveLastArcs();
		/**
		* Returns the edges that make up the voronoi diagram. Should be called when algorithm is done(After resolve last arcs)
		* @returns vector of voronoi edges.
		*/
		std::vector<EdgeD> getEdges();

		/**
		* Returns the vector of pointers to half edges.  Used for visualization.
		*/
		std::vector<VoronoiDiagram::HalfEdge*>& getHalfEdgePtrs();

		/**
		* Gets the leftmost arc in the tree.
		* @returns leftmost arc in the tree.  nullptr if tree is empty.
		*/
		Arc* getLeftmostArc();
		
		/**
		* Updates all edges in the beach line.  Only used for visualization purposes.
		*/
		void updateEdges();

		/**
		* Gets the interesection between two arcs.
		* @param arc1 pointer to the first arc.
		* @param arc2 pointer to the second arc.
		* @returns the two intersections as a Vector2D.  If there is only 1 intersection, then the second float value is NaN.
		*/
		Vector2D getArcIntersection(Arc* arc1, Arc* arc2);
		/**
		* Given an x-value, this function returns the y-coordinate of the point that lies on (x, y) of the parabolic arc.
		* @param arc pointer to the arc.
		* @param x the x-value in question.
		* @returns y-coordinate of the point on the arc at that given x-coordinate.
		*/
		double getY(Arc* arc, double x);
		/*
		* Returns the x-coordinate of the arc's left bound.
		* @returns x-coordinate of the arc's left bound.
		*/
		double getLeftX(Arc* arc);
		/*
		* Returns the x-coordinate of the arc's right bound.
		* @returns x-coordinate of the arc's right bound.
		*/
		double getRightX(Arc* arc);
	protected:
		/**
		* Recursive function to remove an arc.
		* @param arc - current arc to check for deletion
		* @param eventQueue - reference to the event queue used to insert new circle events.
		* @param dealloc - whether we should deallocate the arc.  Only true when used for circle events.
		* @returns True if element was successfully found and deleted. False if the element doesn't exist in the tree.
		*/
		bool __remove(Arc* arc, FortuneEventQueue& eventQueue, bool dealloc = true);
		/**
		* Finds the leftmost child in the arc's right subtree.
		* @param arc arc in question.
		* @returns arc of the successor or nullptr if inputted arc has no right child.
		*/
		Arc* findSuccessor(Arc* arc);
		/**
		* Recursive function used to find the successor of an arc.
		* @param arc current arc.
		* @returns arc of the successor.
		*/
		Arc* __findSuccessor(Arc* arc);

		/**
		* Finds the rightmost child in the arc's left subtree.
		* @param arc arc in question.
		* @returns arc of the predecessor or nullptr if inputted arc has no left child.
		*/
		Arc* findPredecessor(Arc* arc);
		/**
		* Recursive function used to find the predecessor of an arc.
		* @param arc current arc.
		* @returns arc of the predecessor.
		*/
		Arc* __findPredecessor(Arc* arc);

		/**
		* Validates the tree after an insertion has occurred.
		* @param arc the newly inserted arc.
		*/
		void validateTreePostInsertion(Arc* arc);

		/**
		* Validates the tree after a deletion has occurred.
		* This method is only necessary if the deleted arc isn't pRoot and the deleted arc and the arc replacing that arc is black
		* (If replacing arc is red, we can change it to black.  If deleted arc is red, it doesn't change the black height of the tree)
		* @param parent parent arc of the deleted arc
		* @param isLeft flag whether the deleted arc was the left child of the parent.
		*/
		void validatePostDeletion(Arc* parent, bool isLeft);
		/**
		* Perform an LL Rotation on the subtree centered at the inputted parent arc.
		*
		*						  WHERE ( 1 ) is PARENT
		*					( 0 )						( 1 )
		*					/	\						/	\
		*				 ( 1 ) ( 2 )   ========>	 ( 3 ) ( 0 )
		*				  /		  							  \
		*			    ( 3	)	 							 ( 2 )
		*
		* @param parent the parent arc that the rotation will be centered around.
		*/
		void LLRotation(Arc* parent);

		/**
		* Perform an RR Rotation on the subtree centered at the inputted parent arc.
		*
		*						  WHERE ( 2 ) is PARENT
		*					( 0 )						( 2 )
		*					/	\						/	\
		*				 ( 1 ) ( 2 )   ========>	 ( 0 ) ( 3 )
		*						  \					  /
		*						 ( 3 )			   ( 1 )
		*
		* @param parent the parent arc that the rotation will be centered around.
		*/
		void RRRotation(Arc* parent);
		/**
		* Perform an LR Rotation on the subtree centered at the inputted parent arc.
		*
		*						  WHERE ( 1 ) is PARENT
		*					( 0 )						( 0 )
		*					/	\						/	\
		*				 ( 1 ) ( 2 )   ========>	 ( 3 ) ( 2 )
		*				    \		  				  /
		*				   ( 3 )	 			   ( 1 )
		*
		* @param parent the parent arc that the rotation will be centered around.
		*/
		void LRRotation(Arc* parent);
		/**
		* Perform an RL Rotation on the subtree centered at the inputted parent arc.
		*
		*						  WHERE ( 2 ) is PARENT
		*					( 0 )						( 0 )
		*					/	\						/	\
		*				 ( 1 ) ( 2 )   ========>	 ( 1 ) ( 3 )
		*						/		  					  \
		*					 ( 3 )	 						 ( 2 )
		*
		* @param parent the parent arc that the rotation will be centered around.
		*/
		void RLRotation(Arc* parent);
		/**
		* Given an arc, allocates a NEW circle event for the arc.
		* @param arc pointer to the arc.
		* @returns pointer to the circle event.  nullptr if no valid circle event exists for the arc.
		*/
		Event* getCircleEvent(Arc* arc);

	private:
		//Root of the tree.
		Arc* pRoot = nullptr;
		//Number of arcs in the beachline.
		size_t mSize = 0;
		//Y-coordinate of directrix/sweepline.
		double mDirectrix;
		//VoronoiDiagram that this beach line will build.
		VoronoiDiagram mVoronoi;
		//Vector of pointers to half edges.  Used for visualization.
		std::vector<VoronoiDiagram::HalfEdge*> mHalfEdgePtrs;
};

