#pragma once
#include <fc/vector.hpp>
#include <fc/string.hpp>
#include <fc/shared_impl.hpp>

namespace fc { 
  namespace ip { class endpoint; }
  class tcp_socket;

  namespace http {

  struct header {
    header( fc::string k, fc::string v )
    :key(fc::move(k)),val(fc::move(v)){}
    header(){}
    fc::string key;
    fc::string val;
  };

  struct reply {
     enum status_code {
         OK                  = 200,
         RecordCreated       = 201,
         NotFound            = 404,
         Found               = 302,
         InternalServerError = 500
     };
     reply( status_code c = OK):status(c){}
     int                     status;
     fc::vector<header>      headers;
     fc::vector<char>        body;
  };

  struct request {
     fc::string get_header( const fc::string& key )const;
     fc::string              method;
     fc::string              domain;
     fc::string              path;
     fc::vector<header>      headers;
     fc::vector<char>        body;
  };

  fc::vector<header> parse_urlencoded_params( const fc::string& f );

  /**
   *  Connections have reference semantics, all copies refer to the same
   *  underlying socket.  
   */
  class connection {
    public:
      // used for clients
      void         connect_to( const fc::ip::endpoint& ep );
      http::reply  request( const fc::string& method, const fc::string& url, const fc::string& body );

      // used for servers
      fc::tcp_socket& get_socket()const;

      http::request    read_request()const;

      FC_REFERENCE_TYPE(connection)
  };

} } // fc::http

