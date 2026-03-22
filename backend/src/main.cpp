#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/url.hpp>
#include <iostream>
#include <bits/stdc++.h>
// #include <map>

using namespace boost::urls;
using std::cout, std::endl;
using namespace std;


#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;
using tcp = boost::asio::ip::tcp;

void parse_params(std::map<string, string> *params_map, string url_str){
  // Absolutely the most disgusting piece of code I've made, but it works.
  string url_delimiter = "?";
  string param_delimiter = "=";
  size_t url_pos = 0;
  string params = url_str.substr(url_pos = url_str.find(url_delimiter) + url_delimiter.length());
  std::vector<std::string> tokens;
  size_t pos = 0;
  string token;
  while ( true){
    pos = params.find("&");
    string delimiter = "&";
    token = params.substr(0, pos);
    if(token != "" && params.find("&") < 1000){
      int test = 0;
      (*params_map).insert({token.substr(0, token.find(param_delimiter)), token.substr(0 + token.find("=") + param_delimiter.length())});
    }

    params.erase(0, pos + delimiter.length());

    if(params.find("&") > 1000){
      (*params_map).insert({params.substr(0, params.find(param_delimiter)), params.substr(0 + params.find("=") + param_delimiter.length())});
      params.erase(0, pos + delimiter.length());
      break;
    }
  }
}

// actual request handler, generates json response from http request
void handle_request(const http::request<http::string_body>& req, http::response<http::string_body>& res){

  // if request method is get
  if(req.method() == http::verb::get){

    // parses the params and path
    url_view parsed_url(req.target());
    auto params = parsed_url.params();
    auto path = parsed_url.path();

    string url_str(req.target());

    std::cout << "useful info " << url_str << '\n';

    map<string, string> params_map;
    parse_params(&params_map, url_str);

    // map<string, string> params_map;
    // parse_params(&params_map, params.to_string());
    

    // basically whats after the port (like /api/auth?username=USERNAME&password=PASSWORD) 

    if(path == "/hello"){
      json::object json_response;
      json_response["message"] = "Hello, World!";
      json_response["status"] = "success";

      std::cout << "connection to '127.0.0.1:8080/hello' received\n";

      res.result(http::status::ok);
      res.set(http::field::content_type, "application/json");
      res.body() = json::serialize(json_response);
      res.prepare_payload();
    } else if (path == "/world") {
      json::object json_response;
      json_response["message"] = "World, Hello!";
      json_response["status"] = "success";

      std::cout << "connection to '127.0.0.1:8080/world' received\n";

      res.result(http::status::ok);
      res.set(http::field::content_type, "application/json");
      res.body() = json::serialize(json_response);
      res.prepare_payload();
    } else if (path == "/test") {
      json::object json_response;
      json_response["params"] = "test";
      json_response["status"] = "success";

      std::cout << "connection to '127.0.0.1:8080/test' received with params; " << params << '\n';
      
      res.result(http::status::ok);
      res.set(http::field::content_type, "application/json");
      res.body() = json::serialize(json_response);
      res.prepare_payload();
    } else if (path == "/api/stocks"){
      json::object json_response;
      json_response["status"] = "200";
      json_response["ticker"] = params_map["ticker"];


      // url_view u = parsed_url.params();

      // std::cout << "connection to '127.0.0.1:8080/test' received with params; " << p << '\n';
      // auto test = params.params().find( "ticker" ) != u.params().find( "ticker", ignore_case ) ;

      // string ticker;
      // ticker == 
      // std::cout << "connection to '127.0.0.1:8080/api/stocks' received with params; " << params << '\n';
      // std::cout << "connection to '127.0.0.1:8080/api/stocks' received, ; " << test << '\n';

      res.result(http::status::ok);
      res.set(http::field::content_type, "application/json");
      res.body() = json::serialize(json_response);
      res.prepare_payload();
    }
    else {
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

  if(req.method() == http::verb::post){
    url_view parsed_url(req.target());
    auto params = parsed_url.params();
    auto path = parsed_url.path();

    if(path == "/post/test"){


    }
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

