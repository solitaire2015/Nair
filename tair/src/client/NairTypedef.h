/*
 * NairTypedef.h
 *
 *  Created on: May 12, 2015
 *      Author: rl5c
 */

#ifndef NAIRTYPEDEF_H_
#define NAIRTYPEDEF_H_

#include <string>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

#define NAIR_SPLIT_VALUE_CHR "[#N-V#]"
#define NAIR_SPLIT_VALUE_SIZE strlen(NAIR_SPLIT_VALUE_CHR)

class NairKey
{
public:

	NairKey()
	{
		_key = NULL;
		_value = NULL;
		_keysize = 0;
		_valuesize = 0;
	}

	NairKey(const char* skey)
	{
		_key = NULL;
		_value = NULL;
		_keysize = 0;
		_valuesize = 0;
		SetKey(skey);
	}

	~NairKey()
	{
		if(_key != NULL)
		{
			delete[] _key;
			_key = NULL;
		}
		
		if(_value != NULL)
		{
			delete[] _value;
			_value = NULL;
		}
		_keysize = 0;
		_valuesize = 0;
	}

	const char* GetKey()
	{
		return this->_key;
	}

	unsigned int GetKeySize()
	{
		return this->_keysize;
	}

	void SetKey(const char* skey)
	{
		_key = skey;
		_keysize = strlen(skey);
	}

	const char* GetValue()
	{
		return this->_value;
	}

	unsigned int GetValueSize()
	{
		return this->_valuesize;
	}

	void SetValue(const char* svalue)
	{
		_value = svalue;
		_valuesize = strlen(svalue);
	}

private:
	const char* _key;
	const char* _value;
	unsigned int _keysize;
	unsigned int _valuesize;
};

static inline void itoa(int i, char* pchr)
{
  int power,j;
  j=i;
  for(power=1;j>=10;j/=10)
    power*=10;
  for(;power>0;power/=10)
  {
    *pchr++='0'+i/power;
     i%=power;
  }
  *pchr='\0';
}

#endif /* NAIRTYPEDEF_H_ */
