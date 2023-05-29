
#include "ecs36b_openai.hpp"
#include <iostream>

#ifdef _ECS36B_JSONCPP_
// JSON RPC part
#include "ecs36b_openaiclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

int
main(int _argc, char *_argv[])
{
  int rc;
  if (_argc != 2)
    {
      std::cout << "usage: " << _argv[0] << " <JSON file name> <ret>\n";
      return -1;
    }

  Json::Value input_json;
  Json::Value myv;
  
  rc = myFile2JSON(_argv[1], &input_json);
  if (rc != 0)
    {
      std::cout << "Input JSON file " << _argv[1] << " JSON parsing error \n";
      return -2;
    }
  
  HttpClient httpclient("https://ethikos.ngrok.io");
  ecs36b_openAIClient myClient(httpclient, JSONRPC_CLIENT_V2);
  
  try
    {
      myv = myClient.completion(input_json, "ecs36b_teamA");
      std::cout << "Response is:\n" << myv.toStyledString() << '\n';
    }
  catch (JsonRpcException &e)
    {
      cerr << e.what() << endl;
    }

  return 0;
}
#endif  /* _ECS36B_JSONCPP_ */
