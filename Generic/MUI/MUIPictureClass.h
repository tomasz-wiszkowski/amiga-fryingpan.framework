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


#ifndef _GENERIC_MUI_MUIPICTURECLASS_H_
#define _GENERIC_MUI_MUIPICTURECLASS_H_ 

#include <Generic/GenericOOP.h>
#include <Generic/String.h>
#include <Generic/LibrarySpool.h>
#include <libclass/datatypes.h>
#include <datatypes/pictureclass.h>
#include <Generic/MUICustomClassT.h>


namespace GenNS
{
    class MUIPictureClass : public GenericOOP
    {
    public:
	enum Tags
	{
	    MUIA_Picture_NormalImage      =  TAG_USER + 0x100,
	    MUIA_Picture_SelectedImage
	};

    private:
	static MUICustomClassT<MUIPictureClass> *MUIPictureClass::muiclass;

    protected:
	IClass           *parent;
	String            image1;
	String            image2;
	uint16            width;
	uint16            height;
	bool              isDisabled;
	bool              isSelected;
	DatatypesIFace   *dt;
	Object           *dtimg1;
	Object           *dtimg2;

    protected:
	virtual void      openImages();
	virtual void      closeImages();

    public:
	MUIPictureClass(IClass* parent);
	virtual          ~MUIPictureClass();                  
	virtual iptr      DoMtd(iptr *obj, iptr *msg);

	static iptr*	  CreateClass();
	static void	  DestroyClass();
    };
};

#endif

