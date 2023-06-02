
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
  virtual Json::Value chat(const Json::Value& input, const std::string& team_name);
  virtual Json::Value contrary(const Json::Value& input, const std::string& team_name);
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

Json::Value
MyopenAIServer::chat
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

    if((input["messages"].isNull() == true) ||
       (input["messages"].isArray() == false))
      {
	error_code = -2;
	throw ecs36b_Exception
	  { "input JSON message missing or incorrect format" };
      }

    if((input["max_tokens"].isNull() == true) ||
       (input["max_tokens"].isInt() == false))
      {
	error_code = -3;
	throw ecs36b_Exception
	  { "input JSON max tokens missing" };
      }
    
    auto response_json = openai::chat().create(input);
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

Json::Value
MyopenAIServer::contrary
(const Json::Value& input, const std::string& team_name)
{
  Json::Value lv_log;
  lv_log["arg input"] = input;
  lv_log["arg team_name"] = team_name;
  myPrintLog(lv_log.toStyledString(), "./config/ecs36b_openai.log");
  
  int i;
  int error_code = 0;
  int rc = 0;
  Json::Value input_w;
  Json::Value result;
  Json::Value response_1_json;
  Json::Value response_2_json;
  Json::Value response_3_json;
  std::string new_prompt;
  
  result["status"] = "successful";

  try {

#ifdef _ECS36B_INCLASS_
    // for ecs36b only
    
    char buf_fname1[256];
    bzero(buf_fname1, 256);
    snprintf(buf_fname1, 255, "./config/ecs36b_teams.json");

    char buf_fname2[256];
    bzero(buf_fname2, 256);
    snprintf(buf_fname2, 255, "./config/ecs36b_students.json");

    Json::Value lv_teams;
    Json::Value lv_students;

    rc = myFile2JSON(buf_fname1, &lv_teams);
    if (rc != 0)
      {
	error_code = rc;
	throw ecs36b_Exception
	  { ("myFile2JSON error " + std::string { buf_fname1 }) };
      }

    if((lv_teams.isNull() == true) ||
       (lv_teams.isObject() == false))
      {
	error_code = -1;
	throw ecs36b_Exception
	  { ("teams JSON format incorrect " + std::string { buf_fname1 }) };
      }

    rc = myFile2JSON(buf_fname2, &lv_students);
    if (rc != 0)
      {
	error_code = rc;
	throw ecs36b_Exception
	  { ("myFile2JSON error " + std::string { buf_fname2 }) };
      }

    if ((lv_students.isNull() == true) ||
	(lv_students.isObject() == false))
      {
	error_code = -2;
	throw ecs36b_Exception
	  { ("students JSON format incorrect " + std::string { buf_fname2 }) };
      }

    if (team_name.empty())
      {
	error_code = -3;
	throw ecs36b_Exception { ("team name empty") };
      }

    if ((lv_teams[team_name]).isNull() == true)
      {
	// this team name has not been registered
	error_code = -5;
	throw ecs36b_Exception
	  { ("team name unrecognized: " + team_name) };
      }
#endif /* _ECS36B_INCLASS_ */

    result["team"] = team_name;
    
    if((input["endpoint"].isNull() == true) ||
       (input["endpoint"].isString() == false))
      {
	error_code = -7;
	throw ecs36b_Exception
	  { "input JSON endpoint missing" };
      }

    if((input["model"].isNull() == true) ||
       (input["model"].isString() == false))
      {
	error_code = -1;
	throw ecs36b_Exception
	  { "input JSON model missing" };
      }
    input_w["model"] = input["model"].asString();

    if (input["endpoint"].asString() == "completion")
      {
	if((input["prompt 1"].isNull() == true) ||
	   (input["prompt 1"].isString() == false))
	  {
	    error_code = -2;
	    throw ecs36b_Exception
	      { "input JSON prompt 1 missing" };
	  }

	if((input["prompt 2"].isNull() == true) ||
	   (input["prompt 2"].isString() == false))
	  {
	    error_code = -2;
	    throw ecs36b_Exception
	      { "input JSON prompt 2 missing" };
	  }

	if((input["max_tokens"].isNull() == true) ||
	   (input["max_tokens"].isInt() == false))
	  {
	    error_code = -3;
	    throw ecs36b_Exception
	      { "input JSON max tokens missing" };
	  }
	input_w["max_tokens"] = input["max_tokens"].asInt();
	input_w["temperature"] = 0;
    
	input_w["prompt"] = input["prompt 1"].asString();
	input_w["max_tokens"] = 128; // make sure that the response sentence is short.
	response_1_json = openai::completion().create(input_w);
	std::cout << "Response 1 is:\n" << response_1_json.toStyledString() << '\n';
	result["response 1"] = response_1_json;

	if ((response_1_json["choices"].isNull() == true) ||
	    (response_1_json["choices"].isArray() == false) ||
	    (response_1_json["choices"][0].isObject() == false) ||
	    (response_1_json["choices"][0]["text"].isNull() == true) ||
	    (response_1_json["choices"][0]["text"].isString() == false))
	  {
	    error_code = -4;
	    throw ecs36b_Exception
	      { "response 1 ill-formated" };
	  }
	
	input_w["prompt"] = input["prompt 2"].asString();
	input_w["max_tokens"] = 128; // make sure that the response sentence is short.
	response_2_json = openai::completion().create(input_w);
	std::cout << "Response 2 is:\n" << response_2_json.toStyledString() << '\n';
	result["response 2"] = response_2_json;

	if ((response_2_json["choices"].isNull() == true) ||
	    (response_2_json["choices"].isArray() == false) ||
	    (response_2_json["choices"][0].isObject() == false) ||
	    (response_2_json["choices"][0]["text"].isNull() == true) ||
	    (response_2_json["choices"][0]["text"].isString() == false))
	  {
	    error_code = -4;
	    throw ecs36b_Exception
	      { "response 2 ill-formated" };
	  }
	
	new_prompt = "Given the following two statements: (1) \"" +
	  response_1_json["choices"][0]["text"].asString() +
	  "\" (2) \"" +
	  response_2_json["choices"][0]["text"].asString() +
	  "\" Please answer if the above two statements are contrary to each other?";
	std::cout << new_prompt << std::endl;
    
	input_w["prompt"] = new_prompt;
	response_3_json = openai::completion().create(input_w);
      }
    else
      {
	if (input["endpoint"].asString() == "chat")
	  {
	    if((input["messages 1"].isNull() == true) ||
	       (input["messages 1"].isArray() == false) ||
	       (input["messages 1"][0].isNull() == true) ||
	       (input["messages 1"][0].isObject() == false) ||
	       (input["messages 1"][0]["content"].isNull() == true) ||
	       (input["messages 1"][0]["content"].isString() == false))
	      {
		error_code = -2;
		throw ecs36b_Exception
		  { "input JSON messages 1 missing" };
	      }
	  
	    if((input["messages 2"].isNull() == true) ||
	       (input["messages 2"].isArray() == false) ||
	       (input["messages 2"][0].isNull() == true) ||
	       (input["messages 2"][0].isObject() == false) ||
	       (input["messages 2"][0]["content"].isNull() == true) ||
	       (input["messages 2"][0]["content"].isString() == false))
	      {
		error_code = -2;
		throw ecs36b_Exception
		  { "input JSON messages 2 missing" };
	      }

	    if((input["max_tokens"].isNull() == true) ||
	       (input["max_tokens"].isInt() == false))
	      {
		error_code = -3;
		throw ecs36b_Exception
		  { "input JSON max tokens missing" };
	      }

	    input_w["max_tokens"] = input["max_tokens"].asInt();
	    input_w["temperature"] = 0;
    
	    input_w["messages"] = input["messages 1"];
	    input_w["max_tokens"] = 128; // make sure that the response sentence is short.
	    response_1_json = openai::chat().create(input_w);
	    std::cout << "Response 1 is:\n" << response_1_json.toStyledString() << '\n';
	    result["response 1"] = response_1_json;
	  
	    if ((response_1_json["choices"].isNull() == true) ||
		(response_1_json["choices"].isArray() == false) ||
		(response_1_json["choices"][0].isObject() == false) ||
		(response_1_json["choices"][0]["message"].isNull() == true) ||
		(response_1_json["choices"][0]["message"].isObject() == false) ||
		(response_1_json["choices"][0]["message"]["content"].isNull() == true) ||
		(response_1_json["choices"][0]["message"]["content"].isString() == false))
	      {
		error_code = -4;
		throw ecs36b_Exception
		  { "response 1 ill-formated" };
	      }
	  
	    input_w["messages"] = input["messages 2"];
	    input_w["max_tokens"] = 128; // make sure that the response sentence is short.
	    response_2_json = openai::chat().create(input_w);
	    std::cout << "Response 2 is:\n" << response_2_json.toStyledString() << '\n';
	    result["response 2"] = response_2_json;

	    if ((response_2_json["choices"].isNull() == true) ||
		(response_2_json["choices"].isArray() == false) ||
		(response_2_json["choices"][0].isObject() == false) ||
		(response_2_json["choices"][0]["message"].isNull() == true) ||
		(response_2_json["choices"][0]["message"].isObject() == false) ||
		(response_2_json["choices"][0]["message"]["content"].isNull() == true) ||
		(response_2_json["choices"][0]["message"]["content"].isString() == false))
	      {
		error_code = -4;
		throw ecs36b_Exception
		  { "response 2 ill-formated" };
	      }

	    new_prompt = "Given the following two statements: (1) \"" +
	      response_1_json["choices"][0]["message"]["content"].asString() +
	      "\" (2) \"" +
	      response_2_json["choices"][0]["message"]["content"].asString() +
	      "\" Please answer if the above two statements are contrary to each other?";
	    std::cout << new_prompt << std::endl;
    
	    input_w["messages"][0]["content"] = new_prompt;
	    response_3_json = openai::chat().create(input_w);
	  }
	else
	  {
	    error_code = -5;
	    throw ecs36b_Exception
	      { "endpoint unrecognized" };
	  }
      }

    std::cout << "Response 3 is:\n" << response_3_json.toStyledString() << '\n';
    result["response 3"] = response_3_json;

#ifdef _ECS36B_INCLASS_
    // now register the score
    Json::Value lv_team_list = lv_teams[team_name];
    if ((lv_team_list.isNull() == false) &&
	(lv_team_list.isArray() == true))
      {
	int i;
	Json::Value lv_score_list;
	for (i = 0; i < lv_team_list.size(); i++)
	  {
	    std::string lv_student_id = (lv_team_list[i]).asString();
	    int lv_score = lv_students[lv_student_id]["score"].asInt();
	    lv_score += 1;
	    lv_students[lv_student_id]["score"] = lv_score;
	    // send the scores back
	  }

	// save to all three files
	rc = myJSON2File(buf_fname1, &lv_teams);
	if (rc != 0)
	  {
	    error_code = rc;
	    throw ecs36b_Exception
	      { ("myJSON2File error " + std::string { buf_fname1 }) };
	  }

	rc = myJSON2File(buf_fname2, &lv_students);
	if (rc != 0)
	  {
	    error_code = rc;
	    throw ecs36b_Exception
	      { ("myJSON2File error " + std::string { buf_fname2 }) };
	  }
      }
#endif /* _ECS36B_INCLASS_ */

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
