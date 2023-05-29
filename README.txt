
Important Configuration for running ecs36b_openaiserver --
$ mkdir config
$ touch ./config/ecs36b_openai.log
(or you can modify the log file name in ecs36b_openaiserver.cpp)

Information regarding this Github folder --

This folder is a modified version of openai API C++ code,
i.e., openai.hpp and json.hpp (Nlohmann's JSON library),
for ecs36b, an Intermediate C++ Object-Oriented Programming course
at University of California, Davis.

I modified the code so it is now working with JSON-CPP (instead of Nlokmann).
I used the _ECS36b_JSONCPP_ def to separate the original code and my JSONCPP updates.

Two main reasons motivated me to perform such updates --
(1) JSON-CPP has been the instructional JSON library for ecs36b at UC Davis.
(2) JSON-CPP will work correctly with JSON-RPC-CPP and the jsonrpcstub code generator.

And, the reason for this JSONRPC agent/wrapper such that the students can access the
service without paying additional for such interesting educational activities for
programming. Instead, they will use my ngrok link
(i.e., the instructor's laptop with a paid OpenAI account) --
https://ethikos.ngrok.io

The current code only supports "completion" and I am pretty sure that there are bugs,
e.g., I don't believe that I have done all the necessary JSON error checking and threw
all the needed exceptions.

Anyway, any question, advice, or comment, please contact --

Prof. S. Felix Wu
Computer Science Department
University of California, Davis
sfwu@ucdavis.edu
