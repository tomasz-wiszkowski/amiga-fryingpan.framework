#ifndef _GENERIC_LOCALE_H_
#define _GENERIC_LOCALE_H_

#include "Types.h"
#include "HashT.h"
#include "String.h"

class LocaleIFace;
struct Locale;

namespace GenNS
{
    /** Interface to localization.
     */
    class ILocalization
    {
    public:
	/** Index bases are meant to simplify localization management in application.
	*/
	enum IndexOffset
	{
	    /** Item enums are used to separate specific of text from each other.
	     *  A good example would be a number of UI pages, where you want to
	     *  separate translations for every page.
	     */
	    Item_Main	=	0,
	    Item_1	=	(1 << 6),
	    Item_2	=	(2 << 6),
	    Item_3	=	(3 << 6),
	    Item_4	=	(4 << 6),
	    Item_5	=	(5 << 6),
	    Item_6	=	(6 << 6),
	    Item_7	=	(7 << 6),
	    Item_8	=	(8 << 6),
	    Item_9	=	(9 << 6),
	    Item_10	=	(10 << 6),
	    Item_11	=	(11 << 6),
	    Item_12	=	(12 << 6),
	    Item_13	=	(13 << 6),
	    Item_14	=	(14 << 6),
	    Item_15	=	(15 << 6),

	    /** Groups are good to split larger portions of localization sets.
	     *  A good example would be an UI constructed from a number of child elements
	     *  such as popup frames, requests, etc., where you want these to have their own ID.
	     */
	    Grp_Main	=	0,
	    Grp_1	=	(1 << 10),
	    Grp_2	=	(2 << 10),
	    Grp_3	=	(3 << 10),
	    Grp_4	=	(4 << 10),
	    Grp_5	=	(5 << 10),
	    Grp_6	=	(6 << 10),
	    Grp_7	=	(7 << 10),
	    Grp_8	=	(8 << 10),
	    Grp_9	=	(9 << 10),
	    Grp_10	=	(10 << 10),
	    Grp_11	=	(11 << 10),
	    Grp_12	=	(12 << 10),
	    Grp_13	=	(13 << 10),
	    Grp_14	=	(14 << 10),
	    Grp_15	=	(15 << 10),

	    /** Sections are even larger than items. Here you could imagine 
	     *  every section to be a separate window.
	     */
	    Section_Main =	0,
	    Section_1	=	(1 << 14),
	    Section_2	=	(2 << 14),
	    Section_3	=	(3 << 14),
	    Section_4	=	(4 << 14),
	    Section_5	=	(5 << 14),
	    Section_6	=	(6 << 14),
	    Section_7	=	(7 << 14),
	    Section_8	=	(8 << 14),
	    Section_9	=	(9 << 14),
	    Section_10	=	(10 << 14),
	    Section_11	=	(11 << 14),
	    Section_12	=	(12 << 14),
	    Section_13	=	(13 << 14),
	    Section_14	=	(14 << 14),
	    Section_16	=	(16 << 14),

	    /** Modules are individual separate components of the application.
	     *  Every module can contain its 16 sections, 16 items and 16 groups.
	     *  That should be fairly sufficient.
	     */
	    Module_Main =	0,
	    Module_1	=	(1 << 18),
	    Module_2	=	(2 << 18),
	    Module_3	=	(3 << 18),
	    Module_4	=	(4 << 18),
	    Module_5	=	(5 << 18),
	    Module_6	=	(6 << 18),
	    Module_7	=	(7 << 18),
	    Module_8	=	(8 << 18),
	    Module_9	=	(9 << 18),
	    Module_10	=	(10 << 18),
	    Module_11	=	(11 << 18),
	    Module_12	=	(12 << 18),
	    Module_13	=	(13 << 18),
	    Module_14	=	(14 << 18),
	    Module_15	=	(15 << 18),

	    /** Applications. This is most likely nothing you will want to use.
	     */
	    App_Main =	0,
	    App_1	=	(1 << 22),
	    App_2	=	(2 << 22),
	    App_3	=	(3 << 22),
	    App_4	=	(4 << 22),
	    App_5	=	(5 << 22),
	    App_6	=	(6 << 22),
	    App_7	=	(7 << 22),
	    App_8	=	(8 << 22),
	    App_9	=	(9 << 22),
	    App_10	=	(10 << 22),
	    App_11	=	(11 << 22),
	    App_12	=	(12 << 22),
	    App_13	=	(13 << 22),
	    App_14	=	(14 << 22),
	    App_15	=	(15 << 22),
	};

	/** Locale element descriptor. Use an array of these to register the names you want translated.
	 */
	struct LocaleSet 
	{
	    /** Key index. This value uniquely identifies the locale string.
	     */
	    uint32      key;

	    /** String (the default string that you want to fall back to).
	     */
	    const char* value;

	    /** Locale name, in case you want to generate CT/CD files.
	     */
	    const char* locale_id;

