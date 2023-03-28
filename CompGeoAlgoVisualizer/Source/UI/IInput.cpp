#include "PCH.h"
#include "IInput.h"
#include "Direct2D/Drawable/Text.h"
#include "Direct2D/Drawable/Rectangle.h"
#include "Direct2D/Drawable/Line.h"
#include "Direct2D/Drawable/Polygon.h"

using namespace D2D;

IInput::IInput(const std::wstring& initialText, const TextFormat& textFormat, float width, float height)
	:
	IComponent(0, 0, width, height),
	mInputString(L""),
	pInitialText(new Text(initialText, textFormat, width - 2 * PADDING, height)),
	pInputBox(new D2D::Rectangle({ 0, 0 }, width, height, 0, (width + height) / 100, true, { 1, 0.9f, 0.85f, 1 })),
	pCaret(new Line({ 0, 0.1f * height }, { 0, 0.8f * height }, Color{ 0, 0, 0, 1 })),
	pInputText(new Text(std::wstring(L""), textFormat, width - 2 * PADDING, height)),
	pSelectionHighlight(new D2D::Rectangle({ 0, 0 }, 0, height, true, { 0, 0.5f, 1 , 0.8f })),
	isFocused(false),
	mCaretIdx(0),
	mSelectionStartIdx(0),
	mCaretCounter(0),
	mOffset(0)
{
	FLOAT2 maskVertices[4] = { FLOAT2{0, 0}, FLOAT2{0, (float)height}, FLOAT2{ pInputText->getWidth(), (float)height}, FLOAT2{pInputText->getWidth(), 0.0f}};
	pMask = new D2D::Polygon(maskVertices, 4, true);
	pInitialText->setOffset({ PADDING, 0 });
	pInitialText->setColor({ 0.7f, 0.7f, 0.7f, 0.7f });
	pInitialText->setTextFormat(TextFormat(textFormat.fontFamily, textFormat.size, textFormat.bold, textFormat.style, TextAlignment::LEFT, ParagraphAlignment::CENTER));
	pInputText->setPos({ PADDING, 0 });
	pInputText->setTextFormat(TextFormat(textFormat.fontFamily, textFormat.size, textFormat.bold, textFormat.style, TextAlignment::LEFT, ParagraphAlignment::CENTER));
	pInputBox->setBorderColor({ 0, 0, 0, 1 });
	pSelectionHighlight->setBorderColor({ 0, 0, 0, 0 });

	mDrawables.push_back(pInputBox);
	mDrawables.push_back(pSelectionHighlight);
	mDrawables.push_back(pInitialText);
	mDrawables.push_back(pCaret);
	mDrawables.push_back(pInputText);
}

IInput::~IInput()
{
	delete pMask;
}


void IInput::setInitialText(std::wstring& text)
{
	pInitialText->setText(text);
}

void IInput::setTextFormat(TextFormat& textFormat)
{
	pInitialText->setTextFormat(textFormat);
	pInputText->setTextFormat(textFormat);
}

std::wstring IInput::getInputString()
{
	return mInputString;
}

void IInput::setCaretPosition(UINT32 pos)
{
	auto caretMetrics = pInputText->getCaretMetrics(pos);
	float left = caretMetrics.left + mOffset + PADDING;
	if (left < PADDING)
		mOffset = -caretMetrics.left;
	else if (left > pInputText->getWidth() + PADDING)
		mOffset = -(caretMetrics.left - pInputText->getWidth());

	pInputText->setOffset({ mOffset + PADDING, 0 });
	pCaret->setOffset({ caretMetrics.left + mOffset + PADDING, caretMetrics.top });
	pCaret->setPoints({ 0, 0 }, { 0, caretMetrics.height });
	mCaretIdx = pos;
}

void IInput::setHighlightPosition(UINT32 pos)
{
	auto caretMetrics = pInputText->getCaretMetrics(pos);
	float left = caretMetrics.left + mOffset + PADDING;
	if (left < PADDING)
		mOffset = -caretMetrics.left;
	else if (left > pInputText->getWidth() + PADDING)
		mOffset = -(caretMetrics.left - pInputText->getWidth());
	pInputText->setOffset({ mOffset, 0 });
	pCaret->setOffset({ caretMetrics.left + mOffset + PADDING, caretMetrics.top });
	pCaret->setPoints({ 0, 0 }, { 0, caretMetrics.height });
	mCaretIdx = pos;
	pSelectionHighlight->setWidth(pCaret->getOffset().x - pSelectionHighlight->getOffset().x);
	pSelectionHighlight->setHeight(pCaret->getP2().y - pCaret->getP1().y);
	pSelectionHighlight->setVisibility(true);
}

void IInput::onPress(float x, float y, MouseEvent& mouseEvent)
{
	if (mouseEvent.isConsumed)
		return;
	mouseEvent.isConsumed = true;
	isFocused = true;
	mCaretCounter = 0;
	float relativeX = x - mPos.x;
	float relativeY = y - mPos.y;
	BOOL isTrailing = false;
	BOOL isInside = false;
	auto metrics = pInputText->getCoordinateMetrics({ (float)relativeX - mOffset - PADDING, (float)relativeY }, &isTrailing, &isInside);
	UINT32 pos = metrics.textPosition;
	if (isTrailing && !isInside)
		pos = pos + 1;
	setCaretPosition(pos);
	moveSelectionToCaret();
}

