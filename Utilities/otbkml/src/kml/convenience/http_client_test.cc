// Copyright 2009, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the unit test for the HttpClient class.

#include "kml/convenience/http_client.h"

#include "gtest/gtest.h"
#include "kml/base/string_util.h"

namespace kmlconvenience {

TEST(HttpClientTest, FetchSetHeader) {
  HttpClient http_client("my-app");
  const string kFieldName("Some-Header");
  const string kFieldValue("the value of the header");
  const size_t size_before = http_client.get_headers().size();
  http_client.AddHeader(kFieldName, kFieldValue);
  const StringPairVector& headers = http_client.get_headers();
  ASSERT_EQ(size_before + 1, headers.size());
  ASSERT_EQ(kFieldName, headers[size_before].first);
  ASSERT_EQ(kFieldValue, headers[size_before].second);
  const string kField1Name("Another-Header");
  const string kField1Value("the value of this other header");
  http_client.AddHeader(kField1Name, kField1Value);
  ASSERT_EQ(size_before + 2, headers.size());
  ASSERT_EQ(kField1Name, headers[size_before + 1].first);
  ASSERT_EQ(kField1Value, headers[size_before + 1].second);
}

TEST(HttpClientTest, TestFormatHeader) {
  const string kHi("hi");
  const string kThere("there");
  const StringPair header = std::make_pair(kHi, kThere);
  string header_line = HttpClient::FormatHeader(header);
  ASSERT_EQ(static_cast<size_t>(0), header_line.find(kHi));
  const size_t there_offset = header_line.find(kThere);
  ASSERT_EQ(header_line.size(), there_offset + kThere.size());
  ASSERT_EQ(": ", header_line.substr(there_offset - 2, 2));
}

TEST(HttpClientTest, PushFindHeader) {
  const string kFieldName0("Content-Length");
  const string kFieldValue0("42");
  const string kFieldName1("Content-Type");
  const string kFieldValue1("application/atom+xml");
  StringPairVector headers;
  HttpClient::PushHeader(kFieldName0, kFieldValue0, &headers);
  HttpClient::PushHeader(kFieldName1, kFieldValue1, &headers);
  ASSERT_EQ(static_cast<size_t>(2), headers.size());
  ASSERT_EQ(kFieldName0, headers[0].first);
  ASSERT_EQ(kFieldValue0, headers[0].second);
  ASSERT_EQ(kFieldName1, headers[1].first);
  ASSERT_EQ(kFieldValue1, headers[1].second);
  string val;
  ASSERT_TRUE(HttpClient::FindHeader(kFieldName0, headers, &val));
  ASSERT_TRUE(HttpClient::FindHeader(kFieldName0, headers, NULL));
  ASSERT_EQ(kFieldValue0, val);
  ASSERT_TRUE(HttpClient::FindHeader(kFieldName1, headers, &val));
  ASSERT_TRUE(HttpClient::FindHeader(kFieldName1, headers, NULL));
  ASSERT_EQ(kFieldValue1, val);
  val.clear();
  ASSERT_FALSE(HttpClient::FindHeader("foo", headers, &val));
  ASSERT_TRUE(val.empty());
  ASSERT_FALSE(HttpClient::FindHeader("foo", headers, NULL));
}

TEST(HttpClientTest, AppendHeaders) {
  const string kKey0("k0");
  const string kVal0("v0");
  const string kKey1("k1");
  const string kVal1("v1");
  StringPairVector src;
  src.push_back(std::make_pair(kKey0, kVal0));
  src.push_back(std::make_pair(kKey1, kVal1));
  StringPairVector dest;
  HttpClient::AppendHeaders(src, &dest);
  ASSERT_EQ(static_cast<size_t>(2), dest.size());
  ASSERT_EQ(kKey0, dest[0].first);
  ASSERT_EQ(kVal0, dest[0].second);
  ASSERT_EQ(kKey1, dest[1].first);
  ASSERT_EQ(kVal1, dest[1].second);
}

TEST(HttpClientTest, InitHeader) {
  const string kAppName("client-app-name");
  HttpClient http_client(kAppName);
  // Verify that the initial state of the request headers includes the
  // 2nd ctor arg to HttpClient as the User-Agent.
  const StringPairVector& headers = http_client.get_headers();
  ASSERT_LT(static_cast<size_t>(0), headers.size());
  string user_agent;
  ASSERT_TRUE(HttpClient::FindHeader("User-Agent", headers, &user_agent));
  ASSERT_EQ(kAppName, user_agent.substr(0, kAppName.size()));
}

TEST(HttpClientTest, Login) {
  const string kAppName("client-app-name");
  const string kServiceName("service-name");
  const string kEmail("user@gmail.com");
  const string kPasswd("big-secret");
  HttpClient http_client(kAppName);
  // The default implementation of SendRequest does not return an Auth= token
  ASSERT_FALSE(http_client.Login(kServiceName, kEmail, kPasswd));
};

TEST(HttpClientTest, LoginWithAuth) {
  // This HttpClient implementation fakes the Google ClientLogin response.
  // See http://code.google.com/apis/accounts/docs/
  //     AuthForInstalledApps.html#Response
  class AuthHttpClient: public HttpClient {
   public:
    AuthHttpClient(const string& app) 
      : HttpClient(app) {
    }
    // The default Login() calls SendRequest with something like this as the
    // request data:
    //   accountType=HOSTED_OR_GOOGLE&Email=jondoe@gmail.com&Passwd=north23AZ&
    //       service=cl&source=Gulp-CalGulp-1.05
    // The default Login() expects to find an "Auth=" somewhere in the response
    // data.
    virtual bool SendRequest(HttpMethodEnum http_method,
                             const string& request_uri,
                             const StringPairVector* request_headers,
                             const string* data,
                             string* response) const {
      if (http_method == HTTP_POST && data && response) {
        *response = string("SID=DQAAAGgA...7Zg8CTN") + "\n" +
                                "LSID=DQAAAGsA...lk8BBbG" + "\n" +
                                "Auth=" + *data + "\n";
        return true;
      }
      return false;
    }
  };
  const string kAppName("app-name");
  const string kServiceName("service-name");
  const string kEmailValue("user@gmail.com");
  const string kPasswdValue("big-secret");
  AuthHttpClient http_client(kAppName);

  // Login() returns true if the SendRequest response has an Auth=.
  ASSERT_TRUE(http_client.Login(kServiceName, kEmailValue, kPasswdValue));

  // The authorization token is saved within HttpClient.
  const string& auth_token = http_client.get_auth_token();

  const string kEmail("Email=");
  size_t email_offset = auth_token.find(kEmail);
  ASSERT_NE(string::npos, email_offset);
  ASSERT_EQ(kEmailValue, auth_token.substr(email_offset + kEmail.size(),
                                           kEmailValue.size()));

  const string kPasswd("Passwd=");
  const size_t passwd_offset = auth_token.find(kPasswd);
  ASSERT_NE(string::npos, passwd_offset);
  ASSERT_EQ(kPasswdValue, auth_token.substr(passwd_offset + kPasswd.size(),
                                           kPasswdValue.size()));

  ASSERT_NE(string::npos, passwd_offset);

  // The "Authorization" header is set and the value is "GoogleLogin auth=" +
  // the authorization token (same as that returned by get_auth_token()).
  const StringPairVector& headers = http_client.get_headers();
  string authorization;
  ASSERT_TRUE(HttpClient::FindHeader("Authorization", headers, &authorization));
  const string kGoogleLoginAuth("GoogleLogin auth=");
  ASSERT_EQ(kGoogleLoginAuth, authorization.substr(0, kGoogleLoginAuth.size()));
  ASSERT_EQ(auth_token, authorization.substr(kGoogleLoginAuth.size()));
}

TEST(HttpClientTest, FetchUrl) {
  const string kServiceName("service-name");
  const string kApplicationName("application-name");
  HttpClient http_client(kApplicationName);
  string response;
  const string kUrl("http://dummy.com/foo");
  ASSERT_TRUE(http_client.FetchUrl(kUrl, &response));
  std::vector<string> response_lines;
  kmlbase::SplitStringUsing(response, "\n", &response_lines);
  // Don't want to complete box ourselves in.  Assert that we expect one
  // line echoing back the request in addition to the header lines.
  ASSERT_LT(http_client.get_headers().size() + 1, response_lines.size());
  std::vector<string> response_words;
  kmlbase::SplitStringUsing(response_lines[0], " ", &response_words);
  ASSERT_EQ(static_cast<size_t>(2), response_words.size());
  // First line of the response is "GET $url"
  ASSERT_EQ(string("GET"), response_words[0]);
  ASSERT_EQ(kUrl, response_words[1]);
  // Look for the User-Agent line and our application name.
  bool found_user_agent = false;
  for (size_t i = 1; i < response_lines.size(); ++i) {
    response_words.clear();
    kmlbase::SplitStringUsing(response_lines[i], " ", &response_words);
    if (response_words[0] == string("User-Agent:")) {
      ASSERT_LT(static_cast<size_t>(2), response_words.size());
      found_user_agent = true;
      ASSERT_EQ(kApplicationName, response_words[1]);
    }
  }
  ASSERT_TRUE(found_user_agent);
}

}  // end namespace kmlconvenience

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

