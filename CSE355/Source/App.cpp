#include "PCH.h"
#include "App.h"
#include "Direct2D/Window.h"
#include "Direct2D/Drawable/Point.h"
#include "Direct2D/Drawable/Line.h"
#include "Direct2D/Drawable/Polygon.h"
App::App()
{
	pWindow->setOnEvent([this](Event& e)-> void
	{
		if (Event::isMouse(e))
		{
			MouseEvent& mouse = (MouseEvent&)e;
			FLOAT2 mousePos = { mouse.x, mouse.y };
			bool onPoint = mPoints.find(mousePos) != mPoints.end(); //Mouse is hovering over a point
			switch(mouse.mType)
			{
				case Event::EventType::PRESS:
					switch (mouse.mKeycode)
					{
						case VK_LBUTTON:
							if (pWindow->keyPressed(VK_SHIFT) && pSelectedPoint != nullptr)
							{
								if (!onPoint)
									addDrawable(new Line(*pGraphics, pSelectedPoint->getPos(), mousePos));
							}
							if (pWindow->keyPressed(VK_SHIFT) || pWindow->keyPressed(VK_CONTROL))
							{
								if (!onPoint)
								{
									Point* pPoint = new Point(*pGraphics, mousePos);
									addDrawable(pPoint);
									mPoints.insert({ mousePos , pPoint });
									pSelectedPoint = nullptr;
								}
							}
							else
							{
								if (onPoint)
									pSelectedPoint = mPoints[mousePos];
								else
									pSelectedPoint = nullptr;
							}
							break;
						default:
							break;
					}
					break;
				case Event::EventType::RELEASE:
					if (mDragging)
						mDragging = false;
					break;
				case Event::EventType::MOVE:
					if (onPoint && pSelectedPoint != nullptr && pWindow->lButtonPressed() && pSelectedPoint == mPoints[mousePos])
						mDragging = true;
					if (mDragging)
					{
						mPoints.erase(pSelectedPoint->getPos());
						pSelectedPoint->setPos(mousePos);
						mPoints.insert({ pSelectedPoint->getPos(), pSelectedPoint});
					}
					break;
			}
		}
		if (Event::isKeyboard(e))
		{
			if (Event::isPress(e))
			{
				KeyEvent& key = (KeyEvent&)e;
				switch (key.mKeycode)
				{
					case VK_SPACE:
						clear();
						clearPoints();
						pSelectedPoint = nullptr;
						break;
				}
			}
		}
		
	});
}

App::~App()
{
	if (pSelectedOutline)
		delete pSelectedOutline;
}

void App::onDraw()
{
	if (pSelectedPoint)
	{
		if (pSelectedOutline)
			delete pSelectedOutline;
		FLOAT2 f[4] = { {pSelectedPoint->getPos().x - 10, pSelectedPoint->getPos().y - 10 },
						{pSelectedPoint->getPos().x + 10, pSelectedPoint->getPos().y - 10 },
						{pSelectedPoint->getPos().x + 10, pSelectedPoint->getPos().y + 10 },
						{pSelectedPoint->getPos().x - 10, pSelectedPoint->getPos().y + 10 } };
		pSelectedOutline = new Polygon(*pGraphics, f, 4, false, { 0.0f, 0.0f, 1.0f, 0.75f });
		pSelectedOutline->draw(*pGraphics);
	}
}

void App::clearPoints()
{
	mPoints.clear();
}