	    /** Last key ID.
	     */
	    enum 
	    {
		Set_Last = ~0
	    };
	};

    public:
	/** Virtual destructor.
	 */
	virtual ~ILocalization()
	{
	}

	/** Add locale sets to the current locale class.
	 *  \param set is an array of LocaleSet elements (can be static).
	 *  \param group is a title of the locale group.
	 *  Note: this function takes index offset into consideration.
	 */
	virtual void AddGroup(LocaleSet set[], const char* group=0)	    = 0;

	/** Add an individual entry to the current locale class.
	 *  \param entry points to a single LocaleSet element.
	 *  \param group is a name of the group.
	 */
	virtual void AddItem(uint32 key, const char* text, const char* id, const char* group=0)	    = 0;

	/** Collect locale strings by key.
	 *  \param key is the locale key.
	 */
	virtual const String& operator[] (uint32 key) const		    = 0;

	/** Collect locale strings by key. \sa operator[]
	 *  \param key is the locale key.
	 */
	virtual const String& Str(uint32 key) const			    = 0;

	/** Get Accelerator for key.
	 *  This call returns a single character.
	 *  \param key is the locale key.
	 */
	virtual const char Accel(uint32 key) const			    = 0;

	/** Get Accelerator for key.
	 *  This command returns accelerator string.
	 *  \param key is the locale key.
	 */
	virtual const char* Shortcut(uint32 key) const			    = 0; 

	/** Format number with respect to localization standards.
	 *  \param integer is an integer path of the number.
	 *  \param millionth is a fractional part of the number measured in 1/1'000'000-s
	 */
	virtual String FormatNumber(uint32 integer, uint32 millionth=0) const    = 0;
    };

    /** First generation dummy locale class.
     *  This class is not capable of doing anything more than
     *  translating a number into a static string.
     */
    class DummyLocalization : public ILocalization
    {
    protected:
	struct locale_set
	{
	    String      str;
	    const char* group;
	    const char* id;
	    char        accel[2];

	    locale_set() :
		str(0), group(0), id(0)
	    {
		accel[0] = accel[1] = 0;
	    }

	    locale_set(const char* a, const char* b, const char* c, char d=0) :
		str(a), group(b), id(c)
	    {
		accel[0] = d;
		accel[1] = 0;
	    }
	};

    protected:
	/** Default translation set.
	 */
	locale_set                  deflt;

	/** Hash table containing our names.
	 */
	HashT<uint32, locale_set*>  hash;

    public:
	void              split(String s, char &accelerator, String &text); 

	/*
	** interface implementations
	*/
    public:
	virtual void AddGroup(LocaleSet set[], const char* group=0);
	virtual void AddItem(uint32 key, const char* text, const char* id, const char* group=0);
	const String&     operator[] (uint32 key) const;
	const String&     Str(uint32 key) const;
	const char        Accel(uint32 key) const;        // give back single char
	const char*       Shortcut(uint32 key) const;     // give back complete string
	String            FormatNumber(uint32 integer, uint32 millionth=0) const;

    public:
	DummyLocalization(LocaleSet set[] = 0, const char* group=0);
	virtual          ~DummyLocalization();
    };

    /** First generation locale class.
     *  This class is capable of reading, parsing and storing locales and translations.
     *  capable of translating.
     */
    class Localization : public DummyLocalization
    {
    protected:
	/** Locale library pointer.
	 */
	class ::LocaleIFace*	    locale;

	/** Language definition. Used to translate numbers and strings.
	 */
	struct ::Locale*	    lang;

	/*
	** specific implementations
	*/
    public:
	Localization(LocaleSet set[] = 0, const char* group=0);
	virtual          ~Localization();
	bool              ReadCatalog(const char* name, uint32 version=0);
	bool              ExportCD(const char* filename, uint32 version=0) const;
	bool              ExportCT(const char* filename, uint32 version=0) const;
	String            FormatNumber(uint32 integer, uint32 millionth=0) const;
    };

    /** Second and further genetarion of locale class.
     *  This should be used to forward your current localization 
     *  to modules, classes and depending applications.
     */
    class SubLocalization : public ILocalization
    {
	/** Pointer to fully-featured Localization class.
	 */
	ILocalization&	parent;

	/** Base index, used to spawn multiple locales from the same major.
	 */
	IndexOffset 	base;
    public:
	SubLocalization(ILocalization& parent, IndexOffset base);
	virtual ~SubLocalization();
	virtual void AddGroup(LocaleSet set[], const char* group=0);
	virtual void AddItem(uint32 key, const char* text, const char* id, const char* group=0);
	virtual const String& operator[] (uint32 key) const;
	virtual const String& Str(uint32 key) const;
	virtual const char Accel(uint32 key) const;
	virtual const char* Shortcut(uint32 key) const;
	virtual String FormatNumber(uint32 integer, uint32 millionth=0) const;
    };

};

#endif
