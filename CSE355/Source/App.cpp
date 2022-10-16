#include "PCH.h"
#include "App.h"
#include "Primatives.h"
#include "Algorithms.h"
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
		if (Event::isKeyboard(e))
		{
			KeyEvent& keyEvent = (KeyEvent&)e;
			switch (keyEvent.mKeycode)
			{
				case VK_F1:
					if (mState != CONVEX_HULL)
						clear();
					mState = CONVEX_HULL;
					break;
				case VK_F2:
					if (mState != TRIANGULATE)
						clear();
					mState = TRIANGULATE;
					break;
				case VK_SPACE:
					clear();
					pSelectedPoint = nullptr;
					break;
			}
		}
		if (mState == CONVEX_HULL)
			convexHullEventHandler(e);
		else if (mState == TRIANGULATE)
			triangulateEventHandler(e);
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
	for (auto& drawables : mPoints)
	{
		for (auto* drawable : drawables.second)
			drawable->draw(*pGraphics);
	}
	if (mState == CONVEX_HULL)
		for (auto* pLine : mHullLines)
			pLine->draw(*pGraphics);
	else if (mState == TRIANGULATE)
	{
		for (auto* pLine : mPolygonLines)
			pLine->draw(*pGraphics);
		for (auto* pLine : mTriangulationLines)
			pLine->draw(*pGraphics);
	}

	//Selection box
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
	for (auto& drawables : mPoints)
	{
		for (int i = 0; i < drawables.second.size(); i ++)
		{
			Drawable* pDrawable = drawables.second[i];
			delete pDrawable;
		}
		drawables.second.clear();
	}
	if (!mHullLines.empty())
	{
		for (auto* l : mHullLines)
			delete l;
		mHullLines.clear();
	}
	if (!mPolygonLines.empty())
	{
		for (auto* l : mPolygonLines)
			delete l;
		mPolygonLines.clear();
	}
	if (!mTriangulationLines.empty())
	{
		for (auto l : mTriangulationLines)
			delete l;
		mTriangulationLines.clear();
	}
	mPolygon.clear();
}

void App::addPoint(Point* pPoint)
{
	if (mPoints.find(pPoint->getPos()) == mPoints.end())
		mPoints[pPoint->getPos()] = std::vector<Point*>();
	mPoints[pPoint->getPos()].push_back(pPoint);
}

Point* App::getPoint(FLOAT2 pos)
{
	Point* p = nullptr;
	for (auto* pDrawable : mPoints[pos])
	{
		FLOAT2 diff = pDrawable->getPos() - pos;
		if (abs(diff.x) <= 20 && abs(diff.y) <= 20)
			p = pDrawable;
	}
	//If can't find it in current chunk, check the chunk's neighbors.
	if (!p)
	{
		for (int i = -1; i <= 1; i++)
		{
			for (int j = -1; j <= 1; j++)
			{
				if ((j == 0 && i == 0) || mPoints[pos] == mPoints[pos + FLOAT2{ i * 20.0f, j * 20.0f }])
					continue;
				for (auto* pDrawable : mPoints[pos + FLOAT2{ i * 20.0f, j * 20.0f }])
				{
					FLOAT2 diff = pDrawable->getPos() - pos;
					if (abs(diff.x) <= 20 && abs(diff.y) <= 20)
						p = pDrawable;
				}
			}
		}
	}
	return p;
}

bool App::removePoint(Point* pPoint)
{
	std::vector<Point*>::iterator it;
	FLOAT2 pos = pPoint->getPos();
	if ((it = std::find(mPoints[pos].begin(), mPoints[pos].end(), pPoint)) != mPoints[pos].end())
	{
		mPoints[pos].erase(it);
		return true;
	}
	return false;
}

bool App::deletePoint(Point* pPoint)
{
	std::vector<Point*>::iterator it;
	FLOAT2 pos = pPoint->getPos();
	if ((it = std::find(mPoints[pos].begin(), mPoints[pos].end(), pPoint)) != mPoints[pos].end())
	{ 
		delete *it;
		mPoints[pos].erase(it);
		return true;
	}
	return false;
}

