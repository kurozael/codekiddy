/*
**  ClanLib SDK
**  Copyright (c) 1997-2011 The ClanLib Team
**
**  This software is provided 'as-is', without any express or implied
**  warranty.  In no event will the authors be held liable for any damages
**  arising from the use of this software.
**
**  Permission is granted to anyone to use this software for any purpose,
**  including commercial applications, and to alter it and redistribute it
**  freely, subject to the following restrictions:
**
**  1. The origin of this software must not be misrepresented; you must not
**     claim that you wrote the original software. If you use this software
**     in a product, an acknowledgment in the product documentation would be
**     appreciated but is not required.
**  2. Altered source versions must be plainly marked as such, and must not be
**     misrepresented as being the original software.
**  3. This notice may not be removed or altered from any source distribution.
**
**  Note: Some of the libraries ClanLib may link to may have additional
**  requirements or restrictions.
**
**  File Author(s):
**
**    Kenneth Gangstoe
*/

/// \addtogroup clanDisplay_2D clanDisplay 2D
/// \{

#pragma once

#include "../api_core.h"
#include "../System/sharedptr.h"
#include "rect.h"

class CL_Size;
class CL_RectPacker_Impl;

/// \brief Generic rect packer class. Implements an algorithm to pack rects into groups efficiently.
///
/// \xmlonly !group=Display/2D! !header=display.h! \endxmlonly
class CL_API_CORE CL_RectPacker
{
/// \name Enumerations and structs
/// \{
public:
	/// \brief Allocation policy.
	enum AllocationPolicy
	{
		create_new_group,
		search_previous_groups,
		fail_if_full
	};

	struct AllocatedRect
	{
	public:
		AllocatedRect(int group_index, CL_Rect rect) : group_index(group_index), rect(rect) {}
		int group_index;
		CL_Rect rect;
	};

/// \}
/// \name Construction
/// \{
public:
	/// \brief Constructs a null instance.
	CL_RectPacker();

	/// \brief Constructs a rect group.
	CL_RectPacker(const CL_Size &max_group_size, AllocationPolicy policy = create_new_group);

	~CL_RectPacker();

/// \}
/// \name Attributes
/// \{
public:
	/// \brief Returns true if this object is invalid.
	bool is_null() const { return !impl; }

	/// \brief Throw an exception if this object is invalid.
	void throw_if_null() const;

	/// \brief Returns the allocation policy.
	AllocationPolicy get_allocation_policy() const;

	/// \brief Returns the max group size.
	CL_Size get_max_group_size() const;

	/// \brief Returns the total amount of rects.
	int get_total_rect_count() const;

	/// \brief Returns the amount of rects in a group.
	int get_rect_count(unsigned int group_index = 0) const;

	/// \brief Returns the amount of rects used by group.
	int get_group_count() const;

/// \}
/// \name Operations
/// \{
public:
	/// \brief Set the allocation policy.
	void set_allocation_policy(AllocationPolicy policy);

	/// \brief Allocate space for another rect.
	AllocatedRect add(const CL_Size &size);

/// \}
/// \name Implementation
/// \{
private:
	CL_SharedPtr<CL_RectPacker_Impl> impl;
/// \}
};

/// \}
