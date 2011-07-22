#include "Locale.h"
#include <libclass/locale.h>
#include "Types.h"
#include "String.h"
#include <libraries/locale.h>
#include <libclass/dos.h>
#include <dos/dos.h>

using namespace GenNS;

DummyLocalization::DummyLocalization(ILocalization::LocaleSet set[], const char* grp) :
    deflt("*** Undefined ***", "NO_GROUP", "NO_ID"),
    hash(&deflt)
{
    if (set != 0)
	AddGroup(set, grp);
}

DummyLocalization::~DummyLocalization()
{
    for (uint32 i=0; i<hash.Count(); i++)
    {
	delete hash.GetValAt(i);
    }
}

void DummyLocalization::AddItem(uint32 key, const char* text, const char* id, const char* group)
{
    locale_set *ls = new locale_set("", group, id);
    ls->id  = id;
    split(text, ls->accel[0], ls->str);
    hash.Add(key, ls);
}

void DummyLocalization::AddGroup(ILocalization::LocaleSet set[], const char* grp)
{
    int i = 0;
    if (set == 0)
	return;

    while (set[i].key != (uint32)ILocalization::LocaleSet::Set_Last)
    {
	AddItem(set[i].key, set[i].value, set[i].locale_id, grp);
	++i;
    }
}

const String& DummyLocalization::operator[] (uint32 val) const
{
    return hash.GetVal(val)->str;
}

const String& DummyLocalization::Str(uint32 key) const
{
    return (*this)[key];
}

void DummyLocalization::split(String s, char &accelerator, String &text)
{
    if (s[1] == '&')
    {
	accelerator = s[0];
	text = s.SubString(2, -1);
    }
    else
    {
	accelerator = '\0';
	text = s;
    }
}

const char DummyLocalization::Accel(uint32 key) const
{
    return hash.GetVal(key)->accel[0];
}

const char* DummyLocalization::Shortcut(uint32 key) const
{
    return (const char*)(&hash.GetVal(key)->accel);
}

String DummyLocalization::FormatNumber(uint32 integer, uint32 millionth) const
{
    String s;
    if (millionth != 0)
	s.FormatStr("%ld.%06ld", ARRAY(integer, millionth));
    else
	s.FormatStr("%ld", ARRAY(integer));
    return s;
}

/******************************************************************************/
Localization::Localization(ILocalization::LocaleSet set[], const char* grp) :
    DummyLocalization(set, grp)
{
    locale = LocaleIFace::GetInstance(0);
    lang = 0;

    if (locale != 0)
	lang = locale->OpenLocale(0);
}

Localization::~Localization()
{
    if (lang != 0)
	locale->CloseLocale(lang);

    locale->FreeInstance();
}

bool Localization::ReadCatalog(const char* name, uint32 version)
{
    LocaleIFace *locale = LocaleIFace::GetInstance(0);
    bool res = false;

    if (locale != 0)
    {
	Catalog *cat = locale->OpenCatalogA(0, name, TAGARRAY(OC_Version, version, TAG_DONE, 0));
	if (cat != 0)
	{
	    res = true;
	    for (uint32 i=0; i<hash.Count(); i++)
	    {
		uint32 k = hash.GetKeyAt(i);
		const char *t = locale->GetCatalogStr(cat, k, 0);
		if (t == 0)
		    continue;
		locale_set *set = const_cast<locale_set*>(hash.GetVal(k));
		split(t, set->accel[0], set->str);
	    }
	    locale->CloseCatalog(cat);
	}      
	locale->FreeInstance();
    }

    return res;
}

bool Localization::ExportCD(const char* filename, uint32 version) const
{
    ASSERT(DOS != 0);
    if (DOS == 0)
	return false;

    BPTR fh = DOS->Open(filename, MODE_NEWFILE);

    if (fh != 0)
    {
	String s;
	DOS->VFPrintf(fh, ";#version %ld\n", ARRAY(version));
	DOS->VFPrintf(fh, ";#language english\n", 0);
	DOS->VFPrintf(fh, ";\n", 0);
	for (uint32 i=0; i<hash.Count(); i++)
	{
	    uint32 key = hash.GetKeyAt(i);
	    const char* g;
	    const locale_set *set = hash.GetVal(key);

	    if (set->group == 0)
		g = "GLOBAL";
	    else
		g = set->group;

	    if (set->id == 0)
		s.FormatStr("%s_%ld", ARRAY((uint32)g, key));
	    else
		s.FormatStr("%s_%s", ARRAY((uint32)g, (uint32)set->id));

	    DOS->VFPrintf(fh, "%s (%ld//)\n", ARRAY((uint32)s.Data(), key));
	    s = set->str;
	    s.Substitute("\n", "\\n");
	    s.Substitute("\033", "\\033");
	    if (set->accel[0] == 0)
		DOS->VFPrintf(fh, "%s\n", ARRAY((uint32)s.Data()));
	    else
		DOS->VFPrintf(fh, "%lc&%s\n", ARRAY((uint32)set->accel[0], (uint32)s.Data()));
	    DOS->VFPrintf(fh, ";\n", 0);
	}

	DOS->Close(fh);
	return true;
    }
    return false;
}

