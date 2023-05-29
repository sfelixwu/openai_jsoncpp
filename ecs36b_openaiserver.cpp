
#ifdef _ECS36B_JSONCPP_

#include "ecs36b_openai.hpp"
#include <iostream>

// for Json::value
#include <json/json.h>
#include <json/reader.h>
#include <json/writer.h>
#include <json/value.h>
#include <string>

// for JsonRPCCPP
#include <iostream>
#include "ecs36b_openaiserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include "ecs36b_openaiclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

// ecs36b
#include "ecs36b_Common.h"
#include "JvTime.h"

using namespace jsonrpc;
using namespace std;

class MyopenAIServer : public ecs36b_openAIServer
{
public:
  MyopenAIServer(AbstractServerConnector &connector, serverVersion_t type);
  virtual Json::Value completion(const Json::Value& input, const std::string& team_name);
};

MyopenAIServer::MyopenAIServer(AbstractServerConnector &connector, serverVersion_t type)
  : ecs36b_openAIServer(connector, type)
{
  std::cout << "MyopenAIServer Object created" << std::endl;
}

// member functions

Json::Value
MyopenAIServer::completion
(const Json::Value& input, const std::string& team_name)
{
  Json::Value lv_log;
  lv_log["arg input"] = input;
  lv_log["arg team_name"] = team_name;
  myPrintLog(lv_log.toStyledString(), "./config/ecs36b_openai.log");
  
  int i;
  int error_code = 0;
  int rc = 0;
  Json::Value result;
  result["status"] = "successful";

  try {
    if((input["model"].isNull() == true) ||
       (input["model"].isString() == false))
      {
	error_code = -1;
	throw ecs36b_Exception
	  { "input JSON model missing" };
      }

    if((input["prompt"].isNull() == true) ||
       (input["prompt"].isString() == false))
      {
	error_code = -2;
	throw ecs36b_Exception
	  { "input JSON prompt missing" };
      }

    if((input["max_tokens"].isNull() == true) ||
       (input["max_tokens"].isInt() == false))
      {
	error_code = -3;
	throw ecs36b_Exception
	  { "input JSON max tokens missing" };
      }
    
    auto response_json = openai::completion().create(input);
    std::cout << "Response is:\n" << response_json.toStyledString() << '\n';
    result["response"] = response_json;
    
  } catch (ecs36b_Exception& e) {
    std::cerr << e.what() << std::endl;
    result["reason"] = e.what();
    result["error_code"] = error_code;
    result["status"] = "failed";
  }

  lv_log["result"] = result;
  myPrintLog(lv_log.toStyledString(), "./config/ecs36b_openai.log");
  
  return result;
}

int
main(void)
{
  openai::start();
  JvTime * my_ptr = getNowJvTime();

  srand((my_ptr->second) * (my_ptr->minute) * (my_ptr->year));
  HttpServer httpserver(8384);
  MyopenAIServer s(httpserver,
		JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  std::cout << "Hit enter to stop the server" << endl;
  getchar();

  s.StopListening();
  return 0;
}

#endif /* _ECS36B_JSONCPP_ */
