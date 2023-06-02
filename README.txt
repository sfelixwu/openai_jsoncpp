
Important Configuration for running ecs36b_openaiserver --
$ mkdir config
$ touch ./config/ecs36b_openai.log
(or you can modify the log file name in ecs36b_openaiserver.cpp)

For ecs36b students (for participation credits) --
(0) download the whole folder as a zip file, unzip it, and cd to that directory on your device
(1) modify ecs36b_openaiclient.cpp to enter your team name
(2) $ make clean
(3) $ make
(4) construct your own JSON, following the example of contrary_test_*.json, and save it as mine.json
(5) $ ./ecs36b_openaiclient contrary mine.json
(6) if the response says successful, then you are done.

Information regarding this Github folder --

This folder is a modified version of openai API C++ code,
i.e., openai.hpp and json.hpp (Nlohmann's JSON library),
for ecs36b, an Intermediate C++ Object-Oriented Programming course
at University of California, Davis.

The original source code was obtained from --
https://github.com/olrea/openai-cpp

I modified the code so it is now working with JSON-CPP (instead of Nlohmann).
I used the _ECS36b_JSONCPP_ def to separate the original code and my JSONCPP updates.

Two main reasons motivated me to perform such updates --
(1) JSON-CPP has been the instructional JSON library for ecs36b at UC Davis.
(2) JSON-CPP will work correctly with JSON-RPC-CPP and the jsonrpcstub code generator.

And, the reason for this JSONRPC agent/wrapper such that the students can access the
service without paying additional for such interesting educational activities for
programming. Instead, they will use my ngrok link
(i.e., the instructor's laptop with a paid OpenAI account) --
https://ethikos.ngrok.io

The current JSONRPC part of the code (please see the specification file ecs306_openai_jsonrpc.json)
only supports two endpoints "completion" and "chat". I am pretty sure that there are bugs,
e.g., I don't believe that I have done all the necessary JSON error checking and threw
all the needed exceptions. I also implemented a new service call "contrary", which is a new type of
service under the concept of "Differential Prompting for Trustworthiness".

Examples for completion, chat, and contrary are given under this directory. For JSONRPC clients, the
JSON now requires one extra key-value pair: "endpoint": "<endpoint>", where <endpoint> could be either
"completion" or "chat". The ecs36b_openaiserver will need to see this to arrange the right call as
the JSONRPC server is acting as a proxy for both endpoints. If the call is contrary, then the handling
will be more complicated.

Anyway, any question, advice, or comment, please contact --

Prof. S. Felix Wu
Computer Science Department
University of California, Davis
sfwu@ucdavis.edu
