////////////////////////////////////////////////////////////////////////////////
//! \file   TheView.cpp
//! \brief  The TheView class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "TheView.hpp"
#include "TheDoc.hpp"

////////////////////////////////////////////////////////////////////////////////
//! Constructor.

TheView::TheView(TheDoc& doc)
	: CView(doc)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

TheView::~TheView()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle window creation.

void TheView::OnCreate(const CRect& /*clientRect*/)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Handle window resizing.

void TheView::OnResize(int /*flag*/, const CSize& /*newSize*/)
{
}
