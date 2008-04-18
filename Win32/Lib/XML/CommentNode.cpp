////////////////////////////////////////////////////////////////////////////////
//! \file   CommentNode.cpp
//! \brief  The CommentNode class definition.
//! \author Chris Oldwood

#include "Common.hpp"
#include "CommentNode.hpp"

namespace XML
{

////////////////////////////////////////////////////////////////////////////////
//! Default constructor.

CommentNode::CommentNode()
{
}

////////////////////////////////////////////////////////////////////////////////
//! Construction from a string comment.

CommentNode::CommentNode(const tstring& strComment)
	: m_strComment(strComment)
{
}

////////////////////////////////////////////////////////////////////////////////
//! Destructor.

CommentNode::~CommentNode()
{
}

//namespace XML
}
