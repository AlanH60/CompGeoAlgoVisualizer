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
	pVisualizer = new AlgorithmVisualizer();
	//**************** Grid Lines ****************//
	for (int i = 1; i <= pWindow->getWidth() / CHUNK_SIZE; i++)
	{
		mGridLines.push_back(new Line(FLOAT2{ (float)i * CHUNK_SIZE, 0 }, FLOAT2{ (float)i * CHUNK_SIZE, (float)pWindow->getHeight() }, Color{ 0.0f, 0.0f, 0.0f, 0.5f }));
	}
	for (int i = 1; i <= pWindow->getHeight() / CHUNK_SIZE; i++)
	{
		mGridLines.push_back(new Line(FLOAT2{ 0, (float)i * CHUNK_SIZE }, FLOAT2{ (float)pWindow->getWidth(), (float)i * CHUNK_SIZE }, Color{ 0.0f, 0.0f, 0.0f, 0.5f }));
	}
	//*******************************************//

	pWindow->setOnEvent([this](Event& e)-> void
	{
		if (!pVisualizer->isIdle())
			return;
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
	deleteAndClear(mGridLines);
	if (pSelectedOutline)
		delete pSelectedOutline;
	delete pVisualizer;
}

void App::onUpdate()
{
	if (pVisualizer->isFinished())
	{
		std::vector<std::pair<Vector2f, Vector2f>> result = pVisualizer->getResult();
		for (auto& r : result)
		{
			Line* l = new Line(r.first, r.second);
			mHullLines.push_back(l);
		}
	}
}

void App::onDraw()
{
	for (auto* pLine : mGridLines)
		pLine->draw();
	for (auto& drawables : mPoints)
	{
		for (auto* drawable : drawables.second)
			drawable->draw();
	}
	if (mState == CONVEX_HULL)
		for (auto* pLine : mHullLines)
			pLine->draw();
	else if (mState == TRIANGULATE)
	{
		for (auto* pLine : mPolygonLines)
			pLine->draw();
		for (auto* pLine : mTriangulationLines)
			pLine->draw();
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
		pSelectedOutline = new Polygon(f, 4, false, { 0.0f, 0.5f, 1.0f, 0.8f });
		pSelectedOutline->draw();
	}

	//Algorithm Visualizer
	if (pVisualizer->isRunning() && pVisualizer->shouldVisualize())
		for (Drawable* d : pVisualizer->getDrawables())
			d->draw();
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
	deleteAndClear(mHullLines);
	deleteAndClear(mPolygonLines);
	deleteAndClear(mTriangulationLines);
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
								Point* pPoint = new Point(mousePos);
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
					deleteAndClear(mHullLines);
					std::vector<Vector2f> points = std::vector<Vector2f>();
					for (auto& a : mPoints)
					{
						for (Drawable* d : a.second)
							points.push_back(*reinterpret_cast<Vector2f*>(&d->getPos()));
					}
					pVisualizer->computeConvexHull(points, AlgorithmVisualizer::GIFT_WRAPPING);
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
							p = new Point(mousePos);
							addPoint(p);
						}
						if (!mPolygon.empty())
						{
							Line* l = new Line({ mPolygon[mPolygon.size() - 1].x, mPolygon[mPolygon.size() - 1].y }, p->getPos());
							mPolygonLines.push_back(l);
						}
						mPolygon.push_back(Vector2f{ p->getPos().x, p->getPos().y });
						updatePolygonValidity();
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
					deleteAndClear(mTriangulationLines);
					if (isValidPolygon && mPolygon.size() >= 3 && mPolygon[0] == mPolygon[mPolygon.size() - 1])
					{
						mPolygon.erase(--mPolygon.end());
						std::vector<std::pair<Vector2f, Vector2f>> diagonals = triangulate(mPolygon);
						mPolygon.push_back(mPolygon[0]);
						for (auto& d : diagonals)
							mTriangulationLines.push_back(new Line({ d.first.x, d.first.y }, { d.second.x, d.second.y }));
					}
				}
					break;
				default:
					break;
			}
		}
	}
}

void App::updatePolygonValidity()
{
	Vector2f lastVertex = mPolygon[mPolygon.size() - 1];
	for (int i = 0; i < (int)mPolygon.size() - 3; i++)
	{
		if (intersectProp(mPolygon[i], mPolygon[i + 1], mPolygon[mPolygon.size() - 1], lastVertex))
		{
			isValidPolygon = false;
			return;
		}
	}
	isValidPolygon = true;
}
