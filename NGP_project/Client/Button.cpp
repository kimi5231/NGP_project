#include "pch.h"
#include "Button.h"
#include "BoundingBox.h"

void Button::Render(HDC hdc, HDC srcDC)
{
	GetBoundingBox().Render(hdc, srcDC);
}
