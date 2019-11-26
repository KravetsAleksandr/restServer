#ifndef HANDLER_H
#define HANDLER_H
#include <iostream>
#include "stdafx.h"
//#include "../dbms/include/Dbms.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class handler
{
public:
	handler();
	handler(utility::string_t url);
	virtual ~handler();

	pplx::task<void>open() { return m_listener.open(); }
	pplx::task<void>close() { return m_listener.close(); }

protected:

private:
	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void handle_error(pplx::task<void>& t);
	http_listener m_listener;


private:
	int CHUNK = 10485760; //10 MB;
	double _downloadedBytes = 0; // number of bytes already read
	double _fileSize = 0; // size of streambufer
	double _downloadStatus = 0; //% of completeness
	bool _isDownloading = false; // if downloading is in progress
	//bool _isDownloaded = false; // reveal if file has been downloaded successfully
};

#endif // HANDLER_H


