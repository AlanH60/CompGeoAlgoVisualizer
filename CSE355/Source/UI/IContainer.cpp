#include "PCH.h"
#include "IContainer.h"


IContainer::IContainer(int x, int y, int width, int height)
	:
	IComponent(x, y, width, height)
{
}
IContainer::~IContainer()
{
	ILink* pCurrChild = pFrontChild;
	while (pCurrChild != nullptr)
	{
		delete pCurrChild->component;
		ILink* temp = pCurrChild;
		pCurrChild = pCurrChild->next;
		delete temp;
	}
}

ILink* IContainer::getFrontChild()
{
	return pFrontChild;
}

ILink* IContainer::getBackChild()
{
	return pBackChild;
}

void IContainer::addChild(IComponent* child)
{
	ILink* newChild = new ILink(child);
	newChild->next = pFrontChild;
	if (pFrontChild)
		pFrontChild->prev = newChild;
	else
		pBackChild = newChild;
	pFrontChild = newChild;
}

void IContainer::removeChild(IComponent* child)
{
	ILink* node = pFrontChild;
	while (node != nullptr)
	{
		if (node->component == child)
		{
			if (node != pFrontChild)
				node->prev->next = node->next;
			if (node != pBackChild)
				node->next->prev = node->prev;
			if (node == pFrontChild)
				pFrontChild = node->next;
			if (node == pBackChild)
				pBackChild = node->prev;
			delete node;
			break;
		}
		node = node->next;
	}
}

void IContainer::onEvent(Event& e)
{
	if (e.isConsumed)
		return;
	if (e.isMouse())
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
	if (e.isKeyboard())
	{
		KeyEvent& keyEvent = (KeyEvent&)e;
		switch (keyEvent.mType)
		{
		case Event::EventType::PRESS:
			onKeyPress(keyEvent);
			break;

		}
	}
	if (e.isChar())
	{
		CharEvent& charEvent = (CharEvent&)e;
		onChar(charEvent);
	}
}


void IContainer::onPress(int x, int y, MouseEvent& mouseEvent)
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
	isChildFocused = (pCurrChild) ? true : false;
	if (pFrontChild && !isChildFocused || pCurrChild != pFrontChild)
		pFrontChild->component->onFocusLoss();
	if (pCurrChild && pCurrChild != pFrontChild)
	{
		if (pCurrChild == pBackChild)
		{
			pBackChild = pCurrChild->prev;
			pCurrChild->prev->next = nullptr;
		}
		else
		{
			pCurrChild->prev->next = pCurrChild->next;
			pCurrChild->next->prev = pCurrChild->prev;
		}
		pCurrChild->prev = nullptr;
		pCurrChild->next = pFrontChild;
		pFrontChild->prev = pCurrChild;
		pFrontChild = pCurrChild;
	}
}


void IContainer::onHover(int x, int y, MouseEvent& mouseEvent)
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

void IContainer::onExit(MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (pHovered)
	{
		pHovered->onExit(mouseEvent);
		pHovered = nullptr;
	}
}

void IContainer::onMove(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (pHovered)
		pHovered->onMove(x - mPosX, y - mPosY, mouseEvent);
	if (!pDragged && pPressed)
		pDragged = pPressed;
}

void IContainer::onDrag(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (pDragged)
		pDragged->onDrag(x - mPosX, y - mPosY, mouseEvent);
}

void IContainer::onDragRelease(int x, int y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (pDragged)
		pDragged->onDragRelease(x - mPosX, y - mPosY, mouseEvent);
	pDragged = nullptr;
}

void IContainer::onClick(int x, int y, MouseEvent& mouseEvent)
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

void IContainer::onKeyPress(KeyEvent& keyEvent)
{
	if (keyEvent.isConsumed)
		return;
	if (isChildFocused && pFrontChild)
		pFrontChild->component->onKeyPress(keyEvent);
}

void IContainer::onChar(CharEvent& charEvent)
{
	if (charEvent.isConsumed)
		return;
	if (isChildFocused && pFrontChild)
		pFrontChild->component->onChar(charEvent);
}

void IContainer::onFocusLoss()
{
	isChildFocused = false;
	if (pFrontChild)
		pFrontChild->component->onFocusLoss();
}

void IContainer::onUpdate()
{
	ILink* pCurrChild = pFrontChild;
	while (pCurrChild != nullptr)
	{
		pCurrChild->component->onUpdate();
		pCurrChild = pCurrChild->next;
	}
}

void IContainer::draw(int originX, int originY)
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
