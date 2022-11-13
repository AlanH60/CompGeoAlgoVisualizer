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
		IContainer() = default;
		~IContainer();
		ILink* getFrontChild();
		ILink* getBackChild();
		void addChild(IComponent* child);
		virtual void onEvent(Event& e) override
		{
			if (e.isConsumed)
				return;
			ILink* pCurrChild = pFrontChild;
			while (pCurrChild != nullptr)
			{
				pCurrChild->component->onEvent(e);
				pCurrChild = pCurrChild->next;
			}
		}
		virtual void draw(int originX, int originY) override
		{
			IComponent::draw(originX, originY);
			ILink* pCurrChild = pFrontChild;
			while (pCurrChild != nullptr)
			{
				pCurrChild->component->draw(originX + mPosX, originY + mPosY);
				pCurrChild = pCurrChild->next;
			}
				
		}
	private:
		//Pointer to the front of the linked list
		ILink* pFrontChild;
		//Pointer to the back of the linked list(For backwards traversal when drawing back to front)
		ILink* pBackChild;

};