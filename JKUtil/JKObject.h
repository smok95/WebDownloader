#pragma once
#include <Windows.h>
#define CLASS_NAME(class_name) #class_name##;
#define NullReturn(x)	if((x)==NULL)	return

class CJKObject
{
public:
	BYTE* m_buffer;
	CJKObject(void) : m_buffer(NULL){}
	~CJKObject(void){	Delete();	}

	virtual void Delete()
	{
		if(m_buffer != NULL) delete[] m_buffer;
		m_buffer = NULL;
	}

	virtual void New(size_t size)
	{
		Delete();
		m_buffer = new BYTE[size];
		if(m_buffer)
		{
            for(size_t i=0; i<size; i++)		m_buffer[i] = 0;
		}
	}

	virtual char* GetClassName(void) = 0;
};
