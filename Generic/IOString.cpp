#include "IOString.h"

using namespace GenNS;

IOString::IOString(IIO* io)
{
    stream = io;
    buff = new char[8192];
    buff[0] = 0;
    len = 8192;
    actual = 0;
    pos = 0;
}

IOString::~IOString()
{
    delete [] buff;
}

bool IOString::ReadString()
{
    int loc;
    char c;
    str = "";

    while (1)
    {
	//request("info", "pos %ld, act %ld", "ok", ARRAY(pos, actual));
	if (pos >= actual)
	{
	    pos = 0;
	    actual = stream->GetDataAvail() <? (iptr)(len - 1);
	    //request("info", "data avail %ld", "ok", ARRAY(actual));
	    if (actual <= 0)
		return false;
	    actual = stream->ReadData(buff, actual);
	    //request("info", "data read %ld", "ok", ARRAY(actual));
	    if (actual <= 0)
		return false;
	    buff[actual] = 0;
	}

	loc = pos;
	//request("info", "rolling!", "ok", ARRAY(actual));
	
	while (loc < actual)
	{
	    c = buff[loc];
	    if ((c == 10) || (c == 13))
	    {
		//request("info", "found newline marker at %ld", "ok", ARRAY(loc));
		buff[loc++] = 0;
		str += &buff[pos];
		//request("info", "chars: %ld / %ld", "ok", ARRAY(c, buff[loc]));
		if (((buff[loc] == 10) || (buff[loc] == 13)) && (buff[loc] != c))
		    ++loc;

		pos = loc;
		//request("info", "resulting string: %s", "ok", ARRAY((iptr)str.Data()));
		return true;
	    }
	    ++loc;
	}

	pos = loc;
	str += &buff[pos];
    }

    return false;
}

void IOString::WriteString(const String& str)
{
    stream->WriteData(str.Data(), str.Length());
}

IOString::operator const String&()
{
    return Str();
}
	
String& IOString::Str()
{
    return str;
}
