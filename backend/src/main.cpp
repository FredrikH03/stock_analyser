#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/url.hpp>
#include <iostream>

using namespace boost::urls;
using std::cout, std::endl;

#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;
using tcp = boost::asio::ip::tcp;

// actual request handler, generates json response from http request
void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res){

  // if request method is get
  if(req.method() == http::verb::get){

    // parses the params and path
    url_view parsed_url(req.target());
    auto params = parsed_url.params();
    auto path = parsed_url.path();
    // basically whats after the port (like /api/auth?username=USERNAME&password=PASSWORD) 

    if(path == "/hello"){
      json::object json_response;
      json_response["message"] = "Hello, World!";
      json_response["status"] = "success";

      res.result(http::status::ok);
      res.set(http::field::content_type, "application/json");
      res.body() = json::serialize(json_response);
      res.prepare_payload();
    } else if (path == "/world") {
      json::object json_response;
      json_response["message"] = "World, Hello!";
      json_response["status"] = "success";

      res.result(http::status::ok);
      res.set(http::field::content_type, "application/json");
      res.body() = json::serialize(json_response);
      res.prepare_payload();
    } else {
      res.result(http::status::not_found);
      res.set(http::field::content_type, "text/plain");
      res.body() = "Not Found";
      res.prepare_payload();
    }
  } else {
    res.result(http::status::method_not_allowed);
    res.set(http::field::content_type, "text/plain");
    res.body() = "Method Not Allowed";
    res.prepare_payload();
  }
}

// session handles communication with a single client
void session(tcp::socket socket) {
  try{
    beast::flat_buffer buffer;

    // reads the http request
    http::request<http::string_body> req;
    http::read(socket, buffer, req);

    // prepare the response
    http::response<http::string_body>  res;
    handle_request(req, res);

    // writes response
    http::write(socket, res);
  } catch(const std::exception& e){
    std::cerr << "Error in session: " << e.what() << '\n';
  }
}

int main() {

  //starts server
  try{
    const auto address = net::ip::make_address("127.0.0.1");
    const unsigned short port = 8080;

    net::io_context ioc;

    //create and bind the acceptor
    tcp::acceptor acceptor{ioc, {address, port}};
    std::cout << "HTTP server is running at http://127.0.0.1:8080\n";

    while (true) {
      //accept a new connection
      tcp::socket socket{ioc};
      acceptor.accept(socket);

      // handles the session in a new thread
      std::thread{&session, std::move(socket)}.detach();
    }
  } catch (const std::exception& e){
    std::cerr << "Error: " <<  e.what() << '\n';
    return 1;
  }

  return 0;
}