void App::convexHullEventHandler(Event& e)
{
	if (Event::isMouse(e))
	{
		MouseEvent& mouse = (MouseEvent&)e;
		FLOAT2 mousePos = { mouse.x, mouse.y };
		Point* pHoveredPoint = getPoint(mousePos);
		switch (mouse.mType)
		{
			case Event::EventType::PRESS:
				switch (mouse.mKeycode)
				{
					case VK_LBUTTON:
						if (pWindow->keyPressed(VK_SHIFT) || pWindow->keyPressed(VK_CONTROL))
						{
							if (!pHoveredPoint)
							{
								Point* pPoint = new Point(*pGraphics, mousePos);
								if (mPoints.find(mousePos) == mPoints.end())
									mPoints[mousePos] = std::vector<Point*>();
								mPoints[mousePos].push_back(pPoint);
								pSelectedPoint = nullptr;
							}
						}
						else
						{
							if (pHoveredPoint)
								pSelectedPoint = pHoveredPoint;
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
				if (pHoveredPoint && pSelectedPoint != nullptr && pWindow->lButtonPressed() && pSelectedPoint == pHoveredPoint)
					mDragging = true;
				if (mDragging)
				{
					if (removePoint(pSelectedPoint))
					{
						pSelectedPoint->setPos(mousePos);
						addPoint(pSelectedPoint);
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
				case VK_DELETE:
				case VK_BACK:
					if (pSelectedPoint != nullptr)
					{
						deletePoint(pSelectedPoint);
						pSelectedPoint = nullptr;
					}
					break;
				case VK_RETURN:
				{
					if (!mHullLines.empty())
					{
						for (auto* l : mHullLines)
							delete l;
						mHullLines.clear();
					}
					std::vector<Vector2f> points = std::vector<Vector2f>();
					for (auto& a : mPoints)
					{
						for (Drawable* d : a.second)
							points.push_back(*reinterpret_cast<Vector2f*>(&d->getPos()));
					}
					std::vector<Vector2f> hull = convexHullGraham(points);
					for (int i = 0; i < hull.size(); i++)
					{
						Line* l = new Line(*pGraphics, { hull[i].x, hull[i].y }, { hull[(i + 1) % hull.size()].x, hull[(i + 1) % hull.size()].y });
						mHullLines.push_back(l);
					}
				}
					break;
				default:
					break;
			}
		}
	}
}

void App::triangulateEventHandler(Event& e)
{
	if (Event::isMouse(e))
	{
		MouseEvent& mouseEvent = (MouseEvent&)e;
		FLOAT2 mousePos = { mouseEvent.x, mouseEvent.y };

		switch (mouseEvent.mType)
		{
			case Event::EventType::PRESS:
				switch (mouseEvent.mKeycode)
				{
					case VK_LBUTTON:
					{
						Point* p = nullptr;
						if (!(p = getPoint(mousePos)))
						{
							p = new Point(*pGraphics, mousePos);
							addPoint(p);
							if (!mPolygon.empty())
								mPolygonLines.push_back(new Line(*pGraphics, { mPolygon[mPolygon.size() - 1].x, mPolygon[mPolygon.size() - 1].y }, p->getPos()));
							mPolygon.push_back(Vector2f{ p->getPos().x, p->getPos().y });
						}
						else if (!mPolygon.empty())
						{
							Line* l = new Line(*pGraphics, { mPolygon[mPolygon.size() - 1].x, mPolygon[mPolygon.size() - 1].y }, p->getPos());
							mPolygonLines.push_back(l);
						}
					}
						break;
					default:
						break;
				}
		}
	}
	if (Event::isKeyboard(e))
	{
		if (Event::isPress(e))
		{
			KeyEvent& key = (KeyEvent&)e;
			switch (key.mKeycode)
			{
				case VK_RETURN:
				{
					if (!mTriangulationLines.empty())
					{
						for (auto l : mTriangulationLines)
							delete l;
						mTriangulationLines.clear();
					}
					std::vector<std::pair<Vector2f, Vector2f>> diagonals = triangulate(mPolygon);
					for (auto& d : diagonals)
						mTriangulationLines.push_back(new Line(*pGraphics, { d.first.x, d.first.y }, { d.second.x, d.second.y }));
				}
					break;
				default:
					break;
			}
		}
	}
}