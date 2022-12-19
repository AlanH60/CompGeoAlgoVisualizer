#pragma once
#include "IComponent.h"

namespace D2D
{
	class Rectangle;
	class Line;
	class Text;
	class DynamicText;
	class Polygon;
	struct TextFormat;
}

class IInput : public IComponent
{
	private:
		static const int PADDING = 5;
	public:
		IInput(std::wstring text, D2D::TextFormat& textFormat, int width, int height);
		~IInput();
		void onPress(int x, int y, MouseEvent& mouseEvent) override;
		void onDrag(int x, int y, MouseEvent& mouseEvent) override;
		void onChar(CharEvent& charEvent) override;
		void onKeyPress(KeyEvent& keyEvent) override;
		void onFocusLoss() override;
		void onUpdate() override;
		void draw(int originX, int originY) override;

		void setInitialText(std::wstring& text);
		void setTextFormat(D2D::TextFormat& textFormat);

		std::wstring getInputString();
	private:
		//Sets the caret position to this pos/index.  The caret will be placed before the character at the inputted pos/index.
		void setCaretPosition(UINT32 pos);
		//Expands the highlighted position from its initial position to the inputted position.
		void setHighlightPosition(UINT32 pos);
		//Deletes the selected/highlighted text and updates the caret position accordingly.
		void deleteSelection();
		//Clear selected area
		void clearSelection();
		//Clears the selected area and moves its start to the caret position
		void moveSelectionToCaret();
		//Updates the offset of the text
		void updateOffset();

	private:
		//Initial Text(If no text has been inputted)
		D2D::Text* pInitialText;
		//Rectangle that holds the input
		D2D::Rectangle* pInputBox;
		//Input text displaying mInputString
		D2D::Text* pInputText;
		//Line that follows the user's position within the input string.
		D2D::Line* pCaret;
		//Rectangle that displays the blue highlighted area when the user drags.
		D2D::Rectangle* pSelectionHighlight;
		//String that the user has inputted
		std::wstring mInputString;
		//Index of the caret position
		size_t mCaretIdx;
		//Index where the user initiated a drag.
		size_t mSelectionStartIdx;
		//Number of pixels that has been offseted due to overflow.
		float mOffset;
		//Counter used to "animate" the caret.(Have it blink)
		int mCaretCounter;
		//Flag that indicates whether this input is in focus.
		bool isFocused;
		//Mask geometry
		D2D::Polygon* pMask;
		
};

