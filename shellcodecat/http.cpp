//// curl_test3.cpp: 定义控制台应用程序的入口点。
////
//
#include "stdafx.h"
//
#include "stdafx.h"
#include<string>
#include<windows.h>
#include<vector>
#include<iostream>
#include<winhttp.h>
#pragma comment(lib,"Winhttp.lib")
using namespace std;


char* HttpReceive(const char* URL_STRING, size_t& dataSize)
{
	int len = strlen(URL_STRING);
	size_t converted = 0;
	WCHAR pwszUrl1[255];
	mbstowcs_s(&converted, pwszUrl1, len + 1, URL_STRING, 255);

	URL_COMPONENTS urlComp;
	DWORD dwUrlLen = 0;
	WCHAR szHostName[128] = { 0 };
	WCHAR szUrlPath[256] = { 0 };

	// Initialize the URL_COMPONENTS structure.
	ZeroMemory(&urlComp, sizeof(urlComp));
	urlComp.dwStructSize = sizeof(urlComp);

	// Set required component lengths to non-zero so that they are cracked.
	urlComp.dwSchemeLength = (DWORD)-1;
	urlComp.dwHostNameLength = (DWORD)-1;
	urlComp.dwUrlPathLength = (DWORD)-1;
	urlComp.dwExtraInfoLength = (DWORD)-1;

	// Crack the URL.
	if (!WinHttpCrackUrl(pwszUrl1, (DWORD)wcslen(pwszUrl1), 0, &urlComp))
		printf("Error %u in WinHttpCrackUrl.\n", GetLastError());
	memcpy(szHostName, urlComp.lpszHostName, sizeof(WCHAR)*urlComp.dwHostNameLength);

	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer;
	std::vector<BYTE> recData;

	BOOL  bResults = FALSE;
	HINTERNET hSession = NULL,
		hConnect = NULL,
		hRequest = NULL;
	char* buf = (char*)malloc(255);

	// Use WinHttpOpen to obtain a session handle.
	hSession = WinHttpOpen(L"A WinHTTP Example Program/1.0",
		WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		WINHTTP_NO_PROXY_NAME,
		WINHTTP_NO_PROXY_BYPASS, 0);

	// Specify an HTTP server.
	if (hSession)
		hConnect = WinHttpConnect(hSession, szHostName,
			urlComp.nPort, 0);

	// Create an HTTP Request handle.
	if (hConnect)
		hRequest = WinHttpOpenRequest(hConnect, L"GET",
			urlComp.lpszUrlPath,
			NULL, WINHTTP_NO_REFERER,
			WINHTTP_DEFAULT_ACCEPT_TYPES,
			0);

	// Send a Request.
	if (hRequest)
		bResults = WinHttpSendRequest(hRequest,
			WINHTTP_NO_ADDITIONAL_HEADERS,
			0, WINHTTP_NO_REQUEST_DATA, 0,
			0, 0);

	// PLACE ADDITIONAL CODE HERE.
	if (bResults)
		bResults = WinHttpReceiveResponse(hRequest, NULL);

	// Keep checking for data until there is nothing left.
	if (bResults)
	{
		do
		{
			// Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
				printf("Error %u in WinHttpQueryDataAvailable.\n",
					GetLastError());

			// Allocate space for the buffer.
			pszOutBuffer = new char[dwSize + 1];
			if (!pszOutBuffer)
			{
				printf("Out of memory\n");
				dwSize = 0;
			}
			else
			{
				// Read the data.
				ZeroMemory(pszOutBuffer, dwSize + 1);

				if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
					dwSize, &dwDownloaded))
					printf("Error %u in WinHttpReadData.\n", GetLastError());
				else
				{
					//printf("%s", pszOutBuffer);
					for (int i = 0; i < dwSize; i++)
					{
						recData.push_back(pszOutBuffer[i]);
					}

				}

				// Free the memory allocated to the buffer.
				delete pszOutBuffer;
			}
		} while (dwSize > 0);
	}


	// Report any errors.
	if (!bResults)
		printf("Error %d has occurred.\n", GetLastError());

	// Close any open handles.
	if (hRequest) WinHttpCloseHandle(hRequest);
	if (hConnect) WinHttpCloseHandle(hConnect);
	if (hSession) WinHttpCloseHandle(hSession);

	char* bytesData = new char[recData.size()];
	for (int i = 0; i < recData.size(); i++)
		bytesData[i] = recData[i];
	dataSize = recData.size();




	return bytesData;
}
