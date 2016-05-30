#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>
#include <string>
#include <vector>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

class MyRequestHandler : public HTTPRequestHandler
{
public:
  virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
  {
    resp.setStatus(HTTPResponse::HTTP_OK);
    resp.setContentType("text/html");

    ostream& out = resp.send();
    out << "<h1>Heips!</h1>"
        << "<p>Aplikaatio sai juuri pyynnon, muotoa " << req.getMethod() << ",</p>"
        << "<p>osoitteeseen "  << req.getHost() << req.getURI() << ".</p>"
        << "<p>Hienoa!</p>";
    out.flush();

  }

private:
  static int count;
};

int MyRequestHandler::count = 0;

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &)
  {
    return new MyRequestHandler;
  }
};

class MyMLApp : public ServerApplication
{
protected:
  int main(const vector<string> &)
  {
    HTTPServer s(new MyRequestHandlerFactory, ServerSocket(SocketAddress("127.2.41.1:8080")), new HTTPServerParams);

    s.start();

    waitForTerminationRequest();

    s.stop();

    return Application::EXIT_OK;
  }
};

int main(int argc, char** argv)
{
  MyMLApp app;
  return app.run(argc, argv);
}