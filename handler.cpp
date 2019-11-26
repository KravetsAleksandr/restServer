#include "handler.h"
#include <cpprest/filestream.h>
#include <iostream>
#include <sstream>


handler::handler()
{
	//ctor
}
handler::handler(utility::string_t url) :m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&handler::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&handler::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&handler::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&handler::handle_delete, this, std::placeholders::_1));

}
handler::~handler()
{
	//dtor
}

void handler::handle_error(pplx::task<void>& t)
{
	try
	{
		t.get();
	}
	catch (...)
	{
		// Ignore the error, Log it if a logger is available
	}
}


//
// Get Request 
//
void handler::handle_get(http_request message)
{
	//	ucout << message.to_string() << endl;


	//	message.relative_uri().path();

	auto paths = uri::split_path(uri::decode(message.relative_uri().path()));

	if (paths[0] == L"isDownloading")
	{
		json::value postData;
		std::wstring ws = std::to_wstring(_downloadStatus);
		const wchar_t* jsonws = ws.c_str();
		postData[L"progress"] = json::value::string(jsonws);
		_isDownloading ? message.reply(status_codes::Accepted, postData) : message.reply(status_codes::OK, postData);
	}

	message.reply(status_codes::OK);
	return;
	/*

		//Dbms* d  = new Dbms();
		//d->connect();

		concurrency::streams::fstream::open_istream(U("static/index.html"), std::ios::in).then([=](concurrency::streams::istream is)
		{
			//message.reply(status_codes::OK, is, U("text/html"))
			message.reply(status_codes::OK, file_size)
				.then([](pplx::task<void> t)
			{
				try {
					t.get();
				}
				catch (...) {
					//
				}
			});
		}).then([=](pplx::task<void>t)
		{
			try {
				t.get();
			}
			catch (...) {
				message.reply(status_codes::InternalError, U("INTERNAL ERROR "));
			}
		});

		return;
	*/
};


//
// A POST request
//
void handler::handle_post(http_request message)
{
	//ucout << message.to_string() << endl;

	message
		.extract_json()
		.then([](pplx::task<json::value> task) {
		try
		{
			const json::value& jvalue = task.get();
			auto _msg = jvalue.as_object();
			wcout << _msg[L"name"] << " say(s) " << _msg[L"message"] << endl;
		}
		catch (http_exception const& e)
		{
			wcout << e.what() << endl;
		}
			})
		.wait();



			message.reply(status_codes::OK, message.to_string());
			return;
};

//
// A DELETE request
//
void handler::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;

	string rep = "WRITE YOUR OWN DELETE OPERATION";
	message.reply(status_codes::OK, rep);
	return;
};


//
// A PUT request 
//
void handler::handle_put(http_request message)
{
	//ucout << message.to_string() << endl;
	//string rep = "File is downloaded successfully";
	web::http::http_headers headers = message.headers();

	utility::string_t _filename;
	headers[L"FileName"].empty() ? _filename.append(L"downloaded_file") : _filename = headers[L"FileName"];



	auto paths = uri::split_path(uri::decode(message.relative_uri().path()));

	if (paths[0] == L"myfiletoputonserver")

	{
		auto stream = concurrency::streams::fstream::open_ostream(_filename, std::ios_base::out | std::ios_base::binary).get();
		//auto stream = concurrency::streams::fstream::open_ostream(U("downloaded_file.jpg"), std::ios_base::out | std::ios_base::binary).get();
		try
		{
			std::cout << "Start downloading..." << std::endl;
			_isDownloading = true;
			_downloadStatus = 0;
			_downloadedBytes = 0;
			int _bytesRead = 0;
			_fileSize = stream.streambuf().size();
			/*
						do
						{
							_bytesRead = message.body().read(stream.streambuf(), CHUNK).get();
							_downloadedBytes += _bytesRead;
							_fileSize > 0 ? _downloadStatus = _downloadedBytes / _fileSize * 100 : _downloadStatus = 0;
						} while (_bytesRead > 0);
			*/
			message.body().read_to_end(stream.streambuf()).wait();

			stream.close();
			std::cout << "File is downloaded completely" << std::endl;
			_isDownloading = false;
			_downloadedBytes = 0;

		}
		catch (http_exception const& e)
		{
			wcout << e.what() << endl;
		}
		message.reply(status_codes::Created);
		return;
	}

	message.reply(status_codes::OK);
	return;
};
