#include "PCH.h"
#include "App.h"
#include "Direct2D/Window.h"
#include "Direct2D/Drawable/Point.h"
#include "Direct2D/Drawable/Line.h"
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
							if (pWindow->keyPressed(VK_SHIFT) && mSelectedPoint != nullptr)
							{
								if (!onPoint)
									addDrawable(new Line(*pGraphics, mSelectedPoint->getPos(), mousePos));
							}
							if (pWindow->keyPressed(VK_SHIFT) || pWindow->keyPressed(VK_CONTROL))
							{
								if (!onPoint)
								{
									Point* pPoint = new Point(*pGraphics, mousePos);
									addDrawable(pPoint);
									mPoints.insert({ mousePos , pPoint });
									mSelectedPoint = nullptr;
								}
							}
							else
							{
								if (onPoint)
									mSelectedPoint = mPoints[mousePos];
								else
									mSelectedPoint = nullptr;
							}
							break;
						default:
							break;
					}
					break;
				case Event::EventType::RELEASE:
					//switch (mouse.mKeycode)
					//{
					//	case VK_LBUTTON:
					//		mSelectedPoint = nullptr;
					//		break;
					//	default:
					//		break;
					//}
					break;
				case Event::EventType::MOVE:
					if (mSelectedPoint != nullptr && pWindow->lButtonPressed())
					{
						mPoints.erase(mSelectedPoint->getPos());
						mSelectedPoint->setPos(mousePos);
						mPoints.insert({ mousePos, mSelectedPoint });
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
						mSelectedPoint = nullptr;
						break;
				}
			}
		}
		
	});
}

void App::clearPoints()
{
	mPoints.clear();
}
