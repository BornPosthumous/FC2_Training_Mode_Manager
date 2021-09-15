#pragma once

#include <string>
#include <uriparser/Uri.h>
namespace uriparser
{
    class Uri //: boost::noncopyable
    {
    public:
        Uri(std::string uri);
        ~Uri();;

        bool isValid() const;

        std::string scheme() const;
        std::string host() const;
        std::string port() const;
        std::string path() const;
        std::string query() const;
        std::string fragment() const;
        
        struct parsed_uri {
            std::string scheme;
            std::string host;
            std::string path;
        };

        static uriparser::Uri::parsed_uri get_parsed_uri(std::string uri_string);
    
    private:
        std::string uri_;
        UriUriA     uriParse_;
        bool        isValid_;

        std::string fromRange(const UriTextRangeA& rng) const
        {
            return std::string(rng.first, rng.afterLast);
        }

        std::string fromList(UriPathSegmentA* xs, const std::string& delim) const
        {
            UriPathSegmentStructA* head(xs);
            std::string accum;

            while (head)
            {
                accum += delim + fromRange(head->text);
                head = head->next;
            }

            return accum;
        }
    };
}