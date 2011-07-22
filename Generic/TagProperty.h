#ifndef _GENERIC_TAGPROPERTY_H
#define _GENERIC_TAGPROPERTY_H

#include "Types.h"
#include "DistributorT.h"
#include "HookAttrT.h"
#include "IComparable.h"
#include "CallT.h"

struct TagItem;

/**
 * \file TagProperty.h
 * \brief Designed to allow notification on tag updates. useful to support distribution of changes
 */

namespace GenNS
{

    class Tag : public IComparable
    {
	iptr	key;
	iptr	value;
	bool	changed;

    public:
	Tag(iptr nk, iptr nv)
	{
	    key = nk;
	    value = nv;
	    changed = true;
	}

	inline Tag& operator = (iptr nv)
	{
	    if (value != nv)
	    {
		value = nv;
		changed = true;
	    }
	    return *this;
	}

	inline Tag& operator = (const Tag& oth)
	{
	    *this = oth.value;
	    return *this;
	}

	inline bool operator < (const IComparable& oth) const
	{
	    const Tag& d = static_cast<const Tag&>(oth);
	    return key < d.key;
	}

	inline bool operator == (const IComparable& oth) const
	{
	    const Tag& d = static_cast<const Tag&>(oth);
	    return key == d.key;
	}

	inline operator TagItem() const
	{
	    TagItem i = {key, value};
	    return i;
	}

	inline bool isChanged() const
	{
	    return changed;
	}
    };

    class TagProperty 
    {

    protected:
	VectorT<Tag> tags;
	VectorT< ICall1T<void, const TagItem*>* > listeners;
	int nest_update;

    protected:
	VectorT<const TagItem> *CollectTags(bool update_only);

    public:

	/** 
	 * \brief Create new TagProperty structure\n
	 * \b tags parameter is used to specify which tags will be monitored/accumulated
	 */
	TagProperty(const TagItem* tags);

	/** 
	 * \brief Dispose TagProperty structure
	 */
	~TagProperty();

	/**
	 * \brief Start update session that may include one or more consecutive commits\n
	 * which you don't want to be submitted yet. it is NOT synchronized
	 */
	void BeginUpdate();

	/**
	 * \brief Finish update session and submit all commits to the taglist\n
	 * then broadcast the changes
	 */
	void EndUpdate();

	/** 
	 * \brief Obtain current tags
	 */
	const VectorT<const TagItem> *GetSnapshot();

	/**
	 * \brief Set single tag. Please consider using this function
	 * between 'BeginUpdate' and 'EndUpdate'.\n
	 * See also: Update()
	 */
	TagProperty& operator << (const Tag &t);

	/**
	 * \brief Add tag change listener
	 */
	void AddListener(ICall1T<void, const TagItem*>* hook);

	/**
	 * \brief Remove tag change listener
	 */
	void RemListener(ICall1T<void, const TagItem*>* hook);
    };
};
#endif

