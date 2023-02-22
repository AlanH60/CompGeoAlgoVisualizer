#include "PCH.h"
#include "IContainer.h"


IContainer::IContainer(float x, float y, float width, float height)
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
	child->setDirtyFlag(true);
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

void IContainer::setAlwaysConsumeEvent(bool alwaysConsume)
{
	bAlwaysConsumeEvent = alwaysConsume;
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
			case MouseEvent::Type::PRESS:
				onPress(mouseEvent.x, mouseEvent.y, mouseEvent);
				break;
			case MouseEvent::Type::MOVE:
				onDrag(mouseEvent.x, mouseEvent.y, mouseEvent);
				onHover(mouseEvent.x, mouseEvent.y, mouseEvent);
				onMove(mouseEvent.x, mouseEvent.y, mouseEvent);
				break;
			case MouseEvent::Type::RELEASE:
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
			case KeyEvent::Type::PRESS:
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


void IContainer::onPress(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	float relativeX = x - mPos.x;
	float relativeY = y - mPos.y;
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
	if (bAlwaysConsumeEvent)
		mouseEvent.isConsumed = true;
}


void IContainer::onHover(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	float relativeX = x - mPos.x;
	float relativeY = y - mPos.y;
	ILink* pCurrChild = pFrontChild;
	while (pCurrChild != nullptr)
	{
		if (pCurrChild->component->inComponent(relativeX, relativeY))
		{
			//If the child is already hovered there is no need to call onHover() again unless the child is a container.
			if (pHovered != pCurrChild->component || dynamic_cast<IContainer*>(pCurrChild->component))
			{
				if (pHovered)
					pHovered->onExit();
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
		pHovered->onExit();
		pHovered = nullptr;
	}
}

void IContainer::onExit()
{
	if (pHovered)
	{
		pHovered->onExit();
		pHovered = nullptr;
	}
}

void IContainer::onMove(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (pHovered)
		pHovered->onMove(x - mPos.x, y - mPos.y, mouseEvent);
	if (!pDragged && pPressed)
		pDragged = pPressed;
	if (bAlwaysConsumeEvent)
		mouseEvent.isConsumed = true;
}

void IContainer::onDrag(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (pDragged)
		pDragged->onDrag(x - mPos.x, y - mPos.y, mouseEvent);
}

void IContainer::onDragRelease(float x, float y, MouseEvent& mouseEvent)
{
	if (pDragged)
		pDragged->onDragRelease(x - mPos.x, y - mPos.y, mouseEvent);
	pDragged = nullptr;
}

void IContainer::onClick(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	if (pPressed)
	{
		float relativeX = x - mPos.x;
		float relativeY = y - mPos.y;
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

void IContainer::onUpdate(IComponent* parent)
{
	bool dirty = isDirty;
	IComponent::onUpdate(parent);
	if (!parent)
		isDirty = false;
	ILink* pCurrChild = pFrontChild;
	while (pCurrChild != nullptr)
	{ 
		pCurrChild->component->setDirtyFlag(pCurrChild->component->getDirtyFlag() || dirty);
		pCurrChild->component->onUpdate(this);
		pCurrChild = pCurrChild->next;
	}
}

void IContainer::draw(float originX, float originY)
{
	IComponent::draw(originX, originY);
	//Draw back to front, so that the front children cover back children
	ILink* pCurrChild = pBackChild;
	while (pCurrChild != nullptr)
	{
		pCurrChild->component->draw(originX + mPos.x, originY + mPos.y);
		pCurrChild = pCurrChild->prev;
	}

}
