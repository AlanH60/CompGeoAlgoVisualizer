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
