#pragma once
namespace D2D
{
	enum class Style : unsigned char
	{
		NORMAL = 0,
		OBLIQUE,
		ITALIC
	};
	enum class TextAlignment : unsigned char
	{
		LEFT = 0,
		RIGHT = 1,
		CENTER = 2
	};
	enum class ParagraphAlignment : unsigned char
	{
		TOP = 0,
		BOTTOM = 1,
		CENTER = 2
	};
	struct TextFormat
	{
		std::wstring fontFamily;
		float size;
		Style style;
		TextAlignment textAlignment;
		ParagraphAlignment paraAlignment;
		bool bold;
		TextFormat(std::wstring fontFamily, float size, bool bold, Style style = Style::NORMAL, TextAlignment textAlignment = TextAlignment::LEFT, ParagraphAlignment paraAlignment = ParagraphAlignment::TOP)
			:
			fontFamily(fontFamily),
			size(size),
			style(style),
			textAlignment(textAlignment),
			paraAlignment(paraAlignment),
			bold(bold)
		{}
	};
}
