#pragma once
#include "IComponent.h"

struct ILink
{
	ILink* next = nullptr;
	ILink* prev = nullptr;
	IComponent* component;

	ILink(IComponent* component)
		:
		component(component)
	{}
};

class IContainer : public IComponent
{
	public:
		IContainer(int x, int y, int width, int height);
		//Deletes/deallocates all child components.
		virtual ~IContainer();
		/**
		* Adds the child from the container.  Classes overriding this function should call this function.
		* After a child component is added, the container will own the component's pointer and handle its deallocation.
		* @param child pointer to the child component that should be added.
		*/
		virtual void addChild(IComponent* child);
		/**
		* Removes the child from the container.
		* After a child component is removed, the container will not longer own the component's pointer and CAN NOT handle its deallocation.
		* @param child pointer to the child component that should be removed.
		*/
		void removeChild(IComponent* child);
		/**
		* Sets the bAlwaysConsumeEvent flag of the container.
		* @param alwaysConsume the bool the flag is set to.
		*/
		void setAlwaysConsumeEvent(bool alwaysConsume);
		/**
		* Handles events. Used exclusively by the root container to begin probagation of events.
		* Child classes have no need of implementing this function.
		* @param e event to be handled.
		*/
		virtual void onEvent(Event& e) override;
		/**
		* Probagates press to a child. Classes overriding this function should call this function, 
		* unless it has its own implementation of event probagation.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onPress(int x, int y, MouseEvent& mouseEvent) override;
		/**
		* Probagates hover event to a child. Classes overriding this function should call this function,
		* unless it has its own implementation of event probagation.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onHover(int x, int y, MouseEvent& mouseEvent) override;
		/**
		* Calls onExit() for the currently hovered child. Classes overriding this function should call this function,
		* unless it has its own implementation of event probagation.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onExit() override;
		/**
		* Calls onMove() for the currently hovered child. Classes overriding this function should call this function,
		* unless it has its own implementation of event probagation.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onMove(int x, int y, MouseEvent& mouseEvent) override;
		/**
		* Calls onDrag() for the currently dragged child. Classes overriding this function should call this function,
		* unless it has its own implementation of event probagation.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onDrag(int x, int y, MouseEvent& mouseEvent) override;
		/**
		* Calls onDragReleased() for the currently dragged child. Classes overriding this function should call this function,
		* unless it has its own implementation of event probagation.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onDragRelease(int x, int y, MouseEvent& mouseEvent) override;
		/**
		* Calls onClick() for currently pressed child if mouse is currently within that child's bounds. Classes overriding this 
		* function should call this function, unless it has its own implementation of event probagation.
		* @param x x-coordinate of the mouse relative to the parent container.
		* @param y y-coordinate of the mouse relative to the parent container.
		* @param mouseEvent reference to the mouse event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onClick(int x, int y, MouseEvent& mouseEvent) override;
		/**
		* Calls onKeyPress() for the currently focused child. Classes overriding this function should call this function,
		* unless it has its own implementation of event probagation.
		* @param keyEvent reference to the key event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onKeyPress(KeyEvent& keyEvent) override;
		/**
		* Calls onChar() for the currently focused child. Classes overriding this function should call this function,
		* unless it has its own implementation of event probagation.
		* @param charEvent reference to the key event. Will be consumed if bAlwaysConsumeEvent flag is true.
		*/
		virtual void onChar(CharEvent& charEvent) override;
		/**
		* Calls onFocusLoss() for the currently focused child. Classes overriding this function should call this function,
		* unless it has its own implementation of event probagation.
		*/
		virtual void onFocusLoss() override;
		/**
		* Calls onUpdate on its children and probagates isDirty flag from parent to child.
		* @param parent parent of this container.
		*/
		virtual void onUpdate(IComponent* parent) override;
		/**
		* Draws this container and calls draw function of children with updated originX and originY.
		* @param originX absolute x-coordinate of the container's parent
		* @parent originY absolute y-coordinate of the container's parent.
		*/
		virtual void draw(int originX, int originY) override;
	protected:
		ILink* getFrontChild();
		ILink* getBackChild();
	protected:
		//Pointer to the front of the linked list
		ILink* pFrontChild = nullptr;
		//Pointer to the back of the linked list(For backwards traversal when drawing back to front)
		ILink* pBackChild = nullptr;
		//Currently hovered child
		IComponent* pHovered = nullptr;
		//Currently pressed child - for onClick() function.  The component must be both pressed and released.
		IComponent* pPressed = nullptr;
		//Currently dragged child
		IComponent* pDragged = nullptr;
		//Flag indicating whether the front child is being focused on.
		bool isChildFocused;
		//Flag indicating whether it should always consume events that occurs within container.
		bool bAlwaysConsumeEvent = true;

};