bool Localization::ExportCT(const char* filename, uint32 version) const
{
    ASSERT(DOS != 0);
    if (DOS == 0)
	return false;

    BPTR fh = DOS->Open(filename, MODE_NEWFILE);

    if (fh != 0)
    {
	String s;
	DOS->VFPrintf(fh, "##version %s %ld.0\n", ARRAY((uint32)filename, version));
	DOS->VFPrintf(fh, "##language english\n", 0);
	DOS->VFPrintf(fh, ";\n", 0);
	for (uint32 i=0; i<hash.Count(); i++)
	{
	    uint32 key = hash.GetKeyAt(i);
	    const char* g;
	    const locale_set *set = hash.GetVal(key);

	    if (set->group == 0)
		g = "GLOBAL";
	    else
		g = set->group;

	    if (set->id == 0)
		s.FormatStr("%s_%ld", ARRAY((uint32)g, key));
	    else
		s.FormatStr("%s_%s", ARRAY((uint32)g, (uint32)set->id));

	    DOS->VFPrintf(fh, "%s\n", ARRAY((uint32)s.Data()));
	    s = set->str;
	    s.Substitute("\n", "\\n");
	    s.Substitute("\033", "\\033");
	    if (set->accel[0] == 0)
		DOS->VFPrintf(fh, "%s\n", ARRAY((uint32)s.Data()));
	    else
		DOS->VFPrintf(fh, "%lc&%s\n", ARRAY((uint32)set->accel[0], (uint32)s.Data()));
	    DOS->VFPrintf(fh, ";\n", 0);
	}

	DOS->Close(fh);
	return true;
    }
    return false;
}

String Localization::FormatNumber(uint32 integer, uint32 millionth) const
{
    char temp[32];

    uint8 grp[] = { 3, 0 }; 
    const char* decp = lang ? lang->loc_DecimalPoint : ".";
    const char* gsep = lang ? lang->loc_GroupSeparator : " ";
    const char* fsep = lang ? lang->loc_FracGroupSeparator : " ";
    const uint8* ggrp = lang ? lang->loc_Grouping : (uint8*)&grp;
    const uint8* fgrp = lang ? lang->loc_FracGrouping : (uint8*)&grp;
    int gpos = 0;
    int part = 0;
    bool sign = false;
    uint32 pos = sizeof(temp);

    if (integer < 0)
    {
	sign = true;
	integer = -integer;
    }

    do 
    {
	--pos;
	/* 
	* handle group
	*/
	if (gpos == *ggrp)
	{
	    gpos = 0;
	    ggrp++;
	    if (!*ggrp)
		ggrp--;
	    if (*gsep)
		temp[pos--] = *gsep;
	}
	gpos++;

	part = integer % 10;
	integer /= 10;
	temp[pos] = part + '0';
    } 
    while (integer != 0);

    if (sign)
	temp[--pos] = '-';

    /*
    * ok we have integer part parsed now
    */
    for (integer=0; pos<sizeof(temp); integer++, pos++)
	temp[integer] = temp[pos];
    pos = integer;

    /*
    * reverse digit order ;]
    * make life a little easier, too
    */
    integer = 0;
    for (int i=0; i<6; i++)
    {
	part = millionth % 10;
	integer <<= 4;
	integer |= part;
	millionth /= 10;
    }

    if (0 != integer)
    {
	temp[pos++] = *decp;
	gpos = 0;

	while (integer != 0)
	{
	    if (gpos == *fgrp)
	    {
		gpos = 0;
		fgrp++;
		if (!*fgrp)
		    fgrp--;
		if (*fsep)                 // handle broken locales somehow
		    temp[pos++] = *fsep;
	    }
	    gpos++;

	    part = integer & 0xf;
	    integer >>= 4;
	    temp[pos++] = part + '0';
	}


    }

    temp[pos++] = 0;

    return String(temp);
}


/******************************************************************************/
SubLocalization::SubLocalization(ILocalization& p, ILocalization::IndexOffset o) :
    parent(p),
    base(o)
{
}

SubLocalization::~SubLocalization()
{
}

const String& SubLocalization::operator[](uint32 key) const
{
    return parent.operator[](key + base);
}

const String& SubLocalization::Str(uint32 key) const
{
    return parent.Str(key + base);
}

const char SubLocalization::Accel(uint32 key) const
{
    return parent.Accel(key + base);
}

const char* SubLocalization::Shortcut(uint32 key) const
{
    return parent.Shortcut(key + base);
}

void SubLocalization::AddGroup(ILocalization::LocaleSet set[], const char* grp)
{
    int i = 0;
    if (set == 0)
	return;

    while (set[i].key != (uint32)ILocalization::LocaleSet::Set_Last)
    {
	AddItem(set[i].key, set[i].value, set[i].locale_id, grp);
	++i;
    }
}

void SubLocalization::AddItem(uint32 key, const char* text, const char* id, const char* group)
{
    parent.AddItem(base + key, text, id, group);
}

String SubLocalization::FormatNumber(uint32 i, uint32 f) const
{
    return parent.FormatNumber(i, f);
}
