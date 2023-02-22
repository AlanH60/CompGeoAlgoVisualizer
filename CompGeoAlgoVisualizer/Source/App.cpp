#include "PCH.h"
#include "App.h"
#include "Primatives.h"
#include "Algorithms.h"
#include "Direct2D/Window.h"
#include "Direct2D/Drawable/Drawable.h"
#include "Direct2D/Drawable/Point.h"
#include "Direct2D/Drawable/Line.h"
#include "Direct2D/Drawable/Polygon.h"
#include "Direct2D/Drawable/Text.h"
#include "UI/IContainer.h"
#include "UI/IButton.h"
#include "UI/ISlider.h"
#include "UI/ILabel.h"
#include "UI/ITabbedPanel.h"
#include "UI/IPanel.h"
#include "UI/IDropDown.h"

using D2D::Drawable;
using D2D::Line;
using D2D::Point;
using D2D::Polygon;
using D2D::Text;

App::App()
{
	pVisualizer = new AlgorithmVisualizer(this);

	//********************************** UI *****************************************//
	pRoot = new IContainer(0, 0, pWindow->getWidth(), pWindow->getHeight());
	pRoot->setAlwaysConsumeEvent(false);
	IPanel* pMenuPanel = new IPanel(0, 0, { 0.95f, 0.95f, 0.95f, 1 });
	pMenuPanel->setXOrientation(IComponent::XOrientation::RIGHT);
	pMenuPanel->setYOrientation(IComponent::YOrientation::BOTTOM);
	pMenuPanel->setXDimension(IComponent::XDimension::RELATIVEX);
	pMenuPanel->setYDimension(IComponent::YDimension::RELATIVEY);
	pMenuPanel->setRelativeHeight(1.0f);
	pMenuPanel->setRelativeWidth(0.25f);

	//Dropdown menu used to select a purpose for the algorithm
	pAlgorithmDropDown = new IDropDown(L"Convex Hull", 200, 30);
	pAlgorithmDropDown->addOption(L"Triangulation");
	pAlgorithmDropDown->setXOrientation(IComponent::XOrientation::CENTER);
	pAlgorithmDropDown->setYOrientation(IComponent::YOrientation::RELATIVE_HEIGHT);
	pAlgorithmDropDown->setXDimension(IComponent::XDimension::RELATIVEX);
	pAlgorithmDropDown->setYDimension(IComponent::YDimension::RELATIVEY);
	pAlgorithmDropDown->setRelativeHeightOrientation(0.05f);
	pAlgorithmDropDown->setRelativeWidth(0.8f);
	pAlgorithmDropDown->setRelativeHeight(0.05f);
	pAlgorithmDropDown->setColor(Color{ 0.7f, 0.7f, 0.7f, 1 });

	//Label for the speed slider
	ILabel* pSpeedLabel = new ILabel(L"Speed:", D2D::TextFormat(L"Arial", 16, true, D2D::Style::NORMAL, 
									D2D::TextAlignment::CENTER, D2D::ParagraphAlignment::CENTER), 0, 0);
	pSpeedLabel->setXOrientation(IComponent::XOrientation::CENTER);
	pSpeedLabel->setYOrientation(IComponent::YOrientation::RELATIVE_HEIGHT);
	pSpeedLabel->setXDimension(IComponent::XDimension::RELATIVEX);
	pSpeedLabel->setYDimension(IComponent::YDimension::RELATIVEY);
	pSpeedLabel->setRelativeHeightOrientation(0.225f);
	pSpeedLabel->setRelativeWidth(1.0f);
	pSpeedLabel->setRelativeHeight(0.075f);

	//Slider used to modify the speed of the algorithm visualizer
	ISlider* pSpeedSlider = new ISlider(pVisualizer->getSpeedPointer(), 1, 10, 0, 0);
	pSpeedSlider->setXOrientation(IComponent::XOrientation::CENTER);
	pSpeedSlider->setYOrientation(IComponent::YOrientation::RELATIVE_HEIGHT);
	pSpeedSlider->setXDimension(IComponent::XDimension::RELATIVEX);
	pSpeedSlider->setYDimension(IComponent::YDimension::RELATIVEY);
	pSpeedSlider->setRelativeHeightOrientation(0.3f);
	pSpeedSlider->setRelativeWidth(0.6f);
	pSpeedSlider->setRelativeHeight(0.04f);

	//Start button used to start the algorithm visualizer
	IButton* pStartButton = new IButton(L"Start", 50, 50);
	pStartButton->setColor({ 0.3f, 1.0f, 0.3f, 1.0f });
	pStartButton->setTextColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	pStartButton->setXOrientation(IComponent::XOrientation::RIGHT);
	pStartButton->setYOrientation(IComponent::YOrientation::BOTTOM);
	pStartButton->setXDimension(IComponent::XDimension::RELATIVEX);
	pStartButton->setYDimension(IComponent::YDimension::RELATIVEY);
	pStartButton->setRelativeWidth(0.5f);
	pStartButton->setRelativeHeight(0.1f);
	pStartButton->setOnClick([this]()-> void {
		if (pVisualizer->isIdle())
		{
			switch (mState)
			{
				case State::CONVEX_HULL:
					startConvexHull();
					break;
				case State::TRIANGULATE:
					startTriangulation();
					break;
			}
		}
	});
	
	//Clear button used to clear the grid of points and lines
	IButton* pClearButton = new IButton(L"Clear", 0, 0);
	pClearButton->setColor({ 0.7f, 0.7f, 0.7f, 1.0f });
	pClearButton->setTextColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	pClearButton->setXOrientation(IComponent::XOrientation::LEFT);
	pClearButton->setYOrientation(IComponent::YOrientation::BOTTOM);
	pClearButton->setXDimension(IComponent::XDimension::RELATIVEX);
	pClearButton->setYDimension(IComponent::YDimension::RELATIVEY);
	pClearButton->setRelativeWidth(0.5f);
	pClearButton->setRelativeHeight(0.1f);
	pClearButton->setOnClick([this]()-> void {
		if (pVisualizer->isIdle())
		{
			clear();
			pSelectedPoint = nullptr;
		}
	});

	pMenuPanel->addChild(pClearButton);
	pMenuPanel->addChild(pStartButton);
	pMenuPanel->addChild(pSpeedSlider);
	pMenuPanel->addChild(pSpeedLabel);
	pMenuPanel->addChild(pAlgorithmDropDown);

	pRoot->addChild(pMenuPanel);
	//*******************************************************************************//
	
	//**************** Grid Lines ****************//
	for (int i = 1; i <= GRID_SIZE; i++)
	{
		mGridLines.push_back(new Line(FLOAT2{ (float)i * CHUNK_SIZE, 0 }, FLOAT2{ (float)i * CHUNK_SIZE, (float)GRID_SIZE * CHUNK_SIZE }, Color{ 0.0f, 0.0f, 0.0f, 0.5f }));
	}
	for (int i = 1; i <= GRID_SIZE; i++)
	{
		mGridLines.push_back(new Line(FLOAT2{ 0, (float)i * CHUNK_SIZE }, FLOAT2{ (float)GRID_SIZE * CHUNK_SIZE, (float)i * CHUNK_SIZE}, Color{0.0f, 0.0f, 0.0f, 0.5f}));
	}
	//*******************************************//

	pWindow->setOnEvent([this](Event& e)-> void
	{
		pRoot->onEvent(e);
		if (e.isConsumed)
			return;
		if (e.isWindow())
		{
			WindowEvent& windowEvent = (WindowEvent&)e;
			switch (windowEvent.mType)
			{
				case WindowEvent::Type::RESIZE:
					pRoot->setWidth(windowEvent.mWindowWidth);
					pRoot->setHeight(windowEvent.mWindowHeight);
					windowEvent.isConsumed = true;
					break;
			}
		}
		if (e.isKeyboard())
		{
			switch (((KeyEvent&)e).mKeycode)
			{
				case VK_UP:
					pVisualizer->addSpeed(0.1f);
					e.isConsumed = true;
					break;
				case VK_DOWN:
					pVisualizer->addSpeed(-0.1f);
					e.isConsumed = true;
					break;
			}
		}
		if (!pVisualizer->isIdle())
			return;
		if (mState == State::CONVEX_HULL)
			convexHullEventHandler(e);
		else if (mState == State::TRIANGULATE)
			triangulateEventHandler(e);
	});
}

