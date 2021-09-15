
#include <string>
#include <uriparser/Uri.h>
#include "url.h"

namespace uriparser
{

    uriparser::Uri::Uri(std::string uri)
        : uri_(uri)
    {
        UriParserStateA state_;
        state_.uri = &uriParse_;
        isValid_ = uriParseUriA(&state_, uri_.c_str()) == URI_SUCCESS;
    }

    uriparser::Uri::~Uri() { uriFreeUriMembersA(&uriParse_); }

    bool uriparser::Uri::isValid() const { return isValid_; }

    std::string uriparser::Uri::scheme()   const { return fromRange(uriParse_.scheme); }
    std::string uriparser::Uri::host()     const { return fromRange(uriParse_.hostText); }
    std::string uriparser::Uri::port()     const { return fromRange(uriParse_.portText); }
    std::string uriparser::Uri::path()     const { return fromList(uriParse_.pathHead, "/"); }
    std::string uriparser::Uri::query()    const { return fromRange(uriParse_.query); }
    std::string uriparser::Uri::fragment() const { return fromRange(uriParse_.fragment); }
   
    uriparser::Uri::parsed_uri uriparser::Uri::get_parsed_uri(std::string uri_string) {
        uriparser::Uri uri(uri_string);
        std::string x = uri.scheme();
        printf("host: %s", uri.host().c_str());
        printf(" scheme: %s ", uri.scheme().c_str());
        printf(" path: %s", uri.path().c_str());
        return {
            uri.scheme(),
            uri.host(),
            uri.path()
        };
    };

};