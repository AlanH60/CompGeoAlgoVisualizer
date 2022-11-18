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
		~IContainer();
		ILink* getFrontChild();
		ILink* getBackChild();
		void addChild(IComponent* child);
		virtual void onEvent(Event& e) override
		{
			if (e.isConsumed)
				return;
			if (Event::isMouse(e))
			{
				MouseEvent& mouseEvent = (MouseEvent&)e;
				switch (mouseEvent.mType)
				{
					case Event::EventType::PRESS:
						onPress(mouseEvent.x, mouseEvent.y, mouseEvent);
						break;
					case Event::EventType::MOVE:
						onHover(mouseEvent.x, mouseEvent.y, mouseEvent);
						onMove(mouseEvent.x, mouseEvent.y, mouseEvent);
						onDrag(mouseEvent.x, mouseEvent.y, mouseEvent);
						break;
					case Event::EventType::RELEASE:
						onClick(mouseEvent.x, mouseEvent.y, mouseEvent);
						onDragRelease(mouseEvent.x, mouseEvent.y, mouseEvent);
						break;
				}
			}
			ILink* pCurrChild = pFrontChild;
			while (pCurrChild != nullptr)
			{
				pCurrChild->component->onEvent(e);
				pCurrChild = pCurrChild->next;
			}
		}
		//Probagates press to a child - this function should be called by any function overriding this, unless it has its own method of probagation.
		virtual void onPress(int x, int y, MouseEvent& mouseEvent) override
		{
			if (mouseEvent.isConsumed)
				return;
			int relativeX = x - mPosX;
			int relativeY = y - mPosY;
			ILink* pCurrChild = pFrontChild;
			while (pCurrChild != nullptr)
			{
				if (pCurrChild->component->inComponent(relativeX, relativeY))
				{
					pPressed = pCurrChild->component;
					pCurrChild->component->onPress(relativeX, relativeY, mouseEvent);
					break;
				}
				pCurrChild = pCurrChild->next;
			}
		}
		//Probagates hover to a child - this function should be called in any function overriding this, unless it has its own method of probagation
		virtual void onHover(int x, int y, MouseEvent& mouseEvent) override
		{
			if (mouseEvent.isConsumed)
				return;
			int relativeX = x - mPosX;
			int relativeY = y - mPosY;
			ILink* pCurrChild = pFrontChild;
			while (pCurrChild != nullptr)
			{
				if (pCurrChild->component->inComponent(relativeX, relativeY))
				{
					if (pHovered != pCurrChild->component)
					{
						if (pHovered)
							pHovered->onExit(mouseEvent);
						pHovered = pCurrChild->component;
						pHovered->onHover(relativeX, relativeY, mouseEvent);
					}
					break;
				}
				pCurrChild = pCurrChild->next;
			}
			//None of its children are hovered.
			if (pCurrChild == nullptr && pHovered != nullptr)
			{
				pHovered->onExit(mouseEvent);
				pHovered = nullptr;
			}

		}
		//Default on exit - this function should be called in any function overriding this, unless it has its own method of probagation
		virtual void onExit(MouseEvent& mouseEvent) override
		{
			if (mouseEvent.isConsumed)
				return;
			if (pHovered)
			{
				pHovered->onExit(mouseEvent);
				pHovered = nullptr;
			}
		}

		virtual void onMove(int x, int y, MouseEvent& mouseEvent) override
		{
			if (mouseEvent.isConsumed)
				return;
			if (pHovered)
				pHovered->onMove(x - mPosX, y - mPosY, mouseEvent);
			if (!pDragged && pPressed)
				pDragged = pPressed;
		}

		virtual void onDrag(int x, int y, MouseEvent& mouseEvent) override
		{
			if (mouseEvent.isConsumed)
				return;
			if (pDragged)
				pDragged->onDrag(x - mPosX, y - mPosY, mouseEvent);
		}

		virtual void onDragRelease(int x, int y, MouseEvent& mouseEvent) override
		{
			if (mouseEvent.isConsumed)
				return;
			if (pDragged)
				pDragged->onDragRelease(x - mPosX, y - mPosY, mouseEvent);
			pDragged = nullptr;
		}

		//If the pressed component is the same as the released component, then a click has occurred. - this function should be called in any function overriding this.
		virtual void onClick(int x, int y, MouseEvent& mouseEvent) override
		{
			if (mouseEvent.isConsumed)
				return;
			if (pPressed)
			{
				int relativeX = x - mPosX;
				int relativeY = y - mPosY;
				if (pPressed->inComponent(relativeX, relativeY))
					pPressed->onClick(relativeX, relativeY, mouseEvent);
				pPressed = nullptr;
			}
		}

		virtual void draw(int originX, int originY) override
		{
			IComponent::draw(originX, originY);
			//Draw back to front, so that the front children cover back children
			ILink* pCurrChild = pBackChild;
			while (pCurrChild != nullptr)
			{
				pCurrChild->component->draw(originX + mPosX, originY + mPosY);
				pCurrChild = pCurrChild->prev;
			}
				
		}
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

};