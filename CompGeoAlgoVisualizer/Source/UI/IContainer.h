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
		virtual ~IContainer();
		ILink* getFrontChild();
		ILink* getBackChild();
		virtual void addChild(IComponent* child);
		void removeChild(IComponent* child);
		void setAlwaysConsumeEvent(bool alwaysConsume);
		virtual void onEvent(Event& e) override;
		//Probagates press to a child - this function should be called by any function overriding this, unless it has its own method of probagation.
		virtual void onPress(int x, int y, MouseEvent& mouseEvent) override;
		//Probagates hover to a child - this function should be called in any function overriding this, unless it has its own method of probagation
		virtual void onHover(int x, int y, MouseEvent& mouseEvent) override;
		//Default on exit - this function should be called in any function overriding this, unless it has its own method of probagation
		virtual void onExit() override;
		virtual void onMove(int x, int y, MouseEvent& mouseEvent) override;
		virtual void onDrag(int x, int y, MouseEvent& mouseEvent) override;
		virtual void onDragRelease(int x, int y, MouseEvent& mouseEvent) override;
		//If the pressed component is the same as the released component, then a click has occurred. - this function should be called in any function overriding this.
		virtual void onClick(int x, int y, MouseEvent& mouseEvent) override;
		virtual void onKeyPress(KeyEvent& keyEvent) override;
		virtual void onChar(CharEvent& charEvent) override;
		virtual void onFocusLoss() override;
		virtual void onUpdate(IComponent* parent) override;
		virtual void draw(int originX, int originY) override;
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