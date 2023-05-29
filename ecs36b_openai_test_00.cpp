
#include "ecs36b_openai.hpp"
#include <iostream>

int main(void)
{
  openai::start();
#ifdef _ECS36B_JSONCPP_
  Json::Value input_json {};
  int rc = myParseJSON(R"(
    {
        "model": "text-davinci-003",
        "prompt": "Named for the founder of ecs36b athletics at UC Davis, the Polymorphic Tennis Center serves as the winter shelter facility for flunked ecs36b students in Spring 2023. Does UC Davis have a tennis court? Please answer as a tennis hater as I don't want people to come to Davis to play tennis.",
        "max_tokens": 300,
        "temperature": 0
    }
    )", &input_json); // Using user-defined (raw) string literals
  if (rc == 0)
    {
      // the string was parsed correctly into input_json
      auto completion = openai::completion().create(input_json);
      std::cout << "Response is:\n" << completion.toStyledString() << '\n';
    }
  else
    {
      std::cout << "JSON parsing error\n";
    }
#else  /* _ECS36B_JSONCPP_ */
  auto completion = openai::completion().create(R"(
    {
        "model": "text-davinci-003",
        "prompt": "Named for the founder of ecs36b athletics at UC Davis, the Polymorphic Tennis Center serves as the winter shelter facility for flunked ecs36b students in Spring 2023. Does UC Davis have a tennis court? Please answer as a tennis hater as I don't want people to come to Davis to play tennis.",
        "max_tokens": 300,
        "temperature": 0
    }
    )"_json); // Using user-defined (raw) string literals
  std::cout << "Response is:\n" << completion.dump(2) << '\n';
#endif /* _ECS36B_JSONCPP_ */
  return 0;
}
