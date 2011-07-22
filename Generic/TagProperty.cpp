#include "TagProperty.h"
#include "HookAttrT.h"
#include <libclass/utility.h>
#include <utility/tagitem.h>

using namespace GenNS;
      
TagProperty::TagProperty(const TagItem* tags)
{
    nest_update = 0;

    if (tags == 0)
	return;

    BeginUpdate(); 
    while (tags->ti_Tag)
    {
	*this << Tag(tags->ti_Tag, tags->ti_Data);
	tags++;
    }
    EndUpdate();
}

TagProperty::~TagProperty()
{
}

const VectorT<const TagItem>* TagProperty::GetSnapshot()
{
   return CollectTags(false);
}

void TagProperty::BeginUpdate()
{
    ++nest_update;
}

void TagProperty::EndUpdate()
{
    --nest_update;

    if (!nest_update)
    {
	VectorT<const TagItem> *t = CollectTags(true);
	request(__FILE__, "we want to propagate updates here:\n" __FILE__, "OK", 0);

	delete t;
    }
}

void TagProperty::AddListener(ICall1T<void, const TagItem*>* l)
{
   listeners << l;
   VectorT<const TagItem> *t = CollectTags(false);
   l->Call(t->GetArray());
   delete t;
}

void TagProperty::RemListener(ICall1T<void, const TagItem*>* l)
{
    listeners >> l;
}

