#include "PCH.h"
#include "App.h"
#include "Direct2D/Window.h"
#include "Direct2D/Drawable/Drawable.h"
#include "Direct2D/Drawable/Point.h"
#include "Direct2D/Drawable/Line.h"
#include "Direct2D/Drawable/Polygon.h"
App::App()
{
	//**************** Grid Lines ****************//
	for (int i = 1; i <= pWindow->getWidth() / CHUNK_SIZE; i++)
	{
		mGridLines.push_back(new Line(*pGraphics, FLOAT2{ (float)i * CHUNK_SIZE, 0 }, FLOAT2{ (float)i * CHUNK_SIZE, (float)pWindow->getHeight() }, Color{ 0.0f, 0.0f, 0.0f, 0.5f }));
	}
	for (int i = 1; i <= pWindow->getHeight() / CHUNK_SIZE; i++)
	{
		mGridLines.push_back(new Line(*pGraphics, FLOAT2{ 0, (float)i * CHUNK_SIZE }, FLOAT2{ (float)pWindow->getWidth(), (float)i * CHUNK_SIZE }, Color{ 0.0f, 0.0f, 0.0f, 0.5f }));
	}
	//*******************************************//

	pWindow->setOnEvent([this](Event& e)-> void
		{
			if (Event::isMouse(e))
			{
				MouseEvent& mouse = (MouseEvent&)e;
				FLOAT2 mousePos = { mouse.x, mouse.y };
				Drawable* pHoveredDrawable = getDrawable(mousePos);
				switch (mouse.mType)
				{
				case Event::EventType::PRESS:
					switch (mouse.mKeycode)
					{
					case VK_LBUTTON:
						if (pWindow->keyPressed(VK_SHIFT) && pSelectedPoint != nullptr)
						{
							/*if (!onPoint)
								addDrawable(new Line(*pGraphics, pSelectedPoint->getPos(), mousePos));*/
						}
						if (pWindow->keyPressed(VK_SHIFT) || pWindow->keyPressed(VK_CONTROL))
						{
							if (!pHoveredDrawable)
							{
								Point* pPoint = new Point(*pGraphics, mousePos);
								if (mChunks.find(mousePos) == mChunks.end())
									mChunks[mousePos] = std::vector<Drawable*>();
								mChunks[mousePos].push_back(pPoint);
								pSelectedPoint = nullptr;
							}
						}
						else
						{
							if (pHoveredDrawable)
								pSelectedPoint = (Point*)/*Temporary Cast*/pHoveredDrawable;
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
					if (pHoveredDrawable && pSelectedPoint != nullptr && pWindow->lButtonPressed() && pSelectedPoint == pHoveredDrawable)
						mDragging = true;
					if (mDragging)
					{
						if (removeDrawable(pSelectedPoint))
						{
							pSelectedPoint->setPos(mousePos);
							addDrawable(pSelectedPoint);
						}
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
						pSelectedPoint = nullptr;
						break;
					case VK_DELETE:
					case VK_BACK:
						if (pSelectedPoint != nullptr)
						{
							deleteDrawable(pSelectedPoint);
							pSelectedPoint = nullptr;
						}
						break;
				}
			}
		}
		
	});
}

App::~App()
{
	clear();
	for (auto* line : mGridLines)
		delete line;
	mGridLines.clear();
	if (pSelectedOutline)
		delete pSelectedOutline;
}

void App::onDraw()
{
	for (auto* pLine : mGridLines)
		pLine->draw(*pGraphics);
	for (auto& drawables : mChunks)
	{
		for (auto* drawable : drawables.second)
			drawable->draw(*pGraphics);
	}
	if (pSelectedPoint)
	{
		if (pSelectedOutline)
			delete pSelectedOutline;
		FLOAT2 f[4] = { {pSelectedPoint->getPos().x - 15, pSelectedPoint->getPos().y - 15 },
						{pSelectedPoint->getPos().x + 15, pSelectedPoint->getPos().y - 15 },
						{pSelectedPoint->getPos().x + 15, pSelectedPoint->getPos().y + 15 },
						{pSelectedPoint->getPos().x - 15, pSelectedPoint->getPos().y + 15 } };
		pSelectedOutline = new Polygon(*pGraphics, f, 4, false, { 0.0f, 0.5f, 1.0f, 0.8f });
		pSelectedOutline->draw(*pGraphics);
	}
}

void App::clear()
{
	for (auto& drawables : mChunks)
	{
		for (int i = 0; i < drawables.second.size(); i ++)
		{
			Drawable* pDrawable = drawables.second[i];
			delete pDrawable;
		}
		drawables.second.clear();
	}
}

void App::addDrawable(Drawable* pDrawable)
{
	if (mChunks.find(pDrawable->getPos()) == mChunks.end())
		mChunks[pDrawable->getPos()] = std::vector<Drawable*>();
	mChunks[pDrawable->getPos()].push_back(pDrawable);
}

Drawable* App::getDrawable(FLOAT2 pos)
{
	Drawable* d = nullptr;
	for (auto* pDrawable : mChunks[pos])
	{
		FLOAT2 diff = pDrawable->getPos() - pos;
		if (abs(diff.x) <= 20 && abs(diff.y) <= 20)
			d = pDrawable;
	}
	if (!d)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if ((j == 0 && i == 0) || mChunks[pos] == mChunks[pos + FLOAT2{ i * 20.0f, j * 20.0f }])
					continue;
				for (auto* pDrawable : mChunks[pos + FLOAT2{ i * 20.0f, j * 20.0f }])
				{
					FLOAT2 diff = pDrawable->getPos() - pos;
					if (abs(diff.x) <= 20 && abs(diff.y) <= 20)
						d = pDrawable;
				}
			}
		}
	}
	return d;
}

bool App::removeDrawable(Drawable* pDrawable)
{
	std::vector<Drawable*>::iterator it;
	FLOAT2 pos = pDrawable->getPos();
	if ((it = std::find(mChunks[pos].begin(), mChunks[pos].end(), pDrawable)) != mChunks[pos].end())
	{
		mChunks[pos].erase(it);
		return true;
	}
	return false;
}

bool App::deleteDrawable(Drawable* pDrawable)
{
	std::vector<Drawable*>::iterator it;
	FLOAT2 pos = pDrawable->getPos();
	if ((it = std::find(mChunks[pos].begin(), mChunks[pos].end(), pDrawable)) != mChunks[pos].end())
	{ 
		delete *it;
		mChunks[pos].erase(it);
		return true;
	}
	return false;
}
