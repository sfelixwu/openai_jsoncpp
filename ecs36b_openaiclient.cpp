
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
  if (_argc != 3)
    {
      std::cout << "usage: " << _argv[0] << " <end-point> <JSON file name> <ret>\n";
      return -1;
    }

  if ((strcmp(_argv[1], "completion") != 0) &&
      (strcmp(_argv[1], "contrary") != 0) &&
      (strcmp(_argv[1], "chat") != 0))
    {
      std::cout << "usage: " << _argv[0] << " <end-point> must be completion, chat or contrary\n";
      return -1;
    }
  Json::Value input_json;
  Json::Value myv;
  
  rc = myFile2JSON(_argv[2], &input_json);
  if (rc != 0)
    {
      std::cout << "Input JSON file " << _argv[2] << " JSON parsing error \n";
      return -2;
    }
  
  HttpClient httpclient("https://ethikos.ngrok.io");
  httpclient.SetTimeout(0); // to get ride of curl timeout error
  ecs36b_openAIClient myClient(httpclient, JSONRPC_CLIENT_V2);

  // ecs36b s2023 students, please modify the following line for your own team name.
  std::string team_name = "fire ball";
  
  try
    {
      if (strcmp(_argv[1], "completion") == 0)
	{
	  myv = myClient.completion(input_json, team_name);
	}
      else
	{
	  if (strcmp(_argv[1], "chat") == 0)
	    {
	      myv = myClient.chat(input_json, team_name);
	    }
	  else
	    {
	      if (strcmp(_argv[1], "contrary") == 0)
		{
		  myv = myClient.contrary(input_json, team_name);
		}
	    }
	}
      std::cout << "Response is:\n" << myv.toStyledString() << '\n';
    }
  catch (JsonRpcException &e)
    {
      cerr << e.what() << endl;
    }

  return 0;
}
#endif  /* _ECS36B_JSONCPP_ */
