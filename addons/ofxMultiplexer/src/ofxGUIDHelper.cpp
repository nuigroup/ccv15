#include "ofxGUIDHelper.h"


template<class T>
T HexToInt(const std::string &str)
{
  if(str.size()==0)return 0;
  std::istringstream iss(str);
  T result=0;
  iss>>std::hex>>result;
  return result;
}

template<class T>
std::string ToHex(const T &value)
{
  std::ostringstream oss;
  oss.setf(std::ios_base::uppercase);
  oss<<std::hex<<value;
  return oss.str();
}

int StringToInt(const std::string &str)
{
  if(str.size()==0)return 0;
  std::istringstream iss(str);
  int result=0;
  iss>>result;
  return result;
}

std::string IntToString(const int &value)
{
  std::ostringstream oss;
  oss.setf(std::ios_base::uppercase);
  oss<<value;
  return oss.str();
}


//I’m not writing shitty code -> I’m creating refactoring opportunities

std::string GUIDToString(const GUID &guid)
{
	if ((guid.Data2 == 0) && (guid.Data3 == 0) && 
		(guid.Data4[0] == 0) && 
		(guid.Data4[1] == 0) &&
		(guid.Data4[2] == 0) &&
		(guid.Data4[3] == 0) &&
		(guid.Data4[4] == 0) &&
		(guid.Data4[5] == 0) &&
		(guid.Data4[6] == 0) &&
		(guid.Data4[7] == 0))
	{
		std::string result = "";
		unsigned long data = guid.Data1;
		if (data == 0)
			result = "0";
		while (data)
		{
			result = (char)((data%10) + '0') + result;
			data/=10;
		}
		return result;
	}
	unsigned char* data = (unsigned char*)(&guid.Data1);
	std::string part1 = "";
	for (int i=0;i<4;i++)
	{
		part1 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part1;
		data++;
	}
	std::string part2 = "";
	data = (unsigned char*)(&guid.Data2);
	for (int i=0;i<2;i++)
	{
		part2 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part2;
		data++;
	}
	std::string part3 = "";
	data = (unsigned char*)(&guid.Data3);
	for (int i=0;i<2;i++)
	{
		part3 = (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data)) + part3;
		data++;
	}
	std::string part4 = "";
	data = (unsigned char*)(&guid.Data4);
	for (int i=0;i<2;i++)
	{
		part4 += (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data));
		data++;
	}
	std::string part5 = "";
	for (int i=0;i<6;i++)
	{
		part5 += (((*data < 16)?"0":"") + ToHex<unsigned short>((unsigned short)*data));
		data++;
	}
	return '{'+part1 + '-' + part2 + '-' + part3 +'-' + part4 + '-' + part5 + '}';
}

GUID StringToGUID(const std::string &str)
{
	GUID result;
	memset((void*)&result,0,sizeof(result));
	if (str.length()==38)
	{
		result.Data1 = HexToInt<unsigned long>(str.substr(1,8));
		result.Data2 = HexToInt<unsigned short>(str.substr(10,4));
		result.Data3 = HexToInt<unsigned short>(str.substr(15,4));
		unsigned char* dataPtr = (unsigned char*)&result.Data4;
		*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(20,2));
		*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(22,2));
		*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(25,2));
		*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(27,2));
		*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(29,2));
		*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(31,2));
		*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(33,2));
		*(dataPtr++) = (unsigned char)HexToInt<unsigned short>(str.substr(35,2));
	}
	else
	{
		unsigned long ret = 0;
		for (int i=0;i<str.length();i++)
		{
			ret*=10;
			ret+=(str[i]-'0');
		}
		result.Data1 = ret;
	}
	return result;
}

/*
int SearchDirectory(std::vector<std::string> &refvecFiles,
                    const std::string        &refcstrRootDirectory,
                    const std::string        &refcstrExtension,
                    bool                     bSearchSubdirectories = true)
{
  std::string     strFilePath;             // Filepath
  std::string     strPattern;              // Pattern
  std::string     strExtension;            // Extension
  std::string     strName;				   // Name without extension
  HANDLE          hFile;                   // Handle to file
  WIN32_FIND_DATA FileInformation;         // File information


  strPattern = refcstrRootDirectory+"/*.*";

  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(FileInformation.cFileName[0] != '.')
      {
        strFilePath.erase();
        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          if(bSearchSubdirectories)
          {
            // Search subdirectory
            int iRC = SearchDirectory(refvecFiles,
                                      strFilePath,
                                      refcstrExtension,
                                      bSearchSubdirectories);
            if(iRC)
              return iRC;
          }
        }
        else
        {
          // Check extension
          strExtension = FileInformation.cFileName;
          strExtension = strExtension.substr(strExtension.rfind(".") + 1);
		  strName = FileInformation.cFileName;
		  strName = strName.substr(strName.rfind("/") + 1);
		  strName = strName.substr(0,strName.rfind("."));
          if(strExtension == refcstrExtension)
          {
            refvecFiles.push_back(strName);
          }
        }
      }
    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);

    DWORD dwError = ::GetLastError();
    if(dwError != ERROR_NO_MORE_FILES)
      return dwError;
  }

  return 0;
}
*/
