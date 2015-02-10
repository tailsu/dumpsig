#include "stdafx.h"

// Code copied from:
// "Finding the Raw Strong Name Signature"
// by ShawnFa
// http://blogs.msdn.com/b/shawnfa/archive/2005/01/26/361109.aspx

HRESULT DisplaySignature(BYTE *pbFile)
{
    _ASSERTE(pbFile != NULL);

    PIMAGE_NT_HEADERS pNtHeader = ImageNtHeader(pbFile);
    if(NULL == pNtHeader)
    {
        std::cerr << "Error: Could not get NT headers for file" << std::endl;
        return E_INVALIDARG;
    }

    // find the data directories in the file
    PIMAGE_DATA_DIRECTORY pDataDirectories = NULL;
    switch(pNtHeader->OptionalHeader.Magic)
    {
        case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
            pDataDirectories = reinterpret_cast<PIMAGE_NT_HEADERS32>(pNtHeader)->OptionalHeader.DataDirectory;
            break;

        case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
            pDataDirectories = reinterpret_cast<PIMAGE_NT_HEADERS64>(pNtHeader)->OptionalHeader.DataDirectory;
            break;
        
        default:
            return E_NOTIMPL;
    }
    
    // Make sure there is a CLR header
    _ASSERTE(pDataDirectories != NULL);
    if(0 == pDataDirectories[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress ||
       0 == pDataDirectories[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size)
    {
        std::cerr << "Error: No CLR header" << std::endl;
        return E_INVALIDARG;
    }
    
    // get the CLR header
    PIMAGE_COR20_HEADER pCorHeader = reinterpret_cast<PIMAGE_COR20_HEADER>(
        ImageRvaToVa(
            pNtHeader,
            pbFile,
            pDataDirectories[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress,
            NULL));
    if(NULL == pCorHeader)
    {
        std::cerr << "Error: Could not get CLR header" << std::endl;
        return E_INVALIDARG;
    }

    // check to see if there is a signature
    if( 0 == pCorHeader->StrongNameSignature.VirtualAddress ||
        0 == pCorHeader->StrongNameSignature.Size)
    {
        std::cerr << "Error: Image is not signed" << std::endl;
        return S_FALSE;
    }

    // pull the signature from the header
    BYTE *pbSignature = reinterpret_cast<BYTE *>(
        ImageRvaToVa(
            pNtHeader,
            pbFile,
            pCorHeader->StrongNameSignature.VirtualAddress,
            NULL));
    if(NULL == pbSignature)
    {
        std::cerr << "Error: Could not find signature directory" << std::endl;
        return E_INVALIDARG;
    }

    // display it
    std::cout << "Strong name signature (" << pCorHeader->StrongNameSignature.Size << " bytes):";
    std::cout << std::hex;
    for(DWORD i = 0; i < pCorHeader->StrongNameSignature.Size; i++)
    {
        if(i % 16 == 0)
            std::cout << std::endl;
        std::cout << std::setw(2) << (DWORD)(pbSignature[i]) << " ";
    }
    
    std::cout << std::dec << std::endl;
    return S_OK;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage: dumpsig <assembly>" << std::endl;
		return 1;
	}

	auto hfile = CreateFile(argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Error: Cannot open file" << std::endl;
		return 2;
	}

	LARGE_INTEGER fileSize;
	GetFileSizeEx(hfile, &fileSize);

	auto mapping = CreateFileMapping(hfile, NULL, PAGE_READONLY, fileSize.HighPart, fileSize.LowPart, NULL);
	auto data = (BYTE*) MapViewOfFile(mapping, FILE_MAP_READ, 0, 0, (size_t) fileSize.QuadPart);

	if (data == NULL)
	{
		std::cerr << "Error: Cannot open file mapping" << std::endl;
		return 3;
	}

	return S_OK == DisplaySignature(data) ? 0 : 10;
}

