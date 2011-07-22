/*
 * Amiga Generic Set - set of libraries and includes to ease sw development for all Amiga platforms
 * Copyright (C) 2004-2008 Tomasz Wiszkowski Tomasz.Wiszkowski at gmail.com.
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef _GENERIC_MUI_MUI_H_
#define _GENERIC_MUI_MUI_H_ 

#include <Generic/GenericMUI.h>
#include <Generic/HookAttrT.h>
#include <Generic/HookT.h>
#include <Generic/VectorT.h>

namespace GenNS
{

    class MUI : public virtual GenericMUI
    {
    public:
	enum Alignment
	{
	    Align_Left  =  -1,
	    Align_Center,
	    Align_Right
	};

	enum ID
	{
	    ID_Default  = -1
	};

    protected:
	enum elemType
	{
	    T_Unknown,
	    T_Label,
	    T_Button,
	    T_CheckBox,
	    T_Cycle,
	    T_String,
	    T_Gauge,
	    T_Slider
	};

	struct elemAssoc 
	{
	    elemType          type;
	    int32             id;
	    iptr	     *elem;

	    elemAssoc(elemType t, int32 i, iptr *e) :
		type(t),
		id(i),
		elem(e)
	    {
	    };
	};

    private:
	HookT<MUI, iptr, iptr*>              gateBtns;
	HookAttrT<iptr, iptr>               cbButtonHook;

    protected:
	VectorT<elemAssoc*>                 muiElements;

	static bool             freeItem(elemAssoc* const& item);

    protected:
	virtual iptr            gateButtons(iptr btn, iptr *idptr);

    protected:
	virtual void            setButtonCallBack(const Hook *hook);

    public:
	MUI();
	virtual                ~MUI();
	virtual iptr           *muiLabel(const char* contents, char key=0, int32 id=ID_Default, int32 weight=20, Alignment align=Align_Center);
	virtual iptr           *muiButton(const char* contents, char key=0, int32 id=ID_Default);
	virtual iptr	       *muiImageButton(iptr image, char key=0, int32 id=ID_Default);
	virtual iptr           *muiCycle(const char** contents, char key=0, int32 id=ID_Default, int32 active=0);
	virtual iptr           *muiCheckBox(const char* name, char key=0, int32 id=ID_Default, Alignment align=Align_Left, bool checked=false);
	virtual iptr           *muiBar(const char *name=0, bool vertical=false);        
	virtual iptr           *muiString(const char* contents, char key=0, int32 id=ID_Default, Alignment align=Align_Left);
	virtual iptr           *muiGauge(const char* contents, int32 id=ID_Default);
	virtual iptr           *muiSpace(int32 weight=100);
	virtual iptr           *muiSlider(int32 min, int32 max, int32 level, char key=0, int32 id=ID_Default);

	virtual void            muiSetEnabled(int32 id, bool enabled);
	virtual void            muiSetSelected(int32 id, int32 selected);
	virtual void            muiSetVisible(int32 id, bool visible);
	virtual void            muiSetText(int32 id, const char *text);
	virtual void            muiSetValue(int32 id, int32 value);
    };

};

#endif

