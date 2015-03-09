#include "curl/curl.h"
#include <iostream>

int main(int argc,char * argv[])
{
  CURLM*  multiHandle;
  multiHandle = curl_multi_init();
  curl_multi_setopt(multiHandle, CURLMOPT_MAXCONNECTS, 10);
  curl_multi_cleanup(multiHandle);

  return 0;
}