void IInput::onDrag(float x, float y, MouseEvent& mouseEvent)
{
	float relativeX = x - mPos.x;
	float relativeY = y - mPos.y;
	BOOL isTrailing = false;
	BOOL isInside = false;
	UINT32 pos = pInputText->getCoordinateMetrics({ (float)relativeX - mOffset - PADDING, (float)relativeY }, &isTrailing, &isInside).textPosition;
	if (isTrailing && !isInside)
		pos = pos + 1;
	setHighlightPosition(pos);
}

void IInput::onChar(CharEvent& charEvent)
{
	mCaretCounter = 0;
	switch (charEvent.mChar)
	{
		case '\b':
			if (pSelectionHighlight->getWidth() != 0)
			{
				deleteSelection();
				break;
			}
			if (mCaretIdx != 0)
			{
				mInputString.erase(--mCaretIdx, 1);
				pInputText->setText(mInputString);
				setCaretPosition(mCaretIdx);
				moveSelectionToCaret();
			}
			break;
		default:
			if (charEvent.mChar < 32 || charEvent.mChar > 126)
				break;
			if (pSelectionHighlight->getWidth() != 0)
				deleteSelection();
			mInputString.insert(mCaretIdx++, 1, charEvent.mChar);
			pInputText->setText(mInputString);
			setCaretPosition(mCaretIdx);
			moveSelectionToCaret();
			break;
			
	}
	if (mInputString.length() == 0)
		pInitialText->setVisibility(true);
	else
		pInitialText->setVisibility(false);
	charEvent.isConsumed = true;
}

void IInput::onKeyPress(KeyEvent& keyEvent)
{
	mCaretCounter = 0;
	switch (keyEvent.mKeycode)
	{
		case VK_LEFT:
			if (mCaretIdx != 0)
			{
				if (keyEvent.shiftHeld())
				{
					setCaretPosition(--mCaretIdx);
					setHighlightPosition(mCaretIdx);
					break;
				}
				if (pSelectionHighlight->getWidth() != 0)
					setCaretPosition(std::min(mCaretIdx, mSelectionStartIdx));
				else
					setCaretPosition(--mCaretIdx);
			}
			moveSelectionToCaret();
			break;
		case VK_RIGHT:
			if (mCaretIdx != mInputString.length())
			{
				if (keyEvent.shiftHeld())
				{
					setCaretPosition(++mCaretIdx);
					setHighlightPosition(mCaretIdx);
					break;
				}
				if (pSelectionHighlight->getWidth() != 0)
					setCaretPosition(std::max(mCaretIdx, mSelectionStartIdx));
				else
					setCaretPosition(++mCaretIdx);
			}
			moveSelectionToCaret();
			break;
		case VK_DELETE:
			if (pSelectionHighlight->getWidth() != 0)
			{
				deleteSelection();
				break;
			}
			if (mCaretIdx != mInputString.length())
			{
				mInputString.erase(mCaretIdx, 1);
				pInputText->setText(mInputString);
				setCaretPosition(mCaretIdx);
			}
			break;
		case 'A':
			if (keyEvent.ctrlHeld())
			{
				setCaretPosition(0);
				moveSelectionToCaret();
				setHighlightPosition(mInputString.length());
			}
			break;
	}
	//Reveal the initial text if the input string is empty.
	if (mInputString.length() == 0)
		pInitialText->setVisibility(true);
	else
		pInitialText->setVisibility(false);
	keyEvent.isConsumed = true;
}

void IInput::onFocusLoss()
{
	isFocused = false;
	clearSelection();
}

void IInput::onUpdate(IComponent* parent)
{
	IComponent::onUpdate(parent);
	if (!isFocused || ++mCaretCounter >= 30)
	{
		pCaret->setVisibility(false);
		mCaretCounter %= 60;
	}
	else
		pCaret->setVisibility(true);
}

void IInput::draw(float originX, float originY)
{
	for (Drawable* d : mDrawables)
	{
		d->setPos({ (float)originX + mPos.x, (float)originY + mPos.y });
		if (d == pInputText)
		{
			pMask->pushLayer(*pInputBox, D2D1::Matrix3x2F::Translation((float)(originX + mPos.x) + PADDING, (float)(originY + mPos.y)));
			d->draw();
			pMask->popLayer();
		}
		else
			d->draw();
	}
	
}

void IInput::deleteSelection()
{
	if (pSelectionHighlight->getWidth() == 0)
		return;
	if (mSelectionStartIdx > mCaretIdx)
		mInputString.erase(mCaretIdx, mSelectionStartIdx - mCaretIdx);
	else
	{
		mInputString.erase(mSelectionStartIdx, mCaretIdx - mSelectionStartIdx);
		mCaretIdx = mSelectionStartIdx;
		setCaretPosition(mCaretIdx);
	}
	pInputText->setText(mInputString);
	moveSelectionToCaret();
}

void IInput::clearSelection()
{
	pSelectionHighlight->setVisibility(false);
	pSelectionHighlight->setWidth(0);
}

void IInput::moveSelectionToCaret()
{
	clearSelection();
	mSelectionStartIdx = mCaretIdx;
	pSelectionHighlight->setOffset(pCaret->getOffset());
}

void IInput::updateOffset()
{
	mOffset = std::min(-pInputText->getOverhangMetrics().right, 0.0f);
	pInputText->setOffset({ mOffset + PADDING , 0.0f });
}