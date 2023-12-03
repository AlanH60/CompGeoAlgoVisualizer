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
#include "Direct2D/Drawable/QuadBezierCurve.h"
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
using D2D::QuadBezierCurve;

App::App()
{
	pVisualizer = new AlgorithmVisualizer(this);
	mVisualizerSpeed = pVisualizer->getSpeed();
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

	//Label for the speed slider
	ILabel* pAlgorithmLabel = new ILabel(L"Select an Algorithm:", D2D::TextFormat(L"Arial", 16, true, D2D::Style::NORMAL,
		D2D::TextAlignment::CENTER, D2D::ParagraphAlignment::CENTER), 0, 0);
	pAlgorithmLabel->setXOrientation(IComponent::XOrientation::CENTER);
	pAlgorithmLabel->setYOrientation(IComponent::YOrientation::RELATIVE_HEIGHT);
	pAlgorithmLabel->setXDimension(IComponent::XDimension::RELATIVEX);
	pAlgorithmLabel->setYDimension(IComponent::YDimension::RELATIVEY);
	pAlgorithmLabel->setRelativeHeightOrientation(0.03f);
	pAlgorithmLabel->setRelativeWidth(1.0f);
	pAlgorithmLabel->setRelativeHeight(0.075f);

	//Dropdown menu used to select a purpose for the algorithm
	pAlgorithmDropDown = new IDropDown(L"Convex Hull", 200, 30);
	pAlgorithmDropDown->addOption(L"Triangulation");
	pAlgorithmDropDown->addOption(L"Voronoi Diagram");
	pAlgorithmDropDown->setXOrientation(IComponent::XOrientation::CENTER);
	pAlgorithmDropDown->setYOrientation(IComponent::YOrientation::RELATIVE_HEIGHT);
	pAlgorithmDropDown->setXDimension(IComponent::XDimension::RELATIVEX);
	pAlgorithmDropDown->setYDimension(IComponent::YDimension::RELATIVEY);
	pAlgorithmDropDown->setRelativeHeightOrientation(0.10f);
	pAlgorithmDropDown->setRelativeWidth(0.8f);
	pAlgorithmDropDown->setRelativeHeight(0.05f);
	pAlgorithmDropDown->setColor(Color{ 0.7f, 0.7f, 0.7f, 1 });

	pAlgorithmTypeDropDown = new IDropDown(L"", 200, 30);
	pAlgorithmTypeDropDown->setXOrientation(IComponent::XOrientation::CENTER);
	pAlgorithmTypeDropDown->setYOrientation(IComponent::YOrientation::RELATIVE_HEIGHT);
	pAlgorithmTypeDropDown->setXDimension(IComponent::XDimension::RELATIVEX);
	pAlgorithmTypeDropDown->setYDimension(IComponent::YDimension::RELATIVEY);
	pAlgorithmTypeDropDown->setRelativeHeightOrientation(0.2f);
	pAlgorithmTypeDropDown->setRelativeWidth(0.8f);
	pAlgorithmTypeDropDown->setRelativeHeight(0.05f);
	pAlgorithmTypeDropDown->setColor(Color{ 0.7f, 0.7f, 0.7f, 1 });
	updateAlgorithmTypeDropdown();

	//Label for the speed slider
	ILabel* pSpeedLabel = new ILabel(L"Speed:", D2D::TextFormat(L"Arial", 16, true, D2D::Style::NORMAL, 
									D2D::TextAlignment::CENTER, D2D::ParagraphAlignment::CENTER), 0, 0);
	pSpeedLabel->setXOrientation(IComponent::XOrientation::CENTER);
	pSpeedLabel->setYOrientation(IComponent::YOrientation::RELATIVE_HEIGHT);
	pSpeedLabel->setXDimension(IComponent::XDimension::RELATIVEX);
	pSpeedLabel->setYDimension(IComponent::YDimension::RELATIVEY);
	pSpeedLabel->setRelativeHeightOrientation(0.325f);
	pSpeedLabel->setRelativeWidth(1.0f);
	pSpeedLabel->setRelativeHeight(0.075f);

	//Slider used to modify the speed of the algorithm visualizer
	ISlider* pSpeedSlider = new ISlider(&mVisualizerSpeed, 1, 10, 0, 0);
	pSpeedSlider->setXOrientation(IComponent::XOrientation::CENTER);
	pSpeedSlider->setYOrientation(IComponent::YOrientation::RELATIVE_HEIGHT);
	pSpeedSlider->setXDimension(IComponent::XDimension::RELATIVEX);
	pSpeedSlider->setYDimension(IComponent::YDimension::RELATIVEY);
	pSpeedSlider->setRelativeHeightOrientation(0.4f);
	pSpeedSlider->setRelativeWidth(0.6f);
	pSpeedSlider->setRelativeHeight(0.04f);

	//Start button used to start the algorithm visualizer
	pStartButton = new IButton(L"Start", 50, 50);
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
			//Unselect selected point if exists
			if (pSelectedPoint)
			{
				if (pSelectedOutline)
					delete pSelectedOutline;
				pSelectedOutline = nullptr;
				pSelectedPoint = nullptr;
			}
			switch (mState)
			{
				case State::CONVEX_HULL:
					if (!startConvexHull())
						return;
					break;
				case State::TRIANGULATE:
					if (!startTriangulation())
						return;
					break;
				case State::VORONOI:
					startVoronoiDiagram();
					break;
			}
		}
		pStartButton->setVisibleFlag(false);
		pClearButton->setVisibleFlag(false);
		pPauseButton->setVisibleFlag(true);
	});

	//Start button used to start the algorithm visualizer
	pPauseButton = new IButton(L"Pause", 50, 50);
	pPauseButton->setColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	pPauseButton->setTextColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	pPauseButton->setXOrientation(IComponent::XOrientation::RIGHT);
	pPauseButton->setYOrientation(IComponent::YOrientation::BOTTOM);
	pPauseButton->setXDimension(IComponent::XDimension::RELATIVEX);
	pPauseButton->setYDimension(IComponent::YDimension::RELATIVEY);
	pPauseButton->setRelativeWidth(0.5f);
	pPauseButton->setRelativeHeight(0.1f);
	pPauseButton->setVisibleFlag(false);
	pPauseButton->setOnClick([this]()-> void {
		if (pVisualizer->shouldPause())
		{
			pPauseButton->setText(L"Pause");
			pVisualizer->setShouldPause(false);
			pPauseButton->setColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}
		else
		{
			pPauseButton->setText(L"Resume");
			pVisualizer->setShouldPause(true);
			pPauseButton->setColor({ 0.3f, 1.0f, 0.3f, 1.0f });
		}
	});
	
	//Clear button used to clear the grid of points and lines
	pClearButton = new IButton(L"Clear", 0, 0);
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
	pMenuPanel->addChild(pPauseButton);
	pMenuPanel->addChild(pSpeedSlider);
	pMenuPanel->addChild(pSpeedLabel);
	pMenuPanel->addChild(pAlgorithmDropDown);
	pMenuPanel->addChild(pAlgorithmTypeDropDown);
	pMenuPanel->addChild(pAlgorithmLabel);

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
		else if (mState == State::VORONOI)
			voronoiDiagramEventHandler(e);


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
		std::vector<Edge> result = pVisualizer->getResult();
		switch (mState)
		{
			case State::CONVEX_HULL:
				for (auto& r : result)
				{
					Line* l = new Line(r.v1, r.v2);
					mHullLines.push_back(l);
				}
				break;
			case State::TRIANGULATE:
				for (auto& r : result)
				{
					Line* l = new Line(r.v1, r.v2);
					mTriangulationLines.push_back(l);
				}
				break;
			case State::VORONOI:
				for (auto& r : result)
				{
					Line* l = new Line(r.v1, r.v2);
					mVoronoiEdges.push_back(l);
				}
				break;
			default:
				break;
		}
		pStartButton->setVisibleFlag(true);
		pClearButton->setVisibleFlag(true);
		pPauseButton->setVisibleFlag(false);
	}
	//Update the speed of the visualizer if UI changed its value.
	pVisualizer->setSpeed(mVisualizerSpeed);
	if (pVisualizer->isIdle())
	{
		//Update the algorithm depending on which drop down option the user chooses.
		if ((int)mState != pAlgorithmDropDown->getSelectedIndex())
		{
			mState = (State)pAlgorithmDropDown->getSelectedIndex();
			updateAlgorithmTypeDropdown();
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
	switch (mState)
	{
		case State::CONVEX_HULL:
			for (auto* pLine : mHullLines)
				pLine->draw();
			break;
		case State::TRIANGULATE:
			for (auto* pLine : mPolygonLines)
				pLine->draw();
			for (auto* pLine : mTriangulationLines)
				pLine->draw();
			break;
		case State::VORONOI:
			for (auto* pLine : mVoronoiEdges)
				pLine->draw();
			break;
		default:
			break;
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
	if (pVisualizer->isRunning() && pVisualizer->shouldVisualize())
	{
		//Block till we're able to draw
		pVisualizer->getDrawMutex().lock();
	
		std::vector<Point*>& points = pVisualizer->getPoints();
		std::vector<Line*>& lines = pVisualizer->getLines();
		std::vector<QuadBezierCurve*>& arcs = pVisualizer->getArcs();
		for (int i = 0; i < lines.size(); i ++)
			lines[i]->draw();
		for (int i = 0; i < points.size(); i ++)
			points[i]->draw();
		for (int i = 0; i < arcs.size(); i ++)
			arcs[i]->draw();

		pVisualizer->getDrawMutex().unlock();
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
	deleteAndClear(mVoronoiEdges);
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

int App::getWidth()
{
	return pWindow->getWidth();
}

int App::getHeight()
{
	return pWindow->getHeight();
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
						if (!pHoveredPoint)
						{
							Point* pPoint = new Point(mousePos);
							if (mPoints.find(mousePos) == mPoints.end())
								mPoints[mousePos] = std::vector<Point*>();
							mPoints[mousePos].push_back(pPoint);
							pSelectedPoint = nullptr;
						}
						else
							pSelectedPoint = pHoveredPoint;
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
							//If the point is the same as the last polygon point, we reject.
							if (mPolygon[mPolygon.size() - 1] != Vector2f{ p->getPos().x, p->getPos().y })
							{
								Line* l = new Line(mPolygon[mPolygon.size() - 1], p->getPos());
								mPolygonLines.push_back(l);
								mPolygon.push_back(Vector2f{ p->getPos().x, p->getPos().y });
							}
						}
						else
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

void App::voronoiDiagramEventHandler(Event& e) 
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
						if (!pHoveredPoint)
						{
							Point* pPoint = new Point(mousePos);
							if (mPoints.find(mousePos) == mPoints.end())
								mPoints[mousePos] = std::vector<Point*>();
							mPoints[mousePos].push_back(pPoint);
							pSelectedPoint = nullptr;
						}
						else
							pSelectedPoint = pHoveredPoint;
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
					mVoronoiAlgorithm = AlgorithmVisualizer::VoronoiDiagramAlgorithm::FORTUNE;
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

bool App::startConvexHull()
{
	deleteAndClear(mHullLines);
	std::vector<Vector2f> points = std::vector<Vector2f>();
	for (auto& a : mPoints)
	{
		for (Drawable* d : a.second)
			points.push_back({ d->getPos().x, d->getPos().y });
	}
	//There must be at least 3 points for a convex hull to exist.
	if (points.size() < 3)
		return false;
	mCHAlgorithm = (AlgorithmVisualizer::ConvexHullAlgorithm)(pAlgorithmTypeDropDown->getSelectedIndex());
	pVisualizer->computeConvexHull(points, mCHAlgorithm);
	return true;
}

bool App::startTriangulation()
{
	deleteAndClear(mTriangulationLines);
	if (isValidPolygon && mPolygon.size() >= 3 && mPolygon[0] == mPolygon[mPolygon.size() - 1])
	{
		if (!isCCW())
		{
			std::reverse(++mPolygon.begin(), --mPolygon.end());
		}
		mPolygon.erase(--mPolygon.end());
		std::unordered_map<Vector2f, std::vector<Vector2f>> edges;
		for (size_t i = 0; i < mPolygon.size() - 1; i++)
		{
			size_t next = (i + 1) % mPolygon.size();
			if (edges.find(mPolygon[i]) == edges.end())
				edges[mPolygon[i]] = std::vector<Vector2f>();
			if (edges.find(mPolygon[next]) == edges.end())
				edges[mPolygon[next]] = std::vector<Vector2f>();
			edges[mPolygon[i]].push_back(mPolygon[next]);
			edges[mPolygon[next]].push_back(mPolygon[i]);
		}
		edges[mPolygon[0]].insert(edges[mPolygon[0]].begin(), mPolygon[mPolygon.size() - 1]);
		edges[mPolygon[mPolygon.size() - 1]].push_back(mPolygon[0]);
		mTriAlgorithm = (AlgorithmVisualizer::TriangulationAlgorithm)(pAlgorithmTypeDropDown->getSelectedIndex());
		pVisualizer->computeTriangulation(mPolygon, edges, mTriAlgorithm);
		mPolygon.push_back(mPolygon[0]);
		return true;
	}
	return false;
}

void App::startVoronoiDiagram()
{
	deleteAndClear(mVoronoiEdges);
	std::vector<Vector2f> points = std::vector<Vector2f>();
	for (auto& a : mPoints)
		for (Drawable* d : a.second)
			points.push_back({ d->getPos().x, d->getPos().y });
	mVoronoiAlgorithm = (AlgorithmVisualizer::VoronoiDiagramAlgorithm)(pAlgorithmTypeDropDown->getSelectedIndex());
	pVisualizer->computeVoronoiDiagram(points, mVoronoiAlgorithm);
}

void App::updateAlgorithmTypeDropdown()
{
	switch (mState) 
	{
		case State::CONVEX_HULL:
			pAlgorithmTypeDropDown->clearOptions(L"Gift Wrapping");
			pAlgorithmTypeDropDown->addOption(L"Graham Scan");
			pAlgorithmTypeDropDown->addOption(L"Quick Hull");
			break;
		case State::TRIANGULATE:
			pAlgorithmTypeDropDown->clearOptions(L"Ear Clipping");
			pAlgorithmTypeDropDown->addOption(L"Sweep");
			break;
		case State::VORONOI:
			pAlgorithmTypeDropDown->clearOptions(L"Fortune");
			break;
	}
}