App::~App()
{
	delete pRoot;
	clear();
	deleteAndClear(mGridLines);
	if (pSelectedOutline)
		delete pSelectedOutline;
	delete pVisualizer;
}

void App::onUpdate()
{
	pRoot->onUpdate(nullptr);
	if (pVisualizer->isFinished())
	{
		std::vector<std::pair<Vector2f, Vector2f>> result = pVisualizer->getResult();
		if (mState == State::CONVEX_HULL)
			for (auto& r : result)
			{
				Line* l = new Line(r.first, r.second);
				mHullLines.push_back(l);
			}
		else
			for (auto& r : result)
			{
				Line* l = new Line(r.first, r.second);
				mTriangulationLines.push_back(l);
			}
	}
	if (!pVisualizer->isRunning())
	{
		if ((int)mState != pAlgorithmDropDown->getSelectedIndex())
		{
			mState = (State)pAlgorithmDropDown->getSelectedIndex();
			clear();
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
	if (mState == State::CONVEX_HULL)
		for (auto* pLine : mHullLines)
			pLine->draw();
	else if (mState == State::TRIANGULATE)
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
		pSelectedOutline = new D2D::Polygon(f, 4, false, { 0.0f, 0.5f, 1.0f, 0.8f });
		pSelectedOutline->draw();
	}



	//Algorithm Visualizer
	if ((pVisualizer->isRunning() || pVisualizer->isSleeping()) && pVisualizer->shouldVisualize())
	{
		isDrawingAlgo = true;
		//Wait until the algorithm is sleeping to draw.
		while (!pVisualizer->isSleeping()) {}
		std::vector<Point*>& points = pVisualizer->getPoints();
		std::vector<Line*>& lines = pVisualizer->getLines();
		for (Line* l : lines)
			l->draw();
		for (Point* p : points)
			p->draw();
		isDrawingAlgo = false;
	}

	//UI
	pRoot->draw(0, 0);
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
	for (Point* pPoint : mPoints[pos])
	{
		FLOAT2 diff = pPoint->getPos() - pos;
		if (abs(diff.x) <= 20 && abs(diff.y) <= 20)
			p = pPoint;
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
				for (Point* pPoint : mPoints[pos + FLOAT2{ i * 20.0f, j * 20.0f }])
				{
					FLOAT2 diff = pPoint->getPos() - pos;
					if (abs(diff.x) <= 20 && abs(diff.y) <= 20)
						p = pPoint;
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

bool App::isDrawingAlgorithmVisualize()
{
	return isDrawingAlgo;
}

void App::convexHullEventHandler(Event& e)
{
	if (e.isMouse())
	{
		MouseEvent& mouse = (MouseEvent&)e;
		FLOAT2 mousePos = { (float)mouse.x, (float)mouse.y };
		Point* pHoveredPoint = getPoint(mousePos);
		switch (mouse.mType)
		{
			case MouseEvent::Type::PRESS:
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
			case MouseEvent::Type::RELEASE:
				if (mDragging)
					mDragging = false;
				break;
			case MouseEvent::Type::MOVE:
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
	if (e.isKeyboard())
	{
		KeyEvent& key = (KeyEvent&)e;
		if (key.isPress())
		{
			switch (key.mKeycode)
			{
				case '1':
					mCHAlgorithm = AlgorithmVisualizer::ConvexHullAlgorithm::GIFT_WRAPPING;
					e.isConsumed = true;
					break;
				case '2':
					mCHAlgorithm = AlgorithmVisualizer::ConvexHullAlgorithm::GRAHAM_SCAN;
					e.isConsumed = true;
					break;
				case '3':
					mCHAlgorithm = AlgorithmVisualizer::ConvexHullAlgorithm::QUICK_HULL;
					e.isConsumed = true;
					break;
				case VK_DELETE:
				case VK_BACK:
					if (pSelectedPoint != nullptr)
					{
						deletePoint(pSelectedPoint);
						pSelectedPoint = nullptr;
					}
					e.isConsumed = true;
					break;
				default:
					break;
			}
		}
	}
}

void App::triangulateEventHandler(Event& e)
{
	if (e.isMouse())
	{
		MouseEvent& mouseEvent = (MouseEvent&)e;
		FLOAT2 mousePos = { mouseEvent.x, mouseEvent.y };

		switch (mouseEvent.mType)
		{
			case MouseEvent::Type::PRESS:
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
						e.isConsumed = true;
						break;
					default:
						break;
				}
		}
	}
	if (e.isKeyboard())
	{
		KeyEvent& key = (KeyEvent&)e;
		if (key.isPress())
		{
			switch (key.mKeycode)
			{
				case '1':
					mTriAlgorithm = AlgorithmVisualizer::TriangulationAlgorithm::EAR_CLIPPING;
					e.isConsumed = true;
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

bool App::isCCW()
{
	int minY = 0;
	for (int i = 0; i < mPolygon.size() - 1; i++)
	{
		if (mPolygon[i].y < mPolygon[minY].y)
			minY = i;
	}
	return leftOf(mPolygon[minY], mPolygon[(minY + 1) % (mPolygon.size() - 1)], mPolygon[(minY + mPolygon.size() - 2) % (mPolygon.size() - 1)]);
}

void App::startConvexHull()
{
	deleteAndClear(mHullLines);
	std::vector<Vector2f> points = std::vector<Vector2f>();
	for (auto& a : mPoints)
	{
		for (Drawable* d : a.second)
			points.push_back({ d->getPos().x, d->getPos().y });
	}
	pVisualizer->computeConvexHull(points, mCHAlgorithm);
}

void App::startTriangulation()
{
	deleteAndClear(mTriangulationLines);
	if (isValidPolygon && mPolygon.size() >= 3 && mPolygon[0] == mPolygon[mPolygon.size() - 1])
	{
		if (!isCCW())
		{
			std::reverse(++mPolygon.begin(), --mPolygon.end());
		}
		mPolygon.erase(--mPolygon.end());
		std::vector<std::pair<int, int>> a;
		pVisualizer->computeTriangulation(mPolygon, a, mTriAlgorithm);
		mPolygon.push_back(mPolygon[0]);
	}
}
